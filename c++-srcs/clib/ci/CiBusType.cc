
/// @file CiBusType.cc
/// @brief CiBusType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ci/CiBusType.h"
#include "ci/CiPin.h"
#include "ci/Serializer.h"
#include "ci/Deserializer.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
// クラス CiBusType
//////////////////////////////////////////////////////////////////////

// @brief 内容をシリアライズする．
void
CiBusType::serialize(
  Serializer& s
) const
{
  s.reg_obj(this);
}

// @brief 内容をバイナリダンプする．
void
CiBusType::dump(
  Serializer& s
) const
{
  s.dump(name());
  s.dump(bit_from());
  s.dump(bit_to());
}

// @brief バスタイプを読み込む．
std::unique_ptr<CiBusType>
CiBusType::restore(
  Deserializer& s
)
{
  auto ptr = std::unique_ptr<CiBusType>{new CiBusType};
  ptr->_restore(s);
  return ptr;
}

// @brief restore() の本体
void
CiBusType::_restore(
  Deserializer& s
)
{
  s.restore(mName);
  s.restore(mBitFrom);
  s.restore(mBitTo);
  set_bit_width();
}

END_NAMESPACE_YM_CLIB
