
/// @file PdNode4_3_MSV.cc
/// @brief PdNode4_3_MSV の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PdNode4_3_MSV.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス PdNode4_3_MSV
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PdNode4_3_MSV::PdNode4_3_MSV(
  SatSolver& solver,
  SizeType input_num,
  SizeType op_id,
  SizeType opr3
) : PdNode4_3(solver, input_num, op_id, opr3),
    mSel(input_num)
{
  for ( SizeType i = 0; i < input_num; ++ i ) {
    mSel[i] = solver.new_variable(true);
  }

  // mLitList の中で3つのリテラルのみが True になるという制約
  solver.add_exact_k(mSel, 3);
}

// @brief 出力値に関する制約を追加する．
SatLiteral
PdNode4_3_MSV::add_ovar_cond(
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
    auto lit0 = mSel[sel0];
    for ( SizeType sel1 = sel0 + 1; sel1 < input_num() - 1; ++ sel1 ) {
      auto lit1 = mSel[sel1];
      for ( SizeType sel2 = sel1 + 1; sel2 < input_num(); ++ sel2 ) {
	auto lit2 = mSel[sel2];
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
PdNode4_3_MSV::add_op_order_cond(
  SatSolver& solver,
  const OpNode* op
) const
{
  auto op_msv = reinterpret_cast<const PdNode4_3_MSV*>(op);
  // mSel は three-hot エンコーディングだが，
  // binary エンコーディングとみなしても大小関係は変わらない．
  solver.add_le(op_msv->mSel, mSel);
}

// @brief SATモデルから入力の選択結果を得る．
std::vector<SizeType>
PdNode4_3_MSV::get_opr(
  const SatModel& model
) const
{
  SizeType opr0;
  SizeType opr1;
  SizeType opr2;
  SizeType count = 0;
  for ( SizeType i = 0; i < input_num(); ++ i ) {
    if ( model[mSel[i]] == SatBool3::True ) {
      switch ( count ) {
      case 0: opr0 = i; break;
      case 1: opr1 = i; break;
      case 2: opr2 = i; return {opr0, opr1, opr2};
      }
      ++ count;
    }
  }
  throw std::logic_error{"three-hot constraint is not satisfied"};
}

END_NAMESPACE_YM_EXSYN
