
/// @file PdNode3_2_OSV.cc
/// @brief PdNode3_2_OSV の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PdNode3_2_OSV.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス PdNode3_2_OSV
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PdNode3_2_OSV::PdNode3_2_OSV(
  SatSolver& solver,
  SizeType input_num,
  SizeType op_id,
  SizeType opr2
) : PdNode3_2(solver, input_num, op_id, opr2)
{
  mSel0 = solver.new_ordered_set(0, input_num - 1);
  mSel1 = solver.new_ordered_set(0, input_num - 1);

  solver.add_lt(mSel0, mSel1);
}

// @brief 出力値に関する制約を追加する．
SatLiteral
PdNode3_2_OSV::add_ovar_cond(
  SatSolver& solver,
  const std::vector<bool>& ival_list,
  const std::vector<SatLiteral>& opvar_list
) const
{
  // 演算ノードの出力の値を表す変数
  auto ovar = solver.new_variable(false);
  auto ivar2 = opvar_list[opr2_val()];
  for ( SizeType sel0 = 0; sel0 < input_num() - 1; ++ sel0 ) {
    auto lit0 = mSel0.var(sel0);
    for ( SizeType sel1 = sel0 + 1; sel1 < input_num(); ++ sel1 ) {
      auto lit1 = mSel1.var(sel1);
      auto _ = solver.condblock({lit0, lit1});
      // 演算ノードの入力と出力に関する条件
      add_op_cond(solver, ival_list[sel0], ival_list[sel1], ivar2, ovar);
    }
  }
  return ovar;
}

// @brief 自分より前の演算ノードに対する制約を追加する．
void
PdNode3_2_OSV::add_op_order_cond(
  SatSolver& solver,
  const OpNode* op
) const
{
  auto op_osv = reinterpret_cast<const PdNode3_2_OSV*>(op);
  auto opr2 = std::vector<SatOrderedSet>{mSel1, mSel0};
  auto opr1 = std::vector<SatOrderedSet>{op_osv->mSel1, op_osv->mSel0};
  solver.add_le(opr1, opr2);
}

// @brief SATモデルから入力の選択結果を得る．
std::vector<SizeType>
PdNode3_2_OSV::get_opr(
  const SatModel& model
) const
{
  SizeType opr0 = mSel0.val(model);
  SizeType opr1 = mSel1.val(model);
  SizeType opr2 = opr2_val() + input_num();
  return std::vector<SizeType>{opr0, opr1, opr2};
}

END_NAMESPACE_YM_EXSYN
