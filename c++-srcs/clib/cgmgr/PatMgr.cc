
/// @file PatMgr.cc
/// @brief PatMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "cgmgr/PatMgr.h"
#include "cgmgr/PatNode.h"
#include "cgmgr/PatHandle.h"

#include "ym/Expr.h"
#include "ym/UnionFindSet.h"
#include "ym/PermGen.h"
#include "ym/MultiCombiGen.h"
#include "ym/MultiSetPermGen.h"
#include "ym/Range.h"


BEGIN_NONAMESPACE

#include "pat_table"

END_NONAMESPACE


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
// クラス PatMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PatMgr::PatMgr(
) : mHashTable{nullptr},
    mHashSize{0}
{
  alloc_table(1024);
}

// @brief デストラクタ
PatMgr::~PatMgr()
{
  delete [] mHashTable;
}

// @brief 初期化する．
void
PatMgr::init()
{
  mInputList.clear();
  mNodeList.clear();
  delete [] mHashTable;
  mHashTable = nullptr;
  mHashSize = 0;
  alloc_table(1024);
  mClassList.clear();
  mPatList.clear();
}

// @brief 全ノード数を返す．
SizeType
PatMgr::node_num() const
{
  return mNodeList.size();
}

// @brief ノードを返す．
const PatNode&
PatMgr::node(
  SizeType pos
) const
{
  if ( pos < 0 || node_num() <= pos ) {
    throw std::out_of_range{"pos is out of range"};
  }
  return *mNodeList[pos];
}

// @brief パタン数を返す．
SizeType
PatMgr::pat_num() const
{
  return mPatList.size();
}

// @brief パタンの根のハンドルを返す．
PatHandle
PatMgr::pat_root(
  SizeType id
) const
{
  if ( id < 0 || pat_num() <= id ) {
    throw std::out_of_range{"id is out of range"};
  }
  return mPatList[id];
}

// @brief パタンの属している代表クラスを返す．
const CiCellClass*
PatMgr::rep_class(
  SizeType id
) const
{
  if ( id < 0 || pat_num() <= id ) {
    throw std::out_of_range{"id is out of range"};
  }
  return mClassList[id];
}

// @brief 論理式から生成されるパタンを登録する．
void
PatMgr::reg_pat(
  const Expr& expr,
  const CiCellClass* rep_class
)
{
  if ( mExprListDict.count(rep_class) > 0 ) {
    // 同じ論理式を処理済みならなにもしない．
    auto& expr_list = mExprListDict.at(rep_class);
    for ( auto expr1: expr_list ) {
      if ( check_equivalent(expr, expr1) ) {
	return;
      }
    }
    // 論理式を登録しておく．
    expr_list.push_back(expr);
  }
  else {
    mExprListDict.emplace(rep_class, std::vector<Expr>{expr});
  }

  std::vector<PatHandle> tmp_pat_list;
  pg_sub(expr, tmp_pat_list);

  for ( auto pat1: tmp_pat_list ) {
    mClassList.push_back(rep_class);
    mPatList.push_back(pat1);
  }
}

// @brief 2つの論理式が同形かどうか調べる．
bool
PatMgr::check_equivalent(
  const Expr& expr1,
  const Expr& expr2
)
{
  if ( expr1.is_zero() ) {
    if ( expr2.is_zero() ) {
      return true;
    }
    return false;
  }
  if ( expr1.is_one() ) {
    if ( expr2.is_one() ) {
      return true;
    }
    return false;
  }
  if ( expr2.is_constant() ) {
    return false;
  }

  if ( expr1.is_positive_literal() ) {
    if ( expr2.is_positive_literal() && expr1.varid() == expr2.varid() ) {
      return true;
    }
    return false;
  }
  if ( expr1.is_negative_literal() ) {
    if ( expr2.is_negative_literal() && expr1.varid() == expr2.varid() ) {
      return true;
    }
    return false;
  }
  if ( expr2.is_literal() ) {
    return false;
  }

  auto n = expr1.operand_num();
  if ( expr2.operand_num() != n ) {
    return false;
  }

  if ( expr1.is_and() && !expr2.is_and() ) {
    return false;
  }
  if ( expr1.is_or() && !expr2.is_or() ) {
    return false;
  }
  if ( expr1.is_xor() && !expr2.is_xor() ) {
    return false;
  }

  for ( PermGen pg(n, n); !pg.is_end(); ++ pg ) {
    bool match = true;
    for ( auto i: Range(n) ) {
      if ( !check_equivalent(expr1.operand(i), expr2.operand(pg(i))) ) {
	match = false;
	break;
      }
    }
    if ( match ) {
      return true;
    }
  }
  return false;
}

