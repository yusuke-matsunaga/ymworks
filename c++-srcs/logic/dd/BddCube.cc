
/// @file BddCube.cc
/// @brief BddCube の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BddCube.h"
#include "ym/BddLit.h"
#include "ym/BddMgr.h"
#include "DdEdge.h"
#include "DdNode.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
// クラス BddCube
//////////////////////////////////////////////////////////////////////

// @brief 内容を指定したコンストラクタ
BddCube::BddCube(
  const BddMgrHolder& holder,
  DdEdge root
) : BddBase(holder, root)
{
  if ( !is_invalid() && !is_cube() ) {
    throw std::invalid_argument{"not a cube"};
  }
}

// @brief 要素のリストを指定したコンストラクタ
BddCube::BddCube(
  const std::vector<BddLit>& lit_list
) : BddBase(rep(lit_list), DdEdge::one())
{
  for ( auto lit: lit_list ) {
    *this = _cube_and(*this, lit);
  }
}

// @brief BddLit からの変換コンストラクタ
BddCube::BddCube(
  const BddLit& lit
) : BddBase(lit)
{
  // 単一のリテラルもキューブとなる．
}

// @brief 空のキューブを返す．
BddCube
BddCube::universal_cube()
{
  return BddCube(BddMgrHolder{}, DdEdge::one());
}

// @brief 不正値を返す．
BddCube
BddCube::invalid()
{
  return BddCube(BddMgrHolder{}, DdEdge::invalid());
}

// @brief 空のキューブの時に true を返す．
bool
BddCube::is_universal_cube() const
{
  return root().is_one();
}

// @brief リテラル数を返す．
SizeType
BddCube::size() const
{
  auto dummy = lit_list();
  return dummy.size();
}

// @brief リテラルのリストに変換する．
std::vector<BddLit>
BddCube::lit_list() const
{
  _check_valid();
  auto edge = root();
  std::vector<BddLit> lit_list;
  while ( !edge.is_const() ) {
    auto inv = edge.inv();
    auto node = edge.node();
    auto edge0 = node->edge0() * inv;
    auto edge1 = node->edge1() * inv;
    auto var = _var(node->level());
    if ( edge0.is_zero() ) {
      edge = edge1;
      auto lit = var.positive_literal();
      lit_list.push_back(lit);
    }
    else {
      edge = edge0;
      auto lit = var.negative_literal();
      lit_list.push_back(lit);
    }
  }
  return lit_list;
}

/// @brief ストリーム出力演算子
std::ostream&
operator<<(
  std::ostream& s,
  const BddCube& cube
)
{
  const char* sep = "";
  for ( auto lit: cube.lit_list() ) {
    s << sep << lit;
    sep = " ";
  }
  return s;
}

END_NAMESPACE_YM_DD
