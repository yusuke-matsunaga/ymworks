
/// @file PdNode3_2_MSV.cc
/// @brief PdNode3_2_MSV の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PdNode3_2_MSV.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス PdNode3_2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PdNode3_2_MSV::PdNode3_2_MSV(
  SatSolver& solver,
  SizeType input_num,
  SizeType op_id,
  SizeType opr2
) : PdNode3_2(solver, input_num, op_id, opr2),
    mSel(input_num)
{
  for ( SizeType i = 0; i < input_num; ++ i ) {
    mSel[i] = solver.new_variable(true);
  }
  // mSel のなかで2つのリテラルのみが True になるという制約
  solver.add_exact_two(mSel);
}

// @brief 出力値に関する制約を追加する．
SatLiteral
PdNode3_2_MSV::add_ovar_cond(
  SatSolver& solver,                        ///< [in] SATソルバ
  const std::vector<bool>& ival_list,       ///< [in] 入力値のリスト
  const std::vector<SatLiteral>& opvar_list ///< [in] 演算ノードの出力の変数のリスト
) const
{
  // 演算ノードの出力の値を表す変数
  auto ovar = solver.new_variable(false);
  auto ivar2 = opvar_list[opr2_val()];
  for ( SizeType sel0 = 0; sel0 < input_num() - 1; ++ sel0 ) {
    auto lit0 = mSel[sel0];
    for ( SizeType sel1 = sel0 + 1; sel1 < input_num(); ++ sel1 ) {
      auto lit1 = mSel[sel1];
      auto _ = solver.condblock({lit0, lit1});
      // 演算ノードの入力と出力に関する条件
      add_op_cond(solver, ival_list[sel0], ival_list[sel1], ivar2, ovar);
    }
  }
  return ovar;
}

// @brief 自分より前の演算ノードに対する制約を追加する．
void
PdNode3_2_MSV::add_op_order_cond(
  SatSolver& solver,
  const OpNode* op
) const
{
  auto op_msv = reinterpret_cast<const PdNode3_2_MSV*>(op);
  // mSel は two-hot エンコーディングだが，
  // binary エンコーディングとみなしても大小関係は変わらない．
  solver.add_le(op_msv->mSel, mSel);
}

// @brief SATモデルから入力の選択結果を得る．
std::vector<SizeType>
PdNode3_2_MSV::get_opr(
  const SatModel& model
) const
{
  SizeType opr0;
  SizeType opr1;
  auto opr2 = opr2_val() + input_num();
  SizeType count = 0;
  for ( SizeType i = 0; i < input_num(); ++ i ) {
    if ( model[mSel[i]] == SatBool3::True ) {
      switch ( count ) {
      case 0: opr0 = i; break;
      case 1: opr1 = i; return {opr0, opr1, opr2};
      }
      ++ count;
    }
  }
  throw std::logic_error{"two-hot constraint is not satisfied"};
}

END_NAMESPACE_YM_EXSYN
