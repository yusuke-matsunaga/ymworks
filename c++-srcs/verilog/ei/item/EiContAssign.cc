
/// @file EiContAssign.cc
/// @brief EiContAssign の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbMgr.h"
#include "EiContAssign.h"
#include "ym/vl/VlExpr.h"
#include "ym/vl/VlDelay.h"
#include "elaborator/ElbCaHead.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbMgr
//////////////////////////////////////////////////////////////////////

// @brief continuous assignment を生成する．
const VlContAssign*
ElbMgr::new_ContAssign(
  ElbCaHead* head,
  const FileRegion& loc,
  const VlExpr* lhs,
  const VlExpr* rhs
)
{
  return new_contassign(
    [&]() -> const VlContAssign* {
      return new EiContAssign(head, loc, lhs, rhs);
    });
}

// @brief net 宣言中の continuous assignment を生成する．
const VlContAssign*
ElbMgr::new_ContAssign(
  const VlModule* module,
  const FileRegion& loc,
  const VlExpr* lhs,
  const VlExpr* rhs
)
{
  return new_contassign(
    [&]() -> const VlContAssign* {
      return new EiCa_Net(module, loc, lhs, rhs);
    });
}


//////////////////////////////////////////////////////////////////////
// クラス EiCaBase
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiCaBase::type() const
{
  return VpiObjType::ContAssign;
}

// @brief ファイル位置を返す．
FileRegion
EiCaBase::file_region() const
{
  return mFileRegion;
}

// @brief 左辺式を返す．
const VlExpr*
EiCaBase::lhs() const
{
  return mLhs;
}

// @brief 右辺を返す．
const VlExpr*
EiCaBase::rhs() const
{
  return mRhs;
}


//////////////////////////////////////////////////////////////////////
// クラス EiContAssign
//////////////////////////////////////////////////////////////////////

// @brief 親のスコープを返す．
const VlModule*
EiContAssign::module() const
{
  return mHead->module();
}

// @brief 0の強さを返す．
VpiStrength
EiContAssign::drive0() const
{
  return mHead->drive0();
}

// @brief 1の強さを返す．
VpiStrength
EiContAssign::drive1() const
{
  return mHead->drive1();
}

// @brief 遅延を表す式を返す．
const VlDelay*
EiContAssign::delay() const
{
  return mHead->delay();
}

// @brief ネット宣言中の assignment の時に true を返す．
bool
EiContAssign::has_net_decl_assign() const
{
  return false;
}


//////////////////////////////////////////////////////////////////////
// クラス EiCa_Net
//////////////////////////////////////////////////////////////////////

// @brief 親のスコープを返す．
const VlModule*
EiCa_Net::module() const
{
  return mModule;
}

// @brief 0の強さを返す．
VpiStrength
EiCa_Net::drive0() const
{
  return VpiStrength::NoStrength;
}

// @brief 1の強さを返す．
VpiStrength
EiCa_Net::drive1() const
{
  return VpiStrength::NoStrength;
}

// @brief 遅延を表す式を返す．
const VlDelay*
EiCa_Net::delay() const
{
  return nullptr;
}

// @brief ネット宣言中の assignment の時に true を返す．
bool
EiCa_Net::has_net_decl_assign() const
{
  return true;
}

END_NAMESPACE_YM_VERILOG
