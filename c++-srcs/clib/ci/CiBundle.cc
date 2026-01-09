
/// @file CiBundle.cc
/// @brief CiBundle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ci/CiBundle.h"
#include "ci/Serializer.h"
#include "ci/Deserializer.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
// クラス CiBundle
//////////////////////////////////////////////////////////////////////

// @brief 内容をシリアライズする．
void
CiBundle::serialize(
  Serializer& s
) const
{
  s.reg_obj(this);
}

// @brief 内容をバイナリダンプする．
void
CiBundle::dump(
  Serializer& s
) const
{
  s.dump(_name());
  s.dump(pin_list());
}

std::unique_ptr<CiBundle>
CiBundle::restore(
  Deserializer& s
)
{
  auto ptr = std::unique_ptr<CiBundle>{new CiBundle};
  ptr->_restore(s);
  return ptr;
}

// @brief restore() の本体
void
CiBundle::_restore(
  Deserializer& s
)
{
  s.restore(mName);
  s.restore(mPinList);
}

END_NAMESPACE_YM_CLIB
