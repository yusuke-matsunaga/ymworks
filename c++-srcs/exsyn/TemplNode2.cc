
/// @file TemplNode2.cc
/// @brief TemplNode2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "TemplNode2.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス　TemplNode2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TemplNode2::TemplNode2(
  SatSolver& solver
) : mTval{solver.new_variable(true),
	  solver.new_variable(true),
	  solver.new_variable(true)}
{
  // 禁止する組み合わせは以下の通り
  // - (0, 0, 0)
  // - (0, 1, 1)
  // - (1, 0, 1)
  solver.add_clause( mTval[0],  mTval[1],  mTval[2]);
  solver.add_clause( mTval[0], ~mTval[1], ~mTval[2]);
  solver.add_clause(~mTval[0],  mTval[1], ~mTval[2]);
}

// @brief SATモデルから演算タイプを得る．
TemplNode::MapInfo
TemplNode2::get_map_info(
  const SatModel& model
) const
{
  auto tval01 = model[mTval[0]] == SatBool3::True;
  auto tval10 = model[mTval[1]] == SatBool3::True;
  auto tval11 = model[mTval[2]] == SatBool3::True;

  BcBvType tv = BcBv_ALL0;
  if ( tval01 ) {
    tv |= 0x2U;
  }
  if ( tval10 ) {
    tv |= 0x4U;
  }
  if ( tval11 ) {
    tv |= 0x8U;
  }
  auto type = BcOpType::NONE;
  bool inv0 = false;
  bool inv1 = false;
  switch ( tv ) {
  case 0xEU: type = BcOpType::OR;  break;
  case 0x6U: type = BcOpType::XOR; break;
  case 0x8U: type = BcOpType::AND; break;
  case 0x4U: type = BcOpType::AND; inv1 = true; break;
  case 0x2U: type = BcOpType::AND; inv0 = true; break;
  default: throw std::runtime_error{"invalid tval"};
  }
  return TemplNode::MapInfo{type, {tv}, {inv0, inv1}};
}

// @brief 2項演算の出力値に関する制約を追加する．
void
TemplNode2::add_op_cond(
  SatSolver& solver,
  SatLiteral ivar0,
  SatLiteral ivar1,
  SatLiteral ovar
) const
{
  { // ivals = 00 -> ovar = 0
    auto _ = solver.condblock({~ivar0, ~ivar1});
    solver.add_clause(          ~ovar);
  }
  { // ivals = 01 -> ovar = mTval01
    auto _ = solver.condblock({~ivar0,  ivar1});
    solver.add_clause(~mTval[0],  ovar);
    solver.add_clause( mTval[0], ~ovar);
  }
  { // ivals = 10 -> ovar = mTval10
    auto _ = solver.condblock({ ivar0, ~ivar1});
    solver.add_clause(~mTval[1],  ovar);
    solver.add_clause( mTval[1], ~ovar);
  }
  { // ivals = 11 -> ovar = mTval11
    auto _ = solver.condblock({ ivar0,  ivar1});
    solver.add_clause(~mTval[2],  ovar);
    solver.add_clause( mTval[2], ~ovar);
  }
}

// @brief 2項演算の出力値に関する制約を追加する．
void
TemplNode2::add_op_cond(
  SatSolver& solver,
  bool ival0,
  SatLiteral ivar1,
  SatLiteral ovar
) const
{
  if ( ival0 ) {
    { // ivals = 10 -> ovar = mTval10
      auto _ = solver.condblock(~ivar1);
      solver.add_clause(~mTval[1],  ovar);
      solver.add_clause( mTval[1], ~ovar);
    }
    { // ivals = 11 -> ovar = mTval11
      auto _ = solver.condblock(ivar1);
      solver.add_clause(~mTval[2],  ovar);
      solver.add_clause( mTval[2], ~ovar);
    }
  }
  else {
    { // ivals = 00 -> ovar = 0
      auto _ = solver.condblock(~ivar1);
      solver.add_clause(          ~ovar);
    }
    { // ivals = 01 -> ovar = mTval01
      auto _ = solver.condblock(ivar1);
      solver.add_clause(~mTval[0],  ovar);
      solver.add_clause( mTval[0], ~ovar);
    }
  }
}

// @brief 2項演算の出力値に関する制約を追加する．
void
TemplNode2::add_op_cond(
  SatSolver& solver,
  bool ival0,
  bool ival1,
  SatLiteral ovar
) const
{
  if ( ival0 ) {
    if ( ival1 ) {
      // ivals = 11 -> ovar = mTval11
      solver.add_clause(~mTval[2],  ovar);
      solver.add_clause( mTval[2], ~ovar);
    }
    else {
      // ivals = 10 -> ovar = mTval10
      solver.add_clause(~mTval[1],  ovar);
      solver.add_clause( mTval[1], ~ovar);
    }
  }
  else {
    if ( ival1 ) {
      // ivals = 01 -> ovar = mTval01
      solver.add_clause(~mTval[0],  ovar);
      solver.add_clause( mTval[0], ~ovar);
    }
    else {
      // ivals = 00 -> ovar = 0
      solver.add_clause(          ~ovar);
    }
  }
}

END_NAMESPACE_YM_EXSYN
