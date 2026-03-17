
/// @file UdGraph_coloring.cc
/// @brief UdGraph::coloring の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2021, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/UdGraph.h"
#include "Dsatur.h"
#include "IsCov.h"
#include "Isx.h"
#include "Isx2.h"
#include "TabuCol.h"


BEGIN_NAMESPACE_YM_UDGRAPH

// dsatur で彩色問題を解く．
inline
SizeType
dsatur(
  const UdGraph& graph,
  std::vector<SizeType>& color_map
)
{
  nsColoring::Dsatur dsat{graph};
  return dsat.coloring(color_map);
}

// dsatur で彩色問題を解く．
inline
SizeType
dsatur(
  const UdGraph& graph,
  std::vector<SizeType>& color_map,
  const std::vector<SizeType>& initial_color_map
)
{
  nsColoring::Dsatur dsat{graph, initial_color_map};
  return dsat.coloring(color_map);
}

// tabucol で彩色問題を解く．
SizeType
tabucol(
  const UdGraph& graph,
  std::vector<SizeType>& color_map
)
{
  // 初期解を Dsatur で得る．
  // ただし色数しか使わない．
  auto k0 = dsatur(graph, color_map);
  SizeType limit = 100000;
  SizeType L = 9;
  double alpha = 0.6;
  auto k1 = k0;
  for ( auto k = k0; k > 0; -- k ) {
    nsColoring::TabuCol tabucol{graph, k};
    std::vector<SizeType> color_map1;
    if ( tabucol.coloring(limit, L, alpha, color_map1) ) {
      k1 = k;
      color_map = color_map1;
    }
    else {
      break;
    }
  }
  return k1;
}

SizeType
UdGraph::coloring(
  std::vector<SizeType>& color_map,
  const JsonValue& option
) const
{
  auto algorithm = option.get_string_elem("algorithm", "dsatur");
  if ( algorithm == "dsatur" ) {
    return dsatur(*this, color_map);
  }
  if ( algorithm == "iscov" ) {
    nsColoring::IsCov iscsolver(*this);
    auto c = iscsolver.covering(500, color_map);
    return dsatur(*this, color_map, color_map);
  }
  if ( algorithm == "isx" ) {
    nsColoring::Isx isxsolver(*this);
    auto c = isxsolver.coloring(500, color_map);
    //cout << "isx end: c = " << c << endl;
    return dsatur(*this, color_map, color_map);
  }
  if ( algorithm == "isx2" ) {
    nsColoring::Isx2 isxsolver(*this);
    auto c = isxsolver.coloring(500, color_map);
    //cout << "isx2 end: c = " << c << endl;
    return dsatur(*this, color_map, color_map);
  }
  if ( algorithm == "tabucol" ) {
    return tabucol(*this, color_map);
  }

  // デフォルトフォールバック
  return dsatur(*this, color_map);
}

END_NAMESPACE_YM_UDGRAPH
