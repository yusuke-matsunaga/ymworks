
/// @file Scanner.cc
/// @brief Scanner の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2019, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/DotlibScanner.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_DOTLIB

// @brief 属性を読み込む．
Token
DotlibScanner::read_attr()
{
  Token token;

  // 空行を読み飛ばす．
  for ( token = read_token();
	token.type() == TokenType::NL;
	token = read_token() ) { }

  if ( token.type() != TokenType::SYMBOL ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    token.loc(),
		    MsgType::Error,
		    "DOTLIB_SCANNER",
		    "attribute keyword is expected");
    throw std::invalid_argument{"Syntax error"};
  }

  return token;
}

// @brief 次のトークンを調べる．
Token
DotlibScanner::peek_token()
{
  if ( mCurToken.type() == TokenType::ERROR ) {
    auto type = _scan();
    mCurToken = {type, cur_region(), ShString(mCurString)};
  }
  return mCurToken;
}

// @brief 調べたトークンを読み込む．
void
DotlibScanner::accept_token()
{
  mCurToken = Token{}; // 不正値
}

// @brief トークンを一つとってくる．
Token
DotlibScanner::read_token()
{
  auto ans = peek_token();
  accept_token();
  return ans;
}

// @brief トークンを読み込み，期待値と比較する．
Token
DotlibScanner::read_and_verify(
  TokenType token_type
)
{
  auto token = read_token();
  if ( token.type() != token_type ) {
    // エラーメッセージを作る．
    const char* type_str = nullptr;
    switch ( token_type ) {
    case TokenType::COLON:      type_str = "':'"; break;
    case TokenType::SEMI:       type_str = "';'"; break;
    case TokenType::COMMA:      type_str = "','"; break;
    case TokenType::PLUS:       type_str = "'+'"; break;
    case TokenType::MINUS:      type_str = "'-'"; break;
    case TokenType::MULT:       type_str = "'*'"; break;
    case TokenType::DIV:        type_str = "'/'"; break;
    case TokenType::NOT:        type_str = "'!'"; break;
    case TokenType::AND:        type_str = "'&'"; break;
    case TokenType::OR:         type_str = "'|'"; break;
    case TokenType::XOR:        type_str = "'^'"; break;
    case TokenType::PRIME:      type_str = "'";   break;
    case TokenType::LP:         type_str = "'('"; break;
    case TokenType::RP:         type_str = "')'"; break;
    case TokenType::LCB:        type_str = "'{'"; break;
    case TokenType::RCB:        type_str = "'}'"; break;
    case TokenType::SYMBOL:     type_str = "STR"; break;
    case TokenType::BOOL_0:     type_str = "BOOL_0"; break;
    case TokenType::BOOL_1:     type_str = "BOOL_1"; break;
    case TokenType::EXPRESSION: type_str = "EXPRESSION"; break;
    case TokenType::NL:         type_str = "new-line"; break;
    default:                    ASSERT_NOT_REACHED;
    }
    std::ostringstream buf;
    buf << "syntax error. " << type_str << " is expected.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    token.loc(),
		    MsgType::Error,
		    "DOTLIB_PARSER",
		    buf.str());
    throw std::invalid_argument{"Syntax error"};
  }
  return token;
}

