
/// @file BddDecMgr.cc
/// @brief BddDecMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/DjDec.h"
#include "BddDecMgr.h"
#include "BddDecNode.h"
#include "BddDecEdge.h"
#include "BddDec2Bc.h"
#include "BddDecMark.h"
#include "ym/BddVarSet.h"


BEGIN_NAMESPACE_YM_DJDEC

BEGIN_NONAMESPACE

bool debug = false;

END_NONAMESPACE

// @brief 与えられた関数の BddDecGraph を得る．
BcGraph
DjDec::decompose(
  const Bdd& func
)
{
  BddDecMgr mgr(func);
  auto r = mgr.decompose();
  auto sup_list = mgr.bdd().get_support_list();
  BddDec2Bc dg2bc(sup_list);
  auto g = dg2bc.make_bcgraph(r);
  return g;
}


//////////////////////////////////////////////////////////////////////
// クラス BddDecMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BddDecMgr::BddDecMgr(
  const Bdd& func
) : mBdd{mBddMgr.copy(func)}
{
}

// @brief デストラクタ
BddDecMgr::~BddDecMgr()
{
}

// @brief 与えられた関数の BddDecGraph を得る
BddDecEdge
BddDecMgr::decompose()
{
  mNodeList.clear();
  mEdgeDict.clear();
  auto root = decomp_step(mBdd);
  return root;
}

// @brief decomp の下請け関数
BddDecEdge
BddDecMgr::decomp_step(
  const Bdd& func
)
{
  if ( func.is_zero() ) {
    return BddDecEdge::zero();
  }
  if ( func.is_one() ) {
    return BddDecEdge::one();
  }

  if ( debug ) {
    std::cout << "decomp_step begin" << std::endl
	      << " func" << std::endl;
    display(std::cout, func);
  }

  BddDecEdge result;
  // 登録済みかどうか調べる．
  if ( find_node(func, result) ) {
    if ( debug ) {
      std::cout << "  already exists" << std::endl
		<< " result" << std::endl;
      print(std::cout, result);
    }
    // 結果を返す．
    return result;
  }

  // 根本の変数で分解する．
  Bdd func0;
  Bdd func1;
  auto top_var = func.root_decomp(func0, func1);
  if ( debug ) {
    std::cout << " top_var = " << top_var << std::endl
	      << " func0" << std::endl;
    display(std::cout, func0);
    std::cout << " func1" << std::endl;
    display(std::cout, func1);
    std::cout << std::endl;
  }

  // コファクターに対して再帰的に処理を行う．
  auto r0 = decomp_step(func0);
  auto r1 = decomp_step(func1);
  // 結果をマージする．
  if ( debug ) {
    std::cout << "merge at " << top_var << std::endl
	      << "r0" << std::endl;
    print(std::cout, r0);
    std::cout << "r1" << std::endl;
    print(std::cout, r1);
    std::cout << std::endl;
  }
  result = merge(top_var, r0, r1);
  if ( debug ) {
    std::cout << "merge end" << std::endl;
    display(std::cout, func);
    std::cout << "merge at " << top_var << std::endl
	      << "r0" << std::endl;
    print(std::cout, r0);
    std::cout << "r1" << std::endl;
    print(std::cout, r1);
    std::cout << std::endl;
    std::cout << "  ==> ";
    print(std::cout, result);
    ASSERT_COND( result.global_func() == func );
  }
  // 結果を登録する．
  put_node(func, result);

  if ( debug ) {
    std::cout << "decomp_step end" << std::endl
	      << " func" << std::endl;
    display(std::cout, func);
    std::cout << " result" << std::endl;
    print(std::cout, result);
    if ( func != result.global_func() ) {
      abort();
    }
  }

  return result;
}