// @brief 2つのパタンが同型かどうか調べる．
bool
PatMgr::check_equivalent(
  PatHandle handle1,
  PatHandle handle2
)
{
  if ( handle1.inv() != handle2.inv() ) {
    return false;
  }

  std::unordered_map<SizeType, SizeType> map1;
  std::unordered_map<SizeType, SizeType> map2;

  return ceq_sub(handle1.node(), handle2.node(), map1, map2);
}

// @brief check_equivalent の下請け関数
bool
PatMgr::ceq_sub(
  const PatNode& node1,
  const PatNode& node2,
  std::unordered_map<SizeType, SizeType>& map1,
  std::unordered_map<SizeType, SizeType>& map2)
{
  if ( node1.is_input() && node2.is_input() ) {
    auto id1 = node1.input_id();
    auto id2 = node2.input_id();
    if ( map1.count(id1) == 0 ) {
      map1[id1] = id2;
    }
    else {
      auto id2_reg = map1[id1];
      if ( id2_reg != id2 ) {
	return false;
      }
    }
    if ( map2.count(id2) == 0 ) {
      map2[id2] = id1;
    }
    else {
      auto id1_reg = map2[id1];
      if ( id1_reg != id1 ) {
	return false;
      }
    }
    return true;
  }

  if ( (!node1.is_and() || !node2.is_and()) &&
       (!node1.is_xor() || !node2.is_xor()) ) {
    return false;
  }

  if ( node1.fanin_inv0() != node2.fanin_inv0() ||
       node1.fanin_inv1() != node2.fanin_inv1() ) {
    return false;
  }

  if ( !ceq_sub(node1.fanin0(), node2.fanin0(), map1, map2) ) {
    return false;
  }

  if ( !ceq_sub(node1.fanin1(), node2.fanin1(), map1, map2) ) {
    return false;
  }

  return true;
}

