
/// @file Iscas89Parser.cc
/// @brief Iscas89Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "Iscas89Parser.h"
#include "Iscas89Scanner.h"
#include "ym/BnModel.h"
#include "ModelImpl.h"
#include "ym/Expr.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラス BnModel
//////////////////////////////////////////////////////////////////////

// @brief iscas89(.bench) ファイルの読み込みを行う．
BnModel
BnModel::read_iscas89(
  const std::string& filename
)
{
  BnModel model;

  Iscas89Parser parser(model._model_impl());
  if ( !parser.read(filename) ) {
    std::ostringstream buf;
    buf << "BnModel::read_iscas89(\"" << filename << "\") failed.";
    throw std::invalid_argument{buf.str()};
  }

  return model;
}


//////////////////////////////////////////////////////////////////////
// Iscas89Parser
//////////////////////////////////////////////////////////////////////

// @brief デストラクタ
Iscas89Parser::~Iscas89Parser()
{
#if 0
  for ( auto handler: mHandlerList ) {
    delete handler;
  }
#endif
}

// 読み込みを行なう．
//
// 文法：
//
// file   = { line } ;
// line   = input | output | buff | not | and | nand | or | nor | xor | xnor | dff ;
// input  = 'INPUT' '(' NAME ')'
// output = 'OUTPUT' '(' NAME ')'
// const0 = NAME '=' 'CONST0' '(' ')'  // ym-bnet オリジナル
// const1 = NAME '=' 'CONST1' '(' ')'  // ym-bnet オリジナル
// buff   = NAME '=' 'BUFF' '(' NAME ')'
// not    = NAME '=' 'NOT' '(' NAME ')'
// and    = NAME '=' 'AND' '(' NAME ',' NAME { ',' NAME } ')'
// nand   = NAME '=' 'NAND' '(' NAME ',' NAME { ',' NAME } ')'
// or     = NAME '=' 'OR' '(' NAME ',' NAME { ',' NAME } ')'
// nor    = NAME '=' 'NOR' '(' NAME ',' NAME { ',' NAME } ')'
// xor    = NAME '=' 'XOR' '(' NAME ',' NAME { ',' NAME } ')'
// xnor   = NAME '=' 'XNOR' '(' NAME ',' NAME { ',' NAME } ')'
// mux    = NAME '=' 'MUX' '(' NAME, NAME, NAME ')' // host2015 オリジナル．3入力決め打ち？
// dff    = NAME '=' 'DFF' '(' NAME ')'
//
bool
Iscas89Parser::read(
  const std::string& filename
)
{
  // ファイルをオープンする．
  std::ifstream fin{filename};
  if ( !fin ) {
    // エラー
    std::ostringstream buf;
    buf << filename << " : No such file.";
    MsgMgr::put_msg(__FILE__, __LINE__, FileRegion(),
		    MsgType::Failure, "ISCAS89_PARSER", buf.str());
    return false;
  }

  Iscas89Scanner scanner(fin, {filename});
  mScanner = &scanner;

  // パーサー本体
  bool go_on = true;
  bool has_error = false;
  while ( go_on ) {
    SizeType name_id;
    auto token = read_token(name_id);
    auto first_loc = token.loc();
    switch ( token.type() ) {
    case Iscas89Token::INPUT:
      if ( !read_input(first_loc) ) {
	goto error;
      }
      break;

    case Iscas89Token::OUTPUT:
      if ( !read_output(first_loc) ) {
	goto error;
      }
      break;

    case Iscas89Token::NAME:
      if ( !read_gate(first_loc, name_id) ) {
	goto error;
      }
      break;

    case Iscas89Token::_EOF:
      go_on = false;
      break;

    default:
      goto error;
    }
    continue;

  error:
    has_error = true;
    // ')' まで読み進める．
    for ( ; ; ) {
      auto token = mScanner->read_token();
      if ( token.type() == Iscas89Token::RPAR ||
	   token.type() == Iscas89Token::_EOF ) {
	break;
      }
    }
  }

  {
    for ( auto& p: mRefLocDict ) {
      auto id = p.first;
      if ( !is_defined(id) ) {
	std::ostringstream buf;
	buf << id2str(id) << ": Undefined.";
	MsgMgr::put_msg(__FILE__, __LINE__, ref_loc(id),
			MsgType::Error,
			"UNDEF01", buf.str().c_str());
	return false;
      }
    }
  }

  mModel.make_logic_list();

  return !has_error;
}

