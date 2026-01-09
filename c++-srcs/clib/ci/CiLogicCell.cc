
/// @file CiLogicCell.cc
/// @brief CiLogicCell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "CiLogicCell.h"
#include "ci/Serializer.h"
#include "ci/Deserializer.h"


BEGIN_NAMESPACE_YM_CLIB

// @brief 論理セルを生成するクラスメソッド
std::unique_ptr<CiCell>
CiCell::new_Logic(
  const ShString& name,
  ClibArea area
)
{
  auto ptr = new CiLogicCell{name, area};
  return std::unique_ptr<CiCell>{ptr};
}

//////////////////////////////////////////////////////////////////////
// クラス CiLogicCell
//////////////////////////////////////////////////////////////////////

// @brief 内容をバイナリダンプする．
void
CiLogicCell::dump(
  Serializer& s
) const
{
  // シグネチャ
  s.dump(static_cast<std::uint8_t>(0));
  dump_common(s);
}

// @brief 内容を復元する．
void
CiLogicCell::_restore(
  Deserializer& s
)
{
  restore_common(s);
}

END_NAMESPACE_YM_CLIB
