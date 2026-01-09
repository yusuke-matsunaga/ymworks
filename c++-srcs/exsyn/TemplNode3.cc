
/// @file TemplNode3.cc
/// @brief TemplNode3 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "TemplNode3.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_EXSYN

BEGIN_NONAMESPACE

// 禁止する mTval の組み合わせを追加する．
inline
void
add_ngpat(
  SatSolver& solver,
  int p001,
  int p010,
  int p011,
  int p100,
  int p101,
  int p110,
  int p111,
  const std::vector<SatLiteral>& tval
)
{
  auto tmp_lits = std::vector<SatLiteral>{
    p001 ? tval[0] : ~tval[0],
    p010 ? tval[1] : ~tval[1],
    p011 ? tval[2] : ~tval[2],
    p100 ? tval[3] : ~tval[3],
    p101 ? tval[4] : ~tval[4],
    p110 ? tval[5] : ~tval[5],
    p111 ? tval[6] : ~tval[6]
  };
  solver.add_clause(tmp_lits);
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス TemplNode3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TemplNode3::TemplNode3(
  SatSolver& solver
) : mTval{solver.new_variable(true),
	  solver.new_variable(true),
	  solver.new_variable(true),
	  solver.new_variable(true),
	  solver.new_variable(true),
	  solver.new_variable(true),
	  solver.new_variable(true)}
{
  // 演算タイプは全部で 2^7 = 128 通りが考えられるが，
  // 三項演算のみを対象とする．
  // すわなち，
  // - 0入力： 定数0 の1個
  // - 1入力： リテラル関数x0, x1, x2 の3個
  // - 2入力： AND，AND01，AND10，OR，XOR の入力の組わせ3通りの15個
  // の計19個を除外する必要がある．
  //
  // - (0, 0, 0, 0, 0, 0, 0): 定数0
  // - (0, 0, 0, 1, 1, 1, 1): x0
  // - (0, 1, 1, 0, 0, 1, 1): x1
  // - (1, 0, 1, 0, 1, 0, 1): x2
  // - (0, 0, 0, 0, 0, 1, 1): x0 & x1
  // - (0, 0, 0, 0, 1, 0, 1): x0 & x2
  // - (0, 0, 1, 0, 0, 0, 1): x1 & x2
  // - (0, 1, 1, 0, 0, 0, 0): x0' & x1
  // - (0, 0, 0, 1, 1, 0, 0): x0 & x1'
  // - (1, 0, 1, 0, 0, 0, 0): x0' & x2
  // - (0, 0, 0, 1, 0, 1, 0): x0 & x2'
  // - (1, 0, 0, 0, 1, 0, 0): x1' & x2
  // - (0, 1, 0, 0, 0, 1, 0): x1 & x2'
  // - (0, 1, 1, 1, 1, 1, 1): x0 | x1
  // - (1, 0, 1, 1, 1, 1, 1): x0 | x2
  // - (1, 1, 1, 0, 1, 1, 1): x1 | x2
  // - (0, 1, 1, 1, 1, 0, 0): x0 ^ x1
  // - (1, 0, 1, 1, 0, 1, 0): x0 ^ x2
  // - (1, 1, 0, 0, 1, 1, 0): x1 ^ x2

  // utils/tv3gen.py で生成
  add_ngpat(solver, 0, 0, 0, 0, 0, 0, 0, mTval);
  add_ngpat(solver, 1, 0, 1, 0, 0, 0, 0, mTval);
  add_ngpat(solver, 0, 1, 1, 0, 0, 0, 0, mTval);
  add_ngpat(solver, 1, 0, 0, 0, 1, 0, 0, mTval);
  add_ngpat(solver, 0, 0, 0, 1, 1, 0, 0, mTval);
  add_ngpat(solver, 0, 1, 1, 1, 1, 0, 0, mTval);
  add_ngpat(solver, 0, 1, 0, 0, 0, 1, 0, mTval);
  add_ngpat(solver, 0, 0, 0, 1, 0, 1, 0, mTval);
  add_ngpat(solver, 1, 0, 1, 1, 0, 1, 0, mTval);
  add_ngpat(solver, 1, 1, 0, 0, 1, 1, 0, mTval);
  add_ngpat(solver, 0, 0, 1, 0, 0, 0, 1, mTval);
  add_ngpat(solver, 0, 0, 0, 0, 1, 0, 1, mTval);
  add_ngpat(solver, 1, 0, 1, 0, 1, 0, 1, mTval);
  add_ngpat(solver, 0, 0, 0, 0, 0, 1, 1, mTval);
  add_ngpat(solver, 0, 1, 1, 0, 0, 1, 1, mTval);
  add_ngpat(solver, 1, 1, 1, 0, 1, 1, 1, mTval);
  add_ngpat(solver, 0, 0, 0, 1, 1, 1, 1, mTval);
  add_ngpat(solver, 1, 0, 1, 1, 1, 1, 1, mTval);
  add_ngpat(solver, 0, 1, 1, 1, 1, 1, 1, mTval);
}

// @brief SATモデルから演算タイプを得る．
TemplNode::MapInfo
TemplNode3::get_map_info(
  const SatModel& model
) const
{
  auto tval001 = model[mTval[0]] == SatBool3::True;
  auto tval010 = model[mTval[1]] == SatBool3::True;
  auto tval011 = model[mTval[2]] == SatBool3::True;
  auto tval100 = model[mTval[3]] == SatBool3::True;
  auto tval101 = model[mTval[4]] == SatBool3::True;
  auto tval110 = model[mTval[5]] == SatBool3::True;
  auto tval111 = model[mTval[6]] == SatBool3::True;

  BcBvType tv = BcBv_ALL0;
  if ( tval001 ) {
    tv |= 0x02U;
  }
  if ( tval010 ) {
    tv |= 0x04U;
  }
  if ( tval011 ) {
    tv |= 0x08U;
  }
  if ( tval100 ) {
    tv |= 0x10U;
  }
  if ( tval101 ) {
    tv |= 0x20U;
  }
  if ( tval110 ) {
    tv |= 0x40U;
  }
  if ( tval111 ) {
    tv |= 0x80U;
  }

  auto type = BcOpType::NONE;
  bool inv0 = false;
  bool inv1 = false;
  bool inv2 = false;
  switch ( tv ) {
  case 0xFEU: type = BcOpType::OR;  break;
  case 0x96U: type = BcOpType::XOR; break;
  case 0x80U: type = BcOpType::AND; break;
  case 0x40U: type = BcOpType::AND; inv2 = true; break;
  case 0x20U: type = BcOpType::AND; inv1 = true; break;
  case 0x10U: type = BcOpType::AND; inv1 = true; inv2 = true; break;
  case 0x08U: type = BcOpType::AND; inv0 = true; break;
  case 0x04U: type = BcOpType::AND; inv0 = true; inv2 = true; break;
  case 0x02U: type = BcOpType::AND; inv0 = true; inv1 = true; break;
  default:    type = BcOpType::TV;  break;
  }
  return TemplNode::MapInfo{type, {tv}, {inv0, inv1, inv2}};
}

// @brief 3項演算の出力値に関する制約を追加する．
void
TemplNode3::add_op_cond(
  SatSolver& solver,
  SatLiteral ivar0,
  SatLiteral ivar1,
  SatLiteral ivar2,
  SatLiteral ovar
) const
{
  { // ivals = 000 -> ovar = 0
    auto _ = solver.condblock({~ivar0, ~ivar1, ~ivar2});
    solver.add_clause(          ~ovar);
  }
  { // ivals = 001 -> ovar = mTval001
    auto _ = solver.condblock({~ivar0, ~ivar1,  ivar2});
    solver.add_clause(~mTval[0],  ovar);
    solver.add_clause( mTval[0], ~ovar);
  }
  { // ivals = 010 -> ovar = mTval010
    auto _ = solver.condblock({~ivar0,  ivar1, ~ivar2});
    solver.add_clause(~mTval[1],  ovar);
    solver.add_clause( mTval[1], ~ovar);
  }
  { // ivals = 011 -> ovar = mTval011
    auto _ = solver.condblock({~ivar0,  ivar1,  ivar2});
    solver.add_clause(~mTval[2],  ovar);
    solver.add_clause( mTval[2], ~ovar);
  }
  { // ivals = 100 -> ovar = mTval100
    auto _ = solver.condblock({ ivar0, ~ivar1, ~ivar2});
    solver.add_clause(~mTval[3],  ovar);
    solver.add_clause( mTval[3], ~ovar);
  }
  { // ivals = 101 -> ovar = mTval101
    auto _ = solver.condblock({ ivar0, ~ivar1,  ivar2});
    solver.add_clause(~mTval[4],  ovar);
    solver.add_clause( mTval[4], ~ovar);
  }
  { // ivals = 110 -> ovar = mTval110
    auto _ = solver.condblock({ ivar0,  ivar1, ~ivar2});
    solver.add_clause(~mTval[5],  ovar);
    solver.add_clause( mTval[5], ~ovar);
  }
  { // ivals = 111 -> ovar = mTval111
    auto _ = solver.condblock({ ivar0,  ivar1,  ivar2});
    solver.add_clause(~mTval[6],  ovar);
    solver.add_clause( mTval[6], ~ovar);
  }
}

// @brief 3項演算の出力値に関する制約を追加する．
void
TemplNode3::add_op_cond(
  SatSolver& solver,
  bool ival0,
  SatLiteral ivar1,
  SatLiteral ivar2,
  SatLiteral ovar
) const
{
  if ( ival0 ) {
    { // ivals = 100 -> ovar = mTval100
      auto _ = solver.condblock({~ivar1, ~ivar2});
      solver.add_clause(~mTval[3],  ovar);
      solver.add_clause( mTval[3], ~ovar);
    }
    { // ivals = 101 -> ovar = mTval101
      auto _ = solver.condblock({~ivar1,  ivar2});
      solver.add_clause(~mTval[4],  ovar);
      solver.add_clause( mTval[4], ~ovar);
    }
    { // ivals = 110 -> ovar = mTval110
      auto _ = solver.condblock({ ivar1, ~ivar2});
      solver.add_clause(~mTval[5],  ovar);
      solver.add_clause( mTval[5], ~ovar);
    }
    { // ivals = 111 -> ovar = mTval111
      auto _ = solver.condblock({ ivar1,  ivar2});
      solver.add_clause(~mTval[6],  ovar);
      solver.add_clause( mTval[6], ~ovar);
    }
  }
  else {
    { // ivals = 000 -> ovar = 0
      auto _ = solver.condblock({ ~ivar1, ~ivar2});
      solver.add_clause(          ~ovar);
    }
    { // ivals = 001 -> ovar = mTval001
      auto _ = solver.condblock({~ivar1,  ivar2});
      solver.add_clause(~mTval[0],  ovar);
      solver.add_clause( mTval[0], ~ovar);
    }
    { // ivals = 010 -> ovar = mTval010
      auto _ = solver.condblock({ ivar1, ~ivar2});
      solver.add_clause(~mTval[1],  ovar);
      solver.add_clause( mTval[1], ~ovar);
    }
    { // ivals = 011 -> ovar = mTval011
      auto _ = solver.condblock({ ivar1,  ivar2});
      solver.add_clause(~mTval[2],  ovar);
      solver.add_clause( mTval[2], ~ovar);
    }
  }
}

// @brief 3項演算の出力値に関する制約を追加する．
void
TemplNode3::add_op_cond(
  SatSolver& solver,
  bool ival0,
  bool ival1,
  SatLiteral ivar2,
  SatLiteral ovar
) const
{
  if ( ival0 ) {
    if ( ival1 ) {
      { // ivals = 110 -> ovar = mTval110
	auto _ = solver.condblock(~ivar2);
	solver.add_clause(~mTval[5],  ovar);
	solver.add_clause( mTval[5], ~ovar);
      }
      { // ivals = 111 -> ovar = mTval111
	auto _ = solver.condblock( ivar2);
	solver.add_clause(~mTval[6],  ovar);
	solver.add_clause( mTval[6], ~ovar);
      }
    }
    else {
      { // ivals = 100 -> ovar = mTval100
	auto _ = solver.condblock(~ivar2);
	solver.add_clause(~mTval[3],  ovar);
	solver.add_clause( mTval[3], ~ovar);
      }
      { // ivals = 101 -> ovar = mTval101
	auto _ = solver.condblock( ivar2);
	solver.add_clause(~mTval[4],  ovar);
	solver.add_clause( mTval[4], ~ovar);
      }
    }
  }
  else {
    if ( ival1 ) {
      { // ivals = 010 -> ovar = mTval010
	auto _ = solver.condblock(~ivar2);
	solver.add_clause(~mTval[1],  ovar);
	solver.add_clause( mTval[1], ~ovar);
      }
      { // ivals = 011 -> ovar = mTval011
	auto _ = solver.condblock( ivar2);
	solver.add_clause(~mTval[2],  ovar);
	solver.add_clause( mTval[2], ~ovar);
      }
    }
    else {
      { // ivals = 000 -> ovar = 0
	auto _ = solver.condblock(~ivar2);
	solver.add_clause(          ~ovar);
      }
      { // ivals = 001 -> ovar = mTval001
	auto _ = solver.condblock( ivar2);
	solver.add_clause(~mTval[0],  ovar);
	solver.add_clause( mTval[0], ~ovar);
      }
    }
  }

}

// @brief 3項演算の出力値に関する制約を追加する．
void
TemplNode3::add_op_cond(
  SatSolver& solver,
  bool ival0,
  bool ival1,
  bool ival2,
  SatLiteral ovar
) const
{
  if ( ival0 ) {
    if ( ival1 ) {
      if ( ival2 ) {
	// ivals = 111 -> ovar = mTval111
	solver.add_clause(~mTval[6],  ovar);
	solver.add_clause( mTval[6], ~ovar);
      }
      else {
	// ivals = 110 -> ovar = mTval110
	solver.add_clause(~mTval[5],  ovar);
	solver.add_clause( mTval[5], ~ovar);
      }
    }
    else {
      if ( ival2 ) {
	// ivals = 101 -> ovar = mTval101
	solver.add_clause(~mTval[4],  ovar);
	solver.add_clause( mTval[4], ~ovar);
      }
      else {
	// ivals = 100 -> ovar = mTval100
	solver.add_clause(~mTval[3],  ovar);
	solver.add_clause( mTval[3], ~ovar);
      }
    }
  }
  else {
    if ( ival1 ) {
      if ( ival2 ) {
	// ivals = 011 -> ovar = mTval011
	solver.add_clause(~mTval[2],  ovar);
	solver.add_clause( mTval[2], ~ovar);
      }
      else {
	// ivals = 010 -> ovar = mTval010
	solver.add_clause(~mTval[1],  ovar);
	solver.add_clause( mTval[1], ~ovar);
      }
    }
    else {
      if ( ival2 ) {
	// ivals = 001 -> ovar = mTval001
	solver.add_clause(~mTval[0],  ovar);
	solver.add_clause( mTval[0], ~ovar);
      }
      else {
	// ivals = 000 -> ovar = 0
	solver.add_clause(           ~ovar);
      }
    }
  }
}

END_NAMESPACE_YM_EXSYN
