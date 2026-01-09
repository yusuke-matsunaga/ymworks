
/// @file UdGraph_max_clique.cc
/// @brief UdGraph::max_clique() の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/UdGraph.h"
#include "MclqGraph.h"


BEGIN_NAMESPACE_YM_MAXCLIQUE

extern std::vector<SizeType> MclqGreedy(const MclqGraph&);
extern std::vector<SizeType> MclqExact(const MclqGraph&);

END_NAMESPACE_YM_MAXCLIQUE

BEGIN_NAMESPACE_YM_UDGRAPH

// @brief 最大クリークを求める．
std::vector<SizeType>
UdGraph::max_clique(
  const JsonValue& option
) const
{
  nsMaxClique::MclqGraph graph{*this};

  auto algorithm = std::string{"greedy"}; // デフォルトフォールバック
  if ( option.is_object() && option.has_key("algorithm") ) {
    auto alg_obj = option.get("algorithm");
    algorithm = alg_obj.get_string();
  }

  if ( algorithm == "exact" ) {
    return MclqExact(graph);
  }
  if ( algorithm == "greedy" ) {
    return MclqGreedy(graph);
  }
  // デフォルトフォールバック
  return MclqGreedy(graph);
}

END_NAMESPACE_YM_UDGRAPH