// @brief パタングラフを生成する再帰関数
void
PatMgr::pg_sub(
  const Expr& expr,
  std::vector<PatHandle>& pg_list
)
{
  if ( expr.is_literal() ) {
    PatNode* node = make_input(expr.varid());
    bool inv = expr.is_negative_literal();
    pg_list.push_back(PatHandle{node, inv});
  }
  else {
    auto n = expr.operand_num();
    // ファンインの式に対するパタングラフを求める．
    std::vector<std::vector<PatHandle>> input_pg_list(n);
    std::vector<MultiGenBase::NK_pair> nk_array(n);
    for ( auto i = 0; i < n; ++ i ) {
      pg_sub(expr.operand(i), input_pg_list[i]);
      nk_array[i] = std::make_pair(input_pg_list[i].size(), 1);
    }

    // ファンインのパタンの組み合わせを列挙するオブジェクト
    for ( MultiCombiGen mcg(nk_array); !mcg.is_end(); ++ mcg ) {
      // 各ファンインから1つずつパタンを取り出して tmp_input に入れる．
      std::vector<PatHandle> tmp_input(n);
      for ( auto i = 0; i < n; ++ i ) {
	tmp_input[i] = input_pg_list[i][mcg(i, 0)];
      }

      // tmp_input のなかで同形なパタンを求める．
      UnionFindSet ufset(n);
      for ( int i1 = 1; i1 < n; ++ i1 ) {
	PatHandle pat1 = tmp_input[i1];
	for ( int i2 = 0; i2 < i1; ++ i2 ) {
	  PatHandle pat2 = tmp_input[i2];
	  if ( check_equivalent(pat1, pat2) ) {
	    ufset.merge(i1, i2);
	  }
	}
      }
      std::vector<SizeType> rep_id;
      rep_id.reserve(n);
      std::vector<SizeType> rev_map(n);
      for ( auto i = 0; i < n; ++ i ) {
	auto x = ufset.find(i);
	if ( x == i ) {
	  rev_map[i] = rep_id.size();
	  rep_id.push_back(x);
	}
      }
      auto ng = rep_id.size();

      // group_list[0〜(ng - 1)] に同形なパタンのリストを格納する．
      std::vector<std::vector<SizeType> > group_list(ng);
      for ( auto i = 0; i < n; ++ i ) {
	auto x = ufset.find(i);
	auto id = rev_map[x];
	group_list[id].push_back(i);
      }

      std::vector<SizeType> num_array(ng);
      for ( SizeType g = 0; g < ng; ++ g ) {
	num_array[g] = group_list[g].size();
      }

      std::vector<PatHandle> input(n);
      for ( MultiSetPermGen mspg(num_array, n); !mspg.is_end(); ++ mspg ) {
	std::vector<SizeType> count(ng, 0);
	for ( SizeType i = 0; i < n; ++ i ) {
	  auto g = mspg(i);
	  input[i] = tmp_input[group_list[g][count[g]]];
	  ++ count[g];
	}
	switch ( n ) {
	case 2:
	  {
	    PatHandle handle = make_node(expr, input[0], input[1]);
	    pg_list.push_back(handle);
	  }
	  break;

	case 3:
	  for ( SizeType i = 0; i < n_pat3; ++ i ) {
	    SizeType pos = 0;
	    PatHandle handle = make_bintree(expr, input, pat3[i], pos);
	    pg_list.push_back(handle);
	  }
	  break;

	case 4:
	  for ( SizeType i = 0; i < n_pat4; ++ i ) {
	    SizeType pos = 0;
	    PatHandle handle = make_bintree(expr, input, pat4[i], pos);
	    pg_list.push_back(handle);
	  }
	  break;

	case 5:
	  for ( SizeType i = 0; i < n_pat5; ++ i ) {
	    SizeType pos = 0;
	    PatHandle handle = make_bintree(expr, input, pat5[i], pos);
	    pg_list.push_back(handle);
	  }
	  break;

	case 6:
	  for ( SizeType i = 0; i < n_pat6; ++ i ) {
	    SizeType pos = 0;
	    PatHandle handle = make_bintree(expr, input, pat6[i], pos);
	    pg_list.push_back(handle);
	  }
	  break;

	case 7:
	  for ( SizeType i = 0; i < n_pat7; ++ i ) {
	    SizeType pos = 0;
	    PatHandle handle = make_bintree(expr, input, pat7[i], pos);
	    pg_list.push_back(handle);
	  }
	  break;

	case 8:
	  for ( SizeType i = 0; i < n_pat8; ++ i ) {
	    SizeType pos = 0;
	    PatHandle handle = make_bintree(expr, input, pat8[i], pos);
	    pg_list.push_back(handle);
	  }
	  break;

	default:
	  ASSERT_NOT_REACHED;
	  break;
	}
      }
    }
  }
}

// @brief テンプレートにしたがって2分木を作る．
PatHandle
PatMgr::make_bintree(
  const Expr& expr,
  const std::vector<PatHandle>& input,
  int pat[],
  SizeType& pos
)
{
  int p = pat[pos];
  ++ pos;
  if ( p == -1 ) {
    // 演算ノード
    PatHandle l_handle = make_bintree(expr, input, pat, pos);
    PatHandle r_handle = make_bintree(expr, input, pat, pos);
    return make_node(expr, l_handle, r_handle);
  }
  else {
    // 終端ノード
    return input[p];
  }
}

