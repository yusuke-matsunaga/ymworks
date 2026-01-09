
/// @file CiCellClass.cc
/// @brief CiCellClass の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ci/CiCellClass.h"
#include "ci/CiCellGroup.h"
#include "ci/Serializer.h"
#include "ci/Deserializer.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
// クラス CiCellClass
//////////////////////////////////////////////////////////////////////

// @brief 内容をシリアライズする．
void
CiCellClass::serialize(
  Serializer& s
) const
{
  s.reg_obj(this);
}

// @brief 内容をバイナリダンプする．
void
CiCellClass::dump(
  Serializer& s
) const
{
  // 同位体変換情報のダンプ
  s.dump(idmap_list());
  // セルグループのダンプ
  s.dump(cell_group_list());
}

// @brief 内容を読み込む．
std::unique_ptr<CiCellClass>
CiCellClass::restore(
  Deserializer& s
)
{
  auto ptr = std::unique_ptr<CiCellClass>{new CiCellClass};
  ptr->_restore(s);
  return ptr;
}

// @brief restore() の本体
void
CiCellClass::_restore(
  Deserializer& s
)
{
  s.restore(mIdMapList);
  std::vector<CiCellGroup*> group_list;
  s.restore(group_list);
  for ( auto group: group_list ) {
    add_group(group);
    group->set_rep_class(this);
  }
}

END_NAMESPACE_YM_CLIB
