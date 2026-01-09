
/// @file PdNode2_0.cc
/// @brief PdNode2_0 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PdNode2_0.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス PdNode2_0
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PdNode2_0::PdNode2_0(
  SatSolver& solver,
  SizeType input_num,
  SizeType op_id,
  SizeType opr0,
  SizeType opr1
) : PdNode2(solver, input_num, op_id),
    mOpr0{opr0},
    mOpr1{opr1}
{
}

// @brief 出力値に関する制約を追加する．
SatLiteral
PdNode2_0::add_ovar_cond(
  SatSolver& solver,
  const std::vector<bool>& ival_list,
  const std::vector<SatLiteral>& opvar_list
) const
{
  // 演算ノードのオペランドの値を表す変数
  auto ivar0 = opvar_list[opr0_val()];
  auto ivar1 = opvar_list[opr1_val()];
  // 演算ノードの出力の値を表す変数
  auto ovar = solver.new_variable(false);
  // 演算ノードの入力と出力に関する条件
  add_op_cond(solver, ivar0, ivar1, ovar);
  return ovar;
}

// @brief no_reapplication 制約を追加する．
void
PdNode2_0::add_no_reapplication_cond(
  SatSolver& solver,
  const OpNode* op2
) const
{
  // なにもしない．
}

// @brief オペランドの組み合わせを禁止する制約を追加する．
void
PdNode2_0::add_ng_operand_cond(
  SatSolver& solver,
  SizeType opr0,
  SizeType opr1
) const
{
  // なにもしない．
}

// @brief 直前の演算ノードに対する制約を追加する．
void
PdNode2_0::add_op_order_cond(
  SatSolver& solver,
  const OpNode* op
) const
{
  // なにもしない．
}

// @brief op_id 番目の演算を確定で使用している時 true を返す．
bool
PdNode2_0::use(
  SizeType op_id ///< [in] 演算番号
) const
{
  return opr0_val() == op_id || opr1_val() == op_id;
}

// @brief SATモデルから入力の選択結果を得る．
std::vector<SizeType>
PdNode2_0::get_opr(
  const SatModel& model
) const
{
  SizeType opr0 = opr0_val() + input_num();
  SizeType opr1 = opr1_val() + input_num();
  return std::vector<SizeType>{opr0, opr1};
}

END_NAMESPACE_YM_EXSYN
