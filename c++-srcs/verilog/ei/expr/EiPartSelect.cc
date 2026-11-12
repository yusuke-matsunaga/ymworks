
/// @file EiPartSelect.cc
/// @brief EiPartSelect の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbMgr.h"
#include "EiPartSelect.h"
#include "ym/vl/AstExpr.h"
#include "ym/vl/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbMgr
//////////////////////////////////////////////////////////////////////

// @brief 固定部分選択式を生成する．
ElbExpr*
ElbMgr::new_PartSelect(
  const AstExpr& ast_expr,
  ElbExpr* parent_expr,
  SizeType select_width,
  SizeType bit_offset
)
{
  return new_expr(
    [&]() -> ElbExpr* {
      return new EiPartSelect(ast_expr, parent_expr,
			      select_width, bit_offset);
    });
}


//////////////////////////////////////////////////////////////////////
// クラス EiPartSelect
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiPartSelect::type() const
{
  return VpiObjType::PartSelect;
}

// @brief 式のタイプを返す．
VlValueType
EiPartSelect::value_type() const
{
  return VlValueType(false, true, mWidth);
}

// @brief 式のビット幅を返す．
SizeType
EiPartSelect::bit_size() const
{
  return mWidth;
}

// @brief 式のタイプを返す．
VlExprType
EiPartSelect::expr_type() const
{
  return VlExprType::SELECT;
}

// @brief ビット/部分選択の時に true を返す．
bool
EiPartSelect::is_select() const
{
  return true;
}

// @brief 宣言要素に対する固定ビット/部分選択のオフセットを返す．
SizeType
EiPartSelect::bit_offset() const
{
  return mOffset;
}

// @brief 親の式を返す．
const VlExpr*
EiPartSelect::parent_expr() const
{
  return mParentExpr;
}

// @brief 要求される式の型を計算してセットする．
void
EiPartSelect::_set_reqsize(
  const VlValueType& type
)
{
  // なにもしない．
}

END_NAMESPACE_YM_VERILOG