// @brief INPUT 文を読み込む．
bool
Iscas89Parser::read_input(
  const FileRegion& first_loc
)
{
  SizeType name_id;
  FileRegion last_loc;
  if ( !parse_name(name_id, last_loc) ) {
    return false;
  }
  FileRegion loc{first_loc, last_loc};
  auto name = id2str(name_id);
  // 二重定義のチェック
  if ( is_defined(name_id) ) {
    auto loc2 = def_loc(name_id);
    std::ostringstream buf;
    buf << name << ": Defined more than once. Previous definition is at "
	<< loc2;
    MsgMgr::put_msg(__FILE__, __LINE__, loc,
		    MsgType::Error,
		    "ER_MLTDEF01",
		    buf.str());
    return false;
  }

  set_defined(name_id, loc);
  auto iid = mModel.input_num();
  mModel.set_input(name_id);
  mModel.set_input_name(iid, name);

  return true;
}

// @brief OUTPUT 文を読み込む．
bool
Iscas89Parser::read_output(
  const FileRegion& first_loc
)
{
  SizeType name_id;
  FileRegion last_loc;
  if ( !parse_name(name_id, last_loc) ) {
    return false;
  }
  FileRegion loc{first_loc, last_loc};
  auto oid = mModel.output_num();
  mModel.new_output(name_id);
  auto name = id2str(name_id);
  mModel.set_output_name(oid, name);

  return true;
}

// @brief ゲート/DFF 文を読み込む．
bool
Iscas89Parser::read_gate(
  const FileRegion& first_loc,
  SizeType name_id
)
{
  bool ok;
  std::tie(ok, std::ignore, std::ignore) = expect(Iscas89Token::EQ);
  if ( !ok ) {
    return false;
  }

  // 二重定義のチェック
  if ( is_defined(name_id) ) {
    auto name = id2str(name_id);
    auto loc2 = def_loc(name_id);
    std::ostringstream buf;
    buf << name << ": Defined more than once. "
	<< "Previsous Definition is at " << loc2;
    MsgMgr::put_msg(__FILE__, __LINE__, first_loc,
		    MsgType::Error,
		    "ER_MLTDEF01",
		    buf.str());
    return false;
  }

  auto gate_token = mScanner->read_token();
  if ( gate_token.type() == Iscas89Token::GATE ) {
    std::vector<SizeType> iname_id_list;
    FileRegion last_loc;
    if ( !parse_name_list(iname_id_list, last_loc) ) {
      return false;
    }
    FileRegion loc{first_loc, last_loc};
    set_gate(name_id, loc, gate_token.gate_type(), iname_id_list);
    auto name = id2str(name_id);
    mModel.set_node_name(name_id, name);
    return true;
  }
  if ( gate_token.type() == Iscas89Token::DFF ) {
    SizeType iname_id;
    FileRegion last_loc;
    if ( !parse_name(iname_id, last_loc) ) {
      return false;
    }
    FileRegion loc{first_loc, last_loc};
    set_defined(name_id, loc);
    auto name = id2str(name_id);
    auto dff_id = mModel.new_dff(name);
    mModel.set_dff_output(name_id, dff_id);
    mModel.set_dff_src(dff_id, iname_id);
    mModel.set_node_name(name_id, name);
    return true;
  }
#if 0
  if ( gate_token.type() == Iscas89Token::EXGATE ) {
    auto handler = get_handler(gate_token.ex_id());
    return handler->_read(this, first_loc, name_id);
    return false;
  }
#endif
  return false;
}

