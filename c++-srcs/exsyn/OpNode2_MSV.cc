
/// @file OpNode2_MSV.cc
/// @brief OpNode2_MSV の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "OpNode2_MSV.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス OpNode2_MSV
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
OpNode2_MSV::OpNode2_MSV(
  SatSolver& solver,
  SizeType input_num,
  SizeType op_id,
  SizeType cand_num
) : OpNode2(solver, input_num, op_id),
    mSel(cand_num)
{
  for ( SizeType i = 0; i < cand_num; ++ i ) {
    mSel[i] = solver.new_variable(true);
  }
  // mSel の中で2つのリテラルのみが True になるという制約
  solver.add_exact_two(mSel);
}

// @brief 出力値に関する制約を追加する．
SatLiteral
OpNode2_MSV::add_ovar_cond(
  SatSolver& solver,
  const std::vector<bool>& ival_list,
  const std::vector<SatLiteral>& opvar_list
) const
{
  // 演算ノードの出力の値を表す変数
  auto ovar = solver.new_variable(false);
  SizeType N = mSel.size();
  for ( SizeType sel0 = 0; sel0 < N - 1; ++ sel0 ) {
    auto lit0 = mSel[sel0];
    for ( SizeType sel1 = sel0 + 1; sel1 < N; ++ sel1 ) {
      auto lit1 = mSel[sel1];
      auto _ = solver.condblock({lit0, lit1});
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
OpNode2_MSV::add_no_reapplication_cond(
  SatSolver& solver,
  const OpNode* op2
) const
{
  SizeType opr2 = op_id() + input_num();
  // MSV にはオペランド0とオペランド1の区別がない．
  for ( SizeType opr = 0; opr < mSel.size(); ++ opr ) {
    auto svar = mSel[opr];
    auto _ = solver.condblock(svar);
    // opr と i(opr2) を入力とする演算を禁止する．
    op2->add_ng_operand_cond(solver, opr, opr2);
  }
}

// @brief オペランドの組み合わせを禁止する制約を追加する．
void
OpNode2_MSV::add_ng_operand_cond(
  SatSolver& solver,
  SizeType opr0,
  SizeType opr1
) const
{
  auto lit1 = mSel[opr0];
  auto lit2 = mSel[opr1];
  solver.add_clause(~lit1, ~lit2);
}

// @brief 自分より前の演算ノードに対する制約を追加する．
void
OpNode2_MSV::add_op_order_cond(
  SatSolver& solver,
  const OpNode* op
) const
{
  auto op_msv = reinterpret_cast<const OpNode2_MSV*>(op);
  // mSel は two-hot エンコーディングだが，
  // binary エンコーディングとみなしても大小関係は変わらない．
  solver.add_le(mSel, op_msv->mSel);
}

// @breif op_id 番目の演算を使用している時 true となるリテラルを追加する．
void
OpNode2_MSV::add_use_lit(
  SizeType op_id,
  std::vector<SatLiteral>& lit_list
) const
{
  if ( op_id < mSel.size() ) {
    auto lit = mSel[op_id];
    lit_list.push_back(lit);
  }
}

// @brief SATモデルから入力の選択結果を得る．
std::vector<SizeType>
OpNode2_MSV::get_opr(
  const SatModel& model
) const
{
  SizeType opr0;
  SizeType opr1;
  bool first = true;
  SizeType N = mSel.size();
  for ( SizeType i = 0; i < N; ++ i ) {
    if ( model[mSel[i]] == SatBool3::True ) {
      if ( first ) {
	opr0 = i;
	first = false;
      }
      else {
	opr1 = i;
	break;
      }
    }
  }
  return std::vector<SizeType>{opr0, opr1};
}

END_NAMESPACE_YM_EXSYN
