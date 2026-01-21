
/// @file PdNode2_1_OSV.cc
/// @brief PdNode2_1_OSV の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PdNode2_1_OSV.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス PdNode2_1_OSV
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PdNode2_1_OSV::PdNode2_1_OSV(
  SatSolver& solver,
  SizeType input_num,
  SizeType op_id,
  SizeType opr1
) : PdNode2_1(solver, input_num, op_id, opr1),
    mSel0{solver.new_ordered_set(0, input_num - 1)}
{
}

// @brief 出力値に関する制約を追加する．
SatLiteral
PdNode2_1_OSV::add_ovar_cond(
  SatSolver& solver,
  const std::vector<bool>& ival_list,
  const std::vector<SatLiteral>& opvar_list
) const
{
  auto ivar1 = opvar_list[opr1_val()];
  // 演算ノードの出力の値を表す変数
  auto ovar = solver.new_variable(false);
  for ( SizeType sel = 0; sel < input_num(); ++ sel ) {
    auto svar = mSel0.var(sel);
    auto _ = solver.condblock(svar);
    // 演算ノードの入力と出力に関する条件
    add_op_cond(solver, ival_list[sel], ivar1, ovar);
  }
  return ovar;
}

// @brief no_reapplication 制約を追加する．
void
PdNode2_1_OSV::add_no_reapplication_cond(
  SatSolver& solver,
  const OpNode* op2
) const
{
  SizeType opr2 = op_id() + input_num();
  for ( SizeType opr0 = 0; opr0 < input_num(); ++ opr0 ) {
    auto svar = mSel0.var(opr0);
    auto _ = solver.condblock(svar);
    // opr0, opr2 のオペランドを禁止する．
    op2->add_ng_operand_cond(solver, opr0, opr2);
  }
}

// @brief オペランドの組み合わせを禁止する制約を追加する．
void
PdNode2_1_OSV::add_ng_operand_cond(
  SatSolver& solver,
  SizeType opr0,
  SizeType opr1
) const
{
  if ( opr1 != opr1_val() ) {
    // そもそもオペランド1が異なる．
    return;
  }
  auto lit = mSel0.var(opr0);
  solver.add_clause(~lit);
}

// @brief 直前の演算ノードに対する制約を追加する．
void
PdNode2_1_OSV::add_op_order_cond(
  SatSolver& solver,
  const OpNode* op
) const
{
  auto op_osv = reinterpret_cast<const PdNode2_1_OSV*>(op);
  solver.add_le(mSel0, op_osv->mSel0);
}

// @brief SATモデルから入力の選択結果を得る．
std::vector<SizeType>
PdNode2_1_OSV::get_opr(
  const SatModel& model
) const
{
  SizeType opr0 = mSel0.val(model);
  SizeType opr1 = opr1_val() + input_num();
  return std::vector<SizeType>{opr0, opr1};
}

// @brief step 番目の入力(演算)を使用している時 true となるリテラルを追加する．
void
PdNode2_1_OSV::add_use_lit(
  SizeType step,
  std::vector<SatLiteral>& lit_list ///< [out] 格納先の変数
) const
{
  if ( mSel0.min() <= step && step <= mSel0.max() ) {
    auto var = mSel0.var(step);
    lit_list.push_back(var);
  }
}

END_NAMESPACE_YM_EXSYN