// @brief '(' ')' で囲まれた名前を読み込む．
bool
Iscas89Parser::parse_name(
  SizeType& name_id,
  FileRegion& last_loc
)
{
  bool ok;
  std::tie(ok, std::ignore, std::ignore) = expect(Iscas89Token::LPAR);
  if ( !ok ) {
    return false;
  }

  tie(ok, name_id, std::ignore) = expect(Iscas89Token::NAME);
  if ( !ok ) {
    return false;
  }

  tie(ok, std::ignore, last_loc) = expect(Iscas89Token::RPAR);
  if ( !ok ) {
    return false;
  }

  return true;
}

// @brief '(' ')' で囲まれた名前のリストを読み込む．
bool
Iscas89Parser::parse_name_list(
  std::vector<SizeType>& name_id_list,
  FileRegion& last_loc
)
{
  name_id_list.clear();

  bool ok;
  tie(ok, std::ignore, std::ignore) = expect(Iscas89Token::LPAR);
  if ( !ok ) {
    // '(' を期待していたシンタックスエラー
    return false;
  }

  for ( ; ; ) {
    SizeType name_id;
    tie(ok, name_id, std::ignore) = expect(Iscas89Token::NAME);
    if ( !ok ) {
      // NAME を期待したシンタックスエラー
      return false;
    }
    name_id_list.push_back(name_id);

    auto token = mScanner->read_token();
    if ( token.type() == Iscas89Token::RPAR ) {
      last_loc = token.loc();
      break;
    }
    if ( token.type() != Iscas89Token::COMMA ) {
      // ')' か ',' を期待していたシンタックスエラー
      std::ostringstream buf;
      buf << "Syntax error: ')' or ',' are expected.";
      MsgMgr::put_msg(__FILE__, __LINE__, token.loc(),
		      MsgType::Error,
		      "ER_SYNTAX03",
		      buf.str());
      return false;
    }
  }
  return true;
}

BEGIN_NONAMESPACE

const char*
token_str(
  Iscas89Token::Type token
)
{
  switch (token) {
  case Iscas89Token::LPAR:   return "(";
  case Iscas89Token::RPAR:   return ")";
  case Iscas89Token::EQ:     return "=";
  case Iscas89Token::COMMA:  return ",";
  case Iscas89Token::INPUT:  return "INPUT";
  case Iscas89Token::OUTPUT: return "OUTPUT";
  case Iscas89Token::GATE:   return "GATE";
  case Iscas89Token::EXGATE: return "EXGATE";
  case Iscas89Token::DFF:    return "DFF";
  case Iscas89Token::NAME:   return "__name__";
  case Iscas89Token::_EOF:   return "__eof__";
  case Iscas89Token::ERROR:  return "__error__";
  }
  ASSERT_NOT_REACHED;
  return "";
}

END_NONAMESPACE

// @brief 次のトークンが期待されている型か調べる．
std::tuple<bool, SizeType, FileRegion>
Iscas89Parser::expect(
  Iscas89Token::Type exp_type
)
{
  SizeType name_id;
  auto token = read_token(name_id);
  if ( token.type() != exp_type ) {
    // トークンが期待値と異なっていた
    std::ostringstream buf;
    buf << "Syntax error: '" << token_str(exp_type) << "' is expected.";
    MsgMgr::put_msg(__FILE__, __LINE__, token.loc(),
		    MsgType::Error,
		    "ER_SYNTAX01",
		    buf.str());
    return std::make_tuple(false, 0, token.loc());
  }

  return std::make_tuple(true, name_id, token.loc());
}

// @brief yylex() 用の処理を行う．
Iscas89Token
Iscas89Parser::read_token(
  SizeType& name_id
)
{
  auto token = mScanner->read_token();
  if ( token.type() == Iscas89Token::NAME ) {
    auto name = token.name();
    name_id = find_id(name, token.loc());
  }
  return token;
}

END_NAMESPACE_YM_BN
