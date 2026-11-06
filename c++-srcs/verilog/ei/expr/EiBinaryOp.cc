
/// @file EiBinaryOp.cc
/// @brief EiBinaryOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbMgr.h"
#include "EiBinaryOp.h"
#include "EiCompareOp.h"
#include "EiBinLogOp.h"
#include "EiBinBitOp.h"
#include "EiBinArithOp.h"
#include "EiPowerOp.h"
#include "EiShiftOp.h"
#include "ym/vl/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbMgr
//////////////////////////////////////////////////////////////////////

// @brief 2項演算子を生成する．
ElbExpr*
ElbMgr::new_BinaryOp(
  const AstExpr& ast_expr,
  VpiOpType op_type,
  ElbExpr* opr0,
  ElbExpr* opr1
)
{
  return new_expr(
    [&]() -> ElbExpr* {
      switch( op_type ) {
      case VpiOpType::BitAnd:
      case VpiOpType::BitOr:
      case VpiOpType::BitXNor:
      case VpiOpType::BitXor:
	return new EiBinBitOp(ast_expr, opr0, opr1);

      case VpiOpType::Add:
      case VpiOpType::Sub:
      case VpiOpType::Mult:
      case VpiOpType::Div:
      case VpiOpType::Mod:
	return new EiBinArithOp(ast_expr, opr0, opr1);

      case VpiOpType::Power:
	return new EiPowerOp(ast_expr, opr0, opr1);

      case VpiOpType::LShift:
      case VpiOpType::RShift:
      case VpiOpType::ArithLShift:
      case VpiOpType::ArithRShift:
	return new EiShiftOp(ast_expr, opr0, opr1);

      case VpiOpType::LogAnd:
      case VpiOpType::LogOr:
	return new EiBinLogOp(ast_expr, opr0, opr1);

      case VpiOpType::CaseEq:
      case VpiOpType::CaseNeq:
      case VpiOpType::Eq:
      case VpiOpType::Neq:
      case VpiOpType::Ge:
      case VpiOpType::Gt:
      case VpiOpType::Le:
      case VpiOpType::Lt:
	return new EiCompareOp(ast_expr, opr0, opr1);

      default:
	break;
      }

      throw std::logic_error{"Should not be reached"};
    });
}


//////////////////////////////////////////////////////////////////////
// クラス EIBinaryOp
//////////////////////////////////////////////////////////////////////

// @brief 定数の時 true を返す．
bool
EiBinaryOp::is_const() const
{
  return mOpr[0]->is_const() && mOpr[1]->is_const();
}

// @brief オペランド数を返す．
SizeType
EiBinaryOp::operand_num() const
{
  return 2;
}

// @brief オペランドを返す．
const VlExpr*
EiBinaryOp::operand(
  SizeType pos
) const
{
  if ( pos >= 2 ) {
    throw std::out_of_range{"pos is out of range"};
  }
  return mOpr[pos];
}

// @brief オペランドのリストを返す．
std::vector<const VlExpr*>
EiBinaryOp::operand_list() const
{
  return {mOpr[0], mOpr[1]};
}


//////////////////////////////////////////////////////////////////////
// クラス EiCompareOp
//////////////////////////////////////////////////////////////////////

// @brief 式のタイプを返す．
VlValueType
EiCompareOp::value_type() const
{
  // 常に1ビット符号なし
  return VlValueType(false, true, 1);
}

// @brief 要求される式の型を計算してセットする．
void
EiCompareOp::_set_reqsize(
  const VlValueType& type
)
{
  // なにもしない．
}


//////////////////////////////////////////////////////////////////////
// クラス EiBinLogOp
//////////////////////////////////////////////////////////////////////

// @brief 式のタイプを返す．
VlValueType
EiBinLogOp::value_type() const
{
  // 常に1ビット符号なし
  return VlValueType{false, true, 1};
}

// @brief 要求される式の型を計算してセットする．
void
EiBinLogOp::_set_reqsize(
  const VlValueType& type
)
{
  // なにもしない．
}


//////////////////////////////////////////////////////////////////////
// クラス EiBinBitOp
//////////////////////////////////////////////////////////////////////

// @brief 式のタイプを返す．
VlValueType
EiBinBitOp::value_type() const
{
  return mType;
}

// @brief 要求される式の型を計算してセットする．
void
EiBinBitOp::_set_reqsize(
  const VlValueType& type
)
{
  mType = update_size(mType, type);
  mOpr[0]->set_reqsize(mType);
  mOpr[1]->set_reqsize(mType);
}


//////////////////////////////////////////////////////////////////////
// クラス EiBinArithOp
//////////////////////////////////////////////////////////////////////

// @brief 式のタイプを返す．
VlValueType
EiBinArithOp::value_type() const
{
  return mType;
}

// @brief 要求される式の型を計算してセットする．
void
EiBinArithOp::_set_reqsize(
  const VlValueType& type
)
{
  mType = update_size(mType, type);
  mOpr[0]->set_reqsize(mType);
  mOpr[1]->set_reqsize(mType);
}


//////////////////////////////////////////////////////////////////////
// クラス EiPowerOp
//////////////////////////////////////////////////////////////////////

// @brief 式のタイプを返す．
VlValueType
EiPowerOp::value_type() const
{
  return mType;
}

// @brief 要求される式の型を計算してセットする．
void
EiPowerOp::_set_reqsize(
  const VlValueType& type
)
{
  mType = update_size(mType, type);

  // 第1オペランドは type をそのまま伝える．
  mOpr[0]->set_reqsize(mType);

  // 第2オペランドは不変
}


//////////////////////////////////////////////////////////////////////
// クラス EiShiftOp
//////////////////////////////////////////////////////////////////////

// @brief 式のタイプを返す．
VlValueType
EiShiftOp::value_type() const
{
  return mType;
}

// @brief 要求される式の型を計算してセットする．
void
EiShiftOp::_set_reqsize(
  const VlValueType& type
)
{
  mType = update_size(mType, type);

  // 第1オペランドは type をそのまま伝える．
  mOpr[0]->set_reqsize(mType);

  // 第2オペランドは不変
}

END_NAMESPACE_YM_VERILOG