// @brief 入力ノードを作る．
PatNode*
PatMgr::make_input(
  SizeType var
)
{
  while ( mInputList.size() <= var ) {
    auto node = new_node();
    auto input_id = mInputList.size();
    node->mType = (input_id << 2) | PatNode::INPUT;
    mInputList.push_back(node);
  }
  auto node = mInputList[var];
  ASSERT_COND( node != nullptr );

  return node;
}

// @brief 論理式の種類に応じてノードを作る．
PatHandle
PatMgr::make_node(
  const Expr& expr,
  PatHandle l_handle,
  PatHandle r_handle
)
{
  auto& l_node = l_handle.node();
  auto& r_node = r_handle.node();
  bool l_inv = l_handle.inv();
  bool r_inv = r_handle.inv();

  bool oinv = false;
  SizeType type = 0U;
  if ( expr.is_and() ) {
    type = PatNode::AND;
  }
  else if ( expr.is_or() ) {
    type = PatNode::AND;
    l_inv = !l_inv;
    r_inv = !r_inv;
    oinv = true;
  }
  else if ( expr.is_xor() ) {
    type = PatNode::XOR;
    oinv = l_inv ^ r_inv;
    l_inv = false;
    r_inv = false;
  }
  else {
    ASSERT_NOT_REACHED;
  }
  if ( l_inv ) {
    type |= 4U;
  }
  if ( r_inv ) {
    type |= 8U;
  }

  // (type, l_node, r_node) というノードがすでにあったらそれを使う．
  SizeType pos = hash_func(type, l_node, r_node);
  SizeType idx = pos % mHashSize;
  for ( auto node = mHashTable[idx]; node; node = node->mLink ) {
    if ( node->mType == type &&
	 &node->fanin0() == &l_node &&
	 &node->fanin1() == &r_node ) {
      // おなじノードがあった．
      return PatHandle{node, oinv};
    }
  }

  // 新しいノードを作る．
  auto node = new_node();
  node->mType = type;
  node->mFanin[0] = &l_node;
  node->mFanin[1] = &r_node;

  // ハッシュ表に登録する．
  if ( node_num() >= mNextLimit ) {
    alloc_table(mHashSize * 2);
    // サイズが変わったのでインデックスを再計算する．
    idx = pos % mHashSize;
  }
  node->mLink = mHashTable[idx];
  mHashTable[idx] = node;

  return PatHandle{node, oinv};
}

// @brief ノードを作る．
PatNode*
PatMgr::new_node()
{
  auto id = mNodeList.size();
  auto node = new PatNode{id};
  mNodeList.push_back(std::unique_ptr<PatNode>{node});
  return node;
}

// @brief ハッシュ表を確保する．
void
PatMgr::alloc_table(
  SizeType req_size
)
{
  auto old_table = mHashTable;
  auto old_size = mHashSize;

  if ( mHashSize == 0 ) {
    mHashSize = 1024;
  }
  while ( mHashSize < req_size ) {
    mHashSize <<= 1;
  }
  mNextLimit = static_cast<SizeType>(mHashSize * 1.8);
  mHashTable = new PatNode*[mHashSize];
  for ( SizeType i = 0; i < mHashSize; ++ i) {
    mHashTable[i] = nullptr;
  }
  for ( SizeType i = 0; i < old_size; ++ i ) {
    PatNode* next = nullptr;
    for ( auto node = old_table[i]; node; node = next ) {
      next = node->mLink;
      auto pos = hash_func(node->mType, node->fanin0(), node->fanin1());
      auto idx = pos % mHashSize;
      node->mLink = mHashTable[idx];
      mHashTable[idx] = node;
    }
  }
  delete [] old_table;

}

