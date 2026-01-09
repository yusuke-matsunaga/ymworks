
/// @file PdNode4_2_SSV.cc
/// @brief PdNode4_2_SSV の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PdNode4_2_SSV.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス PdNode4_2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PdNode4_2_SSV::PdNode4_2_SSV(
  SatSolver& solver,
  SizeType input_num,
  SizeType op_id,
  SizeType opr2,
  SizeType opr3
) : PdNode4_2(solver, input_num, op_id, opr2, opr3),
    mLitArray(input_num * input_num)
{
  mLitList.reserve(input_num * (input_num - 1) / 2);
  for ( SizeType sel0 = 0; sel0 < input_num - 1; ++ sel0 ) {
    for ( SizeType sel1 = sel0 + 1; sel1 < input_num; ++ sel1 ) {
      auto lit = solver.new_variable(true);
      mLitArray[sel0 * input_num + sel1] = lit;
      mLitList.push_back(lit);
    }
  }

  // mLitList の中で1つのリテラルのみが True になるという制約
  solver.add_exact_one(mLitList);
}

// @brief 出力値に関する制約を追加する．
SatLiteral
PdNode4_2_SSV::add_ovar_cond(
  SatSolver& solver,
  const std::vector<bool>& ival_list,
  const std::vector<SatLiteral>& opvar_list
) const
{
  // 演算ノードの出力の値を表す変数
  auto ovar = solver.new_variable(false);
  // 確定している入力の値を表す変数
  auto ivar2 = opvar_list[opr2_val()];
  auto ivar3 = opvar_list[opr3_val()];
  for ( SizeType sel0 = 0; sel0 < input_num() - 1; ++ sel0 ) {
    for ( SizeType sel1 = sel0 + 1; sel1 < input_num(); ++ sel1 ) {
      auto lit = sel_lit(sel0, sel1);
      auto _ = solver.condblock(lit);
      // 演算ノードの入力と出力に関する条件
      add_op_cond(solver, ival_list[sel0], ival_list[sel1], ivar2, ivar3, ovar);
    }
  }
  return ovar;
}

// @brief 自分より前の演算ノードに対する制約を追加する．
void
PdNode4_2_SSV::add_op_order_cond(
  SatSolver& solver,
  const OpNode* op
) const
{
  auto op_ssv = reinterpret_cast<const PdNode4_2_SSV*>(op);
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
}

// @brief SATモデルから入力の選択結果を得る．
std::vector<SizeType>
PdNode4_2_SSV::get_opr(
  const SatModel& model
) const
{
  auto opr2 = opr2_val() + input_num();
  auto opr3 = opr3_val() + input_num();
  for ( SizeType opr0 = 0; opr0 < input_num() - 1; ++ opr0 ) {
    for ( SizeType opr1 = opr0 + 1; opr1 < input_num(); ++ opr1 ) {
      auto lit = sel_lit(opr0, opr1);
      if ( model[lit] == SatBool3::True ) {
	return std::vector<SizeType>{opr0, opr1, opr2, opr3};
      }
    }
  }
  throw std::logic_error{"one-hot constraint is not satisfied"};
}

END_NAMESPACE_YM_EXSYN
