
/// @file CiFsmCell.cc
/// @brief CiFsmCell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "CiFsmCell.h"
#include "ci/Serializer.h"
#include "ci/Deserializer.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
// クラス CiCell
//////////////////////////////////////////////////////////////////////

// @brief FSM型の順序セルを生成するクラスメソッド
std::unique_ptr<CiCell>
CiCell::new_FSM(
  const ShString& name,
  ClibArea area
)
{
  auto ptr = new CiFsmCell{name, area};
  return std::unique_ptr<CiCell>{ptr};
}


//////////////////////////////////////////////////////////////////////
// クラス CiFsmCell
//////////////////////////////////////////////////////////////////////

// @brief セルの種類を返す．
ClibCellType
CiFsmCell::type() const
{
  return ClibCellType::FSM;
}

// @brief 順序セル(非FF/非ラッチ)の場合に true を返す．
bool
CiFsmCell::is_fsm() const
{
  return true;
}

// @brief 内容をバイナリダンプする．
void
CiFsmCell::dump(
  Serializer& s
) const
{
  s.dump(static_cast<std::uint8_t>(5));
  dump_common(s);
}

// @brief 内容を復元する．
void
CiFsmCell::_restore(
  Deserializer& s
)
{
  restore_common(s);
}

END_NAMESPACE_YM_CLIB
