
/// @file MclqExact.cc
/// @brief max clique の厳密解法
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "MclqGraph.h"


BEGIN_NAMESPACE_YM_MAXCLIQUE

BEGIN_NONAMESPACE

static const int debug = 0;

struct NodeLt
{
  bool
  operator()(
    const std::pair<SizeType, SizeType>& p1,
    const std::pair<SizeType, SizeType>& p2
  )
  {
    return p1.second > p2.second;
  }
};

static SizeType count;

SizeType
mc_recur(
  const MclqGraph& graph,
  const std::vector<SizeType>& selected_node_list,
  const std::vector<SizeType>& rest_node_list,
  SizeType best_so_far,
  std::vector<SizeType>& node_set
)
{
  if ( debug ) {
    std::cout << "mc_recur(selected_node_list = " << selected_node_list.size()
	      << ", rest_node_list = " << rest_node_list.size()
	      << ", best_so_far = " << best_so_far << ")"
	      << std::endl;
  }

  if ( selected_node_list.size() + rest_node_list.size() <= best_so_far ) {
    return 0;
  }

  ++ count;
  if ( count >= 1000000 ) {
    return 0;
  }

  if ( rest_node_list.empty() ) {
    node_set.clear();
    for ( auto id: selected_node_list ) {
      node_set.push_back(id);
    }
    return node_set.size();
  }

  std::unordered_set<SizeType> mark;
  std::vector<std::pair<SizeType, SizeType>> tmp_node_list;
  tmp_node_list.reserve(rest_node_list.size());
  for ( auto id1: rest_node_list ) {
    mark.insert(id1);
  }

  for ( auto id1: rest_node_list ) {
    SizeType n = 0;
    for ( auto id2: graph.adj_list(id1) ) {
      if ( mark.count(id2) > 0 ) {
	++ n;
      }
    }
    tmp_node_list.push_back(std::make_pair(id1, n));
  }
  sort(tmp_node_list.begin(), tmp_node_list.end(),
       [](const std::pair<SizeType, SizeType>& p1,
	  const std::pair<SizeType, SizeType>& p2) {
	 return p1.second > p2.second;
       });

  auto max_val = best_so_far;
  for ( auto& p: tmp_node_list ) {
    auto id1 = p.first;
    std::unordered_set<SizeType> mark;
    for ( auto id2: graph.adj_list(id1) ) {
      mark.insert(id2);
    }

    std::vector<SizeType> new_node_list;
    new_node_list.reserve(rest_node_list.size());

    for ( auto id2: rest_node_list ) {
      if ( mark.count(id2) > 0 ) {
	new_node_list.push_back(id2);
      }
    }

    auto new_selected_node_list = selected_node_list;
    new_selected_node_list.push_back(id1);
    std::vector<SizeType> tmp_node_set;
    mc_recur(graph, new_selected_node_list, new_node_list, max_val, tmp_node_set);
    auto val = tmp_node_set.size();
    if ( max_val < val ) {
      max_val = val;
      node_set = tmp_node_set;
    }
  }

  if ( debug ) {
    std::cout << "mc_recur(selected_node_list = " << selected_node_list.size()
	      << ", rest_node_list = " << rest_node_list.size()
	      << ", best_so_far = " << best_so_far << ")"
	      << std::endl
	      << "  ==> " << max_val
	      << std::endl;
  }

  return max_val;
}

END_NONAMESPACE

// @brief 最大クリークを求める．
std::vector<SizeType>
MclqExact(
  const MclqGraph& graph
)
{
  // 答を入れるベクタ
  std::vector<SizeType> node_set;

  // 処理対象のノードを収めるリスト
  std::vector<SizeType> node_list;
  node_list.reserve(graph.node_num());
  for ( SizeType id = 0; id < graph.node_num(); ++ id ) {
    node_list.push_back(id);
  }

  count = 0;
  mc_recur(graph, {}, node_list, 0, node_set);

  return node_set;
}

END_NAMESPACE_YM_MAXCLIQUE
