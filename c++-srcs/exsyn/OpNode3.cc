
/// @file OpNode3.cc
/// @brief OpNode3 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "OpNode3.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス OpNode3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
OpNode3::OpNode3(
  SatSolver& solver,
  SizeType input_num,
  SizeType op_id
) : OpNode(input_num, op_id),
    mTemplate(solver)
{
}

// @brief SATモデルから演算タイプを得る．
OpNode::MapInfo
OpNode3::get_map_info(
  const SatModel& model
) const
{
  return mTemplate.get_map_info(model);
}

// @brief no_reapplication 制約を追加する．
void
OpNode3::add_no_reapplication_cond(
  SatSolver& solver,
  const OpNode* op2
) const
{
  // なにもしない
}

// @brief オペランドの組み合わせを禁止する制約を追加する．
void
OpNode3::add_ng_operand_cond(
  SatSolver& solver,
  SizeType opr0,
  SizeType opr1
) const
{
  // なにもしない．
}

END_NAMESPACE_YM_EXSYN
