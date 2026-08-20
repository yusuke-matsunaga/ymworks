
/// @file CptOpr.cc
/// @brief CptOpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptOpr
//////////////////////////////////////////////////////////////////////

// クラスの型を返す．
AstExpr::Type
CptOpr::type() const
{
  return Opr;
}

// 演算子のトークン番号を得る．
VpiOpType
CptOpr::op_type() const
{
  return mOpType;
}


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
// クラス CptOpr3
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptOpr3::file_region() const
{
  return FileRegion(mOpr[0]->file_region(), mOpr[2]->file_region());
}

// @brief 0番目のオペランドの取得
const AstExpr*
CptOpr3::operand0() const
{
  return mOpr[0];
}

// @brief 1番目のオペランドの取得
const AstExpr*
CptOpr3::operand1() const
{
  return mOpr[1];
}

// @brief 2番目のオペランドの取得
const AstExpr*
CptOpr3::operand2() const
{
  return mOpr[2];
}


//////////////////////////////////////////////////////////////////////
// クラス CptConcat
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptConcat::file_region() const
{
  return mFileRegion;
}

// クラスの型を返す．
AstExpr::Type
CptConcat::type() const
{
  return Opr;
}

///演算子の種類の取得
VpiOpType
CptConcat::op_type() const
{
  return VpiOpType::Concat;
}

// @brief オペランドの数の取得
SizeType
CptConcat::operand_num() const
{
  return mExprList.size();
}

// @brief オペランドのリストの取得
const AstExpr*
CptConcat::operand(
  SizeType index
) const
{
  return mExprList[index];
}

// @brief オペランドのリストの取得
AstExprVec
CptConcat::operand_list() const
{
  return mExprList.to_vector();
}


//////////////////////////////////////////////////////////////////////
// クラス CptMultiConcat
//////////////////////////////////////////////////////////////////////

// 演算子の種類の取得
VpiOpType
CptMultiConcat::op_type() const
{
  return VpiOpType::MultiConcat;
}

// @brief multi-concat の繰り返し数
const AstExpr*
CptMultiConcat::rep() const
{
  return mRep;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// 演算子を生成する．
PtExpr*
PtFactory::new_Opr(
  const FileRegion& file_region,
  VpiOpType type,
  const AstExpr* opr
)
{
  auto p = mAlloc.get_memory(sizeof(CptOpr1));
  return new (p) CptOpr1(file_region, type, opr);
}

PtExpr*
PtFactory::new_Opr(
  const FileRegion& file_region,
  VpiOpType type,
  const AstExpr* opr1,
  const AstExpr* opr2
)
{
  // 実は file_region は不要
  auto p = mAlloc.get_memory(sizeof(CptOpr2));
  return new (p) CptOpr2(type, opr1, opr2);
}

PtExpr*
PtFactory::new_Opr(
  const FileRegion& file_region,
  VpiOpType type,
  const AstExpr* opr1,
  const AstExpr* opr2,
  const AstExpr* opr3
)
{
  // 実は file_region は不要
  auto p = mAlloc.get_memory(sizeof(CptOpr3));
  return new (p) CptOpr3(type, opr1, opr2, opr3);
}

// concatination を生成する．
PtExpr*
PtFactory::new_Concat(
  const FileRegion& file_region,
  PtExprList* expr_list
)
{
  auto p = mAlloc.get_memory(sizeof(CptConcat));
  return new (p) CptConcat(file_region,
			   expr_list->to_array(mAlloc));
}

// multiple concatenation を生成する．
PtExpr*
PtFactory::new_MultiConcat(
  const FileRegion& file_region,
  const AstExpr* rep,
  PtExprList* expr_list
)
{
  auto p = mAlloc.get_memory(sizeof(CptMultiConcat));
  return new (p) CptMultiConcat(file_region, rep,
				expr_list->to_array(mAlloc));
}

END_NAMESPACE_YM_VERILOG
