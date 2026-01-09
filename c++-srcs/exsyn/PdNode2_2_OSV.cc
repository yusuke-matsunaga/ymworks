
/// @file PdNode2_2_OSV.cc
/// @brief PdNode2_2_OSV の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PdNode2_2_OSV.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス PdNode2_2_OSV
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PdNode2_2_OSV::PdNode2_2_OSV(
  SatSolver& solver,
  SizeType input_num,
  SizeType op_id
) : PdNode2(solver, input_num, op_id)
{
  mSel0 = solver.new_ordered_set(0, input_num - 1);
  mSel1 = solver.new_ordered_set(0, input_num - 1);

  solver.add_lt(mSel0, mSel1);
}

// @brief 出力値に関する制約を追加する．
SatLiteral
PdNode2_2_OSV::add_ovar_cond(
  SatSolver& solver,
  const std::vector<bool>& ival_list,
  const std::vector<SatLiteral>& opvar_list
) const
{
  // 演算ノードの出力の値を表す変数
  auto ovar = solver.new_variable(false);
  for ( SizeType sel0 = mSel0.min(); sel0 <= mSel0.max(); ++ sel0 ) {
    auto svar0 = mSel0.var(sel0);
    for ( SizeType sel1 = sel0 + 1; sel1 <= mSel1.max(); ++ sel1 ) {
      auto svar1 = mSel1.var(sel1);
      auto _ = solver.condblock({svar0, svar1});
      // 演算ノードの入力と出力に関する条件
      add_op_cond(solver, ival_list[sel0], ival_list[sel1], ovar);
    }
  }
  return ovar;
}

// @brief no_reapplication 制約を追加する．
void
PdNode2_2_OSV::add_no_reapplication_cond(
  SatSolver& solver,
  const OpNode* op2
) const
{
  SizeType opr2 = op_id() + input_num();
  for ( SizeType opr0 = mSel0.min(); opr0 <= mSel0.max(); ++ opr0 ) {
    auto svar = mSel0.var(opr0);
    auto _ = solver.condblock(svar);
    // opr0 と i(opr2) を入力とする演算を禁止する．
    op2->add_ng_operand_cond(solver, opr0, opr2);
  }
  for ( SizeType opr1 = mSel1.min(); opr1 <= mSel1.max(); ++ opr1 ) {
    auto svar = mSel1.var(opr1);
    auto _ = solver.condblock(svar);
    // opr1 と i(opr2) を入力とする演算を禁止する．
    op2->add_ng_operand_cond(solver, opr1, opr2);
  }
}

// @brief オペランドの組み合わせを禁止する制約を追加する．
void
PdNode2_2_OSV::add_ng_operand_cond(
  SatSolver& solver,
  SizeType opr0,
  SizeType opr1
) const
{
  // 2_2 タイプでは禁止パタンはない．
}

// @brief 自分より前の演算ノードに対する制約を追加する．
void
PdNode2_2_OSV::add_op_order_cond(
  SatSolver& solver,
  const OpNode* op
) const
{
  auto op_osv = reinterpret_cast<const PdNode2_2_OSV*>(op);
  auto opr1 = std::vector<SatOrderedSet>{mSel0, mSel1};
  auto opr2 = std::vector<SatOrderedSet>{op_osv->mSel0, op_osv->mSel1};
  solver.add_le(opr1, opr2);
}

// @brief SATモデルから入力の選択結果を得る．
std::vector<SizeType>
PdNode2_2_OSV::get_opr(
  const SatModel& model
) const
{
  SizeType opr0 = mSel0.val(model);
  SizeType opr1 = mSel1.val(model);
  return std::vector<SizeType>{opr0, opr1};
}

// @breif step 番目の入力(演算)を使用している時 true となるリテラルを追加する．
void
PdNode2_2_OSV::add_use_lit(
  SizeType step,
  std::vector<SatLiteral>& lit_list
) const
{
  if ( step < input_num() ) {
    auto var0 = mSel0.var(step);
    lit_list.push_back(var0);
    auto var1 = mSel1.var(step);
    lit_list.push_back(var1);
  }
}

END_NAMESPACE_YM_EXSYN
