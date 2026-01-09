
/// @file PdNode3_1_OneHot.cc
/// @brief PdNode3_1_OneHot の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PdNode3_1_OneHot.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス PdNode3_1_OneHot
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PdNode3_1_OneHot::PdNode3_1_OneHot(
  SatSolver& solver,
  SizeType input_num,
  SizeType op_id,
  SizeType opr1,
  SizeType opr2
) : PdNode3_1(solver, input_num, op_id, opr1, opr2),
    mSel0{solver.new_onehot_var(input_num)}
{
}

// @brief 出力値に関する制約を追加する．
SatLiteral
PdNode3_1_OneHot::add_ovar_cond(
  SatSolver& solver,
  const std::vector<bool>& ival_list,
  const std::vector<SatLiteral>& opvar_list
) const
{
  // 演算ノードの出力の値を表す変数
  auto ovar = solver.new_variable(false);
  auto ivar1 = opvar_list[opr1_val()];
  auto ivar2 = opvar_list[opr2_val()];
  for ( SizeType sel0 = 0; sel0 < input_num(); ++ sel0 ) {
    auto lit0 = mSel0.var(sel0);
    auto _ = solver.condblock(lit0);
    // 演算ノードの入力と出力に関する条件
    add_op_cond(solver, ival_list[sel0], ivar1, ivar2, ovar);
  }
  return ovar;
}

// @brief 直前の演算ノードに対する制約を追加する．
void
PdNode3_1_OneHot::add_op_order_cond(
  SatSolver& solver,
  const OpNode* op
) const
{
  auto op_onehot = reinterpret_cast<const PdNode3_1_OneHot*>(op);
  auto n = mSel0.size();
  for ( SizeType i = 1; i < n; ++ i ) {
    auto lit0 = op_onehot->mSel0.var(i);
    auto _ = solver.condblock(lit0);
    for ( SizeType j = 0; j <= i; ++ j ) {
      auto lit1 = mSel0.var(j);
      solver.add_clause(~lit1);
    }
  }
}

// @brief SATモデルから入力の選択結果を得る．
std::vector<SizeType>
PdNode3_1_OneHot::get_opr(
  const SatModel& model
) const
{
  SizeType opr0 = mSel0.val(model);
  SizeType opr1 = opr1_val() + input_num();
  SizeType opr2 = opr2_val() + input_num();
  return std::vector<SizeType>{opr0, opr1, opr2};
}

END_NAMESPACE_YM_EXSYN
