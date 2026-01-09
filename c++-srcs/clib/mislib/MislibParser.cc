
/// @file MislibParser.cc
/// @brief MislibParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2019, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "MislibParser.h"
#include "MislibScanner.h"
#include "MislibExpr.h"
#include "MislibGate.h"
#include "MislibNode.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_MISLIB

BEGIN_NONAMESPACE

// MislibScanner::read_token() をデバッグする時に true にする．
bool debug_read_token = false;

END_NONAMESPACE

BEGIN_NONAMESPACE

// 論理式中に現れる名前を ipin_set に積む．
void
get_ipin_names(
  const MislibExpr* expr_node,
  std::unordered_set<ShString>& ipin_set
)
{
  ShString name;

  switch ( expr_node->type() ) {
  case MislibExpr::Const0:
  case MislibExpr::Const1:
    // 定数は無視
    return;

  case MislibExpr::VarName:
    if ( ipin_set.count(expr_node->varname()) == 0 ) {
      ipin_set.insert(expr_node->varname());
    }
    break;

  case MislibExpr::Not:
    get_ipin_names(expr_node->opr1(), ipin_set);
    break;

  case MislibExpr::And:
  case MislibExpr::Or:
  case MislibExpr::Xor:
    get_ipin_names(expr_node->opr1(), ipin_set);
    get_ipin_names(expr_node->opr2(), ipin_set);
    break;

  default:
    ASSERT_NOT_REACHED;
  }
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス MislibParser
//////////////////////////////////////////////////////////////////////

// @brief ゲートを読み込む．
std::vector<MislibGatePtr>
MislibParser::parse(
  const std::string& filename
)
{
  std::ifstream fin{filename};
  if ( !fin ) {
    // ファイルが読み込めなかった．
    std::ostringstream buf;
    buf << filename << " : No such file.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    FileRegion(),
		    MsgType::Failure,
		    "MISLIB_PARSER",
		    buf.str());
    throw std::invalid_argument{buf.str()};
  }

  MislibScanner scanner(fin, {filename});
  mScanner = &scanner;

  std::vector<MislibGatePtr> gate_list;
  for ( ; ; ) {
    MislibToken tok = scan();
    auto type = tok.type();
    if ( type == MislibToken::END ) {
      break;
    }

    if ( type != MislibToken::GATE ) {
      // シンタックスエラー
      syntax_error(tok.loc(), "'gate' expected.");
    }

    FileRegion loc0 = tok.loc();

    // 次は STR
    auto name = read_str();

    // 次は NUM
    auto area = read_num();

    // 次は STR
    auto opin = read_str();

    // 次は EQ
    expect_token(MislibToken::EQ);

    // 次は式
    auto expr = read_expr(MislibToken::SEMI);

    // 次は SEMI
    expect_token(MislibToken::SEMI);

    // 次はピンリスト
    auto ipin_list = read_pin_list();

    // 末尾のノードを位置を loc1 に設定する．
    FileRegion loc1;
    if ( ipin_list.size() > 0 ) {
      loc1 = ipin_list.back()->loc();
    }

    auto gate = new_gate(FileRegion(loc0, loc1),
			 std::move(name), std::move(area),
			 std::move(opin), std::move(expr),
			 std::move(ipin_list));
    gate_list.push_back(std::move(gate));
  }

  // 内容が正しいかチェックする．
  check_gate_list(gate_list);

  return gate_list;
}

