
/// @file AigMgr_sop.cc
/// @brief AigMgr::from_cover, from_cube の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AigMgr.h"
#include "ym/SopCover.h"
#include "AigMgrImpl.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス AigMgr
//////////////////////////////////////////////////////////////////////

// @brief SopCover から変換する．
AigLiteral
AigMgr::from_cover(
  const SopCover& cover
)
{
  auto nmax = cover.variable_num();
  auto input_list = _input_list(nmax);
  return from_cover(cover, input_list);
}

// @brief SopCover から変換する．
AigLiteral
AigMgr::from_cover(
  const SopCover& cover,
  const std::vector<AigLiteral>& input_list
)
{
  auto nc = cover.cube_num();
  if ( nc == 0 ) {
    // 空のカバーは 0
    return AigLiteral::zero();
  }
  std::vector<AigLiteral> aig_list(nc);
  for ( SizeType i = 0; i < nc; ++ i ) {
    auto cube = cover.get_cube(i);
    auto aig = from_cube(cube, input_list);
    aig_list[i] = aig;
  }
  return new_or(aig_list);
}

// @brief SopCube から変換する．
AigLiteral
AigMgr::from_cube(
  const SopCube& cube
)
{
  auto nmax = cube.variable_num();
  auto input_list = _input_list(nmax);
  return from_cube(cube, input_list);
}

// @brief SopCube から変換する．
AigLiteral
AigMgr::from_cube(
  const SopCube& cube,
  const std::vector<AigLiteral>& input_list
)
{
  auto lit_list = cube.literal_list();
  if ( lit_list.empty() ) {
    // 空のキューブは 1
    return AigLiteral::one();
  }
  auto n = lit_list.size();
  std::vector<AigLiteral> aig_list;
  aig_list.reserve(lit_list.size());
  for ( auto lit: lit_list ) {
    auto aig = input_list[lit.varid()];
    aig *= lit.is_negative();
    aig_list.push_back(aig);
  }
  return new_and(aig_list);
}

END_NAMESPACE_YM_AIG
