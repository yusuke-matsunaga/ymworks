
/// @file FraigMgrImpl.cc
/// @brief FraigMgrImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "FraigMgrImpl.h"
#include "FraigNode.h"
#include "ym/Range.h"
#include "ym/Timer.h"


#if defined(YM_DEBUG)
#define DEBUG_FLAG 1
#endif

#if !defined(DEBUG_FLAG)
#define DEBUG_FLAG 0
#endif


BEGIN_NAMESPACE_YM_FRAIG

BEGIN_NONAMESPACE

const int debug = DEBUG_FLAG;

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// FraigMgrImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FraigMgrImpl::FraigMgrImpl(
  SizeType sig_size,
  const SatInitParam& init_param
) : mSolver{init_param},
    mLogLevel{0},
    mLogStream{new std::ofstream("/dev/null")}
{
  FraigNode::mPatSize = sig_size * 2;
  FraigNode::mPatUsed = sig_size;
}

// @brief デストラクタ
FraigMgrImpl::~FraigMgrImpl()
{
  if ( mLogStream != &std::cout ) {
    delete mLogStream;
  }
}

// @brief 外部入力を作る．
FraigLiteral
FraigMgrImpl::new_input()
{
  if ( debug ) {
    std::cout << "new_input ...";
    std::cout.flush();
  }

  auto id = mAllNodes.size();
  auto iid = mInputNodes.size();
  // 乱数で初期化したパタンを作る．
  auto init_pat = std::vector<std::uint64_t>(FraigNode::mPatUsed);
  auto rd = std::uniform_int_distribution<std::uint64_t>{};
  for ( auto i: Range(FraigNode::mPatUsed) ) {
    init_pat[i] = rd(mRandGen);
  }
  auto node = new FraigNode(id, iid, init_pat);
  reg_node(node);

  mInputNodes.push_back(node);

  auto ans = node2literal(node);

  if ( debug ) {
    std::cout << " -> " << print_literal(ans)
	      << std::endl;
  }

  return ans;
}

// @brief 2つのノードの AND を取る．
FraigLiteral
FraigMgrImpl::new_and(
  FraigLiteral handle1,
  FraigLiteral handle2
)
{
  if ( debug ) {
    std::cout << "new_and("
	      << print_literal(handle1)
	      << ", "
	      << print_literal(handle2)
	      << ") ..."
	      << std::endl;
  }

  FraigLiteral ans;

  // trivial な場合の処理
  if ( handle1.is_zero() || handle2.is_zero() ) {
    ans = FraigLiteral::zero();
  }
  else if ( handle1.is_one() ) {
    ans = handle2;
  }
  else if ( handle2.is_one() ) {
    ans = handle1;
  }
  else if ( handle1 == handle2 ) {
    ans = handle1;
  }
  else if ( handle1.node_id() == handle2.node_id() ) {
    // handle1.inv != handle2.inv() のはず
    ans = FraigLiteral::zero();
  }
  else {
    // 順番の正規化
    if ( handle1.node_id() > handle2.node_id() ) {
      std::swap(handle1, handle2);
    }

    if ( debug ) {
      std::cout << "  after normalize: "
		<< print_literal(handle1)
		<< ", "
		<< print_literal(handle2)
		<< std::endl;
    }

    // 同じ構造を持つノードが既にないか調べる．
    auto node1 = literal2node(handle1);
    auto node2 = literal2node(handle2);
    auto key = FraigNode(0, node1, handle1.inv(), node2, handle2.inv());
    auto p = mStructTable.find(&key);
    if ( p != mStructTable.end() ) {
      // 等価なノードが存在した．
      auto node = *p;
      ans = node2literal(node);
    }
    else {
      // ノードを作る．
      SizeType id = mAllNodes.size();
      auto node = new FraigNode(id, node1, handle1.inv(), node2, handle2.inv());
      reg_node(node);

      // 構造ハッシュに追加する．
      mStructTable.insert(node);

      // 入出力の関係を表す CNF を作る．
      mSolver.make_cnf(node);

      if ( debug ) {
	auto h = node2literal(node);
	std::cout << "  new node: " << print_literal(h)
		  << std::endl;
      }

      // 縮退検査を行う．
      if ( verify_const(node, ans) == SatBool3::True ) {
	// 縮退していた．
	goto exit;
      }

      // 等価なノードを探す．
      PatEq eq;
      while ( true ) {
	auto range = mPatTable.equal_range(node);
	auto inv0 = node->pat_hash_inv();
	auto change = false;
	auto end = range.second;
	for ( auto p = range.first; p != end; ++ p ) {
	  auto node1 = *p;
	  auto inv = inv0 != node1->pat_hash_inv();
	  if ( eq(node, node1) ) {
	    // node と node1 が等価かどうか調べる．
	    auto stat = mSolver.check_equiv(node, node1, inv);
	    if ( stat == SatBool3::True ) {
	      // 等価なノードが見つかった．
	      ans = node2literal(node, inv);
	      goto exit;
	    }
	    else if ( stat == SatBool3::False ) {
	      // 反例をパタンに加えて再ハッシュする．
	      add_pat(node);

	      ASSERT_COND( !eq(node, node1) );
	      change = true;
	      break;
	    }
	  }
	}
	if ( !change ) {
	  ans = node2literal(node);
	  break;
	}
      }
    }
  }

 exit:
  if ( debug ) {
    std::cout << "  -> " << print_literal(ans)
	      << std::endl;
  }

  return ans;
}

