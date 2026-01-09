
/// @file BdNode4.cc
/// @brief BdNode4 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "BdNode4.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス BdNode4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BdNode4::BdNode4(
  SatSolver& solver,
  SizeType input_num,
  SizeType op_id,
  SizeType opr0,
  SizeType opr1,
  SizeType opr2,
  SizeType opr3
) : OpNode4(solver, input_num, op_id),
    mOpr0{opr0},
    mOpr1{opr1},
    mOpr2{opr2},
    mOpr3{opr3}
{
}

// @brief 出力値に関する制約を追加する．
SatLiteral
BdNode4::add_ovar_cond(
  SatSolver& solver,
  const std::vector<bool>& ival_list,
  const std::vector<SatLiteral>& opvar_list
) const
{
  // 演算ノードの出力の値を表す変数
  auto ovar = solver.new_variable(false);

  auto opr0 = opr0_val();
  auto opr1 = opr1_val();
  auto opr2 = opr2_val();
  auto opr3 = opr3_val();
  if ( opr3 < input_num() ) {
    // 4つとも入力
    auto ival0 = ival_list[opr0];
    auto ival1 = ival_list[opr1];
    auto ival2 = ival_list[opr2];
    auto ival3 = ival_list[opr3];
    add_op_cond(solver, ival0, ival1, ival2, ival3, ovar);
  }
  else if ( opr2 < input_num() ) {
    // opr0, opr1, opr2が入力
    auto ival0 = ival_list[opr0];
    auto ival1 = ival_list[opr1];
    auto ival2 = ival_list[opr2];
    auto ivar3 = opvar_list[opr3 - input_num()];
    add_op_cond(solver, ival0, ival1, ival2, ivar3, ovar);
  }
  else if ( opr1 < input_num() ) {
    // opr0 と opr1 が入力
    auto ival0 = ival_list[opr0];
    auto ival1 = ival_list[opr1];
    auto ivar2 = opvar_list[opr2 - input_num()];
    auto ivar3 = opvar_list[opr3 - input_num()];
    add_op_cond(solver, ival0, ival1, ivar2, ivar3, ovar);
  }
  else if ( opr1 < input_num() ) {
    // opr0 が入力
    auto ival0 = ival_list[opr0];
    auto ivar1 = opvar_list[opr1 - input_num()];
    auto ivar2 = opvar_list[opr2 - input_num()];
    auto ivar3 = opvar_list[opr3 - input_num()];
    add_op_cond(solver, ival0, ivar1, ivar2, ivar3, ovar);
  }
  else {
    // 4つとも演算ノード
    auto ivar0 = opvar_list[opr0 - input_num()];
    auto ivar1 = opvar_list[opr1 - input_num()];
    auto ivar2 = opvar_list[opr2 - input_num()];
    auto ivar3 = opvar_list[opr3 - input_num()];
    add_op_cond(solver, ivar0, ivar1, ivar2, ivar3, ovar);
  }
  return ovar;
}

// @brief no_reapplication 制約を追加する．
void
BdNode4::add_no_reapplication_cond(
  SatSolver& solver,
  const OpNode* op2
) const
{
  // なにもしない．
}

// @brief オペランドの組み合わせを禁止する制約を追加する．
void
BdNode4::add_ng_operand_cond(
  SatSolver& solver,
  SizeType opr0,
  SizeType opr1
) const
{
  // なにもしない．
}

// @brief 直前の演算ノードに対する制約を追加する．
void
BdNode4::add_op_order_cond(
  SatSolver& solver,
  const OpNode* op
) const
{
  // なにもしない．
}

// @brief op_id 番目の演算を確定で使用している時 true を返す．
bool
BdNode4::use(
  SizeType op_id
) const
{
  auto tmp_val = op_id + input_num();
  return opr0_val() == tmp_val ||
    opr1_val() == tmp_val ||
    opr2_val() == tmp_val ||
    opr3_val() == tmp_val;
}

// @brief SATモデルから入力の選択結果を得る．
std::vector<SizeType>
BdNode4::get_opr(
  const SatModel& model
) const
{
  SizeType opr0 = opr0_val();
  SizeType opr1 = opr1_val();
  SizeType opr2 = opr2_val();
  SizeType opr3 = opr3_val();
  return std::vector<SizeType>{opr0, opr1, opr2, opr3};
}

END_NAMESPACE_YM_EXSYN
