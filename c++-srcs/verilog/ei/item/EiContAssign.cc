
/// @file EiContAssign.cc
/// @brief EiContAssign の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiContAssign.h"
#include "elaborator/ElbExpr.h"

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

// @brief continuous assignment を生成する．
const VlContAssign*
EiFactory::new_ContAssign(
  ElbCaHead* head,
  const AstBase& ast_obj,
  const VlExpr* lhs,
  const VlExpr* rhs
)
{
  return  new EiContAssign1(head, ast_obj, lhs, rhs);
}

// @brief net 宣言中の continuous assignment を生成する．
const VlContAssign*
EiFactory::new_ContAssign(
  const VlModule* module,
  const AstBase& ast_obj,
  const VlExpr* lhs,
  const VlExpr* rhs
)
{
  return new EiContAssign2(module, ast_obj, lhs, rhs);
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


//////////////////////////////////////////////////////////////////////
// クラス EiContAssign
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiContAssign::EiContAssign(
  const AstBase& ast_obj,
  const VlExpr* lhs,
  const VlExpr* rhs
) : mAstObj{ast_obj},
    mLhs{lhs},
    mRhs{rhs}
{
}

// @brief デストラクタ
EiContAssign::~EiContAssign()
{
}

// @brief 型の取得
VpiObjType
EiContAssign::type() const
{
  return VpiObjType::ContAssign;
}

// @brief ファイル位置を返す．
FileRegion
EiContAssign::file_region() const
{
  return mAstObj.file_region();
}

// @brief ビット幅を返す．
int
EiContAssign::bit_size() const
{
  return lhs()->bit_size();
}

// @brief 左辺を返す．
const VlExpr*
EiContAssign::lhs() const
{
  return mLhs;
}

// @brief 右辺を返す．
const VlExpr*
EiContAssign::rhs() const
{
  return mRhs;
}


//////////////////////////////////////////////////////////////////////
// クラス EiContAssign1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiContAssign1::EiContAssign1(
  ElbCaHead* head,
  const AstBase& ast_obj,
  const VlExpr* lhs,
  const VlExpr* rhs
) : EiContAssign(ast_obj, lhs, rhs),
    mHead{head}
{
}

// @brief デストラクタ
EiContAssign1::~EiContAssign1()
{
}

// @brief 親のスコープを返す．
const VlModule*
EiContAssign1::module() const
{
  return mHead->module();
}

// @brief 0の強さを返す．
VpiStrength
EiContAssign1::drive0() const
{
  return mHead->drive0();
}

// @brief 1の強さを返す．
VpiStrength
EiContAssign1::drive1() const
{
  return mHead->drive1();
}

// @brief 遅延を表す式を返す．
const VlDelay*
EiContAssign1::delay() const
{
  return mHead->delay();
}

// @brief ネット宣言中の assignment の時に true を返す．
bool
EiContAssign1::has_net_decl_assign() const
{
  return false;
}


//////////////////////////////////////////////////////////////////////
// クラス EiContAssign2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiContAssign2::EiContAssign2(
  const VlModule* module,
  const AstBase& ast_obj,
  const VlExpr* lhs,
  const VlExpr* rhs
) : EiContAssign(ast_obj, lhs, rhs),
    mModule{module}
{
}

// @brief デストラクタ
EiContAssign2::~EiContAssign2()
{
}

// @brief 親のスコープを返す．
const VlModule*
EiContAssign2::module() const
{
  return mModule;
}

// @brief 0の強さを返す．
VpiStrength
EiContAssign2::drive0() const
{
  return VpiStrength::NoStrength;
}

// @brief 1の強さを返す．
VpiStrength
EiContAssign2::drive1() const
{
  return VpiStrength::NoStrength;
}

// @brief 遅延を表す式を返す．
const VlDelay*
EiContAssign2::delay() const
{
  return nullptr;
}

// @brief ネット宣言中の assignment の時に true を返す．
bool
EiContAssign2::has_net_decl_assign() const
{
  return true;
}

END_NAMESPACE_YM_VERILOG
