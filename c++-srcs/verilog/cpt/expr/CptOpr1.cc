
/// @file CptOpr1.cc
/// @brief CptOpr1 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr1.h"
#include "CptOpr_Minus.h"
#include "CptOpr_Plus.h"
#include "CptOpr_Posedge.h"
#include "CptOpr_Negedge.h"
#include "CptOpr_Null.h"
#include "CptOpr_Not.h"
#include "CptOpr_BitNeg.h"
#include "CptOpr_UnaryAnd.h"
#include "CptOpr_UnaryNand.h"
#include "CptOpr_UnaryOr.h"
#include "CptOpr_UnaryNor.h"
#include "CptOpr_UnaryXor.h"
#include "CptOpr_UnaryXNor.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptOpr1
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptOpr1::file_region() const
{
  return mFileRegion;
}

// 階層名の添字として使える式の時に true を返す．
bool
CptOpr1::is_index_expr() const
{
  // posedge, negedge はNG
  if ( op_type() == VpiOpType::Posedge || op_type() == VpiOpType::Negedge ) {
    return false;
  }
  // それ以外はオペランドの型で決まる．
  return operand0()->is_index_expr();
}

inline
int
reduction_xor(
  int val
)
{
  int rval = 0;
  for ( SizeType i = 0; i < 64; ++ i ) {
    if ( val & 1 ) {
      rval ^= 1;
    }
    val >>= 1;
  }
  return rval;
}

// 階層名の添字として使える式の時にその値を返す．
int
CptOpr1::index_value() const
{
  auto index0 = operand0()->index_value();
  switch ( op_type() ) {
  case VpiOpType::Minus:
    return - index0;

  case VpiOpType::Plus:
    return index0;

  case VpiOpType::Null:
    return index0;

  case VpiOpType::Not:
    if ( index0 == 0 ) {
      return 1;
    }
    else {
      return 0;
    }

  case VpiOpType::BitNeg:
    return ~index0;

  case VpiOpType::UnaryAnd:
    if ( index0 == ~0 ) {
      return 1;
    }
    else {
      return 0;
    }

  case VpiOpType::UnaryNand:
    if ( index0 == ~0 ) {
      return 0;
    }
    else {
      return 1;
    }

  case VpiOpType::UnaryOr:
    if ( index0 == 0 ) {
      return 0;
    }
    else {
      return 1;
    }

  case VpiOpType::UnaryNor:
    if ( index0 == 0 ) {
      return 1;
    }
    else {
      return 0;
    }

  case VpiOpType::UnaryXor:
    if ( reduction_xor(index0) ) {
      return 1;
    }
    else {
      return 0;
    }

  case VpiOpType::UnaryXNor:
    if ( reduction_xor(index0) ) {
      return 0;
    }
    else {
      return 1;
    }

  default:
    break;
  }
  throw std::logic_error{"is_index_expr() == false"};
}

// @brief 0番目のオペランドの取得
const AstExpr*
CptOpr1::operand0() const
{
  return mOpr;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_Minus
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_Minus::op_type() const
{
  return VpiOpType::Minus;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_Plus
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_Plus::op_type() const
{
  return VpiOpType::Plus;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_Not
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_Not::op_type() const
{
  return VpiOpType::Not;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_BitNeg
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_BitNeg::op_type() const
{
  return VpiOpType::BitNeg;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_UnaryAnd
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_UnaryAnd::op_type() const
{
  return VpiOpType::UnaryAnd;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_UnaryNand
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_UnaryNand::op_type() const
{
  return VpiOpType::UnaryNand;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_UnaryOr
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_UnaryOr::op_type() const
{
  return VpiOpType::UnaryOr;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_UnaryNor
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_UnaryNor::op_type() const
{
  return VpiOpType::UnaryNor;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_UnaryXor
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_UnaryXor::op_type() const
{
  return VpiOpType::UnaryXor;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_UnaryXNor
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_UnaryXNor::op_type() const
{
  return VpiOpType::UnaryXNor;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_Posedge
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_Posedge::op_type() const
{
  return VpiOpType::Posedge;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_Negedge
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_Negedge::op_type() const
{
  return VpiOpType::Negedge;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_Null
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_Null::op_type() const
{
  return VpiOpType::Null;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

template <typename T>
PtExpr*
new_opr(
  Alloc& alloc,
  const FileRegion& file_region,
  const AstExpr* opr
)
{
  auto p = alloc.get_memory(sizeof(T));
  return new (p) T(file_region, opr);
}

// 演算子を生成する．
PtExpr*
PtFactory::new_Opr(
  const FileRegion& file_region,
  VpiOpType type,
  const AstExpr* opr
)
{
  switch ( type ) {
  case VpiOpType::Minus:     return new_opr<CptOpr_Minus>(mAlloc, file_region, opr);
  case VpiOpType::Plus:      return new_opr<CptOpr_Plus>(mAlloc, file_region, opr);
  case VpiOpType::Null:      return new_opr<CptOpr_Null>(mAlloc, file_region, opr);
  case VpiOpType::Not:       return new_opr<CptOpr_Not>(mAlloc, file_region, opr);
  case VpiOpType::BitNeg:    return new_opr<CptOpr_BitNeg>(mAlloc, file_region, opr);
  case VpiOpType::UnaryAnd:  return new_opr<CptOpr_UnaryAnd>(mAlloc, file_region, opr);
  case VpiOpType::UnaryNand: return new_opr<CptOpr_UnaryNand>(mAlloc, file_region, opr);
  case VpiOpType::UnaryOr:   return new_opr<CptOpr_UnaryOr>(mAlloc, file_region, opr);
  case VpiOpType::UnaryNor:  return new_opr<CptOpr_UnaryNor>(mAlloc, file_region, opr);
  case VpiOpType::UnaryXor:  return new_opr<CptOpr_UnaryXor>(mAlloc, file_region, opr);
  case VpiOpType::UnaryXNor: return new_opr<CptOpr_UnaryXNor>(mAlloc, file_region, opr);
  case VpiOpType::Posedge:   return new_opr<CptOpr_Posedge>(mAlloc, file_region, opr);
  case VpiOpType::Negedge:   return new_opr<CptOpr_Negedge>(mAlloc, file_region, opr);
  default: break;
  }
  throw std::logic_error{"is_index_expr() == false"};
}

END_NAMESPACE_YM_VERILOG
