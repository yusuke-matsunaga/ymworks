
/// @file PdNode2_2_OSSV.cc
/// @brief PdNode2_2_OSSV の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PdNode2_2_OSSV.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス PdNode2_2_OSSV
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PdNode2_2_OSSV::PdNode2_2_OSSV(
  SatSolver& solver,
  SizeType input_num,
  SizeType op_id
) : PdNode2(solver, input_num, op_id),
    mLitArray(input_num * input_num),
    mLitSet{solver.new_ordered_set(0, (input_num * (input_num - 1) / 2) - 1)}
{
  SizeType index = 0;
  for ( SizeType sel0 = 0; sel0 < input_num - 1; ++ sel0 ) {
    for ( SizeType sel1 = sel0 + 1; sel1 < input_num; ++ sel1 ) {
      auto lit = mLitSet.var(index);
      ++ index;
      mLitArray[sel0 * input_num + sel1] = lit;
    }
  }
}

// @brief 出力値に関する制約を追加する．
SatLiteral
PdNode2_2_OSSV::add_ovar_cond(
  SatSolver& solver,
  const std::vector<bool>& ival_list,
  const std::vector<SatLiteral>& opvar_list
) const
{
  // 演算ノードの出力の値を表す変数
  auto ovar = solver.new_variable(false);
  for ( SizeType sel0 = 0; sel0 < input_num() - 1; ++ sel0 ) {
    for ( SizeType sel1 = sel0 + 1; sel1 < input_num(); ++ sel1 ) {
      auto lit = sel_lit(sel0, sel1);
      auto _ = solver.condblock(lit);
      // 演算ノードの入力と出力に関する条件
      add_op_cond(solver, ival_list[sel0], ival_list[sel1], ovar);
    }
  }
  return ovar;
}

// @brief no_reapplication 制約を追加する．
void
PdNode2_2_OSSV::add_no_reapplication_cond(
  SatSolver& solver,
  const OpNode* op2
) const
{
  SizeType opr2 = op_id() + input_num();
  for ( SizeType opr0 = 0; opr0 < input_num() - 1; ++ opr0 ) {
    for ( SizeType opr1 = opr0 + 1; opr1 < input_num(); ++ opr1 ) {
      auto svar = sel_lit(opr0, opr1);
      auto _ = solver.condblock(svar);
      // opr0 と i(opr2) をオペランドに持つ演算を禁止する．
      op2->add_ng_operand_cond(solver, opr0, opr2);
      // opr1 と i(opr2) をオペランドに持つ演算を禁止する．
      op2->add_ng_operand_cond(solver, opr1, opr2);
    }
  }
}

// @brief オペランドの組み合わせを禁止する制約を追加する．
void
PdNode2_2_OSSV::add_ng_operand_cond(
  SatSolver& solver,
  SizeType opr0,
  SizeType opr1
) const
{
  // 2_2 タイプでは禁止パタンはない．
}

// @brief 自分より前の演算ノードに対する制約を追加する．
void
PdNode2_2_OSSV::add_op_order_cond(
  SatSolver& solver,
  const OpNode* op
) const
{
  auto op_ssv = reinterpret_cast<const PdNode2_2_OSSV*>(op);
#if 0
  SizeType N = input_num();
  for ( SizeType op2_i1 = 0; op2_i1 < N - 1; ++ op2_i1 ) {
    for ( SizeType op2_i2 = op2_i1 + 1; op2_i2 < N; ++ op2_i2 ) {
      auto lit2 = sel_lit(op2_i1, op2_i2);
      for ( SizeType op1_i1 = 0; op1_i1 < N - 1; ++ op1_i1 ) {
	for ( SizeType op1_i2 = op1_i1 + 1; op1_i2 < N; ++ op1_i2 ) {
	  if ( op2_i1 < op1_i1 || (op2_i1 == op1_i1 && op2_i2 < op1_i2) ) {
	    auto lit1 = op_ssv->sel_lit(op1_i1, op1_i2);
	    solver.add_clause(~lit2, ~lit1);
	  }
	}
      }
    }
  }
#else
  solver.add_le(mLitSet, op_ssv->mLitSet);
#endif
}

// @brief SATモデルから入力の選択結果を得る．
std::vector<SizeType>
PdNode2_2_OSSV::get_opr(
  const SatModel& model
) const
{
  for ( SizeType opr0 = 0; opr0 < input_num() - 1; ++ opr0 ) {
    for ( SizeType opr1 = opr0 + 1; opr1 < input_num(); ++ opr1 ) {
      auto lit = sel_lit(opr0, opr1);
      if ( model[lit] == SatBool3::True ) {
	return std::vector<SizeType>{opr0, opr1};
      }
    }
  }
  throw std::logic_error{"one-hot constraint is not satisfied"};
}

END_NAMESPACE_YM_EXSYN
