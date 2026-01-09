
/// @file BdNode2.cc
/// @brief BdNode2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "BdNode2.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス　BdNode2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BdNode2::BdNode2(
  const TemplNode* templ_node,
  SizeType input_num,
  SizeType op_id,
  const std::vector<SizeType>& opr_list
) : BdNode(templ_node, input_num, op_id, opr_list)
{
}

// @brief 出力値に関する制約を追加する．
SatLiteral
BdNode2::add_ovar_cond(
  SatSolver& solver,
  const std::vector<bool>& ival_list,
  const std::vector<SatLiteral>& opvar_list
) const
{
  auto templ_node = dynamic_cast<const TemplNode2*>(_template());
  // 演算ノードの出力の値を表す変数
  auto ovar = solver.new_variable(false);
  auto opr0 = opr(0);
  auto opr1 = opr(1);
  if ( opr1 < input_num() ) {
    // 両方入力
    templ_node->add_op_cond(solver, ival_list[opr0], ival_list[opr1], ovar);
  }
  else if ( opr0 < input_num() ) {
    // opr0 のみ入力
    auto ivar1 = opvar_list[opr1 - input_num()];
    templ_node->add_op_cond(solver, ival_list[opr0], ivar1, ovar);
  }
  else {
    // 両方演算
    auto ivar0 = opvar_list[opr0 - input_num()];
    auto ivar1 = opvar_list[opr1 - input_num()];
    templ_node->add_op_cond(solver, ivar0, ivar1, ovar);
  }
  return ovar;
}

END_NAMESPACE_YM_EXSYN