// @brief コファクターの結果をマージする．
BddDecEdge
BddDecMgr::merge(
  const BddVar& var,
  BddDecEdge r0,
  BddDecEdge r1
)
{
  // 一方もしくは両方の結果が定数の場合の処理
  if ( r0.is_zero() ) {
    if ( r1.is_one() ) {
      // 肯定のリテラル
      auto result = make_lit(var);
      return result;
    }
    // リテラルとのAND ( x & r1 )
    auto result = make_lit_and(var, false, r1);
    return result;
  }
  if ( r0.is_one() ) {
    if ( r1.is_zero() ) {
      // 否定のリテラル
      auto result = make_lit(var, true);
      return result;
    }
    // リテラルとのOR ( ~x | r1 )
    auto result = make_lit_or(var, true, r1);
    return result;
  }
  if ( r1.is_zero() ) {
    // リテラルとのAND ( ~x & r0 )
    auto result = make_lit_and(var, true, r0);
    return result;
  }
  if ( r1.is_one() ) {
    // リテラルとのOR ( x | r0 )
    auto result = make_lit_or(var, false, r0);
    return result;
  }
  if ( BddDecEdge::check_complement(r0, r1) ) {
    // リテラルとのXOR ( ~x & r0 | x & ~r0 )
    auto result = make_lit_xor(var, false, r0);
    return result;
  }

  // ここまで来たらどちらも定数ではない．
  auto inv0 = r0.inv();
  auto inv1 = r1.inv();
  auto node0 = r0.node();
  auto node1 = r1.node();
  auto nc0 = node0->child_num();
  auto nc1 = node1->child_num();

  // node0, node1 に共通な子ノードのリスト
  std::vector<BddDecEdge> common_list;
  // node0 のみの子ノードのリスト
  std::vector<BddDecEdge> rest0_list;
  // node1 のみの子ノードのリスト
  std::vector<BddDecEdge> rest1_list;
  { // 共通な子ノードのリストを作る．
    SizeType i0 = 0;
    SizeType i1 = 0;
    while ( i0 < nc0 && i1 < nc1 ) {
      auto ce0 = node0->child(i0);
      auto ce1 = node1->child(i1);
      auto cnode0 = ce0.node();
      auto cnode1 = ce1.node();
      auto index0 = cnode0->top().varid();
      auto index1 = cnode1->top().varid();
      int diff = index0 - index1;
      if ( diff < 0 ) {
	rest0_list.push_back(ce0);
	++ i0;
      }
      else if ( diff == 0 ) {
	if ( ce0 == ce1 ) {
	  common_list.push_back(ce0);
	}
	else {
	  rest0_list.push_back(ce0);
	  rest1_list.push_back(ce1);
	}
	++ i0;
	++ i1;
      }
      else { // diff > 0
	rest1_list.push_back(ce1);
	++ i1;
      }
    }
    while ( i0 < nc0 ) {
      auto ce0 = node0->child(i0);
      auto cnode0 = ce0.node();
      rest0_list.push_back(ce0);
      ++ i0;
    }
    while ( i1 < nc1 ) {
      auto ce1 = node1->child(i1);
      auto cnode1 = ce1.node();
      rest1_list.push_back(ce1);
      ++ i1;
    }
  }

  if ( debug ) {
    std::cout << "common:" << std::endl;
    for ( auto edge: common_list ) {
      print(std::cout, edge);
      std::cout << "===" << std::endl;
    }
    std::cout << "rest0:" << std::endl;
    for ( auto edge: rest0_list ) {
      print(std::cout, edge);
      std::cout << "===" << std::endl;
    }
    std::cout << "rest1:" << std::endl;
    for ( auto edge: rest1_list ) {
      print(std::cout, edge);
      std::cout << "===" << std::endl;
    }
  }

  if ( common_list.size() >= 1 ) {
    // Case1-OR
    if ( inv0 == inv1 && node0->is_or() && node1->is_or() ) {
      auto result = case1_or(var, common_list, rest0_list, rest1_list) * inv0;
      return result;
    }

    // Case1-XOR
    if ( node0->is_xor() && node1->is_xor() ) {
      auto result = case1_xor(var, common_list, rest0_list, rest1_list, inv0, inv1);
      return result;
    }
  }

  // Case2-OR
  if ( node0->is_or() ) {
    for ( SizeType i = 0; i < nc0; ++ i ) {
      if ( (node0->child(i) * inv0) == r1 ) {
	auto result = case2_or(var, true, node0, inv0, i);
	return result;
      }
    }
  }
  if ( node1->is_or() ) {
    for ( SizeType i = 0; i < nc1; ++ i ) {
      if ( (node1->child(i) * inv1) == r0 ) {
	auto result = case2_or(var, false, node1, inv1, i);
	return result;
      }
    }
  }

  // Case2-XOR
  if ( node0->is_xor() ) {
    for ( SizeType i = 0; i < nc0; ++ i ) {
      if ( node0->child(i).node() == node1 ) {
	auto result = case2_xor(var, true, node0, inv0, i, inv1);
	return result;
      }
    }
  }
  if ( node1->is_xor() ) {
    for ( SizeType i = 0; i < nc1; ++ i ) {
      if ( node1->child(i).node() == node0 ) {
	auto result = case2_xor(var, false, node1, inv1, i, inv0);
	return result;
      }
    }
  }

  // ここまで来ると CPLX タイプなのでグローバル関数が必要になる．
  auto f0 = r0.global_func();
  auto f1 = r1.global_func();
  auto f = ite(var, f1, f0);
  if ( node0->is_cplx() && node1->is_cplx() ) {
    if ( rest0_list.size() == 1 && rest1_list.size() == 1 ) {
      // r0, r1 が共に CPLX でただ一つの子ノード以外共通の場合
      auto r0_1 = rest0_list[0];
      auto r1_1 = rest1_list[0];

      // 根の関数が等しいか調べる．
      // f0 に対して r0_1 が 0 に束縛した関数と
      // f1 に対して r1_1 を 0 に束縛した関数が等価かどうか調べる．
      auto f0_0 = f0 / r0_1.pat_0();
      auto f1_0 = f1 / r1_1.pat_0();

      // f0 に対して r0_1 が 1 に束縛した関数と
      // f1 に対して r1_1 を 1 に束縛した関数が等価かどうか調べる．
      auto f0_1 = f0 / r0_1.pat_1();
      auto f1_1 = f1 / r1_1.pat_1();

      if ( f0_0 == f1_0 && f0_1 == f1_1 ) {
	// 一致した．
	auto result = case1_cplx(f, var, node0, node1, common_list, r0_1, r1_1);
	return result;
      }

      // f0 に対して r0_1 が 0 に束縛した関数と
      // f1 に対して r1_1 を 1 に束縛した関数が等価かどうか調べる．

      // f0 に対して r0_1 が 1 に束縛した関数と
      // f1 に対して r1_1 を 0 に束縛した関数が等価かどうか調べる．
      if ( f0_0 == f1_1 && f0_1 == f1_0 ) {
	// 一致した．
	auto result = case1_cplx(f, var, node0, node1, common_list, ~r0_1, r1_1);
	return result;
      }
    }
    else if ( rest0_list.empty() && rest1_list.empty() ) {
      // どれかの子ノードを反転すると一致する場合
      for ( SizeType i = 0; i < common_list.size(); ++ i ) {
	auto chd = common_list[i];
	auto f0_0 = f0 / chd.pat_0();
	auto f0_1 = f0 / chd.pat_1();
	auto f1_0 = f1 / chd.pat_0();
	auto f1_1 = f1 / chd.pat_1();
	if ( f0_0 == f1_1 && f0_1 == f1_0 ) {
	  // 一致した．
	  auto result = case1_cplx2(f, var, common_list, i);
	  return result;
	}
      }
    }
  }

  auto sup0 = node0->support();
  auto sup1 = node1->support();
  if ( !(sup0 && sup1) ) {
    // ITE(var, r1, r0) となる．
    auto lit = make_lit(var);
    auto result = make_cplx(f, {lit, r0, r1});
    return result;
  }

  auto sup0_diff = sup0 - sup1;
  auto sup1_diff = sup1 - sup0;
  if ( node0->is_cplx() && sup0_diff.size() > 0 && sup1_diff.size() == 0 ) {
    for ( SizeType i = 0; i < nc0; ++ i ) {
      auto cedge = node0->child(i);
      auto cnode = cedge.node();
      auto& csup = cnode->support();
      if ( !(csup && sup1) ) {
	// f0 に対して cedge が 0 になる割当を行うと f1 と一致する場合
	auto f0_0 = f0 / cedge.pat_0();
	if ( f0_0 == f1 ) {
	  auto result = case2_cplx(f, var, cedge, true, true, node0);
	  return result;
	}
	// f0 に対して cedge が 1 になる割当を行うと f1 と一致する場合
	auto f0_1 = f0 / cedge.pat_1();
	if ( f0_1 == f1 ) {
	  auto result = case2_cplx(f, var, cedge, false, false, node0);
	  return result;
	}
      }
    }
  }
  if ( node1->is_cplx() && sup1_diff.size() > 0 && sup0_diff.size() == 0 ) {
    for ( SizeType i = 0; i < nc1; ++ i ) {
      auto cedge = node1->child(i);
      auto cnode = cedge.node();
      auto& csup = cnode->support();
      if ( !(csup && sup0) ) {
	// f1 に対して cedge が 0 になる割当を行うと f0 と一致する場合
	auto f1_0 = f1 / cedge.pat_0();
	if ( f1_0 == f0 ) {
	  auto result = case2_cplx(f, var, cedge, false, true, node1);
	  return result;
	}
	// f1 に対して cedge が 1 になる割当を行うと f0 と一致する場合
	auto f1_1 = f1 / cedge.pat_1();
	if ( f1_1 == f0 ) {
	  auto result = case2_cplx(f, var, cedge, true, false, node1);
	  return result;
	}
      }
    }
  }

  if ( debug ) {
    std::cout << "last resort" << std::endl;
  }

  // node0 と node1 の推移的な子ノードのうち，共通なノードを求める．
  // それらを新しい子供とした CPLX ノードを作る．
  // 面倒なのは OR/XOR ノードが極大化されているということ．
  BddDecMark mark;
  // node0 の推移的ファンインに 1 のマークをつける．
  mark.mark_recur(r0, 1);
  // node1 の推移的ファンインに 2 のマークをつける．
  mark.mark_recur(r1, 2);
  // 自分には 3 のマークがなく，推移的ファンインに 3
  // のマークがついているノードのマークを 0 にする．
  mark.tfimark_recur(r0);
  mark.tfimark_recur(r1);
  // 境界ノードに印をつける．
  std::vector<const BddDecNode*> or_list0;
  std::vector<const BddDecNode*> xor_list0;
  mark.get_boundary(r0, or_list0, xor_list0);
  std::vector<const BddDecNode*> or_list1;
  std::vector<const BddDecNode*> xor_list1;
  mark.get_boundary(r1, or_list1, xor_list1);

  // 入力を求める．
  std::vector<BddDecEdge> tmp_inputs;
  find_uncommon_inputs(or_list0, mark, 1, tmp_inputs);
  find_uncommon_inputs(or_list1, mark, 2, tmp_inputs);
  find_uncommon_inputs(xor_list0, mark, 1, tmp_inputs);
  find_uncommon_inputs(xor_list1, mark, 2, tmp_inputs);
  find_common_inputs(or_list0, or_list1, mark, tmp_inputs);
  find_common_inputs(xor_list0, xor_list1, mark, tmp_inputs);
  mark.find_bnode(node0, tmp_inputs);
  mark.find_bnode(node1, tmp_inputs);
  auto top_edge = make_lit(var);
  tmp_inputs.push_back(top_edge);
  auto result = make_cplx(f, tmp_inputs);
  return result;
}

