
/// @file AigParser.cc
/// @brief AigParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "AigParser.h"
#include "AigMgrImpl.h"
#include "ym/AigMgr.h"
#include "ym/MsgMgr.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_AIG

BEGIN_NONAMESPACE

bool debug = false;

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス AigMgr
//////////////////////////////////////////////////////////////////////

// @brief aag ファイルの読み込みを行う．
AigMgr
AigMgr::read_aag(
  const std::string& filename
)
{
  AigMgr mgr;
  AigParser parser{mgr.get()};
  if ( !parser.read_aag(filename) ) {
    std::ostringstream buf;
    buf << "AigMgr::read_aag(\"" << filename << "\") failed.";
    throw std::invalid_argument{buf.str()};
  }

  return mgr;
}

// @brief aig ファイルの読み込みを行う．
AigMgr
AigMgr::read_aig(
  const std::string& filename
)
{
  AigMgr mgr;
  AigParser parser{mgr.get()};
  if ( !parser.read_aig(filename) ) {
    std::ostringstream buf;
    buf << "AigMgr::read_aig(\"" << filename << "\") failed.";
    throw std::invalid_argument{buf.str()};
  }

  return mgr;
}


//////////////////////////////////////////////////////////////////////
// クラス AigParser
//////////////////////////////////////////////////////////////////////

// @brief Ascii AIG フォーマットを読み込む．
bool
AigParser::read_aag(
  const std::string& filename
)
{
  if ( !open(filename) ) {
    return true;
  }

  // ヘッダ行の読み込み
  SizeType M, I, L, O, A;
  if ( !read_aag_header(M, I, L, O, A) ) {
    return false;
  }

  initialize(M, I, O, L);

  // 入力行の読み込み
  for ( SizeType i = 0; i < I; ++ i ) {
    if ( !read_input(i) ) {
      return false;
    }
  }

  // ラッチ行の読み込み
  for ( SizeType i = 0; i < L; ++ i ) {
    if ( !read_latch(i) ) {
      return false;
    }
  }

  // 出力行の読み込み
  for ( SizeType i = 0; i < O; ++ i ) {
    SizeType src_lit;
    if ( !read_src(src_lit) ) {
      return false;
    }
    if ( debug ) {
      std::cout << "O#" << i << ": " << src_lit
		<< std::endl;
    }
    auto edge = conv_to_edge(src_lit);
    mMgr->_set_output(i, edge);
  }

  // AND行の読み込み
  for ( SizeType i = 0; i < A; ++ i ) {
    if ( !read_and(i) ) {
      return false;
    }
  }

  std::vector<std::string> err_list;
  if ( !mMgr->_sanity_check(err_list) ) {
    for ( auto& err: err_list ) {
      std::cerr << err << std::endl;
    }
    return false;
  }

  // シンボルテーブルとコメントの読み込みを行う．
  read_symbols();

  mMgr->_finalize();

  return true;
}

BEGIN_NONAMESPACE

END_NONAMESPACE

// @brief AIG フォーマットを読み込む．
bool
AigParser::read_aig(
  const std::string& filename
)
{
  if ( !open(filename) ) {
    return true;
  }

  // ヘッダ行の読み込み
  SizeType M, I, L, O, A;
  if ( !read_aig_header(M, I, L, O, A) ) {
    return false;
  }

  if ( debug ) {
    std::cout << "aig " << M
	      << " " << I
	      << " " << L
	      << " " << O
	      << " " << A
	      << std::endl;
  }

  initialize(M, I, O, L);

  // 入力の設定
  for ( SizeType input_id = 0; input_id < I; ++ input_id ) {
    auto id = input_id;
    mMgr->_set_input(id, input_id);
  }

  // ラッチ行の読み込み
  std::vector<SizeType> latch_src_list(L);
  for ( SizeType dff_id = 0; dff_id < L; ++ dff_id ) {
    SizeType src_lit;
    if ( !read_src(src_lit) ) {
      return false;
    }
    auto id = I + dff_id;
    set_dff(dff_id, id, src_lit);
  }

  // 出力行の読み込み
  std::vector<SizeType> output_list(O);
  for ( SizeType oid = 0; oid < O; ++ oid ) {
    SizeType src_lit;
    if ( !read_src(src_lit) ) {
      return false;
    }
    if ( debug ) {
      std::cout << "O#" << oid << ": " << src_lit
		<< std::endl;
    }
    auto edge = conv_to_edge(src_lit);
    mMgr->_set_output(oid, edge);
  }

  // AND行の読み込み
  for ( SizeType i = 0; i < A; ++ i ) {
    SizeType d0 = read_number();
    SizeType d1 = read_number();
    SizeType pos = i + I + L + 1;
    SizeType lhs = pos * 2;
    SizeType rhs0 = lhs - d0;
    SizeType rhs1 = rhs0 - d1;
    if ( debug ) {
      std::cout << "A#" << i << ": "
		<< d0 << " " << d1
		<< " -> " << rhs0 << " " << rhs1
		<< std::endl;
    }
    auto id = I + L + i;
    auto node = mMgr->node(id);
    auto edge0 = conv_to_edge(rhs0);
    auto edge1 = conv_to_edge(rhs1);
    mMgr->_set_and(node, edge0, edge1);
  }

  // シンボルの読み込み
  read_symbols();

  mMgr->_finalize();

  return true;
}

