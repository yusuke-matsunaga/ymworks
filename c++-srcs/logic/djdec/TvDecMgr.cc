
/// @file TvDecMgr.cc
/// @brief TvDecMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "TvDecMgr.h"
#include "TvDecNode.h"
#include "TvDecEdge.h"
#include "TvDec2Bc.h"
#include "TvDecMark.h"


BEGIN_NAMESPACE_YM_DJDEC

BEGIN_NONAMESPACE

bool debug = false;

END_NONAMESPACE

// @brief 与えられた関数の DgGraph を得る．
BcGraph
DjDec::decompose(
  const TvFunc& func
)
{
  TvDecMgr mgr;
  auto r = mgr.decompose(func);
  TvDec2Bc dg2bc(func.input_num());
  auto g = dg2bc.make_bcgraph(r);
  return g;
}

BEGIN_NONAMESPACE

// child_listのシグネチャを作る．
inline
std::string
child_signature(
  const std::vector<TvDecEdge>& child_list
)
{
  std::ostringstream buf;
  for ( auto edge: child_list ) {
    auto node = edge.node();
    auto tmp = node->id() * 2;
    if ( edge.inv() ) {
      ++ tmp;
    }
    buf << ":" << tmp;
  }
  return buf.str();
}

// ORノード用のシグネチャを作る．
inline
std::string
or_signature(
  const std::vector<TvDecEdge>& child_list
)
{
  return std::string{"O"} + child_signature(child_list);
}

// XORノード用のシグネチャを作る．
inline
std::string
xor_signature(
  const std::vector<TvDecEdge>& child_list
)
{
  return std::string{"X"} + child_signature(child_list);
}

// CPLXノード用のシグネチャを作る．
inline
std::string
cplx_signature(
  const TvFunc& local_func,
  const std::vector<TvDecEdge>& child_list
)
{
  return std::string{"C"} + local_func.hex_str() + child_signature(child_list);
}

// デバッグ用に関数を表す文字列を作る．
inline
std::string
func_str(
  const TvFunc& func
)
{
  std::ostringstream buf;
  buf << func.input_num() << ":" << func.hex_str();
  return buf.str();
}

struct EdgeLt
{
  bool
  operator()(
    TvDecEdge e1,
    TvDecEdge e2
  ) const
  {
    return e1.node()->top_var() < e2.node()->top_var();
  }
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス TvDecMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TvDecMgr::TvDecMgr()
{
}

// @brief デストラクタ
TvDecMgr::~TvDecMgr()
{
}

// @brief 与えられた関数の DgGraph を得る
TvDecEdge
TvDecMgr::decompose(
  const TvFunc& func
)
{
  mNodeList.clear();
  mLitList.clear();
  mNodeDict.clear();
  mEdgeDict.clear();
  auto ni = func.input_num();
  mLitList.reserve(ni);
  for ( SizeType i = 0; i < ni; ++ i ) {
    auto id = mNodeList.size();
    auto node = TvDecNode::new_lit(id, i);
    mNodeList.push_back(std::unique_ptr<TvDecNode>{node});
    mLitList.push_back(node);
  }
  auto root = decomp_step(func);
  return root;
}

// @brief decomp の下請け関数
TvDecEdge
TvDecMgr::decomp_step(
  const TvFunc& func
)
{
  if ( func.is_zero() ) {
    return TvDecEdge::zero();
  }
  if ( func.is_one() ) {
    return TvDecEdge::one();
  }

  if ( debug ) {
    std::cout << "decomp_step begin" << std::endl
	      << " func: " << func_str(func) << std::endl;
  }

  TvDecEdge result;
  // 登録済みかどうか調べる．
  if ( find_edge(func, result) ) {
    if ( debug ) {
      std::cout << "  already exists" << std::endl
		<< " result" << std::endl;
      print(std::cout, result);
    }
    // 結果を返す．
    return result;
  }

  // 根本の変数で分解する．
  TvFunc func0;
  TvFunc func1;
  func.decompose(func0, func1);
  SizeType top_var = func.input_num() - 1;
  if ( debug ) {
    std::cout << " top_var = " << top_var << std::endl
	      << " func0: " << func_str(func0) << std::endl
	      << " func1: " << func_str(func1) << std::endl
	      << std::endl;
  }

  // 境界条件
  result = trivial_check(top_var, func0, func1);
  if ( result.is_zero() ) {
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
      std::cout << "merge end: " << func_str(func) << std::endl
		<< "merge at " << top_var << std::endl
		<< "r0: " << std::endl;
      print(std::cout, r0);
      std::cout << "r1: " << std::endl;
      print(std::cout, r1);
      std::cout << "  ==> ";
      print(std::cout, result);
      if ( result.global_func(func.input_num()) != func ) {
	std::cout << "result.global_func(): "
		  << func_str(result.global_func(func.input_num()))
		  << std::endl
		  << "func:                 "
		  << func_str(func) << std::endl;
	throw std::logic_error{"function mismatch"};
      }
    }
  }

  // 結果を登録する．
  put_edge(func, result);

  if ( debug ) {
    std::cout << "decomp_step end" << std::endl
	      << " func:   " << func_str(func) << std::endl
	      << " result" << std::endl;
    print(std::cout, result);
  }

  return result;
}