// @brief 共通でないファンインを求める．
void
BddDecMgr::find_uncommon_inputs(
  const std::vector<const BddDecNode*>& node_list,
  BddDecMark& mark,
  int mval,
  std::vector<BddDecEdge>& inputs
)
{
  for ( auto node: node_list ) {
    auto nc = node->child_num();
    std::vector<BddDecEdge> tmp_inputs;
    for ( SizeType i = 0; i < nc; ++ i ) {
      auto cedge = node->child(i);
      auto cnode = cedge.node();
      if ( mark.bmark(cnode) == 1 && mark.mark(cnode) == mval ) {
	tmp_inputs.push_back(cedge);
      }
    }

    if ( tmp_inputs.size() > 1 ) {
      if ( node_list[0]->is_or() ) {
	auto tmp_edge = make_or(tmp_inputs);
	inputs.push_back(tmp_edge);
      }
      else {
	auto tmp_edge = make_xor(tmp_inputs);
	inputs.push_back(tmp_edge);
      }
      for ( auto edge: tmp_inputs ) {
	auto node = edge.node();
	mark.set_bmark(node, 3);
      }
    }
  }
}

// @brief 共通なファンインを求める．
void
BddDecMgr::find_common_inputs(
  const std::vector<const BddDecNode*>& node_list1,
  const std::vector<const BddDecNode*>& node_list2,
  BddDecMark& mark,
  std::vector<BddDecEdge>& inputs
)
{
  for ( auto node1: node_list1 ) {
    auto nc1 = node1->child_num();
    std::unordered_set<BddDecEdge> common_set;
    for ( SizeType i = 0; i < nc1; ++ i ) {
      auto cedge = node1->child(i);
      auto cnode = cedge.node();
      if ( mark.bmark(cnode) == 1 && mark.mark(cnode) == 3 ) {
	common_set.emplace(cedge);
      }
    }
    for ( auto node2: node_list2 ) {
      auto nc2 = node2->child_num();
      std::vector<BddDecEdge> common;
      for ( SizeType i = 0; i < nc2; ++ i ) {
	auto cedge = node2->child(i);
	auto cnode = cedge.node();
	if ( mark.bmark(cnode) == 1 ) {
	  if ( common_set.count(cedge) > 0 ) {
	    common.push_back(cedge);
	  }
	}
      }
      auto nc = common.size();
      if ( nc > 1 ) {
	if ( nc == nc1 ) {
	  inputs.push_back(BddDecEdge{node1});
	}
	else if ( nc == nc2 ) {
	  inputs.push_back(BddDecEdge{node2});
	}
	else if ( node1->is_or() ) {
	  auto cedge = make_or(common);
	  inputs.push_back(cedge);
	}
	else {
	  auto cedge = make_xor(common);
	  inputs.push_back(cedge);
	}
	for ( auto edge: common ) {
	  auto node = edge.node();
	  mark.set_bmark(node, 3);
	}
      }
    }
  }
}