// @brief aag ヘッダ行を読み込む．
bool
AigParser::read_aag_header(
  SizeType& M,
  SizeType& I,
  SizeType& L,
  SizeType& O,
  SizeType& A
)
{
  std::string linebuf;
  if ( !read_line(linebuf) ) {
    return false;
  }
  if ( linebuf.substr(0, 3) != std::string{"aag"} ) {
    std::ostringstream buf;
    buf << linebuf << ": Illegal header signature, 'aag' expected.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    FileRegion{mFileInfo, mLineNo, 1, mLineNo, mLastCol},
		    MsgType::Error, "AIG_PARSER", buf.str());
    return false;
  }
  std::istringstream tmp{linebuf.substr(4, std::string::npos)};
  tmp >> M >> I >> L >> O >> A;

  if ( debug ) {
    std::cout << "aag " << M
	      << " " << I
	      << " " << L
	      << " " << O
	      << " " << A
	      << std::endl;
  }

  return true;
}

// @brief ファイルを開く
bool
AigParser::open(
  const std::string& filename
)
{
  mS = new std::ifstream{filename};
  if ( !*mS ) {
    std::ostringstream buf;
    buf << filename << ": No such file";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    MsgType::Failure, "AIG_PARSER", buf.str());
    delete mS;
    mS = nullptr;
    return false;
  }
  mFileInfo = FileInfo{filename};
  mLineNo = 0;
  mLastCol = 0;
  return true;
}

// @brief aig ヘッダ行を読み込む．
bool
AigParser::read_aig_header(
  SizeType& M,
  SizeType& I,
  SizeType& L,
  SizeType& O,
  SizeType& A
)
{
  std::string linebuf;
  if ( !read_line(linebuf) ) {
    return false;
  }
  if ( linebuf.substr(0, 3) != std::string{"aig"} ) {
    std::ostringstream buf;
    buf << linebuf << ": Illegal header signature, 'aig' expected.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    FileRegion{mFileInfo, mLineNo, 1, mLineNo, mLastCol},
		    MsgType::Error, "AIG_PARSER", buf.str());
    return false;
  }
  std::istringstream tmp{linebuf.substr(4, std::string::npos)};
  tmp >> M >> I >> L >> O >> A;

  if ( M != (I + L + A) ) {
    std::ostringstream buf;
    buf << linebuf << ": wrong parameters";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    FileRegion{mFileInfo, mLineNo, 1, mLineNo, mLastCol},
		    MsgType::Error, "AIG_PARSER", buf.str());
    return false;
  }

  if ( debug ) {
    std::cout << "aag " << M
	      << " " << I
	      << " " << L
	      << " " << O
	      << " " << A
	      << std::endl;
  }

  return true;
}

// @brief 内容を初期化する．
void
AigParser::initialize(
  SizeType M,
  SizeType I,
  SizeType O,
  SizeType L
)
{
  mMgr->_initialize(M, I, O, L);
  mDefined.clear();
  mDefined.resize(M, false);
}

// @brief aag の入力行の読み込み
bool
AigParser::read_input(
  SizeType input_id
)
{
  std::string linebuf;
  if ( !read_line(linebuf) ) {
    return false;
  }

  std::istringstream tmp{linebuf};
  SizeType lit;
  tmp >> lit;

  SizeType id;
  if ( !set_defined(lit, id) ) {
    return false;
  }
  if ( debug ) {
    std::cout << "I#" << input_id << ": " << lit
	      << "(Node#" << id << ")"
	      << std::endl;
  }
  mMgr->_set_input(id, input_id);
  return true;
}

// @brief aag のラッチ行の読み込み
bool
AigParser::read_latch(
  SizeType dff_id
)
{
  std::string linebuf;
  if ( !read_line(linebuf) ) {
    return false;
  }

  std::istringstream tmp{linebuf};
  SizeType lit, src_lit;
  tmp >> lit >> src_lit;

  SizeType id;
  if ( !set_defined(lit, id) ) {
    return false;
  }
  set_dff(dff_id, id, src_lit);
  return true;
}

// @brief aag の出力行の読み込み
bool
AigParser::read_src(
  SizeType& src
)
{
  std::string linebuf;
  if ( !read_line(linebuf) ) {
    return false;
  }

  std::istringstream tmp{linebuf};
  tmp >> src;
  return true;
}

