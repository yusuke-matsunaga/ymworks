
/// @file TemplNode4.cc
/// @brief TemplNode4 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "TemplNode4.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_EXSYN

BEGIN_NONAMESPACE

// 禁止する mTval の組み合わせを追加する．
inline
void
add_ngpat4(
  SatSolver& solver,
  int p0001,
  int p0010,
  int p0011,
  int p0100,
  int p0101,
  int p0110,
  int p0111,
  int p1000,
  int p1001,
  int p1010,
  int p1011,
  int p1100,
  int p1101,
  int p1110,
  int p1111,
  const std::vector<SatLiteral>& tval
)
{
  auto tmp_lits = std::vector<SatLiteral>{
    p0001 ? tval[0] : ~tval[0],
    p0010 ? tval[1] : ~tval[1],
    p0011 ? tval[2] : ~tval[2],
    p0100 ? tval[3] : ~tval[3],
    p0101 ? tval[4] : ~tval[4],
    p0110 ? tval[5] : ~tval[5],
    p0111 ? tval[6] : ~tval[6],
    p1000 ? tval[7] : ~tval[7],
    p1001 ? tval[8] : ~tval[8],
    p1010 ? tval[9] : ~tval[9],
    p1011 ? tval[10] : ~tval[10],
    p1100 ? tval[11] : ~tval[11],
    p1101 ? tval[12] : ~tval[12],
    p1110 ? tval[13] : ~tval[13],
    p1111 ? tval[14] : ~tval[14]
  };
  solver.add_clause(tmp_lits);
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス TemplNode4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TemplNode4::TemplNode4(
  SatSolver& solver
) : mTval{solver.new_variable(true),
	  solver.new_variable(true),
	  solver.new_variable(true),
	  solver.new_variable(true),
	  solver.new_variable(true),
	  solver.new_variable(true),
	  solver.new_variable(true),
	  solver.new_variable(true),
	  solver.new_variable(true),
	  solver.new_variable(true),
	  solver.new_variable(true),
	  solver.new_variable(true),
	  solver.new_variable(true),
	  solver.new_variable(true),
	  solver.new_variable(true)}
{
  // utils/tv4gen.py で生成
  #include "tv4ngpat.h"
}

// @brief SATモデルから演算タイプを得る．
TemplNode::MapInfo
TemplNode4::get_map_info(
  const SatModel& model
) const
{
  auto tval0001 = model[mTval[0]] == SatBool3::True;
  auto tval0010 = model[mTval[1]] == SatBool3::True;
  auto tval0011 = model[mTval[2]] == SatBool3::True;
  auto tval0100 = model[mTval[3]] == SatBool3::True;
  auto tval0101 = model[mTval[4]] == SatBool3::True;
  auto tval0110 = model[mTval[5]] == SatBool3::True;
  auto tval0111 = model[mTval[6]] == SatBool3::True;
  auto tval1000 = model[mTval[7]] == SatBool3::True;
  auto tval1001 = model[mTval[8]] == SatBool3::True;
  auto tval1010 = model[mTval[9]] == SatBool3::True;
  auto tval1011 = model[mTval[10]] == SatBool3::True;
  auto tval1100 = model[mTval[11]] == SatBool3::True;
  auto tval1101 = model[mTval[12]] == SatBool3::True;
  auto tval1110 = model[mTval[13]] == SatBool3::True;
  auto tval1111 = model[mTval[14]] == SatBool3::True;

  BcBvType tv = BcBv_ALL0;
  if ( tval0001 ) {
    tv |= 0x0002U;
  }
  if ( tval0010 ) {
    tv |= 0x0004U;
  }
  if ( tval0011 ) {
    tv |= 0x0008U;
  }
  if ( tval0100 ) {
    tv |= 0x0010U;
  }
  if ( tval0101 ) {
    tv |= 0x0020U;
  }
  if ( tval0110 ) {
    tv |= 0x0040U;
  }
  if ( tval0111 ) {
    tv |= 0x0080U;
  }
  if ( tval1000 ) {
    tv |= 0x0100U;
  }
  if ( tval1001 ) {
    tv |= 0x0200U;
  }
  if ( tval1010 ) {
    tv |= 0x0400U;
  }
  if ( tval1011 ) {
    tv |= 0x0800U;
  }
  if ( tval1100 ) {
    tv |= 0x1000U;
  }
  if ( tval1101 ) {
    tv |= 0x2000U;
  }
  if ( tval1110 ) {
    tv |= 0x4000U;
  }
  if ( tval1111 ) {
    tv |= 0x8000U;
  }
  auto type = BcOpType::NONE;
  bool inv0 = false;
  bool inv1 = false;
  bool inv2 = false;
  bool inv3 = false;
  switch ( tv ) {
  case 0xFFFEU: type = BcOpType::OR;  break;
  case 0x6996U: type = BcOpType::XOR; break;
  case 0x8000U: type = BcOpType::AND; break;
  case 0x4000U: type = BcOpType::AND; inv3 = true; break;
  case 0x2000U: type = BcOpType::AND; inv2 = true; break;
  case 0x1000U: type = BcOpType::AND; inv2 = true; inv3 = true; break;
  case 0x0800U: type = BcOpType::AND; inv1 = true; break;
  case 0x0400U: type = BcOpType::AND; inv1 = true; inv3 = true; break;
  case 0x0200U: type = BcOpType::AND; inv1 = true; inv2 = true; break;
  case 0x0100U: type = BcOpType::AND; inv1 = true; inv2 = true; inv3 = true; break;
  case 0x0080U: type = BcOpType::AND; inv0 = true; break;
  case 0x0040U: type = BcOpType::AND; inv0 = true; inv3 = true; break;
  case 0x0020U: type = BcOpType::AND; inv0 = true; inv2 = true; break;
  case 0x0010U: type = BcOpType::AND; inv0 = true; inv2 = true; inv3 = true; break;
  case 0x0008U: type = BcOpType::AND; inv0 = true; inv1 = true; break;
  case 0x0004U: type = BcOpType::AND; inv0 = true; inv1 = true; inv3 = true; break;
  case 0x0002U: type = BcOpType::AND; inv0 = true; inv1 = true; inv2 = true; break;
  default:      type = BcOpType::TV;  break;
  }
  return TemplNode::MapInfo{type, {tv}, {inv0, inv1, inv2, inv3}};
}

// @brief 4項演算の出力値に関する制約を追加する．
void
TemplNode4::add_op_cond(
  SatSolver& solver,
  SatLiteral ivar0,
  SatLiteral ivar1,
  SatLiteral ivar2,
  SatLiteral ivar3,
  SatLiteral ovar
) const
{
  { // ivals = 0000 -> ovar = 0
    auto _ = solver.condblock({~ivar0, ~ivar1, ~ivar2, ~ivar3});
    solver.add_clause(          ~ovar);
  }
  { // ivals = 0001 -> ovar = mTval0001
    auto _ = solver.condblock({~ivar0, ~ivar1, ~ivar2,  ivar3});
    solver.add_clause(~mTval[0],  ovar);
    solver.add_clause( mTval[0], ~ovar);
  }
  { // ivals = 0010 -> ovar = mTval0010
    auto _ = solver.condblock({~ivar0, ~ivar1,  ivar2, ~ivar3});
    solver.add_clause(~mTval[1],  ovar);
    solver.add_clause( mTval[1], ~ovar);
  }
  { // ivals = 0011 -> ovar = mTval0011
    auto _ = solver.condblock({~ivar0, ~ivar1,  ivar2,  ivar3});
    solver.add_clause(~mTval[2],  ovar);
    solver.add_clause( mTval[2], ~ovar);
  }
  { // ivals = 0100 -> ovar = mTval0100
    auto _ = solver.condblock({~ivar0,  ivar1, ~ivar2, ~ivar3});
    solver.add_clause(~mTval[3],  ovar);
    solver.add_clause( mTval[3], ~ovar);
  }
  { // ivals = 0101 -> ovar = mTval0101
    auto _ = solver.condblock({~ivar0,  ivar1, ~ivar2,  ivar3});
    solver.add_clause(~mTval[4],  ovar);
    solver.add_clause( mTval[4], ~ovar);
  }
  { // ivals = 0110 -> ovar = mTval0110
    auto _ = solver.condblock({~ivar0,  ivar1,  ivar2, ~ivar3});
    solver.add_clause(~mTval[5],  ovar);
    solver.add_clause( mTval[5], ~ovar);
  }
  { // ivals = 0111 -> ovar = mTval0111
    auto _ = solver.condblock({~ivar0,  ivar1,  ivar2,  ivar3});
    solver.add_clause(~mTval[6],  ovar);
    solver.add_clause( mTval[6], ~ovar);
  }
  { // ivals = 1000 -> ovar = mTval1000
    auto _ = solver.condblock({ ivar0, ~ivar1, ~ivar2, ~ivar3});
    solver.add_clause(~mTval[7],  ovar);
    solver.add_clause( mTval[7], ~ovar);
  }
  { // ivals = 1001 -> ovar = mTval1001
    auto _ = solver.condblock({ ivar0, ~ivar1, ~ivar2,  ivar3});
    solver.add_clause(~mTval[8],  ovar);
    solver.add_clause( mTval[8], ~ovar);
  }
  { // ivals = 1010 -> ovar = mTval1010
    auto _ = solver.condblock({ ivar0, ~ivar1,  ivar2, ~ivar3});
    solver.add_clause(~mTval[9],  ovar);
    solver.add_clause( mTval[9], ~ovar);
  }
  { // ivals = 1011 -> ovar = mTval1011
    auto _ = solver.condblock({ ivar0, ~ivar1,  ivar2,  ivar3});
    solver.add_clause(~mTval[10],  ovar);
    solver.add_clause( mTval[10], ~ovar);
  }
  { // ivals = 1100 -> ovar = mTval1100
    auto _ = solver.condblock({ ivar0,  ivar1, ~ivar2, ~ivar3});
    solver.add_clause(~mTval[11],  ovar);
    solver.add_clause( mTval[11], ~ovar);
  }
  { // ivals = 1101 -> ovar = mTval1101
    auto _ = solver.condblock({ ivar0,  ivar1, ~ivar2,  ivar3});
    solver.add_clause(~mTval[12],  ovar);
    solver.add_clause( mTval[12], ~ovar);
  }
  { // ivals = 1110 -> ovar = mTval1110
    auto _ = solver.condblock({ ivar0,  ivar1,  ivar2, ~ivar3});
    solver.add_clause(~mTval[13],  ovar);
    solver.add_clause( mTval[13], ~ovar);
  }
  { // ivals = 1111 -> ovar = mTval1111
    auto _ = solver.condblock({ ivar0,  ivar1,  ivar2,  ivar3});
    solver.add_clause(~mTval[14],  ovar);
    solver.add_clause( mTval[14], ~ovar);
  }
}

// @brief 4項演算の出力値に関する制約を追加する．
void
TemplNode4::add_op_cond(
  SatSolver& solver,
  bool ival0,
  SatLiteral ivar1,
  SatLiteral ivar2,
  SatLiteral ivar3,
  SatLiteral ovar
) const
{
  if ( ival0 ) {
    { // ivals = 1000 -> ovar = mTval1000
      auto _ = solver.condblock({~ivar1, ~ivar2, ~ivar3});
      solver.add_clause(~mTval[7],  ovar);
      solver.add_clause( mTval[7], ~ovar);
    }
    { // ivals = 1001 -> ovar = mTval1001
      auto _ = solver.condblock({~ivar1, ~ivar2,  ivar3});
      solver.add_clause(~mTval[8],  ovar);
      solver.add_clause( mTval[8], ~ovar);
    }
    { // ivals = 1010 -> ovar = mTval1010
      auto _ = solver.condblock({~ivar1,  ivar2, ~ivar3});
      solver.add_clause(~mTval[9],  ovar);
      solver.add_clause( mTval[9], ~ovar);
    }
    { // ivals = 1011 -> ovar = mTval1011
      auto _ = solver.condblock({~ivar1,  ivar2,  ivar3});
      solver.add_clause(~mTval[10],  ovar);
      solver.add_clause( mTval[10], ~ovar);
    }
    { // ivals = 1100 -> ovar = mTval1100
      auto _ = solver.condblock({ ivar1, ~ivar2, ~ivar3});
      solver.add_clause(~mTval[11],  ovar);
      solver.add_clause( mTval[11], ~ovar);
    }
    { // ivals = 1101 -> ovar = mTval1101
      auto _ = solver.condblock({  ivar1, ~ivar2,  ivar3});
      solver.add_clause(~mTval[12],  ovar);
      solver.add_clause( mTval[12], ~ovar);
    }
    { // ivals = 1110 -> ovar = mTval1110
      auto _ = solver.condblock({ ivar1,  ivar2, ~ivar3});
      solver.add_clause(~mTval[13],  ovar);
      solver.add_clause( mTval[13], ~ovar);
    }
    { // ivals = 1111 -> ovar = mTval1111
      auto _ = solver.condblock({ ivar1,  ivar2,  ivar3});
      solver.add_clause(~mTval[14],  ovar);
      solver.add_clause( mTval[14], ~ovar);
    }
  }
  else {
    { // ivals = 0000 -> ovar = 0
      auto _ = solver.condblock({~ivar1, ~ivar2, ~ivar3});
      solver.add_clause(           ~ovar);
    }
    { // ivals = 0001 -> ovar = mTval0001
      auto _ = solver.condblock({~ivar1, ~ivar2,  ivar3});
      solver.add_clause(~mTval[0],  ovar);
      solver.add_clause( mTval[0], ~ovar);
    }
    { // ivals = 0010 -> ovar = mTval0010
      auto _ = solver.condblock({~ivar1,  ivar2, ~ivar3});
      solver.add_clause(~mTval[1],  ovar);
      solver.add_clause( mTval[1], ~ovar);
    }
    { // ivals = 0011 -> ovar = mTval0011
      auto _ = solver.condblock({~ivar1,  ivar2,  ivar3});
      solver.add_clause(~mTval[2],  ovar);
      solver.add_clause( mTval[2], ~ovar);
    }
    { // ivals = 0100 -> ovar = mTval0100
      auto _ = solver.condblock({ ivar1, ~ivar2, ~ivar3});
      solver.add_clause(~mTval[3],  ovar);
      solver.add_clause( mTval[3], ~ovar);
    }
    { // ivals = 0101 -> ovar = mTval0101
      auto _ = solver.condblock({ ivar1, ~ivar2,  ivar3});
      solver.add_clause(~mTval[4],  ovar);
      solver.add_clause( mTval[4], ~ovar);
    }
    { // ivals = 0110 -> ovar = mTval0110
      auto _ = solver.condblock({ ivar1,  ivar2, ~ivar3});
      solver.add_clause(~mTval[5],  ovar);
      solver.add_clause( mTval[5], ~ovar);
    }
    { // ivals = 0111 -> ovar = mTval0111
      auto _ = solver.condblock({ ivar1,  ivar2,  ivar3});
      solver.add_clause(~mTval[6],  ovar);
      solver.add_clause( mTval[6], ~ovar);
    }
  }
}

// @brief 4項演算の出力値に関する制約を追加する．
void
TemplNode4::add_op_cond(
  SatSolver& solver,
  bool ival0,
  bool ival1,
  SatLiteral ivar2,
  SatLiteral ivar3,
  SatLiteral ovar
) const
{
  if ( ival0 ) {
    if ( ival1 ) {
      { // ivals = 1100 -> ovar = mTval1100
	auto _ = solver.condblock({~ivar2, ~ivar3});
	solver.add_clause(~mTval[11],  ovar);
	solver.add_clause( mTval[11], ~ovar);
      }
      { // ivals = 1101 -> ovar = mTval1101
	auto _ = solver.condblock({~ivar2,  ivar3});
	solver.add_clause(~mTval[12],  ovar);
	solver.add_clause( mTval[12], ~ovar);
      }
      { // ivals = 1110 -> ovar = mTval1110
	auto _ = solver.condblock({ ivar2, ~ivar3});
	solver.add_clause(~mTval[13],  ovar);
	solver.add_clause( mTval[13], ~ovar);
      }
      { // ivals = 1111 -> ovar = mTval1111
	auto _ = solver.condblock({ ivar2,  ivar3});
	solver.add_clause(~mTval[14],  ovar);
	solver.add_clause( mTval[14], ~ovar);
      }
    }
    else {
      { // ivals = 1000 -> ovar = mTval1000
	auto _ = solver.condblock({~ivar2, ~ivar3});
	solver.add_clause(~mTval[7],  ovar);
	solver.add_clause( mTval[7], ~ovar);
      }
      { // ivals = 1001 -> ovar = mTval1001
	auto _ = solver.condblock({~ivar2,  ivar3});
	solver.add_clause(~mTval[8],  ovar);
	solver.add_clause( mTval[8], ~ovar);
      }
      { // ivals = 1010 -> ovar = mTval1010
	auto _ = solver.condblock({ ivar2, ~ivar3});
	solver.add_clause(~mTval[9],  ovar);
	solver.add_clause( mTval[9], ~ovar);
      }
      { // ivals = 1011 -> ovar = mTval1011
	auto _ = solver.condblock({ ivar2,  ivar3});
	solver.add_clause(~mTval[10],  ovar);
	solver.add_clause( mTval[10], ~ovar);
      }
    }
  }
  else {
    if ( ival1 ) {
      { // ivals = 0100 -> ovar = mTval0100
	auto _ = solver.condblock({~ivar2, ~ivar3});
	solver.add_clause(~mTval[3],  ovar);
	solver.add_clause( mTval[3], ~ovar);
      }
      { // ivals = 0101 -> ovar = mTval0101
	auto _ = solver.condblock({~ivar2,  ivar3});
	solver.add_clause(~mTval[4],  ovar);
	solver.add_clause( mTval[4], ~ovar);
      }
      { // ivals = 0110 -> ovar = mTval0110
	auto _ = solver.condblock({ ivar2, ~ivar3});
	solver.add_clause(~mTval[5],  ovar);
	solver.add_clause( mTval[5], ~ovar);
      }
      { // ivals = 0111 -> ovar = mTval0111
	auto _ = solver.condblock({ ivar2,  ivar3});
	solver.add_clause(~mTval[6],  ovar);
	solver.add_clause( mTval[6], ~ovar);
      }
    }
    else {
      { // ivals = 0000 -> ovar = 0
	auto _ = solver.condblock({~ivar2, ~ivar3});
	solver.add_clause(           ~ovar);
      }
      { // ivals = 0001 -> ovar = mTval0001
	auto _ = solver.condblock({~ivar2,  ivar3});
	solver.add_clause(~mTval[0],  ovar);
	solver.add_clause( mTval[0], ~ovar);
      }
      { // ivals = 0010 -> ovar = mTval0010
	auto _ = solver.condblock({ ivar2, ~ivar3});
	solver.add_clause(~mTval[1],  ovar);
	solver.add_clause( mTval[1], ~ovar);
      }
      { // ivals = 0011 -> ovar = mTval0011
	auto _ = solver.condblock({ ivar2,  ivar3});
	solver.add_clause(~mTval[2],  ovar);
	solver.add_clause( mTval[2], ~ovar);
      }
    }
  }
}

// @brief 4項演算の出力値に関する制約を追加する．
void
TemplNode4::add_op_cond(
  SatSolver& solver,
  bool ival0,
  bool ival1,
  bool ival2,
  SatLiteral ivar3,
  SatLiteral ovar
) const
{
  if ( ival0 ) {
    if ( ival1 ) {
      if ( ival2 ) {
	{ // ivals = 1110 -> ovar = mTval1110
	  auto _ = solver.condblock({~ivar3});
	  solver.add_clause(~mTval[13],  ovar);
	  solver.add_clause( mTval[13], ~ovar);
	}
	{ // ivals = 1111 -> ovar = mTval1111
	  auto _ = solver.condblock({ ivar3});
	  solver.add_clause(~mTval[14],  ovar);
	  solver.add_clause( mTval[14], ~ovar);
	}
      }
      else {
	{ // ivals = 1100 -> ovar = mTval1100
	  auto _ = solver.condblock({~ivar3});
	  solver.add_clause(~mTval[11],  ovar);
	  solver.add_clause( mTval[11], ~ovar);
	}
	{ // ivals = 1101 -> ovar = mTval1101
	  auto _ = solver.condblock({ ivar3});
	  solver.add_clause(~mTval[12],  ovar);
	  solver.add_clause( mTval[12], ~ovar);
	}
      }
    }
    else {
      if ( ival2 ) {
	{ // ivals = 1010 -> ovar = mTval1010
	  auto _ = solver.condblock({~ivar3});
	  solver.add_clause(~mTval[9],  ovar);
	  solver.add_clause( mTval[9], ~ovar);
	}
	{ // ivals = 1011 -> ovar = mTval1011
	  auto _ = solver.condblock({ ivar3});
	  solver.add_clause(~mTval[10],  ovar);
	  solver.add_clause( mTval[10], ~ovar);
	}
      }
      else {
	{ // ivals = 1000 -> ovar = mTval1000
	  auto _ = solver.condblock({~ivar3});
	  solver.add_clause(~mTval[7],  ovar);
	  solver.add_clause( mTval[7], ~ovar);
	}
	{ // ivals = 1001 -> ovar = mTval1001
	  auto _ = solver.condblock({ ivar3});
	  solver.add_clause(~mTval[8],  ovar);
	  solver.add_clause( mTval[8], ~ovar);
	}
      }
    }
  }
  else {
    if ( ival1 ) {
      if ( ival2 ) {
	{ // ivals = 0110 -> ovar = mTval0110
	  auto _ = solver.condblock({~ivar3});
	  solver.add_clause(~mTval[5],  ovar);
	  solver.add_clause( mTval[5], ~ovar);
	}
	{ // ivals = 0111 -> ovar = mTval0111
	  auto _ = solver.condblock({ ivar3});
	  solver.add_clause(~mTval[6],  ovar);
	  solver.add_clause( mTval[6], ~ovar);
	}
      }
      else {
	{ // ivals = 0100 -> ovar = mTval0100
	  auto _ = solver.condblock({~ivar3});
	  solver.add_clause(~mTval[3],  ovar);
	  solver.add_clause( mTval[3], ~ovar);
	}
	{ // ivals = 0101 -> ovar = mTval0101
	  auto _ = solver.condblock({ ivar3});
	  solver.add_clause(~mTval[4],  ovar);
	  solver.add_clause( mTval[4], ~ovar);
	}
      }
    }
    else {
      if ( ival2 ) {
	{ // ivals = 0010 -> ovar = mTval0010
	  auto _ = solver.condblock({~ivar3});
	  solver.add_clause(~mTval[1],  ovar);
	  solver.add_clause( mTval[1], ~ovar);
	}
	{ // ivals = 0011 -> ovar = mTval0011
	  auto _ = solver.condblock({ ivar3});
	  solver.add_clause(~mTval[2],  ovar);
	  solver.add_clause( mTval[2], ~ovar);
	}
      }
      else {
	{ // ivals = 0000 -> ovar = 0
	  auto _ = solver.condblock({~ivar3});
	  solver.add_clause(           ~ovar);
	}
	{ // ivals = 0001 -> ovar = mTval0001
	  auto _ = solver.condblock({ ivar3});
	  solver.add_clause(~mTval[0],  ovar);
	  solver.add_clause( mTval[0], ~ovar);
	}
      }
    }
  }
}

// @brief 4項演算の出力値に関する制約を追加する．
void
TemplNode4::add_op_cond(
  SatSolver& solver,
  bool ival0,
  bool ival1,
  bool ival2,
  bool ival3,
  SatLiteral ovar
) const
{
  if ( ival0 ) {
    if ( ival1 ) {
      if ( ival2 ) {
	if ( ival3 ) {
          // ivals = 1111 -> ovar = mTval1111
	  solver.add_clause(~mTval[14],  ovar);
	  solver.add_clause( mTval[14], ~ovar);
	}
	else {
	  // ivals = 1110 -> ovar = mTval1110
	  solver.add_clause(~mTval[13],  ovar);
	  solver.add_clause( mTval[13], ~ovar);
	}
      }
      else {
	if ( ival3 ) {
	  // ivals = 1101 -> ovar = mTval1101
	  solver.add_clause(~mTval[12],  ovar);
	  solver.add_clause( mTval[12], ~ovar);
	}
	else {
	  // ivals = 1100 -> ovar = mTval1100
	  solver.add_clause(~mTval[11],  ovar);
	  solver.add_clause( mTval[11], ~ovar);
	}
      }
    }
    else {
      if ( ival2 ) {
	if ( ival3 ) {
	  // ivals = 1011 -> ovar = mTval1011
	  solver.add_clause(~mTval[10],  ovar);
	  solver.add_clause( mTval[10], ~ovar);
	}
	else {
	  // ivals = 1010 -> ovar = mTval1010
	  solver.add_clause(~mTval[9],  ovar);
	  solver.add_clause( mTval[9], ~ovar);
	}
      }
      else {
	if ( ival3 ) {
	  // ivals = 1001 -> ovar = mTval1001
	  solver.add_clause(~mTval[8],  ovar);
	  solver.add_clause( mTval[8], ~ovar);
	}
	else {
	  // ivals = 1000 -> ovar = mTval1000
	  solver.add_clause(~mTval[7],  ovar);
	  solver.add_clause( mTval[7], ~ovar);
	}
      }
    }
  }
  else {
    if ( ival1 ) {
      if ( ival2 ) {
	if ( ival3 ) {
	  // ivals = 0111 -> ovar = mTval0111
	  solver.add_clause(~mTval[6],  ovar);
	  solver.add_clause( mTval[6], ~ovar);
	}
	else {
	  // ivals = 0110 -> ovar = mTval0110
	  solver.add_clause(~mTval[5],  ovar);
	  solver.add_clause( mTval[5], ~ovar);
	}
      }
      else {
	if ( ival3 ) {
	  // ivals = 0101 -> ovar = mTval0101
	  solver.add_clause(~mTval[4],  ovar);
	  solver.add_clause( mTval[4], ~ovar);
	}
	else {
	  // ivals = 0100 -> ovar = mTval0100
	  solver.add_clause(~mTval[3],  ovar);
	  solver.add_clause( mTval[3], ~ovar);
	}
      }
    }
    else {
      if ( ival2 ) {
	if ( ival3 ) {
	  // ivals = 0011 -> ovar = mTval0011
	  solver.add_clause(~mTval[2],  ovar);
	  solver.add_clause( mTval[2], ~ovar);
	}
	else {
	  // ivals = 0010 -> ovar = mTval0010
	  solver.add_clause(~mTval[1],  ovar);
	  solver.add_clause( mTval[1], ~ovar);
	}
      }
      else {
	if ( ival3 ) {
	  // ivals = 0001 -> ovar = mTval0001
	  solver.add_clause(~mTval[0],  ovar);
	  solver.add_clause( mTval[0], ~ovar);
	}
	else {
	  // ivals = 0000 -> ovar = 0
	  solver.add_clause(           ~ovar);
	}
      }
    }
  }
}

END_NAMESPACE_YM_EXSYN
