
/// @file Parser.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018, 2021, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/Parser.h"
#include "dotlib/AstValue.h"
#include "dotlib/AstAttr.h"

#include "ym/MsgMgr.h"

#include "parse_simple.h"
#include "parse_complex.h"
#include "parse_group.h"


BEGIN_NAMESPACE_YM_DOTLIB

FixedElemHeader Parser::sEmptyHeader({});

FixedElemHeader Parser::sFloatFloatHeader(
  { read_float, read_float }
);

FixedElemHeader Parser::sFloatFloatStrHeader(
  { read_float, read_float, read_string }
);

FixedElemHeader Parser::sFloatStrHeader(
  { read_float, read_string }
);

FixedElemHeader Parser::sFloatVectorHeader(
  { read_float_vector }
);

FixedElemHeader Parser::sIntFloatHeader(
  { read_int, read_float }
);

FixedElemHeader Parser::sIntFloatVectorHeader(
  { read_int, read_float_vector }
);

FixedElemHeader Parser::sIntStrHeader(
  { read_int, read_string }
);

FixedElemHeader Parser::sIntVectorHeader(
  { read_int_vector }
);

FixedElemHeader Parser::sStrFloatHeader(
  { read_string, read_float }
);

FixedElemHeader Parser::sStrHeader(
  { read_string }
);

FixedElemHeader Parser::sStrIntHeader(
  { read_string, read_int }
);

FixedElemHeader Parser::sStrStrHeader(
  { read_string, read_string }
);

FixedElemHeader Parser::sStrStrIntHeader(
  { read_string, read_string, read_int }
);

FixedElemHeader Parser::sStrStrStrHeader(
  { read_string, read_string, read_string }
);

FixedElemHeader Parser::sTechnologyHeader(
  { read_technology }
);

FanoutLengthHeader Parser::sFanoutLengthHeader;

ListHeader Parser::sFloatVectorListHeader( read_float_vector );

ListHeader Parser::sIntListHeader( read_int );

ListHeader Parser::sStrListHeader( read_string );

OptElemHeader Parser::sOptStrHeader( read_string );

ListHeader Parser::sVarTypeListHeader( read_variable_type );

std::unordered_map<std::string, AttrHandler> Parser::sHandlerDict{
#include "Parser_dict.cc"
};

// @brief コンストラクタ
Parser::Parser(
  std::istream& s,
  const FileInfo& file_info,
  bool debug,
  bool allow_no_semi
) : mScanner{s, file_info},
    mDebug{debug},
    mAllowNoSemi{allow_no_semi}
{
}

// @brief デストラクタ
Parser::~Parser()
{
}

// @brief パーズする
AstAttrPtr
Parser::parse()
{
  // 先頭(根本)の属性は 'library' でなければならない．
  auto token = mScanner.read_attr();
  if ( token.value() != "library" ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    token.loc(),
		    MsgType::Error,
		    "DOTLIB_PARSER",
		    "'library' keyword is expected "
		    "on the top of the structure");
    throw std::invalid_argument{"Syntax error"};
  }

  // 本体を読み込む．
  auto library = parse_group_statement(token.value(), token.loc(), "library", sStrHeader);

  read_tail();

  // これより後の内容は無視される．
  for ( ; ; ) {
    Token token = mScanner.read_token();
    if ( token.type() == TokenType::END ) {
      break;
    }
    MsgMgr::put_msg(__FILE__, __LINE__,
		    token.loc(),
		    MsgType::Warning,
		    "DOTLIB_PARSER",
		    "contents after library group are ignored.");
  }

  return library;
}

// @brief Simple Attribute を読み込む．
AstAttrPtr
Parser::parse_simple_attribute(
  const std::string& kwd,
  const FileRegion& kwd_loc,
  SimpleHandler handler
)
{
  // 属性名の直後は必ず ':' でなければならない．
  auto _ = mScanner.read_and_verify(TokenType::COLON);

  // その後の値を読み込む．
  auto value = handler(mScanner);

  // 末尾の ';' を確認
  read_tail();

  // 値を返す．
  return AstAttrPtr{new AstAttr(kwd, kwd_loc, std::move(value))};
}

// @brief Complex Attribute を読み込む．
AstAttrPtr
Parser::parse_complex_attribute(
  const std::string& kwd,
  const FileRegion& kwd_loc,
  HeaderHandler& handler
)
{
  auto value = parse_header(handler);

  read_tail();

  return AstAttrPtr{new AstAttr(kwd, kwd_loc, std::move(value))};
}

// @brief Group Statement を読み込む．
AstAttrPtr
Parser::parse_group_statement(
  const std::string& kwd,
  const FileRegion& kwd_loc,
  const char* group_name,
  HeaderHandler& header_handler
)
{
  // ヘッダをパースする．
  auto header_value = parse_header(header_handler);

  // グループ本体の始まり
  auto lcb_token = mScanner.read_and_verify(TokenType::LCB);

  std::vector<AstAttrPtr> child_list;
  for ( ; ; ) {
    auto token = mScanner.peek_token();
    if ( token.type() == TokenType::NL ) {
      mScanner.accept_token();
      // 改行は読み飛ばす．
      continue;
    }
    if ( token.type() == TokenType::RCB ) {
      // '}' ならグループ記述の終わり
      FileRegion rcb_loc{token.loc()};
      mScanner.accept_token();

      // グループ本体の終わり．
      auto token = mScanner.read_and_verify(TokenType::NL);

      // 値を作る．
      FileRegion loc{lcb_token.loc(), rcb_loc};
      auto group_value = AstValue::new_group(std::move(header_value), child_list, loc);
      return AstAttrPtr{new AstAttr(kwd, kwd_loc, std::move(group_value))};
    }

    // 子供の要素を読み込む．
    auto child_attr = mScanner.read_attr();
    auto key = std::string(group_name) + ":" + std::string(child_attr.value());
    if ( sHandlerDict.count(key) > 0 ) {
      auto handler = sHandlerDict.at(key);
      auto child = handler(*this, child_attr.value(), child_attr.loc());
      child_list.push_back(std::move(child));
    }
    else {
      // 対応するハンドラが登録されていない．
      syntax_error(child_attr.value(), child_attr.loc());
    }
  }
}

