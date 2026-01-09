
/// @file BdNode3.cc
/// @brief BdNode3 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "BdNode3.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス BdNode3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BdNode3::BdNode3(
  const TemplNode3* templ_node,
  SizeType input_num,
  SizeType op_id,
  SizeType opr0,
  SizeType opr1,
  SizeType opr2
) : OpNode3(solver, input_num, op_id),
    mOpr0{opr0},
    mOpr1{opr1},
    mOpr2{opr2}
{
}

// @brief 出力値に関する制約を追加する．
SatLiteral
BdNode3::add_ovar_cond(
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
  if ( opr2 < input_num() ) {
    // 3つとも入力
    auto ival0 = ival_list[opr0];
    auto ival1 = ival_list[opr1];
    auto ival2 = ival_list[opr2];
    add_op_cond(solver, ival0, ival1, ival2, ovar);
  }
  else if ( opr1 < input_num() ) {
    // opr0 と opr1 が入力
    auto ival0 = ival_list[opr0];
    auto ival1 = ival_list[opr1];
    auto ivar2 = opvar_list[opr2 - input_num()];
    add_op_cond(solver, ival0, ival1, ivar2, ovar);
  }
  else if ( opr1 < input_num() ) {
    // opr0 が入力
    auto ival0 = ival_list[opr0];
    auto ivar1 = opvar_list[opr1 - input_num()];
    auto ivar2 = opvar_list[opr2 - input_num()];
    add_op_cond(solver, ival0, ivar1, ivar2, ovar);
  }
  else {
    // 3つとも演算ノード
    auto ivar0 = opvar_list[opr0 - input_num()];
    auto ivar1 = opvar_list[opr1 - input_num()];
    auto ivar2 = opvar_list[opr2 - input_num()];
    add_op_cond(solver, ivar0, ivar1, ivar2, ovar);
  }
  return ovar;
}

END_NAMESPACE_YM_EXSYN
