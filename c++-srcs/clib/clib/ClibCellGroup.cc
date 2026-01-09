
/// @file ClibCellGroup.cc
/// @brief ClibCellGroup の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ClibCellGroup.h"
#include "ym/ClibCell.h"
#include "ym/ClibCellClass.h"
#include "ci/CiCellGroup.h"
#include "ci/CiCellClass.h"
#include "ci/CiCellLibrary.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
// クラス ClibCellGroup
//////////////////////////////////////////////////////////////////////

// @brief 内容を指定したコンストラクタ
ClibCellGroup::ClibCellGroup(
  const CiCellGroup* impl
) : mImpl{impl}
{
  if ( mImpl != nullptr ) {
    mImpl->library()->inc_ref();
  }
}

// @brief デストラクタ
ClibCellGroup::~ClibCellGroup()
{
  if ( mImpl != nullptr ) {
    mImpl->library()->dec_ref();
  }
}

// @brief 代表クラスを返す．
ClibCellClass
ClibCellGroup::rep_class() const
{
  _check_valid();
  auto rep = mImpl->rep_class();
  return ClibCellClass{rep};
}

// @brief 代表クラスに対する変換マップを返す．
const ClibIOMap&
ClibCellGroup::iomap() const
{
  _check_valid();
  return mImpl->iomap();
}

// @brief セルの種類を返す．
ClibCellType
ClibCellGroup::cell_type() const
{
  _check_valid();
  return mImpl->cell_type();
}

// @brief 順序セルタイプの場合の属性を返す．
ClibSeqAttr
ClibCellGroup::seq_attr() const
{
  _check_valid();
  return mImpl->seq_attr();
}

// @brief セル数を返す．
SizeType
ClibCellGroup::cell_num() const
{
  _check_valid();
  return mImpl->cell_num();
}

// @brief セルを返す．
ClibCell
ClibCellGroup::cell(
  SizeType pos
) const
{
  _check_valid();
  auto cell = mImpl->cell(pos);
  return ClibCell{cell};
}

// @brief セルのリストを返す．
ClibCellList
ClibCellGroup::cell_list() const
{
  _check_valid();
  auto& cell_list = mImpl->cell_list();
  return ClibCellList{cell_list};
}

END_NAMESPACE_YM_CLIB
