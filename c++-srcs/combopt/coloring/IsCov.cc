
/// @file IsCov.cc
/// @brief IsCov の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "IsCov.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_COLORING

//////////////////////////////////////////////////////////////////////
// クラス IsCov
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
IsCov::IsCov(
  const UdGraph& graph
) : mGraph{graph}
{
}

// @brief デストラクタ
IsCov::~IsCov()
{
}

// @brief independent set cover を行う．
SizeType
IsCov::covering(
  SizeType limit,
  std::vector<SizeType>& color_map
)
{
  auto remain_num = mGraph.node_num();
  while ( remain_num > limit ) {
    std::vector<SizeType> cand_list;
    cand_list.reserve(remain_num);
    for ( auto node_id: Range(mGraph.node_num()) ) {
      if ( mGraph.color(node_id) == 0 ) {
	cand_list.push_back(node_id);
      }
    }
    auto cur_mark = std::vector<bool>(mGraph.node_num(), false);
    std::vector<SizeType> iset;
    while ( !cand_list.empty() ) {
      auto node_id = select_node(cand_list, cur_mark);
      cur_mark[node_id] = true;
      iset.push_back(node_id);
      update_cand_list(cand_list, node_id);
    }
    auto num = iset.size();
    ASSERT_COND( num > 0 );
    SizeType cur_col = 0;
    for ( auto node_id: iset ) {
      if ( cur_col == 0 ) {
	cur_col = mGraph.new_color();
      }
      mGraph.set_color(node_id, cur_col);
    }
    remain_num -= num;
  }

  return mGraph.get_color_map(color_map);
}

// @brief 候補集合に加えるノードを選ぶ．
SizeType
IsCov::select_node(
  const std::vector<SizeType>& cand_list,
  const std::vector<bool>& cur_mark
)
{
  std::vector<SizeType> min_list;
  auto min_num = mGraph.node_num();
  for ( auto node_id: cand_list ) {
    auto c = mGraph.adj_list(node_id).size();
    if ( min_num >= c ) {
      if ( min_num > c ) {
	min_num = c;
	min_list.clear();
      }
      min_list.push_back(node_id);
    }
  }
  auto n = min_list.size();
  ASSERT_COND( n > 0 );
  if ( n == 1 ) {
    return min_list[0];
  }
  std::uniform_int_distribution<SizeType> rd(0, n - 1);
  auto r = rd(mRandGen);
  return min_list[r];
}

// @brief 候補リストを更新する．
void
IsCov::update_cand_list(
  std::vector<SizeType>& cand_list,
  SizeType node_id
)
{
  // node_id に隣接するノードに印を付ける．
  auto mark = std::vector<bool>(mGraph.node_num(), false);
  mark[node_id] = true;
  for ( auto node1_id: mGraph.adj_list(node_id) ) {
    mark[node1_id] = true;
  }

  auto n = cand_list.size();
  SizeType rpos = 0;
  SizeType wpos = 0;
  for ( rpos = 0; rpos < n; ++ rpos ) {
    auto node1_id = cand_list[rpos];
    if ( !mark[node1_id] ) {
      cand_list[wpos] = node1_id;
      ++ wpos;
    }
  }
  if ( wpos < n ) {
    cand_list.erase(cand_list.begin() + wpos, cand_list.end());
  }
}

END_NAMESPACE_YM_COLORING
