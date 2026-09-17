
/// @file ElbDeclHead.cc
/// @brief ElbDeclHead の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbDeclHead.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbDeclHead
//////////////////////////////////////////////////////////////////////

// @brief データ型の取得
VpiVarType
ElbDeclHead::data_type() const
{
  return VpiVarType::None;
}

// @brief net 型の取得
VpiNetType
ElbDeclHead::net_type() const
{
  return VpiNetType::None;
}

// @brief vectored|scalared 属性の取得
VpiVsType
ElbDeclHead::vs_type() const
{
  return VpiVsType::None;
}

// @brief drive0 strength の取得
VpiStrength
ElbDeclHead::drive0() const
{
  return VpiStrength::NoStrength;
}

// @brief drive1 strength の取得
VpiStrength
ElbDeclHead::drive1() const
{
  return VpiStrength::NoStrength;
}

// @brief charge strength の取得
VpiStrength
ElbDeclHead::charge() const
{
  return VpiStrength::NoStrength;
}

// @brief delay の取得
const VlDelay*
ElbDeclHead::delay() const
{
  return nullptr;
}

END_NAMESPACE_YM_VERILOG
