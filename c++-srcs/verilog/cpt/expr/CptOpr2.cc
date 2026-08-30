
/// @file CptOpr2.cc
/// @brief CptOpr2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr2.h"
#include "CptOpr_Sub.h"
#include "CptOpr_Div.h"
#include "CptOpr_Mod.h"
#include "CptOpr_Eq.h"
#include "CptOpr_Neq.h"
#include "CptOpr_CaseEq.h"
#include "CptOpr_CaseNeq.h"
#include "CptOpr_Gt.h"
#include "CptOpr_Ge.h"
#include "CptOpr_Lt.h"
#include "CptOpr_Le.h"
#include "CptOpr_LShift.h"
#include "CptOpr_RShift.h"
#include "CptOpr_Add.h"
#include "CptOpr_Mult.h"
#include "CptOpr_LogAnd.h"
#include "CptOpr_LogOr.h"
#include "CptOpr_BitAnd.h"
#include "CptOpr_BitOr.h"
#include "CptOpr_BitXor.h"
#include "CptOpr_BitXNor.h"
#include "CptOpr_ArithLShift.h"
#include "CptOpr_ArithRShift.h"
#include "CptOpr_Power.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptOpr2
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptOpr2::file_region() const
{
  return FileRegion(mOpr[0]->file_region(), mOpr[1]->file_region());
}

// @brief 0番目のオペランドの取得
const AstExpr*
CptOpr2::operand0() const
{
  return mOpr[0];
}

// @brief 1番目のオペランドの取得
const AstExpr*
CptOpr2::operand1() const
{
  return mOpr[1];
}

// @brief インデックスとして使える式のチェック
bool
CptOpr2::is_index_expr() const
{
  return operand0()->is_index_expr() && operand1()->is_index_expr();
}

