
/// @file IdentOp.cc
/// @brief IdentOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AigMgr.h"
#include "AigMgrImpl.h"
#include "IdentOp.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス AigMgr
//////////////////////////////////////////////////////////////////////

// @brief 構造が等しいか調べる．
bool
AigMgr::is_identical(
  const AigMgr& right
) const
{
  IdentOp op(get(), right.get());
  return op.check();
}


//////////////////////////////////////////////////////////////////////
// クラス IdentOp
//////////////////////////////////////////////////////////////////////

// @brief 構造が等しければ true を返す．
bool
IdentOp::check()
{
  // まず要素数が一致しているか調べる．
  // ただしノード数は異なっていても未使用のノードがあれば
  // 一致する可能性がある．
  auto ni = mMgr1->input_num();
  if ( mMgr2->input_num() != ni ) {
    return false;
  }
  auto no = mMgr1->output_num();
  if ( mMgr2->output_num() != no ) {
    return false;
  }
  auto nf = mMgr1->dff_num();
  if ( mMgr2->dff_num() != nf ) {
    return false;
  }

  // 入力ノードの対応付けを行う．
  for ( SizeType i = 0; i < ni; ++ i ) {
    auto node1 = mMgr1->input_node(i);
    auto node2 = mMgr2->input_node(i);
    mNodeMap.emplace(node1->id(), node2);
  }
  // DFFの出力の対応付けを行う．
  for ( SizeType i = 0; i < nf; ++ i ) {
    auto node1 = mMgr1->dff_output(i);
    auto node2 = mMgr2->dff_output(i);
    mNodeMap.emplace(node1->id(), node2);
  }

  // 出力の対応付けを行う．
  for ( SizeType i = 0; i < no; ++ i ) {
    auto edge1 = mMgr1->output(i)->edge();
    auto edge2 = mMgr2->output(i)->edge();
    if ( !check_edge(edge1, edge2) ) {
      return false;
    }
  }

  // DFF の入力の対応付けが取れているか調べる．
  for ( SizeType i = 0; i < nf; ++ i ) {
    auto edge1 = mMgr1->dff_input(i)->edge();
    auto edge2 = mMgr2->dff_input(i)->edge();
    if ( !check_edge(edge1, edge2) ) {
      return false;
    }
  }

  return true;
}

// @brief 枝野対応付けが取れているか調べる．
bool
IdentOp::check_edge(
  AigEdge edge1,
  AigEdge edge2
)
{
  if ( edge1.is_const() ) {
    return edge1 == edge2;
  }
  if ( edge1.inv() != edge2.inv() ) {
    return false;
  }

  auto node1 = edge1.node();
  auto node2 = edge2.node();
  auto node3 = node_match(node1);
  return node2 == node3;
}

// @brief 対応するノードを返す．
AigNode*
IdentOp::node_match(
  AigNode* node
)
{
  if ( mNodeMap.count(node->id()) > 0 ) {
    return mNodeMap.at(node->id());
  }
  // 入力ノードは mNodeMap に登録されているはず．

  // ファンイン0に対応するノードを求める．
  auto node0 = node->fanin0_node();
  auto match0 = node_match(node0);
  if ( match0 == nullptr ) {
    return nullptr;
  }

  // ファンイン1に対応するノードを求める．
  auto node1 = node->fanin1_node();
  auto match1 = node_match(node1);
  if ( match1 == nullptr ) {
    return nullptr;
  }

  // 自身に対応するノードを求める．
  auto fanin0 = AigEdge(match0, node->fanin0_inv());
  auto fanin1 = AigEdge(match1, node->fanin1_inv());
  auto match = mMgr2->find_and(fanin0, fanin1);

  // 見つからなくても結果は記録する．
  mNodeMap.emplace(node->id(), match);
  return match;
}

END_NAMESPACE_YM_AIG
