
/// @file EiPartSelect.cc
/// @brief EiPartSelect の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiPartSelect.h"
#include "ym/vl/AstExpr.h"
#include "ym/vl/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 固定部分選択式を生成する．
ElbExpr*
EiFactory::new_PartSelect(
  const AstExpr& ast_expr,
  ElbExpr* parent_expr,
  const AstExpr& index1,
  const AstExpr& index2,
  int index1_val,
  int index2_val
)
{
  return new EiConstPartSelect(ast_expr, parent_expr,
			       index1, index2,
			       index1_val, index2_val);
}

// @brief 固定部分選択式を生成する．
ElbExpr*
EiFactory::new_PartSelect(
  const AstExpr& ast_expr,
  ElbExpr* parent_expr,
  int index1,
  int index2
)
{
  return new EiConstPartSelect(ast_expr, parent_expr,
			       AstExpr{}, AstExpr{},
			       index1, index2);
}

// @brief 可変部分選択式を生成する．
ElbExpr*
EiFactory::new_PlusPartSelect(
  const AstExpr& ast_expr,
  ElbExpr* parent_expr,
  ElbExpr* base,
  const AstExpr& range,
  SizeType range_val
)
{
  return new EiPlusPartSelect(ast_expr, parent_expr,
			      base, range, range_val);
}

// @brief 可変部分選択式を生成する．
ElbExpr*
EiFactory::new_MinusPartSelect(
  const AstExpr& ast_expr,
  ElbExpr* parent_expr,
  ElbExpr* base,
  const AstExpr& range,
  SizeType range_val
)
{
  return new EiMinusPartSelect(ast_expr, parent_expr,
			       base, range, range_val);
}


//////////////////////////////////////////////////////////////////////
// クラス EiPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiPartSelect::EiPartSelect(
  const AstExpr& ast_expr,
  ElbExpr* parent_expr
) : EiExprBase(ast_expr),
    mParentExpr{parent_expr}
{
}

// @brief デストラクタ
EiPartSelect::~EiPartSelect()
{
}

// @brief 型の取得
VpiObjType
EiPartSelect::type() const
{
  return VpiObjType::PartSelect;
}

// @brief 定数の時 true を返す．
bool
EiPartSelect::is_const() const
{
  return parent_expr()->is_const() && is_constant_select();
}

// @brief 範囲指定の時に true を返す．
bool
EiPartSelect::is_partselect() const
{
  return true;
}

// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
const VlDeclBase*
EiPartSelect::decl_base() const
{
  return parent_expr()->decl_base();
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
const VlDecl*
EiPartSelect::decl_obj() const
{
  return parent_expr()->decl_obj();
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
const VlDeclArray*
EiPartSelect::declarray_obj() const
{
  return parent_expr()->declarray_obj();
}

// @brief 配列型宣言要素への参照の場合，配列の次元を返す．
SizeType
EiPartSelect::declarray_dimension() const
{
  return parent_expr()->declarray_dimension();
}

// @brief 配列型宣言要素への参照の場合，配列のインデックスを返す．
const VlExpr*
EiPartSelect::declarray_index(
  SizeType pos
) const
{
  return parent_expr()->declarray_index(pos);
}

// @brief 親の式を返す．
const VlExpr*
EiPartSelect::parent_expr() const
{
  return mParentExpr;
}

// @brief 左辺式の要素数の取得
SizeType
EiPartSelect::lhs_elem_num() const
{
  return 1;
}

// @brief 左辺式の要素の取得
const VlExpr*
EiPartSelect::lhs_elem(
  SizeType pos
) const
{
  if ( pos != 0 ) {
    throw std::logic_error{"pos != 0"};
  }
  return this;
}

// @brief 左辺式の要素のリストの取得
std::vector<const VlExpr*>
EiPartSelect::lhs_elem_list() const
{
  return {this};
}

// @brief 要求される式の型を計算してセットする．
void
EiPartSelect::_set_reqsize(
  const VlValueType& type
)
{
  // なにもしない．
}


//////////////////////////////////////////////////////////////////////
// クラス EiConstPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiConstPartSelect::EiConstPartSelect(
  const AstExpr& ast_expr,
  ElbExpr* parent_expr,
  const AstExpr& index1,
  const AstExpr& index2,
  int index1_val,
  int index2_val
) : EiPartSelect(ast_expr, parent_expr),
    mLeftRange{index1},
    mRightRange{index2},
    mLeftVal{index1_val},
    mRightVal{index2_val}
{
}

// @brief デストラクタ
EiConstPartSelect::~EiConstPartSelect()
{
}

// @brief 式のタイプを返す．
VlValueType
EiConstPartSelect::value_type() const
{
  int w = 0;
  if ( mLeftVal > mRightVal ) {
    w = mLeftVal - mRightVal + 1;
  }
  else {
    w = mRightVal - mLeftVal + 1;
  }
  return VlValueType(false, true, w);
}

// @brief 固定選択子の時 true を返す．
bool
EiConstPartSelect::is_constant_select() const
{
  return true;
}

// @brief 範囲指定のモードを返す．
VpiRangeMode
EiConstPartSelect::range_mode() const
{
  return VpiRangeMode::Const;
}

// @brief 範囲の MSB の式を返す．
const VlExpr*
EiConstPartSelect::left_range() const
{
  return nullptr;
}

// @brief 範囲の LSB の式を返す．
const VlExpr*
EiConstPartSelect::right_range() const
{
  return nullptr;
}

// @brief 範囲の MSB の値を返す．
int
EiConstPartSelect::left_range_val() const
{
  return mLeftVal;
}

// @brief 範囲の LSB の値を返す．
int
EiConstPartSelect::right_range_val() const
{
  return mRightVal;
}


//////////////////////////////////////////////////////////////////////
// クラス EiVarPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiVarPartSelect::EiVarPartSelect(
  const AstExpr& ast_expr,
  ElbExpr* parent_expr,
  ElbExpr* base,
  const AstExpr& range,
  SizeType range_width
) : EiPartSelect(ast_expr, parent_expr),
    mBaseExpr{base},
    mRangeExpr{range},
    mRangeWidth{range_width}
{
}

// @brief デストラクタ
EiVarPartSelect::~EiVarPartSelect()
{
}

// @brief 式のタイプを返す．
VlValueType
EiVarPartSelect::value_type() const
{
  return VlValueType(false, true, mRangeWidth);
}

// @brief 固定選択子の時 true を返す．
bool
EiVarPartSelect::is_constant_select() const
{
  return mBaseExpr->is_const();
}

// @brief 範囲のベースを表す式を返す．
const VlExpr*
EiVarPartSelect::base() const
{
  return mBaseExpr;
}

// @brief 範囲のビット幅を返す．
SizeType
EiVarPartSelect::range_width() const
{
  return mRangeWidth;
}


//////////////////////////////////////////////////////////////////////
// クラス EiPlusPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiPlusPartSelect::EiPlusPartSelect(
  const AstExpr& ast_expr,
  ElbExpr* parent_expr,
  ElbExpr* base,
  const AstExpr& range,
  SizeType range_val
) : EiVarPartSelect(ast_expr, parent_expr, base, range, range_val)
{
}

// @brief デストラクタ
EiPlusPartSelect::~EiPlusPartSelect()
{
}

// @brief 範囲指定のモードを返す．
VpiRangeMode
EiPlusPartSelect::range_mode() const
{
  return VpiRangeMode::Plus;
}


//////////////////////////////////////////////////////////////////////
// クラス EiMinusPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiMinusPartSelect::EiMinusPartSelect(
  const AstExpr& ast_expr,
  ElbExpr* parent_expr,
  ElbExpr* base,
  const AstExpr& range,
  SizeType range_val
) : EiVarPartSelect(ast_expr, parent_expr, base, range, range_val)
{
}

// @brief デストラクタ
EiMinusPartSelect::~EiMinusPartSelect()
{
}

// @brief 範囲指定のモードを返す．
VpiRangeMode
EiMinusPartSelect::range_mode() const
{
  return VpiRangeMode::Minus;
}

END_NAMESPACE_YM_VERILOG
