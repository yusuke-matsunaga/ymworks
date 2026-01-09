
/// @file OpNode.cc
/// @brief OpNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "OpNode.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス OpNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
OpNode::OpNode(
  SizeType input_num,
  SizeType op_id
) : mInputNum{input_num},
    mOpId{op_id}
{
}

// @brief op_id 番目の演算を確定で使用している時 true を返す．
bool
OpNode::use(
  SizeType op_id ///< [in] 演算番号
) const
{
  return false;
}

// @breif step 番目のノードを使用している時 true となるリテラルを追加する．
void
OpNode::add_use_lit(
  SizeType op_id,
  std::vector<SatLiteral>& lit_list
) const
{
}

END_NAMESPACE_YM_EXSYN
