
/// @file AigMgr_cofactor.cc
/// @brief AigMgr::cofactor の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AigMgr.h"
#include "AigMgrImpl.h"
#include "CofactorOp.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス AigMgr
//////////////////////////////////////////////////////////////////////

// @brief コファクター演算
AigLiteral
AigMgr::cofactor(
  AigEdge literal,
  const std::vector<AigLiteral>& cube
)
{
  auto cube_edge_list = literal2edge_list(cube);
  CofactorOp op(get(), cube_edge_list);
  auto edge = op.call(literal);
  return edge2literal(edge);
}

// @brief コファクター演算
std::vector<AigLiteral>
AigMgr::cofactor(
  const std::vector<AigLiteral>& lit_list,
  const std::vector<AigLiteral>& cube
)
{
  auto cube_edge_list = literal2edge_list(cube);
  CofactorOp op(get(), cube_edge_list);


  std::vector<AigLiteral> new_lit_list;
  new_lit_list.reserve(lit_list.size());
  for ( auto lit: lit_list ) {
    auto edge = literal2edge(lit);
    auto new_edge = op.call(edge);
    auto new_lit = edge2literal(new_edge);
    new_lit_list.push_back(new_lit);
  }
  return new_lit_list;
}


//////////////////////////////////////////////////////////////////////
// クラス CofactorOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CofactorOp::CofactorOp(
  AigMgrImpl* mgr,
  const std::vector<AigEdge>& cedge_list
) : mMgr{mgr}
{
  for ( auto edge: cedge_list ) {
    if ( edge.is_const() ) {
      // 定数が来ることはないはず．
      // 今はスキップする．
      continue;
    }
    auto node = edge.node();
    auto inv = edge.inv();
    auto new_edge = edge.inv() ? AigEdge::zero() : AigEdge::one();
    put(node, new_edge);
  }
}

// @brief コファクター演算を行う．
AigEdge
CofactorOp::call(
  AigEdge edge
)
{
  if ( check(edge) ) {
    return get(edge);
  }
  auto node = edge.node();
  auto inv = edge.inv();
  AigEdge new_edge;
  if ( node->is_and() ) {
    auto e0 = node->fanin0();
    auto new_e0 = call(e0);
    auto e1 = node->fanin1();
    auto new_e1 = call(e1);
    new_edge = mMgr->new_and(new_e0, new_e1);
  }
  else {
    new_edge = AigEdge{node, false};
  }
  put(node, new_edge);
  return new_edge * inv;
}

END_NAMESPACE_YM_AIG