// @brief コファクター演算
FraigLiteral
FraigMgrImpl::cofactor(
  FraigLiteral edge,
  SizeType input_id,
  bool val
)
{
  if ( edge.is_const() ) {
    return edge;
  }
  auto node = literal2node(edge);
  return _cofactor_sub(node, input_id, val) * edge.inv();
}

// @brief make_cofactor() の下請け関数
FraigLiteral
FraigMgrImpl::_cofactor_sub(
  FraigNode* node,
  SizeType input_id,
  bool val
)
{
  if ( node->is_input() ) {
    // 入力ノード時は番号が input_id かどうかで処理が変わる．
    if ( node->input_id() == input_id ) {
      if ( val ) {
	return FraigLiteral::one();
      }
      else {
	return FraigLiteral::zero();
      }
    }
    else {
      return node2literal(node);
    }
  }
  else {
    // AND ノードの場合
    // 2つの子供に再帰的な処理を行って結果の AND を計算する．
    auto tmp0 = _cofactor_sub(node->fanin0(), input_id, val) * node->fanin0_inv();
    auto tmp1 = _cofactor_sub(node->fanin1(), input_id, val) * node->fanin1_inv();
    return new_and(tmp0, tmp1);
  }
}

// @brief 2つのハンドルが等価かどうか調べる．
SatBool3
FraigMgrImpl::check_equiv(
  FraigLiteral aig1,
  FraigLiteral aig2
)
{
  if ( aig1 == aig2 ) {
    // もっとも簡単なパタン
    return SatBool3::True;
  }

  auto node1 = literal2node(aig1);
  auto node2 = literal2node(aig2);

  if ( node1 == node2 ) {
    // ということは逆極性なので絶対に等価ではない．
    return SatBool3::False;
  }

  auto inv1 = aig1.inv();
  auto inv2 = aig2.inv();

  if ( aig1.is_zero () ) {
    // 上のチェックで aig2 は定数でないことは明らか
    auto stat = mSolver.check_const(node2, inv2);
    return stat;
  }

  if ( aig1.is_one() ) {
    // 上のチェックで aig2 は定数でないことは明らか
    auto stat = mSolver.check_const(node2, !inv2);
    return stat;
  }

  if ( aig2.is_zero() ) {
    // 上のチェックで aig1 は定数でないことは明らか
    auto stat = mSolver.check_const(node1, inv1);
    return stat;
  }

  if ( aig2.is_one() ) {
    // 上のチェックで aig1 は定数でないことは明らか
    auto stat = mSolver.check_const(node1, !inv1);
    return stat;
  }

  auto inv = inv1 ^ inv2;
  auto stat = mSolver.check_equiv(node1, node2, inv);
  return stat;
}