// @brief 境界条件のチェックを行う．
TvDecEdge
TvDecMgr::trivial_check(
  SizeType var,
  const TvFunc& func0,
  const TvFunc& func1
)
{
  if ( func0.is_zero() ) {
    if ( func1.is_one() ) {
      // 肯定のリテラル
      return make_lit(var, false);
    }
    // リテラルとのAND( x and r1 )
    auto r1 = decomp_step(func1);
    return make_lit_and(var, false, r1);
  }
  if ( func0.is_one() ) {
    if ( func1.is_zero() ) {
      // 否定のリテラル
      return make_lit(var, true);
    }
    // リテラルとのOR( ~x or r1 )
    auto r1 = decomp_step(func1);
    return make_lit_or(var, true, r1);
  }

  // func0 は定数ではない．
  if ( func1.is_zero() ) {
    // リテラルとのAND( ~x and r0 )
    auto r0 = decomp_step(func0);
    return make_lit_and(var, true, r0);
  }
  if ( func1.is_one() ) {
    // リテラルとのOR( x or r0 )
    auto r0 = decomp_step(func0);
    return make_lit_or(var, false, r0);
  }
  if ( func0 == func1 ) {
    // この変数をスキップ
    auto r0 = decomp_step(func0);
    return r0;
  }
  if ( TvFunc::check_complement(func0, func1) ) {
    // リテラルとのXOR ( x xor r0 )
    auto r0 = decomp_step(func0);
    return make_lit_xor(var, false, r0);
  }

  // 自明な解ではなかった．
  return TvDecEdge::zero();
}

BEGIN_NONAMESPACE

inline
std::vector<TvDecEdge>
merge_list(
  TvDecEdge top,
  const std::vector<TvDecEdge>& edge_list
)
{
  auto new_list = edge_list;
  new_list.push_back(top);
  return new_list;
}

// 2つのリストが共通部分を持つ時 true を返す．
bool
check_intersect(
  const std::vector<SizeType>& list1,
  const std::vector<SizeType>& list2
)
{
  // list1, lit2 は整列していると仮定する．
  auto p1 = list1.begin();
  auto p2 = list2.begin();
  auto e1 = list1.end();
  auto e2 = list2.end();
  while ( p1 != e1 && p2 != e2 ) {
    auto v1 = *p1;
    auto v2 = *p2;
    if ( v1 < v2 ) {
      ++ p1;
    }
    else if ( v1 > v2 ) {
      ++ p2;
    }
    else { // v1 == v2
      return true;
    }
  }
  return false;
}

// リストの差分を求める．
std::vector<SizeType>
make_diff(
  const std::vector<SizeType>& list1,
  const std::vector<SizeType>& list2
)
{
  std::vector<SizeType> ans_list;
  ans_list.reserve(list1.size());
  // list1, lit2 は整列していると仮定する．
  auto p1 = list1.begin();
  auto p2 = list2.begin();
  auto e1 = list1.end();
  auto e2 = list2.end();
  while ( p1 != e1 && p2 != e2 ) {
    auto v1 = *p1;
    auto v2 = *p2;
    if ( v1 < v2 ) {
      ++ p1;
      ans_list.push_back(v1);
    }
    else if ( v1 > v2 ) {
      ++ p2;
    }
    else { // v1 == v2
      ++ p1;
      ++ p2;
    }
  }
  for ( ; p1 != e1; ++ p1 ) {
    ans_list.push_back(*p1);
  }
  return ans_list;
}

END_NONAMESPACE