// @brief 読み込んだゲートリストが正しいかチェックする．
void
MislibParser::check_gate_list(
  const std::vector<MislibGatePtr>& gate_list
)
{
  int prev_errnum = MsgMgr::error_num();

  // 重複したセル名がないかチェック
  // また，セル内のピン名が重複していないか，出力ピンの論理式に現れるピン名
  // と入力ピンに齟齬がないかもチェックする．
  std::unordered_map<ShString, const MislibGate*> cell_map;
  for ( auto& gate: gate_list ) {
    ShString name = gate->name()->str();
    if ( cell_map.count(name) > 0 ) {
      auto node = cell_map.at(name);
      std::ostringstream buf;
      buf << "Clib name, " << name << " is defined more than once. "
	  << "Previous definition is " << node->name()->loc() << ".";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      gate->name()->loc(),
		      MsgType::Error,
		      "MISLIB_PARSER",
		      buf.str());
      // このセルについては以降のチェックをスキップする．
      continue;
    }
    // 情報を登録する．
    cell_map.emplace(name, gate.get());

    // 入力ピン名のチェック
    std::unordered_map<ShString, const MislibPin*> ipin_map;
    int npin = gate->ipin_num();
    for ( int i = 0; i < npin; ++ i ) {
      auto ipin = gate->ipin(i);
      ShString name = ipin->name()->str();
      if ( ipin_map.count(name) > 0 ) {
	auto node = ipin_map.at(name);
	std::ostringstream buf;
	buf << "Pin name, " << name << " is defined more than once. "
	    << "Previous definition is "
	    << node->name()->loc() << ".";
	MsgMgr::put_msg(__FILE__, __LINE__,
			ipin->name()->loc(),
			MsgType::Error,
			"MISLIB_PARSER",
			buf.str());
      }
      else {
	ipin_map.emplace(name, ipin);
      }
    }

    // 論理式に現れる名前の集合を求める．
    std::unordered_set<ShString> ipin_set;
    get_ipin_names(gate->opin_expr(), ipin_set);
    for ( const auto p: ipin_map ) {
      ShString name = p.first;
      if ( ipin_set.count(name) == 0 ) {
	// ピン定義に現れる名前が論理式中に現れない．
	// エラーではないが，このピンのタイミング情報は意味をもたない．
	auto node = ipin_map.at(name);
	std::ostringstream buf;
	buf << "Input pin, " << name
	    << " does not appear in the logic expression. "
	    << "Timing information will be ignored.";
	MsgMgr::put_msg(__FILE__, __LINE__,
			node->loc(),
			MsgType::Warning,
			"MISLIB_PARSER",
			buf.str());
      }
    }
    for ( auto name: ipin_set ) {
      if ( ipin_map.count(name) == 0 ) {
	// 論理式中に現れる名前の入力ピンが存在しない．
	// これはエラー
	std::ostringstream buf;
	buf << name << " appears in the logic expression, "
	    << "but is not defined in PIN statement.";
	MsgMgr::put_msg(__FILE__, __LINE__,
			gate->opin_expr()->loc(),
			MsgType::Error,
			"MISLIB_PARSER",
			buf.str());
      }
    }
  }

  if ( MsgMgr::error_num() > prev_errnum ) {
    // 異常終了
    throw std::invalid_argument{"Error in read_mislib()"};
  }
}

// @brief 文字列を読み込む．
MislibStrPtr
MislibParser::read_str()
{
  auto tok = scan();
  if ( tok.type() != MislibToken::STR ) {
    // シンタックスエラー
    syntax_error(tok.loc(), "a string value expected.");
  }
  auto node = new_str(tok.loc());
  return node;
}

// @brief 数値を読み込む．
MislibNumPtr
MislibParser::read_num()
{
  auto tok = scan();
  if ( tok.type() != MislibToken::NUM ) {
    // シンタックスエラー
    syntax_error(tok.loc(), "a number expected.");
  }
  auto node = new_num(tok.loc());
  return node;
}

void
MislibParser::expect_token(
  MislibToken::Type exp_type
)
{
  auto tok = scan();
  if ( tok.type() != exp_type ) {
    std::ostringstream buf;
    buf	<< exp_type << " is expected.";
    syntax_error(tok.loc(), buf.str());
  }
}