// @brief Complex Attribute, GroupStatement のヘッダを読み込む．
AstValuePtr
Parser::parse_header(
  HeaderHandler& handler
)
{
  // まず '(' があるか確認する．
  auto lp_token = mScanner.read_and_verify(TokenType::LP);

  // complex attribute の始まり
  handler.begin_header(lp_token.loc());
  for ( SizeType count = 0; ; ++ count) {
    auto token = mScanner.peek_token();
    if ( token.type() == TokenType::RP ) {
      // ')' を読み込んだので終わる．
      mScanner.accept_token();
      // complex attribute の終わり
      return handler.end_header(token.loc(), count);
    }
    if ( count > 0 ) {
      // 要素と要素の間には ',' が必要
      mScanner.read_and_verify(TokenType::COMMA);
    }
    // ハンドラを用いて値を読み込む．
    handler.read_header_value(mScanner, token.loc(), count);
  }
}

// @brief 行末まで読み込む．
void
Parser::read_tail()
{
  // 通常は ';' '\n' を適正なパタンとみなす．
  // ただし，mAllowNoSemi が true の場合には
  // ';' がなくてもよい．．
  // またファイルの末尾の場合には改行は不要
  auto token = mScanner.read_token();
  if ( token.type() == TokenType::SEMI ) {
    token = mScanner.read_token();
  }
  else if ( !mAllowNoSemi ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    token.loc(),
		    MsgType::Error,
		    "DOTLIB_PARSER",
		    "Syntax error. Semicolon is expected.");
    throw std::invalid_argument{"Syntax error"};
  }
  if ( token.type() != TokenType::NL && token.type() != TokenType::END ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    token.loc(),
		    MsgType::Error,
		    "DOTLIB_PARSER",
		    "Syntax error. New-line is expected.");
    throw std::invalid_argument{"Syntax error"};
  }
}

// @brief デバッグモードの時 true を返す．
bool
Parser::debug()
{
  return mDebug;
}


//////////////////////////////////////////////////////////////////////
// エラー出力用の便利関数
//////////////////////////////////////////////////////////////////////

// @brief 未対応の属性名に対するエラーメッセージを出力する．
// @param[in] attr 対象の属性
void
syntax_error(
  const std::string& kwd,
  const FileRegion& kwd_loc
)
{
  std::ostringstream buf;
  buf << "Syntax error. Unexpected keyword: " << kwd;
  MsgMgr::put_msg(__FILE__, __LINE__,
		  kwd_loc,
		  MsgType::Error,
		  "DOTLIB_PARSER",
		  buf.str());
  throw std::invalid_argument{"Syntax error"};
}

// @brief 同じ属性が重複して定義されている時のエラーを出力する．
void
duplicate_error(
  const std::string& kwd,
  const FileRegion& kwd_loc,
  const AstAttr* prev_node
)
{
  std::ostringstream buf;
  buf << kwd << " appear more than once."
      << " Previously appears at " << prev_node->kwd_loc();
  MsgMgr::put_msg(__FILE__, __LINE__,
		  kwd_loc,
		  MsgType::Error,
		  "DOTLIB_PARSER",
		  buf.str());
  throw std::invalid_argument{"Syntax error"};
}


//////////////////////////////////////////////////////////////////////
// TokenType に関する関数
//////////////////////////////////////////////////////////////////////

// @brief TokenType 内容をストリームに出力する．
std::ostream&
operator<<(
  std::ostream& s,
  TokenType type
)
{
  const char* type_str = nullptr;
  switch ( type ) {
  case TokenType::COLON:      type_str = "':'"; break;
  case TokenType::SEMI:       type_str = "';'"; break;
  case TokenType::COMMA:      type_str = "','"; break;
  case TokenType::PLUS:       type_str = "'+'"; break;
  case TokenType::MINUS:      type_str = "'-'"; break;
  case TokenType::MULT:       type_str = "'*'"; break;
  case TokenType::DIV:        type_str = "'/'"; break;
  case TokenType::LP:         type_str = "'('"; break;
  case TokenType::RP:         type_str = "')'"; break;
  case TokenType::LCB:        type_str = "'{'"; break;
  case TokenType::RCB:        type_str = "'}'"; break;
  case TokenType::SYMBOL:     type_str = "SYMBOL"; break;
  case TokenType::BOOL_0:     type_str = "BOOL_0"; break;
  case TokenType::BOOL_1:     type_str = "BOOL_1"; break;
  case TokenType::EXPRESSION: type_str = "EXPRESSION"; break;
  case TokenType::NL:         type_str = "new-line"; break;
  case TokenType::ERROR :     type_str = "error"; break;
  case TokenType::END:        type_str = "end-of-file"; break;
  default:
    ASSERT_NOT_REACHED;
  }
  s << type_str;
  return s;
}

END_NAMESPACE_YM_DOTLIB
