
/// @file FoMgr.cc
/// @brief FoMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "FoMgr.h"
#include "AigMgrImpl.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス FoMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FoMgr::FoMgr(
  AigMgrImpl* mgr
) : mMgr{mgr}
{
  mMgr->_attach(this);
  sanity_check();
}

// @brief デストラクタ
FoMgr::~FoMgr()
{
  mMgr->_dettach(this);
}

// @brief 入力側からBFSを行ってトポロジカル順のノードリストを得る．
std::vector<AigNode*>
FoMgr::make_node_list() const
{
  std::vector<AigNode*> node_list;
  node_list.reserve(mMgr->and_num());
  std::unordered_set<SizeType> mark;
  std::deque<AigNode*> queue;
  auto ni = mMgr->input_num();
  for ( SizeType i = 0; i < ni; ++ i ) {
    auto input = mMgr->input_node(i);
    if ( has_fo_info(input) ) {
      mark.emplace(input->id());
      queue.push_back(input);
    }
  }
  auto nf = mMgr->dff_num();
  for ( SizeType i = 0; i < nf; ++ i ) {
    auto output = mMgr->dff_output(i);
    if ( has_fo_info(output) ) {
      mark.emplace(output->id());
      queue.push_back(output);
    }
  }
  while ( !queue.empty() ) {
    auto node = queue.front();
    queue.pop_front();
    auto& fo = fo_info(node);
    for ( auto onode: fo.node_list ) {
      if ( mark.count(onode->id()) > 0 ) {
	continue;
      }
      auto node0 = onode->fanin0_node();
      auto node1 = onode->fanin1_node();
      if ( mark.count(node0->id()) == 0 ||
	   mark.count(node1->id()) == 0 ) {
	continue;
      }
      mark.emplace(onode->id());
      node_list.push_back(onode);
      queue.push_back(onode);
    }
  }
  return node_list;
}

// @brief 異常検査を行う．
void
FoMgr::sanity_check()
{
  for ( auto node: mMgr->and_list() ) {
    auto node0 = node->fanin0_node();
    auto& fo_info0 = fo_info(node0);
    if ( !fo_info0.check_node(node) ) {
      std::cout << "Node#" << node0->id()
		<< " -> Node#" << node->id() << std::endl;
      abort();
    }
    auto node1 = node->fanin1_node();
    auto& fo_info1 = fo_info(node1);
    if ( !fo_info1.check_node(node) ) {
      std::cout << "Node#" << node1->id()
		<< " -> Node#" << node->id() << std::endl;
      abort();
    }
  }
}

END_NAMESPACE_YM_AIG