// @brief Case1-OR のマージを行う．
BddDecEdge
BddDecMgr::case1_or(
  const BddVar& var,
  const std::vector<BddDecEdge>& common_list,
  const std::vector<BddDecEdge>& rest0_list,
  const std::vector<BddDecEdge>& rest1_list
)
{
  // rest0_list, rest1_list が空の場合でも merge() が
  // 適切に処理してくれる．
  auto tmp0_edge = make_or(rest0_list);
  auto tmp1_edge = make_or(rest1_list);
  if ( debug ) {
    std::cout << "[case1_or] merge at " << var.varid() << std::endl
	      << "r0" << std::endl;
    print(std::cout, tmp0_edge);
    std::cout << "r1" << std::endl;
    print(std::cout, tmp1_edge);
    std::cout << std::endl;
  }
  auto new_edge = merge(var, tmp0_edge, tmp1_edge);
  if ( debug ) {
    std::cout << "[case1_or] merge end" << std::endl
	      << "merge at " << var.varid() << std::endl
	      << "r0" << std::endl;
    print(std::cout, tmp0_edge);
    std::cout << "r1" << std::endl;
    print(std::cout, tmp1_edge);
    std::cout << std::endl
	      << "  ==> ";
    print(std::cout, new_edge);
  }

  std::vector<BddDecEdge> child_list{new_edge};
  child_list.insert(child_list.end(),
		    common_list.begin(), common_list.end());
  return make_or(child_list);
}

