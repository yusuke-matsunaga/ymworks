
/// @file AigMgrImpl_check.cc
/// @brief AigMgrImpl::_sanity_check() の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "AigMgrImpl.h"
#include "CheckOp.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス AigMgrImpl
//////////////////////////////////////////////////////////////////////

// @brief 整合性が取れているかチェックする．
bool
AigMgrImpl::_sanity_check(
  std::vector<std::string>& err_list
)
{
  { // 入力リストのチェック
    auto ni = input_num();
    for ( SizeType input_id = 0; input_id < ni; ++ input_id ) {
      _check_input(input_id, err_list);
    }
  }
  { // 出力リストのチェック
    auto no = output_num();
    for ( SizeType output_id = 0; output_id < no; ++ output_id ) {
      _check_output(output_id, err_list);
    }
  }
  { // DFFリストのチェック
    auto nd = dff_num();
    for ( SizeType dff_id = 0; dff_id < nd; ++ dff_id ) {
      _check_dff(dff_id, err_list);
    }
  }
  { // ノードリストのチェック
    auto nn = node_num();
    for ( SizeType node_id = 0; node_id < nn; ++ node_id ) {
      _check_node(node_id, err_list);
    }
  }

  // 疑似出力の枝から入力側にDFSを行い接続が正しいかチェックする．
  CheckOp op(err_list);
  for ( auto& output: mOutputList ) {
    op.dfs(output.terminal->edge());
  }
  for ( auto& dff: mDffList ) {
    op.dfs(dff.input->edge());
  }

  return err_list.empty();
}

// @brief 入力のチェックを行う．
void
AigMgrImpl::_check_input(
  SizeType input_id,
  std::vector<std::string>& err_list
)
{
  auto node = input_node(input_id);
  if ( !node->is_primary_input() ) {
    std::ostringstream buf;
    buf << "Node#" << node->id()
	<< " is supposed to be an input node";
    err_list.push_back(buf.str());
    return;
  }
  if ( node->input_id() != input_id ) {
    std::ostringstream buf;
    buf << "Node#" << node->id()
	<< "->input_id() != "
	<< input_id;
    err_list.push_back(buf.str());
    return;
  }
}

// @brief 出力のチェックを行う．
void
AigMgrImpl::_check_output(
  SizeType output_id,
  std::vector<std::string>& err_list
)
{
  auto output = mOutputList[output_id].terminal.get();
  if ( !output->is_output() ) {
    std::ostringstream buf;
    buf << "Output#" << output_id
	<< " is supposed to be a primary output";
    err_list.push_back(buf.str());
    return;
  }
  if ( output->output_id() != output_id ) {
    std::ostringstream buf;
    buf << "Output#" << output_id
	<< "->output_id() != " << output_id;
    err_list.push_back(buf.str());
    return;
  }
}

// @brief DFFのチェックを行う．
void
AigMgrImpl::_check_dff(
  SizeType dff_id,
  std::vector<std::string>& err_list
)
{
  auto& dff = mDffList[dff_id];
  auto output = dff.output;
  if ( output == nullptr ) {
    std::ostringstream buf;
    buf << "DFF#" << dff_id
	<< "->output() is nullptr";
    err_list.push_back(buf.str());
    return;
  }
  if ( output->dff_id() != dff_id ) {
    std::ostringstream buf;
    buf << "DFF#" << dff_id
	<< "->output()->dff_id() is " << output->dff_id();
    err_list.push_back(buf.str());
    return;
  }

  auto input = dff.input.get();
  if ( input == nullptr ) {
    std::ostringstream buf;
    buf << "DFF#" << dff_id
	<< "->input() is nullptr";
    err_list.push_back(buf.str());
    return;
  }
  if ( input->dff_id() != dff_id ) {
    std::ostringstream buf;
    buf << "DFF#" << dff_id
	<< "->input()->dff_id() is " << input->dff_id();
    err_list.push_back(buf.str());
    return;
  }
}

// @brief ノードのチェックを行う．
void
AigMgrImpl::_check_node(
  SizeType node_id,
  std::vector<std::string>& err_list
)
{
  auto node = this->node(node_id);
  if ( node->id() != node_id ) {
    std::ostringstream buf;
    buf << "Node#" << node_id
	<< "->id() is " << node->id();
    err_list.push_back(buf.str());
    return;
  }

  if ( node->is_primary_input() ) {
    auto input_id = node->input_id();
    if ( node != input_node(input_id) ) {
      {
	std::cout << "node = Node#" << node->id()
		  << ", input_node(" << input_id
		  << ") = Node3" << input_node(input_id)->id()
		  << std::endl;
      }
      std::ostringstream buf;
      buf << "Node#" << node->id()
	  << "(Input#" << input_id << ")"
	  << " is not in mInputList";
      err_list.push_back(buf.str());
    }
    return;
  }

  if ( node->is_dff_output() ) {
    auto dff_id = node->dff_id();
    auto dff_output = mDffList[dff_id].output;
    if ( dff_output != node ) {
      std::ostringstream buf;
      buf << "Node#" << node_id
	  << " is not an output of DFF#" << dff_id;
      err_list.push_back(buf.str());
    }
    return;
  }

  if ( node->is_and() ) {
    return;
  }

  std::ostringstream buf;
  buf << "Node#" << node_id
      << "'s type is unkown";
  err_list.push_back(buf.str());
}