// @brief 式を読み込む．
MislibExprPtr
MislibParser::read_expr(
  MislibToken::Type end_type
)
{
  auto expr1 = read_product();
  for ( ; ; ) {
    auto tok = peek();
    auto type = tok.type();
    if ( type == end_type ) {
      return expr1;
    }
    if ( type != MislibToken::PLUS && type != MislibToken::HAT ) {
      // シンタックスエラー
      syntax_error(tok.loc(), "'+' or '^' is expected.");
    }
    skip_token();

    auto expr2 = read_product();
    if ( type == MislibToken::PLUS ) {
      expr1 = new_or(FileRegion(expr1->loc(), expr2->loc()),
		     std::move(expr1),
		     std::move(expr2));
    }
    else {
      expr1 = new_xor(FileRegion(expr1->loc(), expr2->loc()),
		      std::move(expr1),
		      std::move(expr2));
    }
  }
}

// @brief 積項を読み込む．
MislibExprPtr
MislibParser::read_product()
{
  auto expr1 = read_literal();
  while ( peek().type() == MislibToken::STAR ) {
    skip_token();

    auto expr2 = read_literal();
    expr1 = new_and(FileRegion(expr1->loc(), expr2->loc()),
		    std::move(expr1),
		    std::move(expr2));
  }
  return expr1;
}

// @brief リテラルを読み込む．
MislibExprPtr
MislibParser::read_literal()
{
  auto tok = scan();
  switch ( tok.type() ) {
  case MislibToken::STR:
    return new_varname(tok.loc(), ShString(mScanner->cur_string()));

  case MislibToken::CONST0:
    return new_const0(tok.loc());

  case MislibToken::CONST1:
    return new_const1(tok.loc());

  case MislibToken::LP:
    {
      auto expr = read_expr(MislibToken::RP);
      tok = scan();
      ASSERT_COND( tok.type() == MislibToken::RP );
      return expr;
    }

  case MislibToken::NOT:
    {
      auto expr = read_literal();
      return new_not(FileRegion(tok.loc(), expr->loc()),
		     std::move(expr));
    }

  default:
    // シンタックスエラー
    {
      std::ostringstream buf;
      buf << "wrong token(" << tok.type() << ")";
      syntax_error(tok.loc(), buf.str());
    }
  }

  return nullptr;
}

// @brief ピンリスト記述を読み込む．
std::vector<MislibPinPtr>
MislibParser::read_pin_list()
{
  std::vector<MislibPinPtr> pin_list;
  for ( ; ; ) {
    // 最初は PIN
    auto tok = peek();
    if ( tok.type() != MislibToken::PIN ) {
      // 終わる．
      break;
    }

    // 'pin' の位置
    FileRegion pin_loc = tok.loc();
    // peek したトークンを読み込む．
    skip_token();

    // 次は STR か STAR
    tok = scan();
    auto type = tok.type();
    MislibStrPtr name;
    if ( type == MislibToken::STR ) {
      name = new_str(tok.loc());
    }
    else if ( type == MislibToken::STAR ) {
      // name は nullptr のまま
    }
    else {
      // シンタックスエラー
      syntax_error(tok.loc(), "string or '*' is expected.");
    }

    // 次は NONINV/INV/UNKNOWN のいずれか
    tok = scan();
    MislibPhasePtr phase;
    switch ( tok.type() ) {
    case MislibToken::NONINV:  phase = new_noninv(tok.loc()); break;
    case MislibToken::INV:     phase = new_inv(tok.loc()); break;
    case MislibToken::UNKNOWN: phase = new_unknown(tok.loc()); break;
    default:
      syntax_error(tok.loc(), "only 'NONINV'/'INV'/'UNKNOWN' is accepted.");
    }

    // あとは 6 個の NUM
    MislibNumPtr val[6];
    for ( int i = 0; i < 6; ++ i ) {
      auto node = read_num();
      val[i] = std::move(node);
    }

    auto pin = new_pin(FileRegion(pin_loc, val[5]->loc()),
		       std::move(name),  std::move(phase),
		       std::move(val[0]), std::move(val[1]),
		       std::move(val[2]), std::move(val[3]),
		       std::move(val[4]), std::move(val[5]));
    pin_list.push_back(std::move(pin));
  }

  // 名前が nullptr (STAR) のピンがある場合はそれが唯一の要素である場合に限る．
  bool has_star = false;
  FileRegion star_loc;
  for ( auto& pin: pin_list ) {
    if ( pin->name() == nullptr ) {
      has_star = true;
      star_loc = pin->loc();
    }
  }
  if ( has_star && pin_list.size() > 1 ) {
    // シンタックスエラー
    syntax_error(star_loc, "pin name and '*' are mutually exclusive.");
  }

  return pin_list;
}

