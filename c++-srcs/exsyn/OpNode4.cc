
/// @file OpNode4.cc
/// @brief OpNode4 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "OpNode4.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス OpNode4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
OpNode4::OpNode4(
  SatSolver& solver,
  SizeType input_num,
  SizeType op_id
) : OpNode(input_num, op_id),
    mTemplate(solver)
{
}

// @brief no_reapplication 制約を追加する．
void
OpNode4::add_no_reapplication_cond(
  SatSolver& solver,
  const OpNode* op2
) const
{
  // なにもしない
}

// @brief オペランドの組み合わせを禁止する制約を追加する．
void
OpNode4::add_ng_operand_cond(
  SatSolver& solver,
  SizeType opr0,
  SizeType opr1
) const
{
  // なにもしない．
}

// @brief SATモデルから演算タイプを得る．
OpNode::MapInfo
OpNode4::get_map_info(
  const SatModel& model
) const
{
  return mTemplate.get_map_info(model);
}

END_NAMESPACE_YM_EXSYN