// @brief 0縮退検査を行う．
SatBool3
FraigMgrImpl::verify_const(
  FraigNode* node,
  FraigLiteral& ans
)
{
  auto stat = SatBool3::False;
  if ( !node->check_1mark() ) {
    // 定数0の可能性があるか調べる．
    stat = mSolver.check_const(node, false);
    if ( stat == SatBool3::True ) {
      // 定数0と等価だった．
      ans = FraigLiteral::zero();
    }
    else if ( stat == SatBool3::False ) {
      // 反例をパタンに加えておく．
      add_pat(node);

      ASSERT_COND( node->check_1mark() );
    }
  }
  if ( !node->check_0mark() ) {
    // 定数1の可能性があるか調べる．
    stat = mSolver.check_const(node, true);
    if ( stat == SatBool3::True ) {
      // 定数1と等価だった．
      ans = FraigLiteral::one();
    }
    else if ( stat == SatBool3::False ) {
      // 反例をパタンに加えておく．
      add_pat(node);

      ASSERT_COND( node->check_0mark() );
    }
  }

  return stat;
}

// @brief 直前の SAT の反例を加えて再ハッシュする．
void
FraigMgrImpl::add_pat(
  FraigNode* node
)
{
  if ( FraigNode::mPatSize <= FraigNode::mPatUsed ) {
    resize_pat(FraigNode::mPatSize * 2);
  }
  mPatTable.clear();

  // 反例をパタンに加える．
  auto rd100 = std::uniform_int_distribution<int>(0, 99);
  for ( auto& node1_ptr: mAllNodes ) {
    auto node1 = node1_ptr.get();
    if ( node1->is_input() ) {
      auto pat = std::uint64_t{0UL};
      if ( mSolver.model_val(node1) == SatBool3::True ) {
	pat = ~0UL;
      }
      else {
	pat = 0UL;
      }
      // ただし一度に64個のパタンを加えるので63個は
      // 適当にばらつかせる．
      for ( int b = 1; b < 64; ++ b ) {
	if ( rd100(mRandGen) <= 3 ) {
	  pat ^= (1UL << b);
	}
      }
      node1->add_pat(pat);
    }
    else {
      node1->calc_pat(FraigNode::mPatUsed, FraigNode::mPatUsed + 1);
    }

    if ( node1 != node ) {
      mPatTable.insert(node1);
    }
  }
  ++ FraigNode::mPatUsed;
}

// @brief ノードを登録する．
void
FraigMgrImpl::reg_node(
  FraigNode* node
)
{
  mSolver.reg_node(node);
  mAllNodes.push_back(std::unique_ptr<FraigNode>(node));
}

// @brief ログレベルを設定する．
void
FraigMgrImpl::set_loglevel(
  SizeType level
)
{
  mLogLevel = level;
}

// @brief ログ出力用ストリームを設定する．
void
FraigMgrImpl::set_logstream(
  std::ostream* out
)
{
  if ( mLogStream != &std::cout ) {
    delete mLogStream;
  }
  mLogStream = out;
}

// @brief 全ノードのシミュレーションパタン用配列を拡大する．
void
FraigMgrImpl::resize_pat(
  SizeType size
)
{
  for ( auto& node: mAllNodes ) {
    node->resize_pat(size);
  }
  FraigNode::mPatSize = size;
}

// @brief 内部の統計情報を出力する．
void
FraigMgrImpl::dump_stats(
  std::ostream& s
)
{
  mSolver.dump_stats(s);
}

// @brief リテラルの内容を表す文字列を作る．
std::string
FraigMgrImpl::print_literal(
  FraigLiteral lit
) const
{
  if ( lit.is_zero() ) {
    return "ZERO";
  }
  if ( lit.is_one() ) {
    return "ONE";
  }
  auto node = literal2node(lit);
  std::ostringstream buf;
  if ( lit.inv() ) {
    buf << "~";
  }
  if ( node->is_input() ) {
    buf << "I" << node->input_id();
  }
  else {
    buf << "A" << node->id();
  }
  return buf.str();
}

END_NAMESPACE_YM_FRAIG