// @brief 次のトークンを盗み見る．
MislibToken
MislibParser::peek()
{
  if ( mUngetToken.type() == MislibToken::ERROR ) {
    mUngetToken = mScanner->read_token();
  }
  if ( debug_read_token ) {
    std::cout << "MislibParser::peek(): "
	      << mUngetToken.type()
	      << std::endl;
  }
  return mUngetToken;
}

// @brief peek() で読んだトークンを捨てる．
void
MislibParser::skip_token()
{
  if ( debug_read_token ) {
    std::cout << "MislibParser::skip_token(): "
	      << mUngetToken.type() << " is discarded"
	      << std::endl;
  }
  mUngetToken = {};
}

// @brief 字句解析を行う．
// @return トークンの型を返す．
MislibToken
MislibParser::scan()
{
  MislibToken tok = mUngetToken;
  mUngetToken = {};
  if ( tok.type() == MislibToken::ERROR ) {
    tok = mScanner->read_token();
  }

  if ( debug_read_token ) {
    std::cout << "MislibParser::scan(): ";
    switch ( tok.type() ) {
    case MislibToken::STR:
      std::cout << "STR(" << mScanner->cur_string() << ")";
      break;

    case MislibToken::NUM:
      std::cout << "NUM(" << mScanner->cur_float() << ")";
      break;

    case MislibToken::NONINV:
      std::cout << "NONINV";
      break;

    case MislibToken::INV:
      std::cout << "INV";
      break;

    case MislibToken::UNKNOWN:
      std::cout << "UNKNOWN";
      break;

    case MislibToken::CONST0:
      std::cout << "CONST0";
      break;

    case MislibToken::CONST1:
      std::cout << "CONST1";
      break;

    case MislibToken::LP:
      std::cout << "LP";
      break;

    case MislibToken::RP:
      std::cout << "RP";
      break;

    case MislibToken::SEMI:
      std::cout << "SEMI";
      break;

    case MislibToken::EQ:
      std::cout << "EQ";
      break;

    case MislibToken::GATE:
      std::cout << "GATE";
      break;

    case MislibToken::PIN:
      std::cout << "PIN";
      break;

    case MislibToken::ERROR:
      std::cout << "ERROR";
      break;

    case MislibToken::PLUS:
      std::cout << "PLUS";
      break;

    case MislibToken::HAT:
      std::cout << "HAT";
      break;

    case MislibToken::STAR:
      std::cout << "STAR";
      break;

    case MislibToken::NOT:
      std::cout << "NOT";
      break;

    case MislibToken::END:
      std::cout << "END";
      break;

    default:
      std::cout << tok.type();
      break;
    }
    std::cout << std::endl;
  }
  return tok;
}

// GATE ノードを生成する．
MislibGatePtr
MislibParser::new_gate(
  const FileRegion& loc,
  MislibStrPtr&& name,
  MislibNumPtr&& area,
  MislibStrPtr&& oname,
  MislibExprPtr&& expr,
  std::vector<MislibPinPtr>&& ipin_list)
{
  ASSERT_COND( name != nullptr );
  ASSERT_COND( area != nullptr );
  ASSERT_COND( oname != nullptr );
  ASSERT_COND( expr != nullptr );

  auto node = new MislibGate(loc,
			     std::move(name),
			     std::move(area),
			     std::move(oname),
			     std::move(expr),
			     std::move(ipin_list));
  return MislibGatePtr{node};
}

