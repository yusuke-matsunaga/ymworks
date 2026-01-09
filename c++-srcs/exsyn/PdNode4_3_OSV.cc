
/// @file PdNode4_3_OSV.cc
/// @brief PdNode4_3_OSV の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PdNode4_3_OSV.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス PdNode4_3_OSV
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PdNode4_3_OSV::PdNode4_3_OSV(
  SatSolver& solver,
  SizeType input_num,
  SizeType op_id,
  SizeType opr3
) : PdNode4_3(solver, input_num, op_id, opr3),
    mSel0{solver.new_ordered_set(0, input_num - 1)},
    mSel1{solver.new_ordered_set(0, input_num - 1)},
    mSel2{solver.new_ordered_set(0, input_num - 1)}
{
  solver.add_lt(mSel0, mSel1);
  solver.add_lt(mSel1, mSel2);
}

// @brief 出力値に関する制約を追加する．
SatLiteral
PdNode4_3_OSV::add_ovar_cond(
  SatSolver& solver,
  const std::vector<bool>& ival_list,
  const std::vector<SatLiteral>& opvar_list
) const
{
  // 演算ノードの出力の値を表す変数
  auto ovar = solver.new_variable(false);
  // 確定している入力の値を表す変数
  auto ivar3 = opvar_list[opr3_val()];
  for ( SizeType sel0 = 0; sel0 < input_num() - 2; ++ sel0 ) {
    auto lit0 = mSel0.var(sel0);
    for ( SizeType sel1 = sel0 + 1; sel1 < input_num() - 1; ++ sel1 ) {
      auto lit1 = mSel1.var(sel1);
      for ( SizeType sel2 = sel1 + 1; sel2 < input_num(); ++ sel2 ) {
	auto lit2 = mSel2.var(sel2);
	auto _ = solver.condblock({lit0, lit1, lit2});
	// 演算ノードの入力と出力に関する条件
	add_op_cond(solver, ival_list[sel0], ival_list[sel1],
		    ival_list[sel2], ivar3, ovar);
      }
    }
  }
  return ovar;
}

// @brief 直前の演算ノードに対する制約を追加する．
void
PdNode4_3_OSV::add_op_order_cond(
  SatSolver& solver,
  const OpNode* op
) const
{
  auto op_osv = reinterpret_cast<const PdNode4_3_OSV*>(op);
  auto opr2 = std::vector<SatOrderedSet>{mSel2, mSel1, mSel0};
  auto opr1 = std::vector<SatOrderedSet>{op_osv->mSel2, op_osv->mSel1, op_osv->mSel0};
  solver.add_le(opr1, opr2);
}

// @brief SATモデルから入力の選択結果を得る．
std::vector<SizeType>
PdNode4_3_OSV::get_opr(
  const SatModel& model
) const
{
  SizeType opr0 = mSel0.val(model);
  SizeType opr1 = mSel1.val(model);
  SizeType opr2 = mSel2.val(model);
  SizeType opr3 = opr3_val() + input_num();
  return std::vector<SizeType>{opr0, opr1, opr2, opr3};
}

END_NAMESPACE_YM_EXSYN
