
/// @file AigMgrImpl_simulate.cc
/// @brief AigMgrImpl::simulate() の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "AigMgrImpl.h"
#include "AigValArray.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス AigMgrImpl
//////////////////////////////////////////////////////////////////////

// @brief シミュレーションを行う．
std::vector<AigBitVect>
AigMgrImpl::simulate(
  const std::vector<AigBitVect>& input_vals
) const
{
  AigValArray val_array;
  val_array.mValArray.resize(node_num());

  // 入力の値を val_array に設定する．
  auto ni = input_num();
  auto nf = dff_num();
  if ( input_vals.size() != ni + nf ) {
    throw std::invalid_argument{"input_vals.size() != input_num() + dff_num()"};
  }
  for ( SizeType i = 0; i < ni; ++ i ) {
    auto id = input_node(i)->id();
    val_array.mValArray[id] = input_vals[i];
  }
  for ( SizeType i = 0; i < nf; ++ i ) {
    auto id = dff_output(i)->id();
    val_array.mValArray[id] = input_vals[i + ni];
  }
  // ANDノードの値を計算する．
  auto node_list = and_list();
  for ( auto node: node_list ) {
    auto fanin0 = node->fanin0();
    auto val0 = val_array[fanin0];
    auto fanin1 = node->fanin1();
    auto val1 = val_array[fanin1];
    auto val = val0 & val1;
    val_array.mValArray[node->id()] = val;
  }
  // 出力の値を output_vals に記録する．
  auto no = output_num();
  std::vector<AigBitVect> output_vals(no + nf);
  for ( SizeType i = 0; i < no; ++ i ) {
    auto edge = output_edge(i);
    output_vals[i] = val_array[edge];
  }
  for ( SizeType i = 0; i < nf; ++ i ) {
    auto edge = dff_input_edge(i);
    output_vals[i + no] = val_array[edge];
  }
  return output_vals;
}

END_NAMESPACE_YM_AIG