// @brief Case1-XOR のマージを行う．
BddDecEdge
BddDecMgr::case1_xor(
  const BddVar& var,
  const std::vector<BddDecEdge>& common_list,
  const std::vector<BddDecEdge>& rest0_list,
  const std::vector<BddDecEdge>& rest1_list,
  bool inv0,
  bool inv1
)
{
  // rest0_list, rest1_list が空の場合でも merge() が
  // 適切に処理してくれる．
  auto tmp0_edge = make_xor(rest0_list) * inv0;
  auto tmp1_edge = make_xor(rest1_list) * inv1;
  if ( debug ) {
    std::cout << "[case1_xor] merge at " << var.varid() << std::endl
	      << "r0" << std::endl;
    print(std::cout, tmp0_edge);
    std::cout << "r1" << std::endl;
    print(std::cout, tmp1_edge);
    std::cout << std::endl;
  }
  auto new_edge = merge(var, tmp0_edge, tmp1_edge);
  if ( debug ) {
    std::cout << "[case1_xor] merge end" << std::endl
	      << "merge at " << var.varid() << std::endl
	      << "r0" << std::endl;
    print(std::cout, tmp0_edge);
    std::cout << "r1" << std::endl;
    print(std::cout, tmp1_edge);
    std::cout << std::endl
	      << "  ==> ";
    print(std::cout, new_edge);
  }

  std::vector<BddDecEdge> child_list{new_edge};
  child_list.insert(child_list.end(),
		    common_list.begin(), common_list.end());
  return make_xor(child_list);
}