// @brief 一文字読み込む．
// @return 読み込んだトークンを返す．
TokenType
DotlibScanner::_scan()
{
  int c;
  mCurString.clear();

 ST_INIT: // 初期状態
  c = get();
  set_first_loc();

  switch (c) {
  case EOF:
    return TokenType::END;

  case ' ':
  case '\t':
    // 先頭の空白文字はスキップ
    goto ST_INIT;

  case '\n':
    // 改行
    return TokenType::NL;

  case '\"':
    // " によるクォート
    goto ST_DQ;

  case '\\':
    // エスケープシーケンスは改行のみが有効
    c = peek();
    if ( c == '\n' ) {
      // ただの空白とみなす．
      accept();
      goto ST_INIT;
    }
    // それ以外はバックスラッシュがなかったことにする．
    goto ST_INIT;

  case '/':
    // コメントの可能性を調べる．
    goto ST_comment1;

  case ':':
    return TokenType::COLON;

  case ';':
    return TokenType::SEMI;

  case ',':
    return TokenType::COMMA;

  case '+':
    return TokenType::PLUS;

  case '-':
    return TokenType::MINUS;

  case '*':
    return TokenType::MULT;

  case '(':
    return TokenType::LP;

  case ')':
    return TokenType::RP;

  case '{':
    return TokenType::LCB;

  case '}':
    return TokenType::RCB;

  default:
    // それ以外は一旦文字列の要素とみなす．
    _put_char(c);
    goto ST_SYMBOL;
  }
  ASSERT_NOT_REACHED;

 ST_SYMBOL: // シンボルモード
  c = peek();
  switch ( c ) {
  case EOF:
  case ' ':
  case '\t':
  case '\n':
  case '/':
  case ':':
  case ';':
  case ',':
  case '+':
  case '-':
  case '*':
  case '(':
  case ')':
  case '{':
  case '}':
    return TokenType::SYMBOL;

  default:
    accept();
    _put_char(c);
    goto ST_SYMBOL;
  }
  ASSERT_NOT_REACHED;

 ST_DQ: // '"'があったら次の'"'までを強制的に文字列だと思う．
  c = get();
  if ( c == '\"' ) {
    return TokenType::SYMBOL;
  }
  if ( c == '\n' ) {
    std::ostringstream buf;
    buf << "unexpected newline in quoted string.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_region(),
		    MsgType::Error,
		    "DOTLIB_SCANNER",
		    buf.str());
    throw std::invalid_argument{"Syntax error"};
  }
  if ( c == EOF ) {
    std::ostringstream buf;
    buf << "unexpected end-of-file in quoted string.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_region(),
		    MsgType::Error,
		    "DOTLIB_SCANNER",
		    buf.str());
    throw std::invalid_argument{"Syntax error"};
  }
  if ( c == '\\' ) {
    c = get();
    if ( c == '\n' ) {
      // エスケープされた改行は空白に置き換える．
      c = ' ';
    }
  }
  _put_char(c);
  goto ST_DQ;

 ST_comment1: // '/' を読み込んだ直後
  c = peek();
  if ( c == '/' ) { // C++ スタイルのコメント
    accept();
    goto ST_comment2;
  }
  if ( c == '*' ) { // C スタイルのコメント
    accept();
    goto ST_comment3;
  }
  return TokenType::DIV;

 ST_comment2: // 改行まで読み飛ばす．
  c = get();
  if ( c == '\n' ) {
    goto ST_INIT;
  }
  if ( c == EOF ) {
    return TokenType::END;
  }
  goto ST_comment2;

 ST_comment3: // "/*" を読み込んだ直後
  c = get();
  if ( c == EOF ) {
    goto ST_comment_EOF;
  }
  if ( c == '*' ) {
    goto ST_comment4;
  }
  goto ST_comment3;

 ST_comment4: // "/* 〜 *" まで読み込んだ直後
  c = get();
  if ( c == EOF ) {
    goto ST_comment_EOF;
  }
  if ( c == '/' ) {
    goto ST_INIT;
  }
  if ( c == '*' ) {
    goto ST_comment4;
  }
  goto ST_comment3;

 ST_comment_EOF:
  {
    std::ostringstream buf;
    buf << "Unexpected end-of-file in comment block.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_region(),
		    MsgType::Error,
		    "DOTLIB_SCANNER",
		    buf.str());
  }
  throw std::invalid_argument{"Syntax error"};
  return TokenType::ERROR;
}


//////////////////////////////////////////////////////////////////////
// クラス Token
//////////////////////////////////////////////////////////////////////

// @brief トークンを表す文字列を返す．
std::string
Token::str_value() const
{
  switch ( type() ) {
  case TokenType::COLON:  return ":";
  case TokenType::SEMI:   return ";";
  case TokenType::COMMA:  return ",";
  case TokenType::PLUS:   return "+";
  case TokenType::MINUS:  return "-";
  case TokenType::MULT:   return "*";
  case TokenType::DIV:    return "/";
  case TokenType::NOT:    return "~";
  case TokenType::AND:    return "&";
  case TokenType::OR:     return "|";
  case TokenType::XOR:    return "^";
  case TokenType::LP:     return "(";
  case TokenType::RP:     return ")";
  case TokenType::LCB:    return "{";
  case TokenType::RCB:    return "*";
  case TokenType::PRIME:  return "'";
  case TokenType::SYMBOL: return value();
  case TokenType::BOOL_0: return "0";
  case TokenType::BOOL_1: return "1";
  default: break;
  }
  return std::string();
}

END_NAMESPACE_YM_DOTLIB
