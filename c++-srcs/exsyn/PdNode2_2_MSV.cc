
/// @file PdNode2_2_MSV.cc
/// @brief PdNode2_2_MSV の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PdNode2_2_MSV.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス PdNode2_2_MSV
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PdNode2_2_MSV::PdNode2_2_MSV(
  SatSolver& solver,
  SizeType input_num,
  SizeType op_id
) : PdNode2(solver, input_num, op_id),
    mSel(input_num)
{
  for ( SizeType i = 0; i < input_num; ++ i ) {
    mSel[i] = solver.new_variable(true);
  }
  // mSel の中で2つのリテラルのみが True になるという制約
  solver.add_exact_two(mSel);
}

// @brief 出力値に関する制約を追加する．
SatLiteral
PdNode2_2_MSV::add_ovar_cond(
  SatSolver& solver,
  const std::vector<bool>& ival_list,
  const std::vector<SatLiteral>& opvar_list
) const
{
  SizeType N = mSel.size();
  // 演算ノードの出力の値を表す変数
  auto ovar = solver.new_variable(false);
  for ( SizeType sel0 = 0; sel0 < N - 1; ++ sel0 ) {
    auto lit0 = mSel[sel0];
    for ( SizeType sel1 = sel0 + 1; sel1 < N; ++ sel1 ) {
      auto lit1 = mSel[sel1];
      auto _ = solver.condblock({lit0, lit1});
      add_op_cond(solver, ival_list[sel0], ival_list[sel1], ovar);
    }
  }
  return ovar;
}

// @brief no_reapplication 制約を追加する．
void
PdNode2_2_MSV::add_no_reapplication_cond(
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
PdNode2_2_MSV::add_ng_operand_cond(
  SatSolver& solver,
  SizeType opr0,
  SizeType opr1
) const
{
  // 2_2 タイプでは禁止パタンはない．
}

// @brief 自分より前の演算ノードに対する制約を追加する．
void
PdNode2_2_MSV::add_op_order_cond(
  SatSolver& solver,
  const OpNode* op
) const
{
  auto op_msv = reinterpret_cast<const PdNode2_2_MSV*>(op);
  // mSel は two-hot エンコーディングだが，
  // binary エンコーディングとみなしても大小関係は変わらない．
  solver.add_le(mSel, op_msv->mSel);
}

// @brief SATモデルから入力の選択結果を得る．
std::vector<SizeType>
PdNode2_2_MSV::get_opr(
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

// @brief step 番目の入力(演算)を使用している時 true となるリテラルを追加する．
void
PdNode2_2_MSV::add_use_lit(
  SizeType step,
  std::vector<SatLiteral>& lit_list ///< [out] 格納先の変数
) const
{
  if ( step < mSel.size() ) {
    auto var = mSel[step];
    lit_list.push_back(var);
  }
}

END_NAMESPACE_YM_EXSYN