// @brief Case1-CPLX のマージを行う．
BddDecEdge
BddDecMgr::case1_cplx(
  const Bdd& f,
  const BddVar& var,
  const BddDecNode* node0,
  const BddDecNode* node1,
  const std::vector<BddDecEdge>& common_list,
  BddDecEdge rest0,
  BddDecEdge rest1
)
{
  if ( debug ) {
    std::cout << "merge at " << var.varid() << std::endl
	      << "r0" << std::endl;
    print(std::cout, rest0);
    std::cout << "r1" << std::endl;
    print(std::cout, rest1);
    std::cout << std::endl;
  }
  auto new_edge = merge(var, rest0, rest1);
  if ( debug ) {
    std::cout << "merge end" << std::endl;
    display(std::cout, f);
    std::cout << "merge at " << var.varid() << std::endl
	      << "r0" << std::endl;
    print(std::cout, rest0);
    std::cout << "r1" << std::endl;
    print(std::cout, rest1);
    std::cout << std::endl
	      << "  ==> ";
    print(std::cout, new_edge);
  }
  std::vector<BddDecEdge> tmp_list{new_edge};
  tmp_list.insert(tmp_list.end(),
		  common_list.begin(), common_list.end());
  auto result = make_cplx(f, tmp_list);
  return result;
}

// @brief CASE1-CPLX(Type 2) のマージを行う．
BddDecEdge
BddDecMgr::case1_cplx2(
  const Bdd& f,
  const BddVar& var,
  const std::vector<BddDecEdge>& child_list,
  SizeType pos
)
{
  // child_list の pos 番目の子ノードと var との XOR を作る．
  auto chd0 = child_list[pos];
  auto tmp_edge = make_lit_xor(var, false, chd0);
  std::vector<BddDecEdge> tmp_list{child_list};
  tmp_list[pos] = tmp_edge;
  auto result = make_cplx(f, tmp_list);
  return result;
}

// @brief CASE2-OR のマージを行う．
BddDecEdge
BddDecMgr::case2_or(
  const BddVar& var,
  bool lit_inv,
  const BddDecNode* node,
  bool oinv,
  SizeType pos
)
{
  // pos 以外の子ノードのORノードを作る．
  std::vector<BddDecEdge> tmp_list;
  auto n = node->child_num();
  tmp_list.reserve(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    if ( i != pos ) {
      tmp_list.push_back(node->child(i));
    }
  }
  auto tmp_edge = make_or(tmp_list);
  // それと var とのANDを作る．
  auto new_edge = make_lit_and(var, lit_inv, tmp_edge);
  // それと r1 との ORを作る．
  auto r1 = node->child(pos);
  auto result = make_or({new_edge, r1}) * oinv;
  return result;
}

// @brief CASE2-XOR のマージを行う．
BddDecEdge
BddDecMgr::case2_xor(
  const BddVar& var,
  bool lit_inv,
  const BddDecNode* node,
  bool oinv,
  SizeType pos,
  bool inv1
)
{
  if ( debug ) {
    std::cout << "case2_xor" << std::endl;
  }
  // pos 以外の子ノードのXORノードを作る．
  std::vector<BddDecEdge> tmp_list;
  auto n = node->child_num();
  tmp_list.reserve(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    if ( i != pos ) {
      tmp_list.push_back(node->child(i));
    }
  }
  auto tmp_edge = make_xor(tmp_list);
  if ( debug ) {
    std::cout << "tmp_edge" << std::endl;
    print(std::cout, tmp_edge);
  }
  tmp_edge *= oinv;
  // それと var との AND/OR を作る．
  BddDecEdge new_edge;
  if ( inv1 ) {
    new_edge = make_lit_or(var, !lit_inv, tmp_edge);
  }
  else {
    new_edge = make_lit_and(var, lit_inv, tmp_edge);
  }
  // それと r1 との XOR を作る．
  auto r1 = node->child(pos);
  auto result = make_xor({new_edge, r1});
  return result;
}

// @brief CASE2-CPLX のマージを行う．
BddDecEdge
BddDecMgr::case2_cplx(
  const Bdd& f,
  const BddVar& var,
  BddDecEdge cedge,
  bool lit_inv,
  bool is_and,
  const BddDecNode* node
)
{
  BddDecEdge new_edge;
  if ( is_and ) {
    new_edge = make_lit_and(var, lit_inv, cedge);
  }
  else {
    new_edge = make_lit_or(var, lit_inv, cedge);
  }

  auto nc = node->child_num();
  std::vector<BddDecEdge> tmp_list(nc);
  for ( SizeType i = 0; i < nc; ++ i ) {
    auto chd = node->child(i);
    if ( chd == cedge ) {
      tmp_list[i] = new_edge;
    }
    else {
      tmp_list[i] = chd;
    }
  }
  auto result = make_cplx(f, tmp_list);
  return result;
}

