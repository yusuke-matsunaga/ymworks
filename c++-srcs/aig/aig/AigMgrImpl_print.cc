
/// @file AigMgrImpl_print.cc
/// @brief AigMgrImpl::print() の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "AigMgrImpl.h"


BEGIN_NAMESPACE_YM_AIG

BEGIN_NONAMESPACE

void
print_fo_info(
  std::ostream& s,
  const FoInfo& fo_info
)
{
  if ( fo_info.is_node() ) {
    auto node = fo_info.node();
    s << "Node#" << node->id();
  }
  else { // fo_info.is_terminal()
    auto terminal = fo_info.terminal();
    s << terminal->string();
  }
}

void
print_node(
  std::ostream& s,
  const AigNode* node
)
{
  s << node->string() << " ["
    << node->ref_count() << "]->";
  for ( auto& fo_info: node->fo_info_list() ) {
    s << " ";
    print_fo_info(s, fo_info);
  }
  s << std::endl;
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス AigMgrImpl
//////////////////////////////////////////////////////////////////////

// @brief 内容を出力する．
void
AigMgrImpl::print(
  std::ostream& s
) const
{
  SizeType active_count = 0;
  for ( auto& node: mNodeArray ) {
    print_node(s, node.get());
    if ( node->ref_count() > 0 ) {
      ++ active_count;
    }
  }
  for ( SizeType id = 0; id < input_num(); ++ id ) {
    auto& _ = mInputList[id];
    s << "Input#" << id
      << ": Node#" << _.node->id()
      << "[" << _.name << "]"
      << std::endl;
  }
  for ( SizeType id = 0; id < dff_num(); ++ id ) {
    auto& _ = mDffList[id];
    s << "Dff#" << id
      << ": Node#" << _.output->id()
      << "[" << _.name << "] = "
      << _.input->edge()
      << std::endl;
  }
  for ( SizeType id = 0; id < output_num(); ++ id ) {
    auto& _ = mOutputList[id];
    s << "Output#" << id
      << "[" << _.name << "] = "
      << _.terminal->edge()
      << std::endl;
  }
  s << "# of inputs:  " << input_num()
    << std::endl
    << "# of outputs: " << output_num()
    << std::endl
    << "# of ANDs:    " << and_num()
    << "( " << active_count << " )"
    << std::endl;
}

END_NAMESPACE_YM_AIG
