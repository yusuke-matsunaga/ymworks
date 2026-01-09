
/// @file AigMgrImpl_copy.cc
/// @brief AigMgrImpl::copy の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "AigMgrImpl.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス AigMgrImpl
//////////////////////////////////////////////////////////////////////

// @brief 複製を作る．
void
AigMgrImpl::copy(
  AigMgrImpl* src_mgr
)
{
  _initialize(src_mgr->node_num(),
	      src_mgr->input_num(),
	      src_mgr->output_num(),
	      src_mgr->dff_num());

  // ノードの情報をコピーする．
  for ( auto& src_node: src_mgr->mNodeArray ) {
    _copy_node(src_node.get());
  }

  _finalize();
}

// @brief ノードの情報をコピーする．
void
AigMgrImpl::_copy_node(
  AigNode* src_node
)
{
  auto id = src_node->id();
  if ( src_node->is_primary_input() ) {
    auto input_id = src_node->input_id();
    _set_input(id, input_id);
    return;
  }

  if ( src_node->is_dff_output() ) {
    auto dff_id = src_node->dff_id();
    _set_dff_output(id, dff_id);
    return;
  }

  if ( src_node->is_and() ) {
    auto src_fanin0 = src_node->fanin0();
    auto src_fanin1 = src_node->fanin1();
    auto fanin0 = _copy_edge(src_fanin0);
    auto fanin1 = _copy_edge(src_fanin1);
    auto node = this->node(id);
    _set_and(node, fanin0, fanin1);
    return;
  }

  std::ostringstream buf;
  buf << "Node#" << id
      << " has unknown type";
  throw std::logic_error{buf.str()};
}


END_NAMESPACE_YM_AIG
