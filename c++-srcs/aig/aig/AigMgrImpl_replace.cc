
/// @file replace.cc
/// @brief AigMgrImpl::replace() の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "AigMgrImpl.h"
#include "AigEdge.h"

#define DEBUG_AIGMGRIMPL_REPLACE 0
#define DOUT std::cout


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス AigMgrImpl
//////////////////////////////////////////////////////////////////////

// @brief ノード(枝)の置き換えを行う．
void
AigMgrImpl::_replace(
  AigNode* old_node,
  AigEdge new_edge
)
{
  struct RepInfo {
    AigNode* old_node;
    AigEdge new_edge;
  };

#if DEBUG_AIGMGRIMPL_REPLACE
  DOUT << "_replace(Node#" << old_node->id()
       << ", " << new_edge << ")" << std::endl;
  _fo_check();
#endif
  std::deque<RepInfo> queue;
  queue.push_back({old_node, new_edge});
  while ( !queue.empty() ) {
    auto& info = queue.front();
    auto old_node = info.old_node;
    auto new_edge = info.new_edge;
    queue.pop_front();
    for ( auto& fo_info: old_node->fo_info_list() ) {
      if ( fo_info.is_node() ) {
	auto node = fo_info.node();
	if ( node->ref_count() == 0 ) {
	  continue;
	}
	AigEdge edge0;
	AigEdge edge1;
	if ( node->fanin0_node() == old_node ) {
	  edge0 = new_edge * node->fanin0_inv();
	  edge1 = node->fanin1();
	}
	else if ( node->fanin1_node() == old_node ) {
	  edge0 = node->fanin0();
	  edge1 = new_edge * node->fanin1_inv();
	}
	else {
	  std::cout << "Node#" << node->id()
		    << " is in Node#" << old_node->id()
		    << "->mFoList" << std::endl
		    << " Node#" << node->id()
		    << "->fanin0 = Node#" << node->fanin0_node()->id() << std::endl
		    << " Node#" << node->id()
		    << "->fanin1 = Node#" << node->fanin1_node()->id() << std::endl;
	  throw std::logic_error{"FoInfo is broken"};
	}
	AigEdge new_edge1;
	if ( _special_case(edge0, edge1, new_edge1) ) {
	  queue.push_back({node, new_edge1});
	}
	else {
	  mAndTable.erase(node);
	  node->_set_and(edge0, edge1);
	  mAndTable.insert(node);
	}
      }
      else {
	auto terminal = fo_info.terminal();
	auto old_edge = terminal->edge();
	if ( old_edge.node() != old_node ) {
	  throw std::logic_error{"FoInfo is broken"};
	}
	auto edge0 = new_edge * old_edge.inv();
	terminal->_set_edge(edge0);
      }
    }
    if ( !new_edge.is_const() ) {
      auto new_node = new_edge.node();
      // old_node のファンアウト情報を new_node に移す．
      auto& fo_list1 = old_node->mFoList;
      auto nfo = fo_list1.size();
      auto& fo_list2 = new_node->mFoList;
      fo_list2.insert(fo_list2.end(), fo_list1.begin(), fo_list1.end());
      old_node->mFoList.clear();
      _deactivate(old_node);
      _inc_node_ref(new_node);
      new_node->mRefCount += (nfo - 1);
    }
  }
}

END_NAMESPACE_YM_AIG