// @brief aag のAND行の読み込み
bool
AigParser::read_and(
  SizeType and_id
)
{
  std::string linebuf;
  if ( !read_line(linebuf) ) {
    return false;
  }

  std::istringstream tmp{linebuf};
  SizeType lit;
  SizeType src0;
  SizeType src1;
  tmp >> lit >> src0 >> src1;

  SizeType id;
  if ( !set_defined(lit, id) ) {
    return false;
  }
  if ( debug ) {
    std::cout << "A#" << and_id << ": " << src0 << " " << src1
	      << std::endl;
  }
  auto edge0 = conv_to_edge(src0);
  auto edge1 = conv_to_edge(src1);
  auto node = mMgr->node(id);
  mMgr->_set_and(node, edge0, edge1);
  return true;
}

// @brief 数字を一つ読み出す．
SizeType
AigParser::read_number()
{
  SizeType num = 0;
  for ( SizeType weight = 1; ; weight *= 128 ) {
    SizeType c = mS->get();
    num += (c & 127) * weight;
    if ( (c & 128) == 0 ) {
      break;
    }
  }
  return num;
}

// @brief シンボルテーブルとコメントを読み込む．
void
AigParser::read_symbols()
{
  std::string linebuf;
  std::string comment;
  bool symbol_mode = true;
  while ( std::getline(*mS, linebuf) ) {
    if ( symbol_mode ) {
      if ( linebuf == "c" ) {
	symbol_mode = false;
      }
      else {
	auto p = linebuf.find_first_of(" ");
	auto pos_str = linebuf.substr(0, p);
	auto name = linebuf.substr(p + 1, std::string::npos);
	auto pos = atoi(pos_str.substr(1, std::string::npos).c_str());
	std::ostringstream buf;
	buf << pos;
	char prefix = pos_str[0];
	if ( prefix == 'i' ) {
	  mMgr->set_input_name(pos, name);
	}
	else if ( prefix == 'o' ) {
	  mMgr->set_output_name(pos, name);
	}
	else if ( prefix == 'l' ) {
	  mMgr->set_dff_name(pos, name);
	}
	else {
	  std::ostringstream buf;
	  buf << prefix << ": Illegal prefix";
	  throw std::invalid_argument{buf.str()};
	}
      }
    }
    else {
      // コメント行
      mMgr->add_comment(linebuf);
    }
  }
}

// @brief 1行読み出す．
bool
AigParser::read_line(
  std::string& buf
)
{
  ++ mLineNo;
  if ( !getline(*mS, buf) ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    FileRegion{mFileInfo, mLineNo, 1, mLineNo, 1},
		    MsgType::Error, "AIG_PARSER", "Unexpected EOF");
    return false;
  }
  mLastCol = buf.size();
  return true;
}

// @brief リテラルを定義済みにする．
bool
AigParser:: set_defined(
  SizeType lit,
  SizeType& id
)
{
  if ( (lit % 2) == 1 ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    FileRegion{mFileInfo, mLineNo, 1, mLineNo, mLastCol},
		    MsgType::Error, "AIG_PARSER",
		    "Positive Literal(even number) expected");
    return false;
  }
  if ( lit <= 1 ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    FileRegion{mFileInfo, mLineNo, 1, mLineNo, mLastCol},
		    MsgType::Error, "AIG_PARSER",
		    "Unexpected constant literal");
    return false;
  }

  id = lit_to_id(lit);
  if ( mDefined[id] ) {
    std::ostringstream buf;
    buf << id << " is already defined.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    FileRegion{mFileInfo, mLineNo, 1, mLineNo, mLastCol},
		    MsgType::Error, "AIG_PARSER", buf.str());
    return false;
  }
  mDefined[id] = true;
  return true;
}

// @brief リテラルが定義済みか調べる．
bool
AigParser::check_defined(
  SizeType lit,
  const std::string& ref
)
{
  if ( lit <= 1 ) {
    // 定数リテラルは常に定義されている．
    return true;
  }
  auto id = lit_to_id(lit);
  if ( !mDefined[id] ) {
    std::ostringstream buf;
    buf << lit << " is not defined required by " << ref << ".";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    FileRegion{mFileInfo, mLineNo, 1, mLineNo, mLastCol},
		    MsgType::Error, "AIG_PARSER", buf.str());
    return false;
  }
  return true;
}

// @brief DFF の設定を行う．
void
AigParser::set_dff(
  SizeType dff_id,
  SizeType oid,
  SizeType src_lit
)
{
  if ( debug ) {
    std::cout << "L#" << dff_id << ": Node#" << oid << " " << src_lit
	      << std::endl;
  }
  mMgr->_set_dff_output(oid, dff_id);

  auto input_edge = conv_to_edge(src_lit);
  mMgr->_set_dff_input(dff_id, input_edge);
}

// @brief リテラルを枝に変換する
AigEdge
AigParser::conv_to_edge(
  SizeType lit
)
{
  if ( lit == 0 ) {
    return AigEdge::zero();
  }
  if ( lit == 1 ) {
    return AigEdge::one();
  }
  auto id = lit_to_id(lit);
  auto inv = lit_to_inv(lit);
  auto node = mMgr->node(id);
  return AigEdge(node, inv);
}

END_NAMESPACE_YM_AIG