void
AigMgrImpl::_fo_check()
{
  for ( auto& node_ptr: mNodeArray ) {
    auto node = node_ptr.get();
    if ( node->ref_count() == 0 ) {
      continue;
    }
    for ( auto& fo_info: node->mFoList ) {
      if ( fo_info.is_node() ) {
	auto fo_node = fo_info.node();
	if ( fo_node->fanin0_node() != node &&
	     fo_node->fanin1_node() != node ) {
	  std::cout << "Node#" << fo_node->id()
		    << " is in Node#" << node->id()
		    << "->mFoList" << std::endl
		    << " Node#" << fo_node->id()
		    << "->fanin0 = Node#" << fo_node->fanin0_node()->id()
		    << std::endl
		    << " Node#" << fo_node->id()
		    << "->fanin1 = Node#" << fo_node->fanin1_node()->id()
		    << std::endl;
	  abort();
	}
      }
      else {
	auto terminal = fo_info.terminal();
	if ( terminal->edge().node() != node ) {
	  if ( terminal->is_output() ) {
	    std::cout << "Output#" << terminal->output_id();
	  }
	  else {
	    std::cout << "DFF#" << terminal->dff_id();
	  }
	  std::cout << " is in Node#" << node->id()
		    << "->mFoList" << std::endl
		    << " src_edge = " << terminal->edge() << std::endl;
	  abort();
	}
      }
    }
  }
  for ( auto& output: mOutputList ) {
    auto terminal = output.terminal.get();
    auto edge = terminal->edge();
    if ( edge.is_and() ) {
      auto node = edge.node();
      bool found = false;
      for ( auto& fo_info: node->fo_info_list() ) {
	if ( fo_info == FoInfo(terminal) ) {
	  found = true;
	  break;
	}
      }
      if ( !found ) {
	std::cout << terminal->string()
		  << " is not in Node#" << node->id()
		  << "->fo_info_list()" << std::endl;
	abort();
      }
    }
  }
  for ( auto& dff: mDffList ) {
    auto input = dff.input.get();
    auto edge = input->edge();
    if ( edge.is_and() ) {
      auto node = edge.node();
      bool found = false;
      for ( auto& fo_info: node->fo_info_list() ) {
	if ( fo_info == FoInfo(input) ) {
	  found = true;
	  break;
	}
      }
      if ( !found ) {
	std::cout << input->string()
		  << " is not in Node#" << node->id()
		  << "->fo_info_list()" << std::endl;
	abort();
      }
    }
  }
}


//////////////////////////////////////////////////////////////////////
// クラス CheckOp
//////////////////////////////////////////////////////////////////////

// @brief DFSを行う．
void
CheckOp::dfs(
  AigEdge edge
)
{
  if ( edge.is_const() ) {
    return;
  }
  auto node = edge.node();
  auto id = node->id();
  if ( mVisiting.count(id) > 0 ) {
    auto visiting = mVisiting.at(id);
    if ( visiting ) {
      std::ostringstream buf;
      buf << "Node#" << id
	  << " has cyclic reference";
      mErrList.push_back(buf.str());
    }
    return;
  }
  mVisiting.emplace(id, true);

  if ( node->is_and() ) {
    auto fanin0 = node->fanin0();
    if ( fanin0.is_const() ) {
      std::ostringstream buf;
      buf << "Node#" << id
	  << "'s fanin0 is constant";
      mErrList.push_back(buf.str());
      return;
    }
    auto fanin1 = node->fanin1();
    if ( fanin1.is_const() ) {
      std::ostringstream buf;
      buf << "Node#" << id
	  << "'s fanin1 is constant";
      mErrList.push_back(buf.str());
      return;
    }
    if ( fanin0 == fanin1 ) {
      std::ostringstream buf;
      buf << "Node#" << id
	  << "'s fanin0 == fanin1";
      mErrList.push_back(buf.str());
      return;
    }
    dfs(fanin0);
    dfs(fanin1);
  }

  mVisiting.at(id) = false;
}

END_NAMESPACE_YM_AIG
