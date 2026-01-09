
/// @file BddIsop.cc
/// @brief BddIsop の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "BddIsop.h"
#include "ym/Bdd.h"
#include "ym/BddVar.h"
#include "BddMgrImpl.h"


BEGIN_NAMESPACE_YM_DD

// @brief 非冗長積和形
SopCover
isop(
  const Bdd& on,
  const Bdd& dc
)
{
  Bdd::_check_valid(on, dc);
  BddIsop op(on.mgr());
  auto cube_list = op.isop_step(on, dc);
  auto support_list = on.get_support_list();
  SizeType max_var = 0;
  for ( auto var: support_list ) {
    max_var = std::max(max_var, var.varid());
  }
  auto ni = max_var + 1;
  return SopCover(ni, cube_list);
}


//////////////////////////////////////////////////////////////////////
// クラス BddIsop
//////////////////////////////////////////////////////////////////////

std::vector<std::vector<Literal>>
BddIsop::isop_step(
  const Bdd& on,
  const Bdd& dc
)
{
  // trivial case のチェック

  // on が dc に完全に含まれている．
  if ( check_containment(dc, on) ) {
    // 0 を返す．
    return {};
  }

  // off(~on) が dc に完全に含まれている．
  if ( check_containment(dc, ~on) ) {
    // 1 を返す．
    return {{}};
  }

  // ここまできたら on は終端ではない．
  // dc も 1 ではない．

  // 演算結果テーブルを調べる．
  IsopKey key{on, dc};
  if ( mTable.count(key) > 0 ) {
    return mTable.at(key);
  }

  // 見つからなかったので実際に apply 演算を行う．
  // 先頭のインデックスで分解する．
  SizeType top;
  Bdd on0, on1;
  Bdd dc0, dc1;
  if ( dc.is_zero() ) {
    auto on_var = on.root_decomp(on0, on1);
    top = on_var.varid();
    dc0 = dc;
    dc1 = dc;
  }
  else {
    auto on_var = on.root_var();
    auto dc_var = dc.root_var();
    if ( on_var < dc_var ) {
      on0 = on.root_cofactor0();
      on1 = on.root_cofactor1();
      dc0 = dc;
      dc1 = dc;
      top = on_var.varid();
    }
    else if ( on_var > dc_var ) {
      on0 = on;
      on1 = on;
      dc0 = dc.root_cofactor0();
      dc1 = dc.root_cofactor1();
      top = dc_var.varid();
    }
    else { // on_var == dc_var
      on0 = on.root_cofactor0();
      on1 = on.root_cofactor1();
      dc0 = dc.root_cofactor0();
      dc1 = dc.root_cofactor1();
      top = on_var.varid();
    }
  }

  CubeList cube_list;
  if ( dc0.is_one() ) {
    cube_list = isop_step(on1, dc1);
  }
  else if ( dc1.is_one() ) {
    cube_list = isop_step(on0, dc0);
  }
  else {
    auto on2 = (on0 | dc0) & (on1 | dc1);
    auto cube0_list = isop_step(on0, dc0 | on2);
    auto cube1_list = isop_step(on1, dc1 | on2);
    auto on0_new = cube2bdd(cube0_list);
    auto on1_new = cube2bdd(cube1_list);
    auto dc2 = (dc0 | on0_new) & (dc1 | on1_new);
    cube_list = isop_step(on2, dc2);
    merge_list(top, cube_list, cube0_list, cube1_list);
  }
  mTable.emplace(key, cube_list);
  return cube_list;
}

Bdd
BddIsop::cube2bdd(
  const CubeList& cube_list
)
{
  auto bdd = Bdd::zero();
  for ( auto& cube: cube_list ) {
    auto bdd1 = Bdd::one();
    for ( auto lit: cube ) {
      auto varid = lit.varid();
      auto var_bdd = mMgr.variable_bdd(varid);
      if ( lit.is_negative() ) {
	var_bdd = ~var_bdd;
      }
      bdd1 &= var_bdd;
    }
    bdd |= bdd1;
  }
  return bdd;
}

// マージする．
void
BddIsop::merge_list(
  SizeType top,
  CubeList& cube_list,
  const CubeList& cube0_list,
  const CubeList& cube1_list
)
{
  auto size = cube_list.size() + cube0_list.size() + cube1_list.size();
  cube_list.reserve(size);
  auto lit1 = Literal(top, false);
  auto lit0 = ~lit1;
  for ( auto cube: cube0_list ) {
    cube.push_back(lit0);
    cube_list.push_back(cube);
  }
  for ( auto cube: cube1_list ) {
    cube.push_back(lit1);
    cube_list.push_back(cube);
  }
}

// @brief キューブのリストを出力する(デバッグ用)
void
BddIsop::print_cover(
  std::ostream& s,
  const CubeList& cube_list
)
{
  const char* or_sep = "";
  for ( auto& cube: cube_list ) {
    s << or_sep;
    or_sep = " + ";
    const char* and_sep = "";
    for ( auto lit: cube ) {
      s << and_sep << lit;
      and_sep = " ";
    }
  }
}

END_NAMESPACE_YM_DD
