
/// @file CiCellGroup.cc
/// @brief CiCellGroup の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ci/CiCellGroup.h"
#include "ci/CiCell.h"
#include "ci/CiCellClass.h"
#include "ci/Serializer.h"
#include "ci/Deserializer.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
// クラス CiCellGroup
//////////////////////////////////////////////////////////////////////

// @brief 親のライブラリを返す．
const CiCellLibrary*
CiCellGroup::library() const
{
  return mRepClass->library();
}

// @brief セルの種類を返す．
ClibCellType
CiCellGroup::cell_type() const
{
  return mRepClass->cell_type();
}

// @brief 順序セルタイプの場合の属性を返す．
ClibSeqAttr
CiCellGroup::seq_attr() const
{
  return mRepClass->seq_attr();
}

// @brief 内容をシリアライズする．
void
CiCellGroup::serialize(
  Serializer& s
) const
{
  s.reg_obj(this);
}

// @brief 内容をバイナリダンプする．
void
CiCellGroup::dump(
  Serializer& s
) const
{
  s.dump(mIoMap);
  s.dump(mCellList);
}

// @brief 内容を読み込む．
std::unique_ptr<CiCellGroup>
CiCellGroup::restore(
  Deserializer& s
)
{
  auto ptr = std::unique_ptr<CiCellGroup>{new CiCellGroup};
  ptr->_restore(s);
  return ptr;
}

// @brief restore() の本体
void
CiCellGroup::_restore(
  Deserializer& s
)
{
  s.restore(mIoMap);
  std::vector<CiCell*> cell_list;
  s.restore(cell_list);
  for ( auto cell: cell_list ) {
    add_cell(cell);
    cell->set_group(this);
  }
}

END_NAMESPACE_YM_CLIB