// @brief コファクターの結果をマージする．
TvDecEdge
TvDecMgr::merge(
  SizeType var,
  TvDecEdge r0,
  TvDecEdge r1
)
{
  // 境界条件のチェック
  // decomp_step() 直下で呼ばれる時には trivial_check()
  // で適切に処理されているが，それ以外のパスではチェックが
  // 必要になる．
  if ( r0.is_zero() ) {
    if ( r1.is_zero() ) {
      // 多分ここに来ることはないはず
      return TvDecEdge::zero();
    }
    if ( r1.is_one() ) {
      return make_lit(var, false);
    }
    return make_lit_and(var, false, r1);
  }
  if ( r0.is_one() ) {
    if ( r1.is_zero() ) {
      return make_lit(var, true);
    }
    if ( r1.is_one() ) {
      // 多分ここに来ることはないはず
      return TvDecEdge::one();
    }
    return make_lit_or(var, true, r1);
  }
  if ( r1.is_zero() ) {
    return make_lit_and(var, true, r0);
  }
  if ( r1.is_one() ) {
    return make_lit_or(var, false, r0);
  }
  if ( r0 == r1 ) {
    // これもないはず．
    return r0;
  }
  if ( TvDecEdge::check_complement(r0, r1) ) {
    // 逆極性で等しい
    return make_lit_xor(var, false, r0);
  }

  auto inv0 = r0.inv();
  auto inv1 = r1.inv();
  auto node0 = r0.node();
  auto node1 = r1.node();
  auto nc0 = node0->child_num();
  auto nc1 = node1->child_num();

  // node0, node1 に共通な子ノードのリスト
  std::vector<TvDecEdge> common_list;
  // node0 のみの子ノードのリスト
  std::vector<TvDecEdge> rest0_list;
  // node1 のみの子ノードのリスト
  std::vector<TvDecEdge> rest1_list;
  // node0 の rest0_list の要素の位置番号(要素数が1の時のみ意味を持つ)
  SizeType rpos0;
  // node1 の rest1_list の要素の位置番号(要素数が1の時のみ意味を持つ)
  SizeType rpos1;
  common_list.reserve(std::min(nc0, nc1));
  rest0_list.reserve(nc0);
  rest1_list.reserve(nc1);
  { // 共通な子ノードのリストを作る．
    // 子供のリストは先頭の変数順にソートされている．
    SizeType i0 = 0;
    SizeType i1 = 0;
    while ( i0 < nc0 && i1 < nc1 ) {
      auto ce0 = node0->child(i0);
      auto ce1 = node1->child(i1);
      auto cnode0 = ce0.node();
      auto cnode1 = ce1.node();
      auto index0 = cnode0->top_var();
      auto index1 = cnode1->top_var();
      int diff = index0 - index1;
      if ( diff < 0 ) {
	rest0_list.push_back(ce0);
	rpos0 = i0;
	++ i0;
      }
      else if ( diff == 0 ) {
	// 先頭の変数が同じ
	if ( ce0 == ce1 ) {
	  // ノード(枝)も同一
	  common_list.push_back(ce0);
	}
	else {
	  // ノードは別
	  rest0_list.push_back(ce0);
	  rest1_list.push_back(ce1);
	  rpos0 = i0;
	  rpos1 = i1;
	}
	++ i0;
	++ i1;
      }
      else { // diff > 0
	rest1_list.push_back(ce1);
	rpos1 = i1;
	++ i1;
      }
    }
    while ( i0 < nc0 ) {
      auto ce0 = node0->child(i0);
      auto cnode0 = ce0.node();
      rest0_list.push_back(ce0);
      rpos0 = i0;
      ++ i0;
    }
    while ( i1 < nc1 ) {
      auto ce1 = node1->child(i1);
      auto cnode1 = ce1.node();
      rest1_list.push_back(ce1);
      rpos1 = i1;
      ++ i1;
    }
  }

  if ( debug ) {
    std::cout << "common[" << std::endl;
    for ( auto edge: common_list ) {
      print(std::cout, edge);
      std::cout << "===" << std::endl;
    }
    std::cout << "]" << std::endl;
    std::cout << "rest0[" << std::endl;
    for ( auto edge: rest0_list ) {
      print(std::cout, edge);
      std::cout << "===" << std::endl;
    }
    std::cout << "]" << std::endl;
    std::cout << "rest1[" << std::endl;
    for ( auto edge: rest1_list ) {
      print(std::cout, edge);
      std::cout << "===" << std::endl;
    }
    std::cout << "]" << std::endl;
  }

  if ( common_list.size() >= 1 ) {
    // Case1-OR
    // - 共にORノード
    // - 反転フラグも同じ
    // - 共通の子供を1つ以上もつ．
    if ( inv0 == inv1 && node0->is_or() && node1->is_or() ) {
      if ( debug ) {
	std::cout << "case1_or" << std::endl;
      }
      auto result = case1_or(var, common_list, rest0_list, rest1_list) * inv0;
      return result;
    }

    // Case1-XOR
    // - 共にXORノード
    // - 反転フラグは任意
    // - 共通の子供を1つ以上もつ．
    if ( node0->is_xor() && node1->is_xor() ) {
      if ( debug ) {
	std::cout << "case1_xor" << std::endl;
      }
      auto result = case1_xor(var, common_list, rest0_list, rest1_list, inv0, inv1);
      return result;
    }
  }

  // Case2-OR
  if ( node0->is_or() ) {
    // - node0 がORノード
    // - node0 の子供の一つが r1 と等しい．
    for ( SizeType i = 0; i < nc0; ++ i ) {
      if ( (node0->child(i) * inv0) == r1 ) {
	if ( debug ) {
	  std::cout << "case2_or(node0, " << i << ")" << std::endl;
	}
	auto result = case2_or(var, true, node0, inv0, i);
	return result;
      }
    }
  }
  if ( node1->is_or() ) {
    // - node1 がORノード
    // - node1 の子供の一つが r0 と等しい．
    for ( SizeType i = 0; i < nc1; ++ i ) {
      if ( (node1->child(i) * inv1) == r0 ) {
	if ( debug ) {
	  std::cout << "case2_or(node1, " << i << ")" << std::endl;
	}
	auto result = case2_or(var, false, node1, inv1, i);
	return result;
      }
    }
  }

  // Case2-XOR
  if ( node0->is_xor() ) {
    // - node0 がXORノード
    // - node0 の子供の一つが node1 と等しい．
    // - 極性は任意
    for ( SizeType i = 0; i < nc0; ++ i ) {
      if ( node0->child(i).node() == node1 ) {
	if ( debug ) {
	  std::cout << "case2_xor(node0, " << i << ")" << std::endl;
	}
	auto result = case2_xor(var, true, node0, inv0, i, inv1);
	return result;
      }
    }
  }
  if ( node1->is_xor() ) {
    // - node1 がXORノード
    // - node1 の子供の一つが node0 と等しい．
    // - 極性は任意
    for ( SizeType i = 0; i < nc1; ++ i ) {
      if ( node1->child(i).node() == node0 ) {
	if ( debug ) {
	  std::cout << "case2_xor(node1, " << i << ")" << std::endl;
	}
	auto result = case2_xor(var, false, node1, inv1, i, inv0);
	return result;
      }
    }
  }

  if ( node0->is_cplx() && node1->is_cplx() ) {
    auto lf0 = r0.local_func();
    auto lf1 = r1.local_func();
    if ( rest0_list.size() == 1 && rest1_list.size() == 1 ) {
      // r0, r1 が共に CPLX でただ一つの子ノード以外共通の場合
      auto r0_1 = rest0_list.front();
      auto r1_1 = rest1_list.front();

      // ローカル関数が等しいか調べる．
      // とはいえ，rpos0 と rpos1 が node0 と node1 で異なるので
      // そのまま直接比較はできない．
      // そこで, rpos0 と rpos1 を 0/1 に固定した関数を比較する．
      auto lf0_0 = lf0.bind(rpos0, true);
      auto lf0_1 = lf0.bind(rpos0, false);
      auto lf1_0 = lf1.bind(rpos1, true);
      auto lf1_1 = lf1.bind(rpos1, false);

      // lf0 に対して rpos0 を 0 に束縛した関数と
      // lf1 に対して rpos1 を 0 に束縛した関数が等価かどうか調べる．
      // lf0 に対して rpos0 を 1 に束縛した関数と
      // lf1 に対して rpos1 を 1 に束縛した関数が等価かどうか調べる．
      if ( lf0_0 == lf1_0 && lf0_1 == lf1_1 ) {
	// 一致した．
	if ( debug ) {
	  std::cout << "case1_cplx(1)" << std::endl;
	}
	auto result = case1_cplx(var, lf0_0, lf0_1, common_list, r0_1, r1_1);
	return result;
      }

      // f0 に対して rpos0 を 0 に束縛した関数と
      // f1 に対して rpos1 を 1 に束縛した関数が等価かどうか調べる．
      // f0 に対して rpos0 を 1 に束縛した関数と
      // f1 に対して rpos1 を 0 に束縛した関数が等価かどうか調べる．
      if ( lf0_0 == lf1_1 && lf0_1 == lf1_0 ) {
	// 一致した．
	if ( debug ) {
	  std::cout << "case1_cplx(2)" << std::endl;
	}
	auto result = case1_cplx(var, lf0_0, lf0_1, common_list, r0_1, ~r1_1);
	return result;
      }
    }
    else if ( rest0_list.empty() && rest1_list.empty() ) {
      // node0 と node1 の子供は完全に一致する．
      // どれかの子ノードを反転すると一致する場合
      auto nc = node0->child_num();
      for ( SizeType i = 0; i < nc; ++ i ) {
	auto lf0_0 = lf0.bind(i, true);
	auto lf0_1 = lf0.bind(i, false);
	auto lf1_0 = lf1.bind(i, true);
	auto lf1_1 = lf1.bind(i, false);
	if ( lf0_0 == lf1_1 && lf0_1 == lf1_0 ) {
	  // node0 と node1 のローカル関数が i 番目の子供を反転すると等しい場合
	  if ( debug ) {
	    std::cout << "case1_cplx2(" << i << ")" << std::endl;
	  }
	  auto result = case1_cplx2(var, lf0_0, lf0_1, common_list, i);
	  return result;
	}
      }
    }
  }

  // 以降はマージ後の根のノードが CPLX 確定
  auto sup0 = node0->support();
  auto sup1 = node1->support();
  if ( !check_intersect(sup0, sup1) ) {
    if ( debug ) {
      std::cout << "caseITE_cplx()" << std::endl;
    }
    // ITE(var, r1, r0) となる．
    // ただし，r0 と r1 の順番に注意
    auto lit = make_lit(var, false);
    if ( sup0.front() < sup1.front() ) {
      auto lf0 = TvFunc::literal(2, 0, r0.inv());
      auto lf1 = TvFunc::literal(2, 1, r1.inv());
      auto lf = TvFunc::compose(lf0, lf1);
      auto result = make_cplx(lf, {r0.normal_edge(), r1.normal_edge(), lit});
      return result;
    }
    else {
      auto lf0 = TvFunc::literal(2, 1, r0.inv());
      auto lf1 = TvFunc::literal(2, 0, r1.inv());
      auto lf = TvFunc::compose(lf0, lf1);
      auto result = make_cplx(lf, {r1.normal_edge(), r0.normal_edge(), lit});
      return result;
    }
  }

  auto sup0_diff = make_diff(sup0, sup1);
  auto sup1_diff = make_diff(sup1, sup0);
  if ( node0->is_cplx() && sup0_diff.size() > 0 && sup1_diff.size() == 0 ) {
    // node0 が cplx で node0 側に余分な入力があり，node1 側にはない場合．
    auto lf0 = r0.local_func();
    for ( SizeType i = 0; i < nc0; ++ i ) {
      auto cedge = node0->child(i);
      auto cnode = cedge.node();
      auto& csup = cnode->support();
      if ( !check_intersect(csup, sup1) ) {
	// lf0 に対して cedge が 0 になる割当を行うと lf1 と一致するかどうか調べる．
	// まず，node1 の推移的ファンインが node0 のファンインと一致するか調べる．
	auto child_list = find_common_inputs(node0, i, node1);
	// node1 のローカル関数をそのファンインベースで作り直す．
	auto lf1 = r1.local_func(child_list);
	auto lf0_0 = lf0.bind(i, true);
	auto lf0_1 = lf0.bind(i, false);
	if ( lf0_0 == lf1 ) {
	  auto lf_new = TvFunc::compose(lf0_0, lf0_1);
	  if ( debug ) {
	    std::cout << "case2_cplx(" << lf_new.hex_str()
		      << ", " << i
		      << ", true, true, node0)" << std::endl;
	  }
	  auto result = case2_cplx(var, lf_new, i, true, true, node0);
	  return result;
	}
	// lf0 に対して cedge が 1 になる割当を行うと lf1 と一致する場合
	if ( lf0_1 == lf1 ) {
	  auto lf_new = TvFunc::compose(lf0_0, lf0_1);
	  if ( debug ) {
	    std::cout << "case2_cplx(" << lf_new.hex_str()
		      << ", " << i
		      << ", false, false, node0)" << std::endl;
	  }
	  auto result = case2_cplx(var, lf_new, i, false, false, node0);
	  return result;
	}
      }
    }
  }
  if ( node1->is_cplx() && sup1_diff.size() > 0 && sup0_diff.size() == 0 ) {
    auto lf1 = r1.local_func();
    for ( SizeType i = 0; i < nc1; ++ i ) {
      auto cedge = node1->child(i);
      auto cnode = cedge.node();
      auto& csup = cnode->support();
      if ( !check_intersect(csup, sup0) ) {
	// lf1 に対して cedge が 0 になる割当を行うと lf0 と一致するかどうか調べる．
	// まず，node0 の推移的ファンインが node1 のファンインと一致するか調べる．
	auto child_list = find_common_inputs(node1, i, node0);
	// node0 のローカル関数をそのファンインベースで作り直す．
	auto lf0 = r0.local_func(child_list);
	auto lf1_0 = lf1.bind(i, true);
	auto lf1_1 = lf1.bind(i, false);
	if ( lf1_0 == lf0 ) {
	  auto lf_new = TvFunc::compose(lf1_0, lf1_1);
	  if ( debug ) {
	    std::cout << "case2_cplx(" << lf_new.hex_str()
		      << ", " << i
		      << "false, true, node1)" << std::endl;
	  }
	  auto result = case2_cplx(var, lf_new, i, false, true, node1);
	  return result;
	}
	// lf1 に対して cedge が 1 になる割当を行うと lf0 と一致する場合
	if ( lf1_1 == lf0 ) {
	  auto lf_new = TvFunc::compose(lf1_0, lf1_1);
	  if ( debug ) {
	    std::cout << "case2_cplx(" << lf_new.hex_str()
		      << ", " << i
		      << "true, false, node1)" << std::endl;
	  }
	  auto result = case2_cplx(var, lf_new, i, true, false, node1);
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
  auto child_list = find_common_inputs(node0, node1);
  if ( debug ) {
    std::cout << "child_list:";
    for ( auto edge: child_list ) {
      std::cout << " " << edge;
    }
    std::cout << std::endl;
  }
  auto top_edge = make_lit(var, false);
  auto lf0 = r0.local_func(child_list);
  auto lf1 = r1.local_func(child_list);
  auto lf = TvFunc::compose(lf0, lf1);
  auto new_child_list = merge_list(top_edge, child_list);
  auto result = make_cplx(lf, new_child_list);
  return result;
}

// @brief 共通な子ノードを求める．
std::vector<TvDecEdge>
TvDecMgr::find_common_inputs(
  const TvDecNode* node0,
  const TvDecNode* node1
)
{
  TvDecEdge r0(node0);
  TvDecEdge r1(node1);

  // node0 と node1 の推移的な子ノードのうち，共通なノードを求める．
  // 面倒なのは OR/XOR ノードが極大化されているということ．
  TvDecMark mark;
  // node0 の推移的ファンインに 1 のマークをつける．
  mark.mark_recur(r0, 1);
  // node1 の推移的ファンインに 2 のマークをつける．
  mark.mark_recur(r1, 2);
  // 自分には 3 のマークがなく，推移的ファンインに 3
  // のマークがついているノードのマークを 0 にする．
  mark.tfimark_recur(r0);
  mark.tfimark_recur(r1);
  // 境界ノードに印をつける．
  std::vector<const TvDecNode*> or_list0;
  std::vector<const TvDecNode*> xor_list0;
  mark.get_boundary(r0, or_list0, xor_list0);
  std::vector<const TvDecNode*> or_list1;
  std::vector<const TvDecNode*> xor_list1;
  mark.get_boundary(r1, or_list1, xor_list1);

  // 入力を求める．
  std::vector<TvDecEdge> tmp_inputs;
  find_uncommon_inputs(or_list0, mark, 1, tmp_inputs);
  find_uncommon_inputs(or_list1, mark, 2, tmp_inputs);
  find_uncommon_inputs(xor_list0, mark, 1, tmp_inputs);
  find_uncommon_inputs(xor_list1, mark, 2, tmp_inputs);
  merge_common_inputs(or_list0, or_list1, mark, tmp_inputs);
  merge_common_inputs(xor_list0, xor_list1, mark, tmp_inputs);
  mark.find_bnode(node0, tmp_inputs);
  mark.find_bnode(node1, tmp_inputs);
  std::sort(tmp_inputs.begin(), tmp_inputs.end(), EdgeLt());

  return tmp_inputs;
}

// @brief 共通な子ノードを求める．
std::vector<TvDecEdge>
TvDecMgr::find_common_inputs(
  const TvDecNode* node0,
  SizeType pos,
  const TvDecNode* node1
)
{
  TvDecEdge r1(node1);

  // node0 と node1 の推移的な子ノードのうち，共通なノードを求める．
  // 面倒なのは OR/XOR ノードが極大化されているということ．
  TvDecMark mark;
  // node0 の推移的ファンインに 1 のマークをつける．
  // ただし pos の子供は除外する．
  for ( SizeType i = 0; i < node0->child_num(); ++ i ) {
    if ( i != pos ) {
      mark.mark_recur(node0->child(i), 1);
    }
  }
  // node1 の推移的ファンインに 2 のマークをつける．
  mark.mark_recur(r1, 2);
  // 自分には 3 のマークがなく，推移的ファンインに 3
  // のマークがついているノードのマークを 0 にする．
  // ただし pos の子供は除外する．
  for ( SizeType i = 0; i < node0->child_num(); ++ i ) {
    if ( i != pos ) {
      mark.tfimark_recur(node0->child(i));
    }
  }
  mark.tfimark_recur(r1);
  // 境界ノードに印をつける．
  std::vector<const TvDecNode*> or_list0;
  std::vector<const TvDecNode*> xor_list0;
  for ( SizeType i = 0; i < node0->child_num(); ++ i ) {
    if ( i != pos ) {
      mark.get_boundary(node0->child(i), or_list0, xor_list0);
    }
  }
  std::vector<const TvDecNode*> or_list1;
  std::vector<const TvDecNode*> xor_list1;
  mark.get_boundary(r1, or_list1, xor_list1);

  // 入力を求める．
  std::vector<TvDecEdge> tmp_inputs;
  find_uncommon_inputs(or_list0, mark, 1, tmp_inputs);
  find_uncommon_inputs(or_list1, mark, 2, tmp_inputs);
  find_uncommon_inputs(xor_list0, mark, 1, tmp_inputs);
  find_uncommon_inputs(xor_list1, mark, 2, tmp_inputs);
  merge_common_inputs(or_list0, or_list1, mark, tmp_inputs);
  merge_common_inputs(xor_list0, xor_list1, mark, tmp_inputs);
  for ( SizeType i = 0; i < node0->child_num(); ++ i ) {
    if ( i != pos ) {
      auto inode = node0->child(i).node();
      mark.find_bnode(inode, tmp_inputs);
    }
  }
  mark.find_bnode(node1, tmp_inputs);
  std::sort(tmp_inputs.begin(), tmp_inputs.end(), EdgeLt());

  return tmp_inputs;
}

// @brief 共通でないファンインを求める．
void
TvDecMgr::find_uncommon_inputs(
  const std::vector<const TvDecNode*>& node_list,
  TvDecMark& mark,
  int mval,
  std::vector<TvDecEdge>& inputs
)
{
  for ( auto node: node_list ) {
    auto nc = node->child_num();
    std::vector<TvDecEdge> tmp_inputs;
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
	inputs.push_back(tmp_edge.normal_edge());
      }
      else {
	auto tmp_edge = make_xor(tmp_inputs);
	inputs.push_back(tmp_edge.normal_edge());
      }
      for ( auto edge: tmp_inputs ) {
	auto node = edge.node();
	mark.set_bmark(node, 3);
      }
    }
  }
}

// @brief 共通なノードリストをマージする．
void
TvDecMgr::merge_common_inputs(
  const std::vector<const TvDecNode*>& node_list1,
  const std::vector<const TvDecNode*>& node_list2,
  TvDecMark& mark,
  std::vector<TvDecEdge>& inputs
)
{
  for ( auto node1: node_list1 ) {
    auto nc1 = node1->child_num();
    std::unordered_set<TvDecEdge> common_set;
    for ( SizeType i = 0; i < nc1; ++ i ) {
      auto cedge = node1->child(i);
      auto cnode = cedge.node();
      if ( mark.bmark(cnode) == 1 && mark.mark(cnode) == 3 ) {
	common_set.emplace(cedge);
      }
    }
    for ( auto node2: node_list2 ) {
      auto nc2 = node2->child_num();
      std::vector<TvDecEdge> common;
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
	  inputs.push_back(TvDecEdge{node1});
	}
	else if ( nc == nc2 ) {
	  inputs.push_back(TvDecEdge{node2});
	}
	else if ( node1->is_or() ) {
	  auto cedge = make_or(common);
	  inputs.push_back(cedge.normal_edge());
	}
	else {
	  auto cedge = make_xor(common);
	  inputs.push_back(cedge.normal_edge());
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
TvDecEdge
TvDecMgr::case1_or(
  SizeType var,
  const std::vector<TvDecEdge>& common_list,
  const std::vector<TvDecEdge>& rest0_list,
  const std::vector<TvDecEdge>& rest1_list
)
{
  if ( debug ) {
    std::cout << "[case1_or] merge at " << var << std::endl
	      << "rest0_list:" << std::endl;
    for ( auto e: rest0_list ) {
      print(std::cout, e);
    }
    std::cout << "res1_list:" << std::endl;
    for ( auto e: rest1_list ) {
      print(std::cout, e);
    }
    std::cout << std::endl;
  }

  // new_edge = ITE(var, OR(rest0_list), OR(rest1_list))
  // OR(common_list, new_edge)
  // の構造を作る．
  // rest0_list/rest1_list の要素数が0や1でも
  // merge() がちゃんと対応してくれる．
  auto tmp0_edge = make_or(rest0_list);
  auto tmp1_edge = make_or(rest1_list);
  auto new_edge = merge(var, tmp0_edge, tmp1_edge);
  if ( debug ) {
    std::cout << "[case1_or] merge end" << std::endl
	      << "merge at " << var << std::endl
	      << "  ==> ";
    print(std::cout, new_edge);
  }
  // new_edge が zero() でも正しく動く．
  return make_or(common_list, new_edge);
}

// @brief Case1-XOR のマージを行う．
TvDecEdge
TvDecMgr::case1_xor(
  SizeType var,
  const std::vector<TvDecEdge>& common_list,
  const std::vector<TvDecEdge>& rest0_list,
  const std::vector<TvDecEdge>& rest1_list,
  bool inv0,
  bool inv1
)
{
  if ( debug ) {
    std::cout << "[case1_xor] merge at " << var << std::endl
	      << "rest0_list:" << std::endl;
    for ( auto e: rest0_list ) {
      print(std::cout, e);
    }
    std::cout << "res1_list:" << std::endl;
    for ( auto e: rest1_list ) {
      print(std::cout, e);
    }
    std::cout << std::endl;
  }

  // new_edge = ITE(var, XOR(rest0_list), OR(rest1_list))
  // XOR(common_list, new_edge)
  // の構造を作る．
  // rest0_list/rest1_list の要素数が0や1でも
  // merge() がちゃんと対応してくれる．
  auto tmp0_edge = make_xor(rest0_list) * inv0;
  auto tmp1_edge = make_xor(rest1_list) * inv1;
  auto new_edge = merge(var, tmp0_edge, tmp1_edge);
  if ( debug ) {
    std::cout << "[case1_xor] merge end" << std::endl
	      << "merge at " << var << std::endl
	      << "  ==> ";
    print(std::cout, new_edge);
  }

  // new_edge が zero でも正しく動く
  return make_xor(common_list, new_edge);
}

// @brief Case1-CPLX のマージを行う．
TvDecEdge
TvDecMgr::case1_cplx(
  SizeType var,
  const TvFunc& lf0,
  const TvFunc& lf1,
  const std::vector<TvDecEdge>& child_list,
  TvDecEdge rest0,
  TvDecEdge rest1
)
{
  auto lf = TvFunc::compose(lf0, lf1);
  auto top_edge = merge(var, rest0, rest1);
  auto new_child_list = merge_list(top_edge, child_list);
  return make_cplx(lf, new_child_list);
}

// @brief CASE1-CPLX(Type 2) のマージを行う．
TvDecEdge
TvDecMgr::case1_cplx2(
  SizeType var,
  const TvFunc& lf0,
  const TvFunc& lf1,
  const std::vector<TvDecEdge>& child_list,
  SizeType pos
)
{
  auto lf = TvFunc::compose(lf0, lf1);
  // child_list の pos 番目の子ノードと var との XOR を作る．
  auto chd0 = child_list[pos];
  auto top_edge = make_lit_xor(var, false, chd0);
  // 残りの子供を new_child_list にコピーする．
  std::vector<TvDecEdge> new_child_list;
  auto nc = child_list.size();
  new_child_list.reserve(nc);
  for ( SizeType i = 0; i < nc; ++ i ) {
    if ( i != pos ) {
      auto chd = child_list[i];
      new_child_list.push_back(chd);
    }
  }
  new_child_list.push_back(top_edge);
  return make_cplx(lf, new_child_list);
}

// @brief CASE2-OR のマージを行う．
TvDecEdge
TvDecMgr::case2_or(
  SizeType var,
  bool lit_inv,
  const TvDecNode* node,
  bool oinv,
  SizeType pos
)
{
  // pos 以外の子ノードのORノードを作る．
  std::vector<TvDecEdge> tmp_list;
  auto n = node->child_num();
  tmp_list.reserve(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    if ( i != pos ) {
      tmp_list.push_back(node->child(i));
    }
  }
  auto tmp_edge = make_or(tmp_list);
  // それと var とのANDを作る．
  auto top_edge = make_lit_and(var, lit_inv, tmp_edge);
  // それと r1 との ORを作る．
  auto r1 = node->child(pos);
  auto result = make_or({r1, top_edge}) * oinv;
  return result;
}

// @brief CASE2-XOR のマージを行う．
TvDecEdge
TvDecMgr::case2_xor(
  SizeType var,
  bool lit_inv,
  const TvDecNode* node,
  bool oinv,
  SizeType pos,
  bool inv1
)
{
  if ( debug ) {
    std::cout << "case2_xor" << std::endl;
  }
  // pos 以外の子ノードのXORノードを作る．
  std::vector<TvDecEdge> tmp_list;
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
  TvDecEdge top_edge;
  if ( inv1 ) {
    top_edge = make_lit_or(var, !lit_inv, tmp_edge);
  }
  else {
    top_edge = make_lit_and(var, lit_inv, tmp_edge);
  }
  // それと r1 との XOR を作る．
  auto r1 = node->child(pos);
  auto result = make_xor({r1, top_edge});
  return result;
}

// @brief CASE2-CPLX のマージを行う．
TvDecEdge
TvDecMgr::case2_cplx(
  SizeType var,
  const TvFunc& lf,
  SizeType pos,
  bool lit_inv,
  bool is_and,
  const TvDecNode* node
)
{
  TvDecEdge top_edge;
  auto cedge = node->child(pos);
  if ( is_and ) {
    top_edge = make_lit_and(var, lit_inv, cedge);
  }
  else {
    top_edge = make_lit_or(var, lit_inv, cedge);
  }
  // pos 以外の子供を tmp_list に入れる．
  auto nc = node->child_num();
  std::vector<TvDecEdge> tmp_list;
  tmp_list.reserve(nc);
  for ( SizeType i = 0; i < nc; ++ i ) {
    if ( i != pos ) {
      auto chd = node->child(i);
      tmp_list.push_back(chd);
    }
  }
  tmp_list.push_back(top_edge);
  auto result = make_cplx(lf, tmp_list);
  return result;
}

// @brief LITノードを作る．
TvDecEdge
TvDecMgr::make_lit(
  SizeType var,
  bool inv
)
{
  if ( var >= mLitList.size() ) {
    throw std::out_of_range{"var is out of range"};
  }
  auto node = mLitList[var];
  auto result = TvDecEdge(node, inv);
  return result;
}

// @brief ORノードを作る．
TvDecEdge
TvDecMgr::make_or(
  const std::vector<TvDecEdge>& child_list,
  TvDecEdge ex_edge
)
{
  if ( child_list.empty() ) {
    return ex_edge;
  }
  if ( child_list.size() == 1 && ex_edge.is_zero() ) {
    // 子供が1人ならその子供を返す．
    return child_list[0];
  }

  if ( debug ) {
    std::cout << "make_or begin" << std::endl;
    for ( auto child: child_list ) {
      print(std::cout, child);
      std::cout << "===" << std::endl;
    }
    if ( !ex_edge.is_zero() ) {
      print(std::cout, ex_edge);
      std::cout << "===" << std::endl;
    }
  }

  // 本当の子供のリストを作る．
  // child_list の中で肯定のORノードがあれば
  // その子ノードを本当の子ノードにする．
  std::vector<TvDecEdge> tmp_list;
  std::vector<SizeType> support;
  for ( auto child: child_list ) {
    auto node = child.node();
    if ( child.inv() == false && node->is_or() ) {
      for ( SizeType i = 0; i < node->child_num(); ++ i ) {
	tmp_list.push_back(node->child(i));
      }
    }
    else {
      tmp_list.push_back(child);
    }
  }
  if ( !ex_edge.is_zero() ) {
    auto node = ex_edge.node();
    if ( ex_edge.inv() == false && node->is_or() ) {
      for ( SizeType i = 0; i < node->child_num(); ++ i ) {
	tmp_list.push_back(node->child(i));
      }
    }
    else {
      tmp_list.push_back(ex_edge);
    }
  }
  // top の昇順になるようにソートする．
  std::sort(tmp_list.begin(), tmp_list.end(), EdgeLt());
  // 等価なノードがあるか調べる．
  auto node = find_or_node(tmp_list);
  if ( node == nullptr ) {
    // なかったので作る．
    SizeType id = mNodeList.size();
    node = TvDecNode::new_or(id, tmp_list);
    mNodeList.push_back(std::unique_ptr<TvDecNode>{node});
    put_or_node(tmp_list, node);
  }
  return TvDecEdge{node};
}

// @brief XORノードを作る．
TvDecEdge
TvDecMgr::make_xor(
  const std::vector<TvDecEdge>& child_list,
  TvDecEdge ex_edge
)
{
  if ( child_list.empty() ) {
    return ex_edge;
  }
  if ( child_list.size() == 1 && ex_edge.is_zero() ) {
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

  // child_list の中でXORノードがあれば
  // その子ノードを本当の子ノードにする．
  // ORの場合と異なり極性は問わない．
  // ただし，XORノードの子供には反転属性をつけない．
  // 極性は根に移す．
  std::vector<TvDecEdge> tmp_list;
  bool oinv = false;
  for ( auto child: child_list ) {
    auto inv = child.inv();
    oinv ^= inv;
    auto node = child.node();
    if ( node->is_xor() ) {
      for ( SizeType i = 0; i < node->child_num(); ++ i ) {
	auto child1 = node->child(i);
	tmp_list.push_back(child1.normal_edge());
	oinv ^= child1.inv();
      }
    }
    else {
      tmp_list.push_back(child.normal_edge());
    }
  }
  if ( !ex_edge.is_zero() ) {
    auto inv = ex_edge.inv();
    oinv ^= inv;
    auto node = ex_edge.node();
    if ( node->is_xor() ) {
      for ( SizeType i = 0; i < node->child_num(); ++ i ) {
	auto child1 = node->child(i);
	tmp_list.push_back(child1.normal_edge());
	oinv ^= child1.inv();
      }
    }
    else {
      tmp_list.push_back(ex_edge.normal_edge());
    }
  }
  // top の昇順になるようにソートする．
  std::sort(tmp_list.begin(), tmp_list.end(), EdgeLt());
  // 等価なノードがあるか調べる．
  auto node = find_xor_node(tmp_list);
  if ( node == nullptr ) {
    // なかったので実際に作る．
    SizeType id = mNodeList.size();
    node = TvDecNode::new_xor(id, tmp_list);
    mNodeList.push_back(std::unique_ptr<TvDecNode>{node});
    put_xor_node(tmp_list, node);
  }
  return TvDecEdge(node, oinv);
}

// @brief CPLXノードを作る．
TvDecEdge
TvDecMgr::make_cplx(
  const TvFunc& lf,
  const std::vector<TvDecEdge>& child_list
)
{
  // lf を正規化する．
  auto inv = lf.value(0) == 1;
  auto lf_normal = lf * inv;

  // 辞書を探す．
  auto node = find_cplx_node(lf_normal, child_list);
  if ( node == nullptr ) {
    SizeType id = mNodeList.size();
    node = TvDecNode::new_cplx(id, lf_normal, child_list);
    mNodeList.push_back(std::unique_ptr<TvDecNode>{node});
    put_cplx_node(lf_normal, child_list, node);
  }
  auto result = TvDecEdge(node, inv);
  return result;
}

// @brief ORノードを探す
TvDecNode*
TvDecMgr::find_or_node(
  const std::vector<TvDecEdge>& child_list
) const
{
  auto sig = or_signature(child_list);
  if ( mNodeDict.count(sig) > 0 ) {
    return mNodeDict.at(sig);
  }
  return nullptr;
}

// @brief ORノードを登録する．
void
TvDecMgr::put_or_node(
  const std::vector<TvDecEdge>& child_list,
  TvDecNode* node
)
{
  auto sig = or_signature(child_list);
  mNodeDict.emplace(sig, node);
}

// @brief XORノードを探す
TvDecNode*
TvDecMgr::find_xor_node(
  const std::vector<TvDecEdge>& child_list
) const
{
  auto sig = xor_signature(child_list);
  if ( mNodeDict.count(sig) > 0 ) {
    return mNodeDict.at(sig);
  }
  return nullptr;
}

// @brief ORノードを登録する．
void
TvDecMgr::put_xor_node(
  const std::vector<TvDecEdge>& child_list,
  TvDecNode* node
)
{
  auto sig = xor_signature(child_list);
  mNodeDict.emplace(sig, node);
}

// @brief CPLXノードを探す．
TvDecNode*
TvDecMgr::find_cplx_node(
  const TvFunc& local_func,
  const std::vector<TvDecEdge>& child_list
) const
{
  auto sig = cplx_signature(local_func, child_list);
  if ( mNodeDict.count(sig) > 0 ) {
    return mNodeDict.at(sig);
  }
  return nullptr;
}

// @brief CPLXノードを登録する．
void
TvDecMgr::put_cplx_node(
  const TvFunc& local_func,
  const std::vector<TvDecEdge>& child_list,
  TvDecNode* node
)
{
  auto sig = cplx_signature(local_func, child_list);
  mNodeDict.emplace(sig, node);
}

// @brief 関数に対応する TvDecEdge を探す．
bool
TvDecMgr::find_edge(
  const TvFunc& f,
  TvDecEdge& result
) const
{
  // (0, 0, 0, ...) の出力が0になるように正規化する．
  auto inv = f.value(0) == 1;
  auto f_normal = f * inv;
  if ( mEdgeDict.count(f_normal) == 0 ) {
    return false;
  }
  result = mEdgeDict.at(f_normal) * inv;
  return true;
}

// @brief 関数に対応する TvDecEdge を登録する．
void
TvDecMgr::put_edge(
  const TvFunc& f,
  TvDecEdge result
)
{
  // (0, 0, 0, ...) の出力が0になるように正規化する．
  auto inv = f.value(0) == 1;
  auto f_normal = f * inv;
  result *= inv;
  mEdgeDict.emplace(f_normal, result);
}

BEGIN_NONAMESPACE

void
print_sub(
  std::ostream& s,
  TvDecEdge e
)
{
  auto node = e.node();
  if ( node == nullptr ) {
    return;
  }

  node->print(s);
  s << "  [";
  const char* sep = "";
  for ( auto x: node->support() ) {
    s << sep << x;
    sep = ", ";
  }
  s << "]" << std::endl;
  auto nc = node->child_num();
  for ( SizeType i = 0; i < nc; ++ i ) {
    auto edge1 = node->child(i);
    print_sub(s, edge1);
  }
}

END_NONAMESPACE

void
TvDecMgr::print(
  std::ostream& s,
  TvDecEdge edge
)
{
  s << "root: ";
  edge.print(s);
  s << std::endl;
  print_sub(s, edge);
  s << std::endl;
}

END_NAMESPACE_YM_DJDEC
