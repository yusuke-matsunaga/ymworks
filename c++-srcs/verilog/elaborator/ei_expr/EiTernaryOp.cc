
/// @file EiTernaryOp.cc
/// @brief EiTernaryOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiTernaryOp.h"
#include "ym/vl/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 3項演算子を生成する．
ElbExpr*
EiFactory::new_TernaryOp(
  const AstExpr* ast_expr,
  VpiOpType op_type,
  ElbExpr* opr0,
  ElbExpr* opr1,
  ElbExpr* opr2
)
{
  switch ( op_type ) {
  case VpiOpType::Condition:
    return new EiConditionOp(ast_expr, opr0, opr1, opr2);

  case VpiOpType::MinTypMax:
    return new EiMinTypMaxOp(ast_expr, opr0, opr1, opr2);

  default:
    break;
  }
  throw std::logic_error{"Should not be reached"};
}


//////////////////////////////////////////////////////////////////////
// クラス EiTernaryOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiTernaryOp::EiTernaryOp(
  const AstExpr* ast_expr,
  ElbExpr* opr1,
  ElbExpr* opr2,
  ElbExpr* opr3
) : EiOperation(ast_expr),
    mOpr{opr1, opr2, opr3}
{
}

// @brief デストラクタ
EiTernaryOp::~EiTernaryOp()
{
}

// @brief 定数の時 true を返す．
bool
EiTernaryOp::is_const() const
{
  return mOpr[0]->is_const() &&
    mOpr[1]->is_const() &&
    mOpr[2]->is_const();

}

// @brief オペランド数を返す．
SizeType
EiTernaryOp::operand_num() const
{
  return 3;
}

// @brief オペランドを返す．
const VlExpr*
EiTernaryOp::operand(
  SizeType pos
) const
{
  if ( pos >= 3 ) {
    throw std::out_of_range{"pos is out of range"};
  }
  return mOpr[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス EiConditionOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiConditionOp::EiConditionOp(
  const AstExpr* ast_expr,
  ElbExpr* opr1,
  ElbExpr* opr2,
  ElbExpr* opr3
) : EiTernaryOp(ast_expr, opr1, opr2, opr3)
{
  // 三項演算子の場合は第1オペランドが self determined で
  // 結果は第2オペランドと第3オペランドから決まる．

  opr1->set_selfsize();

  auto type2 = opr2->value_type();
  auto type3 = opr3->value_type();
  mType = calc_type(type2, type3);
}

// @brief デストラクタ
EiConditionOp::~EiConditionOp()
{
}

// @brief 式のタイプを返す．
VlValueType
EiConditionOp::value_type() const
{
  return mType;
}

// @brief 要求される式の型を計算してセットする．
void
EiConditionOp::_set_reqsize(
  const VlValueType& type
)
{
  mType = update_size(mType, type);

  // 第1オペランドは不変

  // 第2, 第3オペランドは type をそのまま伝える．
  mOpr[1]->set_reqsize(mType);
  mOpr[2]->set_reqsize(mType);
}


//////////////////////////////////////////////////////////////////////
// クラス EiMinTypMaxOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiMinTypMaxOp::EiMinTypMaxOp(
  const AstExpr* ast_expr,
  ElbExpr* opr1,
  ElbExpr* opr2,
  ElbExpr* opr3
) : EiTernaryOp(ast_expr, opr1, opr2, opr3)
{
  // とりあえず真ん中の式を使う．
  mType = opr2->value_type();
}

// @brief デストラクタ
EiMinTypMaxOp::~EiMinTypMaxOp()
{
}

// @brief 式のタイプを返す．
VlValueType
EiMinTypMaxOp::value_type() const
{
  return mType;
}

// @brief 要求される式の型を計算してセットする．
void
EiMinTypMaxOp::_set_reqsize(
  const VlValueType& type
)
{
  mType = update_size(mType, type);

  // 3オペランドには type をそのまま伝える．
  mOpr[0]->set_reqsize(mType);
  mOpr[1]->set_reqsize(mType);
  mOpr[2]->set_reqsize(mType);
}

END_NAMESPACE_YM_VERILOG
