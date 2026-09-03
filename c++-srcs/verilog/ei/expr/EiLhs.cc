
/// @file EiLhs.cc
/// @brief EiLhs の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiLhs.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 連結演算子の左辺式を生成する．
ElbExpr*
EiFactory::new_Lhs(
  const AstExpr& ast_expr,
  const std::vector<ElbExpr*>& opr_array,
  const std::vector<ElbExpr*>& lhs_elem_array
)
{
  return new EiLhs(ast_expr, opr_array, lhs_elem_array);
}


//////////////////////////////////////////////////////////////////////
// クラス EiLhs
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiLhs::EiLhs(
  const AstExpr& ast_expr,
  const std::vector<ElbExpr*>& opr_array,
  const std::vector<ElbExpr*>& lhs_elem_array
) : EiConcatOp(ast_expr, opr_array),
    mArray{lhs_elem_array}
{
}

// @brief デストラクタ
EiLhs::~EiLhs()
{
  // mArray は別のオブジェクトが管理している．
}

// @brief 左辺式の要素数の取得
SizeType
EiLhs::lhs_elem_num() const
{
  return mArray.size();
}

// @brief 左辺式の要素の取得
const VlExpr*
EiLhs::lhs_elem(
  SizeType pos
) const
{
  if ( pos >= lhs_elem_num() ) {
    throw std::out_of_range{"pos is out of range"};
  }
  return mArray[pos];
}

// @brief 左辺式の要素のリストの取得
std::vector<const VlExpr*>
EiLhs::lhs_elem_list() const
{
  return std::vector<const VlExpr*>{mArray.begin(), mArray.end()};
}

END_NAMESPACE_YM_VERILOG
