
/// @file AigMgrImpl.cc
/// @brief AigMgrImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "AigMgrImpl.h"

#define DEBUG_AIGMGRIMPL 0
#define DOUT std::cout


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス AigMgrImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AigMgrImpl::AigMgrImpl()
{
}

// @brief デストラクタ
AigMgrImpl::~AigMgrImpl()
{
}

// @brief ANDノード数を返す．
SizeType
AigMgrImpl::and_num() const
{
  SizeType count = 0;
  for ( auto& node: mNodeArray ) {
    if ( node->is_and() && node->ref_count() > 0 ) {
      ++ count;
    }
  }
  return count;
}

// @brief ノードを無効化する．
void
AigMgrImpl::_deactivate(
  AigNode* node
)
{
  node->_deactivate();
  if ( node->is_and() ) {
    _dec_node_ref(node->fanin0_node());
    _dec_node_ref(node->fanin1_node());
  }
}

// @brief 参照回数が0のノードを取り除く
void
AigMgrImpl::cleanup()
{
  // 各ノードの mFoList から参照回数が0のノードを取り除く．
  for ( auto& node_ptr: mNodeArray ) {
    auto& fo_list = node_ptr->mFoList;
    auto rpos = fo_list.begin();
    auto epos = fo_list.end();
    auto wpos = rpos;
    for ( ; rpos != epos; ++ rpos ) {
      auto& fo_info = *rpos;
      if ( fo_info.is_terminal() || fo_info.node()->ref_count() > 0 ) {
	if ( wpos != rpos ) {
	  *wpos = fo_info;
	}
	++ wpos;
      }
    }
    fo_list.erase(wpos, epos);
  }

  // mNodeArray は unique_ptr<> の配列なのでちょっと面倒
  // とりあえず，削除されるノードを末尾に寄せて最後に erase() する．
  auto rpos = mNodeArray.begin();
  auto epos = mNodeArray.end();
  auto wpos = rpos;
  SizeType id = 0;
  for ( ; rpos != epos; ++ rpos ) {
    auto& node_ptr = *rpos;
    if ( node_ptr->is_input() || node_ptr->ref_count() > 0 ) {
      node_ptr->mId = id;
      if ( wpos != rpos ) {
	std::swap(*wpos, node_ptr);
      }
      ++ id;
      ++ wpos;
    }
    else {
      auto node = node_ptr.get();
      mAndTable.erase(node);
    }
  }
  if ( wpos != epos ) {
    // ここで参照回数0のノードが開放される．
    mNodeArray.erase(wpos, epos);
  }

  _fo_check();
}

BEGIN_NONAMESPACE

void
and_dfs(
  AigNode* node,
  std::unordered_set<SizeType>& mark,
  std::vector<AigNode*>& node_list
)
{
  if ( node->is_input() ) {
    return;
  }
  if ( mark.count(node->id()) > 0 ) {
    return;
  }
  mark.emplace(node->id());
  if ( node->ref_count() == 0 ) {
    std::ostringstream buf;
    buf << "Node#" << node->id() << ".ref_count() == 0";
    std::cout << buf.str() << std::endl;
    throw std::logic_error{buf.str()};
  }
  auto node0 = node->fanin0_node();
  and_dfs(node0, mark, node_list);
  auto node1 = node->fanin1_node();
  and_dfs(node1, mark, node_list);
  node_list.push_back(node);
}

END_NONAMESPACE

// @brief ANDノードの入力側からのトポロジカル順のリストを得る．
std::vector<AigNode*>
AigMgrImpl::and_list() const
{
  std::vector<AigNode*> node_list;
  node_list.reserve(and_num());
  std::unordered_set<SizeType> mark;
  for ( auto& output: mOutputList ) {
    auto edge = output.terminal->edge();
    if ( edge.is_and() ) {
      auto node = edge.node();
      and_dfs(node, mark, node_list);
    }
  }
  for ( auto& dff: mDffList ) {
    auto edge = dff.input->edge();
    if ( edge.is_and() ) {
      auto node = edge.node();
      and_dfs(node, mark, node_list);
    }
  }
  return node_list;
}

// @brief ノードの参照回数を増やす．
void
AigMgrImpl::_inc_node_ref(
  AigNode* node
)
{
  if ( node->_inc_ref() ) {
    if ( node->is_and() ) {
      _inc_node_ref(node->fanin0_node());
      _inc_node_ref(node->fanin1_node());
    }
  }
}

// @brief ノードの参照回数を減らす．
void
AigMgrImpl::_dec_node_ref(
  AigNode* node
)
{
  if ( node->ref_count() == 0 ) {
    return;
  }
  if ( node->_dec_ref() ) {
    if ( node->is_and() ) {
      _deactivate(node);
    }
  }
}

END_NAMESPACE_YM_AIG
