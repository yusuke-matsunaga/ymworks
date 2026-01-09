
/// @file OpNode3_SSV.cc
/// @brief OpNode3_SSV の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "OpNode3_SSV.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス OpNode3_SSV
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
OpNode3_SSV::OpNode3_SSV(
  SatSolver& solver,
  SizeType input_num,
  SizeType op_id,
  SizeType cand_num
) : OpNode3(solver, input_num, op_id),
    mCandNum{cand_num},
    mLitArray2(cand_num * cand_num),
    mLitArray3(cand_num * cand_num * cand_num)
{
  // 2項演算用と3項演算用で別のリテラルを用意する．

  mLitList2.reserve(mCandNum * (mCandNum - 1) / 2);
  for ( SizeType sel0 = 0; sel0 < mCandNum - 1; ++ sel0 ) {
    for ( SizeType sel1 = sel0 + 1; sel1 < mCandNum; ++ sel1 ) {
      auto lit = solver.new_variable(true);
      mLitArray2[sel0 * mCandNum + sel1] = lit;
      mLitList2.push_back(lit);
    }
  }
  // mLitList2 の中で1つのリテラルのみが True になるという制約
  solver.add_exact_one(mLitList2);

  mLitList3.reserve(mCandNum * (mCandNum - 1) * (mCandNum - 2) / 6);
  for ( SizeType sel0 = 0; sel0 < mCandNum - 2; ++ sel0 ) {
    for ( SizeType sel1 = sel0 + 1; sel1 < mCandNum - 1; ++ sel1 ) {
      for ( SizeType sel2 = sel1 + 1; sel2 < mCandNum; ++ sel2 ) {
	auto lit = solver.new_variable(true);
	mLitArray3[((sel0 * mCandNum) + sel1) * mCandNum + sel2 ] = lit;
	mLitList3.push_back(lit);
      }
    }
  }
  // mLitList3 の中で1つのリテラルのみが True になるという制約
  solver.add_exact_one(mLitList3);
}

// @brief 自分より前の演算ノードに対する制約を追加する．
void
OpNode3_SSV::add_op_order_cond(
  SatSolver& solver,
  const OpNode* op
) const
{
  auto op_ssv = reinterpret_cast<const OpNode3_SSV*>(op);
  SizeType N1 = op_ssv->mCandNum;
  SizeType N2 = mCandNum;
  { // 3項演算モード
  }
  { // 2項演算モード
    for ( SizeType op2_i1 = 0; op2_i1 < N2 - 1; ++ op2_i1 ) {
      for ( SizeType op2_i2 = op2_i1 + 1; op2_i2 < N2; ++ op2_i2 ) {
	auto lit2 = sel_lit2(op2_i1, op2_i2);
	for ( SizeType op1_i1 = 0; op1_i1 < N1 - 1; ++ op1_i1 ) {
	  for ( SizeType op1_i2 = op1_i1 + 1; op1_i2 < N1; ++ op1_i2 ) {
	    if ( op2_i1 < op1_i1 || (op2_i1 == op1_i1 && op2_i2 < op1_i2) ) {
	      auto lit1 = op_ssv->sel_lit(op1_i1, op1_i2);
	      solver.add_clause(~lit2, ~lit1);
	    }
	  }
	}
      }
    }
  }
}

// @breif op_id 番目の入力(演算)を使用している時 true となるリテラルを追加する．
void
OpNode3_SSV::add_use_lit(
  SizeType op_id,
  std::vector<SatLiteral>& lit_list
) const
{
  SizeType sel = op_id + input_num();
  if ( sel < mCandNum ) {
    lit_list.reserve(mCandNum - input_num());
    for ( SizeType sel1 = sel + 1; sel1 < mCandNum; ++ sel1 ) {
      auto lit = sel_lit(sel, sel1);
      lit_list.push_back(lit);
    }
    for ( SizeType sel0 = input_num(); sel0 < sel; ++ sel0 ) {
      auto lit = sel_lit(sel0, sel);
      lit_list.push_back(lit);
    }
  }
}

// @brief SATモデルから入力の選択結果を得る．
std::vector<SizeType>
OpNode3_SSV::get_opr(
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

// @brief 演算ノードのオペランドの値を表すリテラルを返す．
std::vector<SatLiteral>
OpNode3_SSV::add_opr_cond(
  SatSolver& solver,
  const std::vector<bool>& ival_list,
  const std::vector<SatLiteral>& opvar_list
) const
{
  auto ivar0 = solver.new_variable(false);
  auto ivar1 = solver.new_variable(false);
  for ( SizeType sel0 = 0; sel0 < mCandNum - 1; ++ sel0 ) {
    for ( SizeType sel1 = sel0 + 1; sel1 < mCandNum; ++ sel1 ) {
      auto lit = sel_lit(sel0, sel1);
      auto _ = solver.condblock(lit);
      if ( sel0 < input_num() ) {
	if ( ival_list[sel0] ) {
	  solver.add_clause( ivar0);
	}
	else {
	  solver.add_clause(~ivar0);
	}
      }
      else {
	auto opr0 = sel0 - input_num();
	auto ovar = opvar_list[opr0];
	solver.add_clause(~ovar,  ivar0);
	solver.add_clause( ovar, ~ivar0);
      }
      if ( sel1 < input_num() ) {
	if ( ival_list[sel1] ) {
	  solver.add_clause( ivar1);
	}
	else {
	  solver.add_clause(~ivar1);
	}
      }
      else {
	auto opr1 = sel1 - input_num();
	auto ovar = opvar_list[opr1];
	solver.add_clause(~ovar,  ivar1);
	solver.add_clause( ovar, ~ivar1);
      }
    }
  }
  return {ivar0, ivar1};
}

END_NAMESPACE_YM_EXSYN