// @brief インデックスの値の取得
int
CptOpr2::index_value() const
{
  if ( !is_index_expr() ) {
    throw std::logic_error{"is_index_expr() = false"};
  }
  auto index0 = operand0()->index_value();
  auto index1 = operand1()->index_value();
  switch ( op_type() ) {
  case VpiOpType::Sub:         return index0 - index1;
  case VpiOpType::Div:         return index0 / index1;
  case VpiOpType::Mod:         return index0 % index1;
  case VpiOpType::Eq:          return index0 == index1;
  case VpiOpType::Neq:         return index0 != index1;
  case VpiOpType::CaseEq:      return index0 == index1;
  case VpiOpType::CaseNeq:     return index0 != index1;
  case VpiOpType::Gt:          return index0 > index1;
  case VpiOpType::Ge:          return index0 >= index1;
  case VpiOpType::Lt:          return index0 < index1;
  case VpiOpType::Le:          return index0 <= index1;
  case VpiOpType::LShift:      return index0 << index1;
  case VpiOpType::RShift:      return index0 >> index1;
  case VpiOpType::Add:         return index0 + index1;
  case VpiOpType::Mult:        return index0 * index1;
  case VpiOpType::LogAnd:      return (index0 != 0) && (index1 != 0);
  case VpiOpType::LogOr:       return (index0 != 0) || (index1 != 0);
  case VpiOpType::BitAnd:      return index0 & index1;
  case VpiOpType::BitOr:       return index0 | index1;
  case VpiOpType::BitXor:      return index0 ^ index1;
  case VpiOpType::BitXNor:     return ~(index0 ^ index1);
  case VpiOpType::ArithLShift: return index0 << index1;
  case VpiOpType::ArithRShift: return index0 >> index1;
  default: break;
  }
  throw std::logic_error{"Should not be reached"};
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_Sub
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_Sub::op_type() const
{
  return VpiOpType::Sub;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_Div
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_Div::op_type() const
{
  return VpiOpType::Div;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_Mod
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_Mod::op_type() const
{
  return VpiOpType::Mod;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_Eq
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_Eq::op_type() const
{
  return VpiOpType::Eq;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_Neq
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_Neq::op_type() const
{
  return VpiOpType::Neq;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_CaseEq
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_CaseEq::op_type() const
{
  return VpiOpType::CaseEq;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_CaseNeq
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_CaseNeq::op_type() const
{
  return VpiOpType::CaseNeq;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_Gt
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_Gt::op_type() const
{
  return VpiOpType::Gt;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_Ge
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_Ge::op_type() const
{
  return VpiOpType::Ge;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_Lt
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_Lt::op_type() const
{
  return VpiOpType::Lt;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_Le
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_Le::op_type() const
{
  return VpiOpType::Le;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_LShift
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_LShift::op_type() const
{
  return VpiOpType::LShift;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_RShift
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_RShift::op_type() const
{
  return VpiOpType::RShift;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_Add
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_Add::op_type() const
{
  return VpiOpType::Add;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_Mult
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_Mult::op_type() const
{
  return VpiOpType::Mult;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_LogAnd
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_LogAnd::op_type() const
{
  return VpiOpType::LogAnd;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_LogOr
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_LogOr::op_type() const
{
  return VpiOpType::LogOr;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_BitAnd
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_BitAnd::op_type() const
{
  return VpiOpType::BitAnd;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_BitOr
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_BitOr::op_type() const
{
  return VpiOpType::BitOr;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_BitXor
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_BitXor::op_type() const
{
  return VpiOpType::BitXor;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_BitXNor
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_BitXNor::op_type() const
{
  return VpiOpType::BitXNor;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_ArithLShift
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_ArithLShift::op_type() const
{
  return VpiOpType::ArithLShift;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_ArighRShift
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_ArithRShift::op_type() const
{
  return VpiOpType::ArithRShift;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_Power
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_Power::op_type() const
{
  return VpiOpType::Power;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

template <typename T>
PtExpr*
new_opr(
  Alloc& alloc,
  const AstExpr* opr1,
  const AstExpr* opr2
)
{
  auto p = alloc.get_memory(sizeof(T));
  return new (p) T(opr1, opr2);
}

PtExpr*
PtFactory::new_Opr(
  VpiOpType type,
  const AstExpr* opr1,
  const AstExpr* opr2
)
{
  switch ( type ) {
  case VpiOpType::Sub:         return new_opr<CptOpr_Sub>(mAlloc, opr1, opr2);
  case VpiOpType::Div:         return new_opr<CptOpr_Div>(mAlloc, opr1, opr2);
  case VpiOpType::Mod:         return new_opr<CptOpr_Mod>(mAlloc, opr1, opr2);
  case VpiOpType::Eq:          return new_opr<CptOpr_Eq>(mAlloc, opr1, opr2);
  case VpiOpType::Neq:         return new_opr<CptOpr_Neq>(mAlloc, opr1, opr2);
  case VpiOpType::CaseEq:      return new_opr<CptOpr_CaseEq>(mAlloc, opr1, opr2);
  case VpiOpType::CaseNeq:     return new_opr<CptOpr_CaseNeq>(mAlloc, opr1, opr2);
  case VpiOpType::Gt:          return new_opr<CptOpr_Gt>(mAlloc, opr1, opr2);
  case VpiOpType::Ge:          return new_opr<CptOpr_Ge>(mAlloc, opr1, opr2);
  case VpiOpType::Lt:          return new_opr<CptOpr_Lt>(mAlloc, opr1, opr2);
  case VpiOpType::Le:          return new_opr<CptOpr_Le>(mAlloc, opr1, opr2);
  case VpiOpType::LShift:      return new_opr<CptOpr_LShift>(mAlloc, opr1, opr2);
  case VpiOpType::RShift:      return new_opr<CptOpr_RShift>(mAlloc, opr1, opr2);
  case VpiOpType::Add:         return new_opr<CptOpr_Add>(mAlloc, opr1, opr2);
  case VpiOpType::Mult:        return new_opr<CptOpr_Mult>(mAlloc, opr1, opr2);
  case VpiOpType::LogAnd:      return new_opr<CptOpr_LogAnd>(mAlloc, opr1, opr2);
  case VpiOpType::LogOr:       return new_opr<CptOpr_LogOr>(mAlloc, opr1, opr2);
  case VpiOpType::BitAnd:      return new_opr<CptOpr_BitAnd>(mAlloc, opr1, opr2);
  case VpiOpType::BitOr:       return new_opr<CptOpr_BitOr>(mAlloc, opr1, opr2);
  case VpiOpType::BitXor:      return new_opr<CptOpr_BitXor>(mAlloc, opr1, opr2);
  case VpiOpType::BitXNor:     return new_opr<CptOpr_BitXNor>(mAlloc, opr1, opr2);
  case VpiOpType::ArithLShift: return new_opr<CptOpr_ArithLShift>(mAlloc, opr1, opr2);
  case VpiOpType::ArithRShift: return new_opr<CptOpr_ArithRShift>(mAlloc, opr1, opr2);
  default: break;
  }
  throw std::logic_error{"Should not be reached"};
}

END_NAMESPACE_YM_VERILOG