// @brief LITノードを作る．
BddDecEdge
BddDecMgr::make_lit(
  const BddVar& var,
  bool inv
)
{
  auto f = var.bdd();
  BddDecEdge result;
  if ( find_node(f, result) ) {
    return result * inv;
  }
  SizeType id = mNodeList.size();
  auto sup = f.get_support();
  auto node = BddDecNode::new_lit(mBddMgr, id, f, sup);
  mNodeList.push_back(std::unique_ptr<BddDecNode>{node});
  result = BddDecEdge{node};
  put_node(f, result);
  return result * inv;
}

BEGIN_NONAMESPACE

struct EdgeLt
{
  bool
  operator()(
    BddDecEdge e1,
    BddDecEdge e2
  ) const
  {
    return e1.node()->top().varid() < e2.node()->top().varid();
  }
};

END_NONAMESPACE

// @brief ORノードを作る．
BddDecEdge
BddDecMgr::make_or(
  const std::vector<BddDecEdge>& child_list
)
{
  if ( child_list.empty() ) {
    // 空の場合は定数0を返す．
    return BddDecEdge::zero();
  }
  if ( child_list.size() == 1 ) {
    // 子供が1人ならその子供を返す．
    return child_list[0];
  }

  if ( debug ) {
    std::cout << "make_or begin" << std::endl;
    for ( auto child: child_list ) {
      print(std::cout, child);
      std::cout << "===" << std::endl;
    }
  }
  // グローバル関数を作る．
  auto f = child_list[0].global_func();
  for ( SizeType i = 1; i < child_list.size(); ++ i ) {
    f |= child_list[i].global_func();
  }
  // 等価なノードがあるか調べる．
  BddDecEdge result;
  if ( !find_node(f, result) ) {
    // child_list の中で肯定のORノードがあれば
    // その子ノードを本当の子ノードにする．
    std::vector<BddDecEdge> tmp_list;
    BddVarSet support;
    for ( auto child: child_list ) {
      if ( child.inv() == false && child.node()->is_or() ) {
	for ( SizeType i = 0; i < child.node()->child_num(); ++ i ) {
	  tmp_list.push_back(child.node()->child(i));
	}
      }
      else {
	tmp_list.push_back(child);
      }
      support += child.node()->support();
    }
    // top の昇順になるようにソートする．
    std::sort(tmp_list.begin(), tmp_list.end(), EdgeLt());
    SizeType id = mNodeList.size();
    auto node = BddDecNode::new_or(mBddMgr, id, f, support, tmp_list);
    mNodeList.push_back(std::unique_ptr<BddDecNode>{node});
    result = BddDecEdge{node};
    put_node(f, result);
  }
  return result;
}

// @brief XORノードを作る．
BddDecEdge
BddDecMgr::make_xor(
  const std::vector<BddDecEdge>& child_list
)
{
  if ( child_list.empty() ) {
    // 空の場合は定数0を返す．
    return BddDecEdge::zero();
  }
  if ( child_list.size() == 1 ) {
    // 子供が1人ならその子供を返す．
    return child_list[0];
  }

  if ( debug ) {
    std::cout << "make_xor begin" << std::endl;
    for ( auto child: child_list ) {
      print(std::cout, child);
      std::cout << "===" << std::endl;
    }
  }
  // グローバル関数を作る．
  auto f = child_list[0].global_func();
  for ( SizeType i = 1; i < child_list.size(); ++ i ) {
    f ^= child_list[i].global_func();
  }
  // 等価なノードがあるか調べる．
  BddDecEdge result;
  if ( !find_node(f, result) ) {
    // child_list の中でXORノードがあれば
    // その子ノードを本当の子ノードにする．
    // ORの場合と異なり極性は問わない．
    // ただし，XORノードの子供には反転属性をつけない．
    // 極性は根に移す．
    std::vector<BddDecEdge> tmp_list;
    BddVarSet support;
    bool oinv = false;
    for ( auto child: child_list ) {
      auto inv = child.inv();
      oinv ^= inv;
      if ( child.node()->is_xor() ) {
	for ( SizeType i = 0; i < child.node()->child_num(); ++ i ) {
	  auto child1 = child.node()->child(i);
	  tmp_list.push_back(child1.normal_edge());
	  oinv ^= child1.inv();
	}
      }
      else {
	tmp_list.push_back(child.normal_edge());
      }
      support += child.node()->support();
    }
    // top の昇順になるようにソートする．
    std::sort(tmp_list.begin(), tmp_list.end(), EdgeLt());
    auto f_normal = f * oinv;
    SizeType id = mNodeList.size();
    auto node = BddDecNode::new_xor(mBddMgr, id, f_normal, support, tmp_list);
    mNodeList.push_back(std::unique_ptr<BddDecNode>{node});
    result = BddDecEdge(node, false);
    put_node(f_normal, result);
    result *= oinv;
  }
  return result;
}

