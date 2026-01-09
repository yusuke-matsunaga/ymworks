
/// @file PdNode4_0.cc
/// @brief PdNode4_0 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PdNode4_0.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス PdNode4_0
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PdNode4_0::PdNode4_0(
  SatSolver& solver,
  SizeType input_num,
  SizeType op_id,
  SizeType opr0,
  SizeType opr1,
  SizeType opr2,
  SizeType opr3
) : PdNode4(solver, input_num, op_id),
    mOpr0{opr0},
    mOpr1{opr1},
    mOpr2{opr2},
    mOpr3{opr3}
{
}

// @brief 出力値に関する制約を追加する．
SatLiteral
PdNode4_0::add_ovar_cond(
  SatSolver& solver,
  const std::vector<bool>& ival_list,
  const std::vector<SatLiteral>& opvar_list
) const
{
  auto ivar0 = opvar_list[opr0_val()];
  auto ivar1 = opvar_list[opr1_val()];
  auto ivar2 = opvar_list[opr2_val()];
  auto ivar3 = opvar_list[opr3_val()];
  // 演算ノードの出力の値を表す変数
  auto ovar = solver.new_variable(false);
  // 演算ノードの入力と出力に関する条件
  add_op_cond(solver, ivar0, ivar1, ivar2, ivar3, ovar);
  return ovar;
}

// @brief 直前の演算ノードに対する制約を追加する．
void
PdNode4_0::add_op_order_cond(
  SatSolver& solver,
  const OpNode* op
) const
{
  // なにもしない．
}

// @brief op_id 番目の演算を確定で使用している時 true を返す．
bool
PdNode4_0::use(
  SizeType op_id
) const
{
  return opr0_val() == op_id ||
    opr1_val() == op_id ||
    opr2_val() == op_id ||
    opr3_val() == op_id;
}

// @brief SATモデルから入力の選択結果を得る．
std::vector<SizeType>
PdNode4_0::get_opr(
  const SatModel& model
) const
{
  SizeType opr0 = opr0_val() + input_num();
  SizeType opr1 = opr1_val() + input_num();
  SizeType opr2 = opr2_val() + input_num();
  SizeType opr3 = opr3_val() + input_num();
  return std::vector<SizeType>{opr0, opr1, opr2, opr3};
}

END_NAMESPACE_YM_EXSYN
