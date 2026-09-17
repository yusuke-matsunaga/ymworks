
/// @file EiContAssign.cc
/// @brief EiContAssign の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiContAssign.h"
#include "ym/vl/VlExpr.h"
#include "ym/vl/VlDelay.h"
#include "elaborator/ElbCaHead.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief continuous assignment を生成する．
const VlContAssign*
EiFactory::new_ContAssign(
  ElbCaHead* head,
  const FileRegion& loc,
  const VlExpr* lhs,
  const VlExpr* rhs
)
{
  return  new EiContAssign1(head, loc, lhs, rhs);
}

// @brief net 宣言中の continuous assignment を生成する．
const VlContAssign*
EiFactory::new_ContAssign(
  const VlModule* module,
  const FileRegion& loc,
  const VlExpr* lhs,
  const VlExpr* rhs
)
{
  return new EiContAssign2(module, loc, lhs, rhs);
}


//////////////////////////////////////////////////////////////////////
// クラス EiContAssign
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiContAssign::EiContAssign(
  const FileRegion& loc,
  const VlExpr* lhs,
  const VlExpr* rhs
) : mFileRegion{loc},
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
  return mFileRegion;
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
  const FileRegion& loc,
  const VlExpr* lhs,
  const VlExpr* rhs
) : EiContAssign(loc, lhs, rhs),
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
  const FileRegion& loc,
  const VlExpr* lhs,
  const VlExpr* rhs
) : EiContAssign(loc, lhs, rhs),
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
