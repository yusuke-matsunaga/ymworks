
/// @file BdNode.cc
/// @brief BdNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "BdNode.h"
#include "BdNode2.h"
#include "BdNode3.h"
#include "BdNode4.h"
#include "TemplNode2.h"
#include "TemplNode3.h"
#include "TemplNode4.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス BdNode
//////////////////////////////////////////////////////////////////////

// @brief 実際の派生クラスを作る．
const BdNode*
BdNode::new_obj(
  const TemplNode* templ_node,
  SizeType input_num,
  SizeType op_id,
  const std::vector<SizeType>& opr_list
)
{
  auto opr_num = opr_list.size();
  switch ( opr_num ) {
  case 2: return new BdNode2(templ_node, input_num, op_id, opr_list);
  case 3: return new BdNode3(templ_node, input_num, op_id, opr_list);
  case 4: return new BdNode4(templ_node, input_num, op_id, opr_list);
  }
  throw std::invalid_argument{"opr_list is too LARGE"};
}


//////////////////////////////////////////////////////////////////////
// クラス BdNode2
//////////////////////////////////////////////////////////////////////

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
    auto ival0 = ival_list[opr0];
    auto ival1 = ival_list[opr1];
    templ_node->add_op_cond(solver, ival0, ival1, ovar);
  }
  else if ( opr0 < input_num() ) {
    // opr0 のみ入力
    auto ival0 = ival_list[opr0];
    auto ivar1 = opvar_list[opr1 - input_num()];
    templ_node->add_op_cond(solver, ival0, ivar1, ovar);
  }
  else {
    // 両方演算
    auto ivar0 = opvar_list[opr0 - input_num()];
    auto ivar1 = opvar_list[opr1 - input_num()];
    templ_node->add_op_cond(solver, ivar0, ivar1, ovar);
  }
  return ovar;
}


//////////////////////////////////////////////////////////////////////
// クラス BdNode3
//////////////////////////////////////////////////////////////////////

// @brief 出力値に関する制約を追加する．
SatLiteral
BdNode3::add_ovar_cond(
  SatSolver& solver,
  const std::vector<bool>& ival_list,
  const std::vector<SatLiteral>& opvar_list
) const
{
  auto templ_node = dynamic_cast<const TemplNode3*>(_template());

  // 演算ノードの出力の値を表す変数
  auto ovar = solver.new_variable(false);

  auto opr0 = opr(0);
  auto opr1 = opr(1);
  auto opr2 = opr(2);
  if ( opr2 < input_num() ) {
    // 3つとも入力
    auto ival0 = ival_list[opr0];
    auto ival1 = ival_list[opr1];
    auto ival2 = ival_list[opr2];
    templ_node->add_op_cond(solver, ival0, ival1, ival2, ovar);
  }
  else if ( opr1 < input_num() ) {
    // opr0 と opr1 が入力
    auto ival0 = ival_list[opr0];
    auto ival1 = ival_list[opr1];
    auto ivar2 = opvar_list[opr2 - input_num()];
    templ_node->add_op_cond(solver, ival0, ival1, ivar2, ovar);
  }
  else if ( opr1 < input_num() ) {
    // opr0 が入力
    auto ival0 = ival_list[opr0];
    auto ivar1 = opvar_list[opr1 - input_num()];
    auto ivar2 = opvar_list[opr2 - input_num()];
    templ_node->add_op_cond(solver, ival0, ivar1, ivar2, ovar);
  }
  else {
    // 3つとも演算ノード
    auto ivar0 = opvar_list[opr0 - input_num()];
    auto ivar1 = opvar_list[opr1 - input_num()];
    auto ivar2 = opvar_list[opr2 - input_num()];
    templ_node->add_op_cond(solver, ivar0, ivar1, ivar2, ovar);
  }
  return ovar;
}


//////////////////////////////////////////////////////////////////////
// クラス BdNode4
//////////////////////////////////////////////////////////////////////

// @brief 出力値に関する制約を追加する．
SatLiteral
BdNode4::add_ovar_cond(
  SatSolver& solver,
  const std::vector<bool>& ival_list,
  const std::vector<SatLiteral>& opvar_list
) const
{
  auto templ_node = dynamic_cast<const TemplNode4*>(_template());

  // 演算ノードの出力の値を表す変数
  auto ovar = solver.new_variable(false);

  auto opr0 = opr(0);
  auto opr1 = opr(1);
  auto opr2 = opr(2);
  auto opr3 = opr(3);
  if ( opr3 < input_num() ) {
    // 4つとも入力
    auto ival0 = ival_list[opr0];
    auto ival1 = ival_list[opr1];
    auto ival2 = ival_list[opr2];
    auto ival3 = ival_list[opr3];
    templ_node->add_op_cond(solver, ival0, ival1, ival2, ival3, ovar);
  }
  else if ( opr2 < input_num() ) {
    // opr0, opr1, opr2が入力
    auto ival0 = ival_list[opr0];
    auto ival1 = ival_list[opr1];
    auto ival2 = ival_list[opr2];
    auto ivar3 = opvar_list[opr3 - input_num()];
    templ_node->add_op_cond(solver, ival0, ival1, ival2, ivar3, ovar);
  }
  else if ( opr1 < input_num() ) {
    // opr0 と opr1 が入力
    auto ival0 = ival_list[opr0];
    auto ival1 = ival_list[opr1];
    auto ivar2 = opvar_list[opr2 - input_num()];
    auto ivar3 = opvar_list[opr3 - input_num()];
    templ_node->add_op_cond(solver, ival0, ival1, ivar2, ivar3, ovar);
  }
  else if ( opr1 < input_num() ) {
    // opr0 が入力
    auto ival0 = ival_list[opr0];
    auto ivar1 = opvar_list[opr1 - input_num()];
    auto ivar2 = opvar_list[opr2 - input_num()];
    auto ivar3 = opvar_list[opr3 - input_num()];
    templ_node->add_op_cond(solver, ival0, ivar1, ivar2, ivar3, ovar);
  }
  else {
    // 4つとも演算ノード
    auto ivar0 = opvar_list[opr0 - input_num()];
    auto ivar1 = opvar_list[opr1 - input_num()];
    auto ivar2 = opvar_list[opr2 - input_num()];
    auto ivar3 = opvar_list[opr3 - input_num()];
    templ_node->add_op_cond(solver, ivar0, ivar1, ivar2, ivar3, ovar);
  }
  return ovar;
}

END_NAMESPACE_YM_EXSYN