// @brief PatNode のハッシュ関数
SizeType
PatMgr::hash_func(
  SizeType type,
  const PatNode& l_node,
  const PatNode& r_node)
{
  PtrIntType l = reinterpret_cast<PtrIntType>(&l_node);
  PtrIntType r = reinterpret_cast<PtrIntType>(&r_node);

  return type + l * 5 + r;
}


BEGIN_NONAMESPACE

// @brief パタングラフを DFS でたどって内容を val_list に入れる．
// @return 最大入力番号+1を返す．
SizeType
dfs(
  const PatNode& node,            ///< [in] ノード
  std::vector<bool>& vmark,       ///< [in] 訪れたかどうかの情報を持つ配列
  std::vector<SizeType>& val_list ///< [in] ノードの情報を格納するリスト
)
{
  if ( node.is_input() ) {
    return node.input_id() + 1;
  }
  if ( vmark[node.id()] ) {
    return 0;
  }
  vmark[node.id()] = true;
  val_list.push_back(node.id() * 2);
  auto id = dfs(node.fanin(0), vmark, val_list);
  val_list.push_back(node.id() * 2 + 1);
  auto id1 = dfs(node.fanin(1), vmark, val_list);
  if ( id < id1 ) {
    id = id1;
  }
  return id;
}

END_NONAMESPACE

// @brief パタンのノードリストを返す．
void
PatMgr::get_pat_info(
  SizeType id,
  SizeType& input_num,
  std::vector<SizeType>& node_list
) const
{
  if ( id < 0 || pat_num() <= id ) {
    throw std::out_of_range{"id is out of range"};
  }

  PatHandle root = pat_root(id);
  node_list.clear();
  node_list.reserve(node_num());
  std::vector<bool> vmark(node_num(), false);
  auto max_input = dfs(root.node(), vmark, node_list);
  input_num = max_input << 1;
  if ( root.inv() ) {
    input_num |= 1U;
  }
}


//////////////////////////////////////////////////////////////////////
// display() 関係の関数
//////////////////////////////////////////////////////////////////////

// @brief 内容を出力する．(デバッグ用)
void
PatMgr::display(
  std::ostream& s ///< [in] 出力先のストリーム
) const
{
  s << "*** PatMgr BEGIN ***" << std::endl;

  s << "*** NODE SECTION ***" << std::endl;
  auto n = node_num();
  for ( SizeType i = 0; i < n; ++ i ) {
    auto& node = this->node(i);

    s << "Node#" << node.id() << ": ";
    if ( node.is_input() ) {
      s << "Input#" << node.input_id();
    }
    else {
      if ( node.is_and() ) {
	s << "And";
      }
      else if ( node.is_xor() ) {
	s << "Xor";
      }
      else {
	ASSERT_NOT_REACHED;
      }
      s << "( ";
      display_edge(s, node, 0);
      s << ", ";
      display_edge(s, node, 1);
      s << ")";
    }
    s << std::endl;
  }
  s << std::endl;

  std::unordered_map<const CiCellClass*, SizeType> class_map;
  SizeType class_id = 0;
  s << "*** PATTERN SECTION ***" << std::endl;
  auto np = pat_num();
  for ( SizeType i = 0; i < np; ++ i ) {
    PatHandle root = pat_root(i);
    s << "Pat#" << i << ": ";
    if ( root.inv() ) {
      s << "~";
    }
    auto rep = rep_class(i);
    if ( class_map.count(rep) == 0 ) {
      class_map.emplace(rep, class_id);
      ++ class_id;
  }
    s << "Node#" << root.node().id()
      << " --> Rep#" << class_map.at(rep)
      << std::endl;
  }
  s << "*** PatMgr END ***" << std::endl;
}

// @brief 枝の情報を出力する．
void
PatMgr::display_edge(
  std::ostream& s,
  const PatNode& node,
  SizeType fanin_pos)
{
  if ( node.fanin_inv(fanin_pos) ) {
    s << "~";
  }
  s << "Node#" << node.fanin(fanin_pos).id();
}

END_NAMESPACE_YM_CLIB
