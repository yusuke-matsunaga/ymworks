
/// @file EiNetSrc.cc
/// @brief EiNetSrc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbMgr.h"
#include "EiNetSrc1.h"
#include "EiNetSrc2.h"
#include "EiNetSrc3.h"
#include "EiNetSrc4.h"
#include "ym/vl/VlExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbMgr
//////////////////////////////////////////////////////////////////////

// @breif VlNetSrc を生成する．
const VlNetSrc*
ElbMgr::new_NetSrc(
  const VlExpr* rhs
)
{
  return new EiNetSrc1(rhs);
}

// @breif VlNetSrc を生成する．
const VlNetSrc*
ElbMgr::new_NetSrc(
  SizeType bit_offset,
  const VlExpr* rhs
)
{
  return new EiNetSrc2(bit_offset, rhs);
}

// @breif VlNetSrc を生成する．
const VlNetSrc*
ElbMgr::new_NetSrc(
  const VlExpr* rhs,
  SizeType rhs_width,
  SizeType rhs_offset
)
{
  return new EiNetSrc3(rhs, rhs_width, rhs_offset);
}

// @breif VlNetSrc を生成する．
const VlNetSrc*
ElbMgr::new_NetSrc(
  SizeType bit_offset,
  const VlExpr* rhs,
  SizeType rhs_width,
  SizeType rhs_offset
)
{
  return new EiNetSrc4(bit_offset, rhs, rhs_width, rhs_offset);
}


//////////////////////////////////////////////////////////////////////
// クラス EiNetSrc1
//////////////////////////////////////////////////////////////////////

// @brief ビット/部分選択のタイプを返す．
VlSelectType
EiNetSrc1::select_type() const
{
  return VlSelectType::NONE;
}

// @brief ビット/部分選択のビット位置のオフセットを返す．
SizeType
EiNetSrc1::bit_offset() const
{
  return 0;
}

// @brief ソースの式を返す．
const VlExpr*
EiNetSrc1::rhs_expr() const
{
  return mRhs;
}

// @brief ソースを分割している場合のビット幅を返す．
SizeType
EiNetSrc1::rhs_width() const
{
  return mRhs->bit_size();
}

// @brief ソースを分割している場合のビット位置オフセットを返す．
SizeType
EiNetSrc1::rhs_offset() const
{
  return 0;
}


//////////////////////////////////////////////////////////////////////
// クラス EiNetSrc2
//////////////////////////////////////////////////////////////////////

// @brief ビット/部分選択のタイプを返す．
VlSelectType
EiNetSrc2::select_type() const
{
  return VlSelectType::CONST;
}

// @brief ビット/部分選択のビット位置のオフセットを返す．
SizeType
EiNetSrc2::bit_offset() const
{
  return mBitOffset;
}


//////////////////////////////////////////////////////////////////////
// クラス EiNetSrc3
//////////////////////////////////////////////////////////////////////

// @brief ソースを分割している場合のビット幅を返す．
SizeType
EiNetSrc3::rhs_width() const
{
  return mRhsWidth;
}

// @brief ソースを分割している場合のビット位置オフセットを返す．
SizeType
EiNetSrc3::rhs_offset() const
{
  return mRhsOffset;
}


//////////////////////////////////////////////////////////////////////
// クラス EiNetSrc4
//////////////////////////////////////////////////////////////////////

// @brief ソースを分割している場合のビット幅を返す．
SizeType
EiNetSrc4::rhs_width() const
{
  return mRhsWidth;
}

// @brief ソースを分割している場合のビット位置オフセットを返す．
SizeType
EiNetSrc4::rhs_offset() const
{
  return mRhsOffset;
}

END_NAMESPACE_YM_VERILOG