// @brief LITノードとのANDを作る．
BddDecEdge
BddDecMgr::make_lit_and(
  const BddVar& var,
  bool inv,
  BddDecEdge e
)
{
  auto lit = make_lit(var, inv);
  return ~make_or({~lit, ~e});
}

// @brief LITノードとのORを作る．
BddDecEdge
BddDecMgr::make_lit_or(
  const BddVar& var,
  bool inv,
  BddDecEdge e
)
{
  auto lit = make_lit(var, inv);
  return make_or({lit, e});
}

// @brief LITノードとのXORを作る．
BddDecEdge
BddDecMgr::make_lit_xor(
  const BddVar& var,
  bool inv,
  BddDecEdge e
)
{
  auto lit = make_lit(var,  inv);
  return make_xor({lit, e});
}

// @brief CPLXノードを作る．
BddDecEdge
BddDecMgr::make_cplx(
  const Bdd& f,
  const std::vector<BddDecEdge>& child_list
)
{
  auto oinv = f.root_inv();
  auto f_normal = f * oinv;
  BddDecEdge result;
  if ( !find_node(f_normal, result) ) {
    auto support = f_normal.get_support();
    std::vector<BddDecEdge> tmp_list;
    tmp_list.reserve(child_list.size());
    for ( auto edge: child_list ) {
      tmp_list.push_back(edge.normal_edge());
    }
    std::sort(tmp_list.begin(), tmp_list.end(), EdgeLt());
    SizeType id = mNodeList.size();
    auto node = BddDecNode::new_cplx(mBddMgr, id, f_normal, support, tmp_list);
    mNodeList.push_back(std::unique_ptr<BddDecNode>{node});
    result = BddDecEdge(node, false);
    put_node(f_normal, result);
  }
  return result * oinv;
}

// @brief 関数に対応する BddDecEdge を探す．
bool
BddDecMgr::find_node(
  const Bdd& f,
  BddDecEdge& result
) const
{
  auto inv = f.root_inv();
  auto f_normal = f * inv;
  if ( mEdgeDict.count(f_normal) == 0 ) {
    return false;
  }
  result = mEdgeDict.at(f_normal) * inv;
  return true;
}

// @brief 関数に対応する BddDecEdge を登録する．
void
BddDecMgr::put_node(
  const Bdd& f,
  BddDecEdge result
)
{
  auto inv = f.root_inv();
  auto f_normal = f * inv;
  result *= inv;
  if ( debug ) {
    std::cout << "put_node" << std::endl;
    display(std::cout, f_normal);
    print(std::cout, result);
  }

  if ( mEdgeDict.count(f_normal) > 0 ) {
    ASSERT_COND( mEdgeDict.at(f_normal) == result );
    return;
  }

  mEdgeDict.emplace(f_normal, result);
}

BEGIN_NONAMESPACE

void
print_sub(
  std::ostream& s,
  BddDecEdge e
)
{
  auto node = e.node();
  if ( node == nullptr ) {
    return;
  }

  node->print(s);
  auto nc = node->child_num();
  for ( SizeType i = 0; i < nc; ++ i ) {
    auto edge1 = node->child(i);
    print_sub(s, edge1);
  }
}

END_NONAMESPACE

void
BddDecMgr::print(
  std::ostream& s,
  BddDecEdge edge
)
{
  s << "root: ";
  edge.print(s);
  s << std::endl;
  print_sub(s, edge);
  s << std::endl;
}

END_NAMESPACE_YM_DJDEC