// PIN ノードを生成する．
MislibPinPtr
MislibParser::new_pin(
  const FileRegion& loc,
  MislibStrPtr&& name,
  MislibPhasePtr&& phase,
  MislibNumPtr&& input_load,
  MislibNumPtr&& max_load,
  MislibNumPtr&& rise_block_delay,
  MislibNumPtr&& rise_fanout_delay,
  MislibNumPtr&& fall_block_delay,
  MislibNumPtr&& fall_fanout_delay
)
{
  auto node = new MislibPin(loc,
			    std::move(name),
			    std::move(phase),
			    std::move(input_load),
			    std::move(max_load),
			    std::move(rise_block_delay),
			    std::move(rise_fanout_delay),
			    std::move(fall_block_delay),
			    std::move(fall_fanout_delay));
  return MislibPinPtr{node};
}

// NONINV ノードを生成する．
MislibPhasePtr
MislibParser::new_noninv(
  const FileRegion& loc
)
{
  auto node = new MislibNoninv(loc);
  return MislibPhasePtr{node};
}

// INV ノードを生成する．
MislibPhasePtr
MislibParser::new_inv(
  const FileRegion& loc
)
{
  auto node = new MislibInv(loc);
  return MislibPhasePtr{node};
}

// UNKNOWN ノードを生成する．
MislibPhasePtr
MislibParser::new_unknown(
  const FileRegion& loc
)
{
  auto node = new MislibUnknown(loc);
  return MislibPhasePtr{node};
}

// 定数0ノードを生成する．
MislibExprPtr
MislibParser::new_const0(
  const FileRegion& loc
)
{
  auto node = new MislibConst0(loc);
  return MislibExprPtr{node};
}

// 定数1ノードを生成する．
MislibExprPtr
MislibParser::new_const1(
  const FileRegion& loc
)
{
  auto node = new MislibConst1(loc);
  return MislibExprPtr{node};
}

// NOT ノードを生成する．
MislibExprPtr
MislibParser::new_not(
  const FileRegion& loc,
  MislibExprPtr&& opr1
)
{
  auto node = new MislibNot(loc, std::move(opr1));
  return MislibExprPtr{node};
}

// AND ノードを生成する．
MislibExprPtr
MislibParser::new_and(
  const FileRegion& loc,
  MislibExprPtr&& opr1,
  MislibExprPtr&& opr2
)
{
  auto node = new MislibAnd(loc,
			    std::move(opr1),
			    std::move(opr2));
  return MislibExprPtr{node};
}

// OR ノードを生成する．
MislibExprPtr
MislibParser::new_or(
  const FileRegion& loc,
  MislibExprPtr&& opr1,
  MislibExprPtr&& opr2
)
{
  auto node = new MislibOr(loc,
			   std::move(opr1),
			   std::move(opr2));
  return MislibExprPtr{node};
}

// XOR ノードを生成する．
MislibExprPtr
MislibParser::new_xor(
  const FileRegion& loc,
  MislibExprPtr&& opr1,
  MislibExprPtr&& opr2
)
{
  auto node = new MislibXor(loc,
			    std::move(opr1),
			    std::move(opr2));
  return MislibExprPtr{node};
}

// @brief 変数ノードを生成する．
MislibExprPtr
MislibParser::new_varname(
  const FileRegion& loc,
  const char* str
)
{
  auto node = new MislibVarName(loc, str);
  return MislibExprPtr{node};
}

// @brief 直前のトークンが文字列の場合に文字列ノードを返す．
MislibStrPtr
MislibParser::new_str(
  const FileRegion& loc
)
{
  auto node = new MislibStr(loc, mScanner->cur_string());
  return MislibStrPtr{node};
}

// @brief 直前のトークンが数字の場合に数字ノードを返す．
MislibNumPtr
MislibParser::new_num(
  const FileRegion& loc
)
{
  auto node = new MislibNum(loc, mScanner->cur_float());
  return MislibNumPtr{node};
}

// @brief エラーメッセージを出力する．
void
MislibParser::syntax_error(
  const FileRegion& loc,
  const std::string& msg
)
{
  auto label = "Syntax error: " + msg;
  MsgMgr::put_msg(__FILE__, __LINE__,
		  loc,
		  MsgType::Error,
		  "MISLIB_PARSER",
		  label);
  throw std::invalid_argument{label};
}

END_NAMESPACE_YM_MISLIB
