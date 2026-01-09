
/// @file CiBus.cc
/// @brief CiBus の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ci/CiBus.h"
#include "ci/Serializer.h"
#include "ci/Deserializer.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
// クラス CiBus
//////////////////////////////////////////////////////////////////////

// @brief 内容をシリアライズする．
void
CiBus::serialize(
  Serializer& s
) const
{
  s.reg_obj(this);
}

// @brief 内容をバイナリダンプする．
void
CiBus::dump(
  Serializer& s
) const
{
  s.dump(name());
  s.dump(bus_type());
  s.dump(pin_list());
}

std::unique_ptr<CiBus>
CiBus::restore(
  Deserializer& s
)
{
  auto ptr = std::unique_ptr<CiBus>{new CiBus};
  ptr->_restore(s);
  return ptr;
}

// @brief restore() の本体
void
CiBus::_restore(
  Deserializer& s
)
{
  s.restore(mName);
  s.restore(mBusType);
  s.restore(mPinList);
}

END_NAMESPACE_YM_CLIB
