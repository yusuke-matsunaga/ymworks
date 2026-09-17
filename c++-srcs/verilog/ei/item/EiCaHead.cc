
/// @file EiCaHead.cc
/// @brief EiCaHead の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiCaHead.h"
#include "ym/vl/VlDelay.h"
#include "ym/vl/AstItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief continuous assignment のヘッダを生成する．
ElbCaHead*
EiFactory::new_CaHead(
  const VlModule* module,
  const AstItem& ast_head,
  const VlDelay* delay
)
{
  if ( delay ) {
    return new EiCaHeadD(module, ast_head, delay);
  }
  else {
    return new EiCaHead(module, ast_head);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス EiCaHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiCaHead::EiCaHead(
  const VlModule* module,
  const AstItem& ast_head
) : mModule{module},
    mAstHead{ast_head}
{
}

// @brief デストラクタ
EiCaHead::~EiCaHead()
{
}

// @brief 親のスコープを返す．
const VlModule*
EiCaHead::module() const
{
  return mModule;
}

// @brief 0の強さを返す．
VpiStrength
EiCaHead::drive0() const
{
  if ( mAstHead.strength().is_invalid() ) {
    return VpiStrength::NoStrength;
  }
  return mAstHead.strength().drive0();
}

// @brief 1の強さを返す．
VpiStrength
EiCaHead::drive1() const
{
  if ( mAstHead.strength().is_invalid() ) {
    return VpiStrength::NoStrength;
  }
  return mAstHead.strength().drive0();
}

// @brief 遅延を表す式を返す．
// @note このクラスでは nullptr を返す．
const VlDelay*
EiCaHead::delay() const
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiCaHeadD
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiCaHeadD::EiCaHeadD(
  const VlModule* module,
  const AstItem& ast_head,
  const VlDelay* delay
) : EiCaHead(module, ast_head),
    mDelay{delay}
{
}

// @brief デストラクタ
EiCaHeadD::~EiCaHeadD()
{
}

// @brief 遅延を表す式を返す．
const VlDelay*
EiCaHeadD::delay() const
{
  return mDelay;
}

END_NAMESPACE_YM_VERILOG
