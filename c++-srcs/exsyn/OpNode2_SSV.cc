
/// @file OpNode2_SSV.cc
/// @brief OpNode2_SSV の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "OpNode2_SSV.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス OpNode2_SSV
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
OpNode2_SSV::OpNode2_SSV(
  SatSolver& solver,
  SizeType input_num,
  SizeType op_id,
  SizeType cand_num
) : OpNode2(solver, input_num, op_id),
    mCandNum{cand_num},
    mLitArray(cand_num * cand_num)
{
  mLitList.reserve(mCandNum * (mCandNum - 1) / 2);
  for ( SizeType sel0 = 0; sel0 < mCandNum - 1; ++ sel0 ) {
    for ( SizeType sel1 = sel0 + 1; sel1 < mCandNum; ++ sel1 ) {
      auto lit = solver.new_variable(true);
      mLitArray[sel0 * mCandNum + sel1] = lit;
      mLitList.push_back(lit);
    }
  }

  // mLitList の中で1つのリテラルのみが True になるという制約
  solver.add_exact_one(mLitList);
}

// @brief 出力値に関する制約を追加する．
SatLiteral
OpNode2_SSV::add_ovar_cond(
  SatSolver& solver,
  const std::vector<bool>& ival_list,
  const std::vector<SatLiteral>& opvar_list
) const
{
  // 演算ノードの出力の値を表す変数
  auto ovar = solver.new_variable(false);
  for ( SizeType sel0 = 0; sel0 < mCandNum - 1; ++ sel0 ) {
    for ( SizeType sel1 = sel0 + 1; sel1 < mCandNum; ++ sel1 ) {
      auto lit = sel_lit(sel0, sel1);
      auto _ = solver.condblock(lit);
      if ( sel1 < input_num() ) {
	// 両方入力
	add_op_cond(solver, ival_list[sel0], ival_list[sel1], ovar);
      }
      else if ( sel0 < input_num() ) {
	// sel0 のみ入力
	auto opr1 = sel1 - input_num();
	auto ivar1 = opvar_list[opr1];
	add_op_cond(solver, ival_list[sel0], ivar1, ovar);
      }
      else {
	// 両方演算
	auto opr0 = sel0 - input_num();
	auto ivar0 = opvar_list[opr0];
	auto opr1 = sel1 - input_num();
	auto ivar1 = opvar_list[opr1];
	add_op_cond(solver, ivar0, ivar1, ovar);
      }
    }
  }
  return ovar;
}

// @brief no_reapplication 制約を追加する．
void
OpNode2_SSV::add_no_reapplication_cond(
  SatSolver& solver,
  const OpNode* op2
) const
{
  SizeType sel2 = op_id() + input_num();
  for ( SizeType sel0 = 0; sel0 < mCandNum - 1; ++ sel0 ) {
    for ( SizeType sel1 = sel0 + 1; sel1 < mCandNum; ++ sel1 ) {
      auto svar = sel_lit(sel0, sel1);
      auto _ = solver.condblock(svar);
      // opr0 と opr2 をオペランドに持つ演算を禁止する．
      op2->add_ng_operand_cond(solver, sel0, sel2);
      // opr1 と opr2 をオペランドに持つ演算を禁止する．
      op2->add_ng_operand_cond(solver, sel1, sel2);
    }
  }
}

// @brief オペランドの組み合わせを禁止する制約を追加する．
void
OpNode2_SSV::add_ng_operand_cond(
  SatSolver& solver,
  SizeType opr0,
  SizeType opr1
) const
{
  auto lit = sel_lit(opr0, opr1);
  solver.add_clause(~lit);
}

// @brief 自分より前の演算ノードに対する制約を追加する．
void
OpNode2_SSV::add_op_order_cond(
  SatSolver& solver,
  const OpNode* op
) const
{
  auto op_ssv = reinterpret_cast<const OpNode2_SSV*>(op);
  SizeType N1 = mCandNum;
  SizeType N2 = op_ssv->mCandNum;
  for ( SizeType op2_i1 = 0; op2_i1 < N2 - 1; ++ op2_i1 ) {
    for ( SizeType op2_i2 = op2_i1 + 1; op2_i2 < N2; ++ op2_i2 ) {
      auto lit2 = op_ssv->sel_lit(op2_i1, op2_i2);
      for ( SizeType op1_i1 = 0; op1_i1 < N1 - 1; ++ op1_i1 ) {
	for ( SizeType op1_i2 = op1_i1 + 1; op1_i2 < N1; ++ op1_i2 ) {
	  if ( op2_i1 < op1_i1 || (op2_i1 == op1_i1 && op2_i2 < op1_i2) ) {
	    auto lit1 = sel_lit(op1_i1, op1_i2);
	    solver.add_clause(~lit2, ~lit1);
	  }
	}
      }
    }
  }
}

// @brief op_id 番目の入力(演算)を使用している時 true となるリテラルを追加する．
void
OpNode2_SSV::add_use_lit(
  SizeType op_id,
  std::vector<SatLiteral>& lit_list
) const
{
  if ( op_id < mCandNum ) {
    lit_list.reserve(lit_list.size() + mCandNum);
    for ( SizeType sel1 = op_id + 1; sel1 < mCandNum; ++ sel1 ) {
      auto lit = sel_lit(op_id, sel1);
      lit_list.push_back(lit);
    }
    for ( SizeType sel0 = 0; sel0 < op_id; ++ sel0 ) {
      auto lit = sel_lit(sel0, op_id);
      lit_list.push_back(lit);
    }
  }
}

// @brief SATモデルから入力の選択結果を得る．
std::vector<SizeType>
OpNode2_SSV::get_opr(
  const SatModel& model
) const
{
  for ( SizeType opr0 = 0; opr0 < mCandNum - 1; ++ opr0 ) {
    for ( SizeType opr1 = opr0 + 1; opr1 < mCandNum; ++ opr1 ) {
      auto lit = sel_lit(opr0, opr1);
      if ( model[lit] == SatBool3::True ) {
	return std::vector<SizeType>{opr0, opr1};
      }
    }
  }
  throw std::logic_error{"one-hot constraint is not satisfied"};
}

END_NAMESPACE_YM_EXSYN
