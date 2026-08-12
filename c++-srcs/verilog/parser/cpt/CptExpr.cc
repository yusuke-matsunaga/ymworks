
/// @file CptExpr.cc
/// @brief CptExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptExpr.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"
#include "parser/PtHierName.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// expression を表す基底クラス
//////////////////////////////////////////////////////////////////////

// @brief 演算子の種類の取得
VpiOpType
CptExpr::op_type() const
{
  throw std::logic_error{"Not an Operation Type"};
}

// @brief 階層ブランチの要素数の取得
SizeType
CptExpr::namebranch_num() const
{
  return 0;
}

// @brief 階層ブランチの取得
const AstNameBranch*
CptExpr::namebranch(
  SizeType pos
) const
{
  throw std::out_of_range{"pos is out of range"};
}

// @brief 末尾の名前の取得
const char*
CptExpr::name() const
{
  return nullptr;
}

// @brief オペランドの数の取得
SizeType
CptExpr::operand_num() const
{
  return 0;
}

// @brief 0番目のオペランドの取得
const AstExpr*
CptExpr::operand0() const
{
  throw std::logic_error{"Does not have operand0"};
}

// @brief 1番目のオペランドの取得
const AstExpr*
CptExpr::operand1() const
{
  throw std::logic_error{"Does not have operand1"};
}

// @brief 2番目のオペランドの取得
const AstExpr*
CptExpr::operand2() const
{
  throw std::logic_error{"Does not have operand2"};
}

// @brief オペランドの取得
const AstExpr*
CptExpr::operand(
  SizeType pos
) const
{
  throw std::out_of_range{"pos is out of range"};
}

// @brief multi-concat の繰り返し数
const AstExpr*
CptExpr::rep() const
{
  throw std::logic_error{"Not a multi-concat oprator"};
}

// @brief 定数インデックスのチェック
bool
CptExpr::is_const_index() const
{
  return false;
}

// @brief インデックスリストのサイズの取得
SizeType
CptExpr::index_num() const
{
  return 0;
}

// @brief インデックスの取得
const AstExpr*
CptExpr::index(
  SizeType pos
) const
{
  throw std::out_of_range{"pos is out of range"};
}

// @brief 範囲指定の取得
const AstPart*
CptExpr::part() const
{
  return nullptr;
}

// @brief 定数の種類の取得
VpiConstType
CptExpr::const_type() const
{
  throw std::logic_error{"Not a CONST type"};
}

// @brief 整数型の定数のサイズの取得
SizeType
CptExpr::const_size() const
{
  throw std::logic_error{"Not a CONST type"};
}

// @brief 整数型の値の取得
BitVector
CptExpr::const_bitvect() const
{
  throw std::logic_error{"Not a INT CONST type"};
}

// @brief 整数型および文字列型の定数の文字列表現の取得
const char*
CptExpr::const_str() const
{
  throw std::logic_error{"Not a INT/STRING CONST type"};
}

// @brief 実数型の値の取得
double
CptExpr::const_real() const
{
  throw std::logic_error{"Not a REAL CONST type"};
}

// @brief インデックスとして使える式のチェック
bool
CptExpr::is_index_expr() const
{
  return false;
}

// @brief インデックスの値の取得
int
CptExpr::index_value() const
{
  throw std::logic_error{"is_index_expr() = false"};
}

// @brief simple primary のチェック
bool
CptExpr::is_simple() const
{
  throw std::logic_error{"Not a PRIMARY type"};
}


//////////////////////////////////////////////////////////////////////
// 演算子のベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptOpr::CptOpr(
  VpiOpType op_type
) : mOpType{op_type}
{
}

// デストラクタ
CptOpr::~CptOpr()
{
}

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
// 単項演算子を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptOpr1::CptOpr1(
  const FileRegion& file_region,
  VpiOpType op_type,
  const AstExpr* opr
) : CptOpr(op_type),
    mFileRegion{file_region},
    mOpr{opr}
{
  if ( opr == nullptr ) {
    throw std::logic_error{"opr == nullptr"};
  }
}

// デストラクタ
CptOpr1::~CptOpr1()
{
}

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

// @brief オペランドの数の取得
SizeType
CptOpr1::operand_num() const
{
  return 1;
}

// @brief 0番目のオペランドの取得
const AstExpr*
CptOpr1::operand0() const
{
  return mOpr;
}

// @brief オペランドの取得
const AstExpr*
CptOpr1::operand(
  SizeType pos
) const
{
  if ( pos == 0 ) {
    return mOpr;
  }
  throw std::out_of_range{"pos is out of range"};
}


//////////////////////////////////////////////////////////////////////
// 二項演算子を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptOpr2::CptOpr2(
  VpiOpType op_type,
  const AstExpr* opr1,
  const AstExpr* opr2
) : CptOpr(op_type),
    mOpr{opr1, opr2}
{
  if ( opr1 == nullptr ) {
    throw std::logic_error{"opr1 == nullptr"};
  }
  if ( opr2 == nullptr ) {
    throw std::logic_error{"opr2 == nullptr"};
  }
}

// デストラクタ
CptOpr2::~CptOpr2()
{
}

// ファイル位置を返す．
FileRegion
CptOpr2::file_region() const
{
  return FileRegion(mOpr[0]->file_region(), mOpr[1]->file_region());
}

// @brief オペランドの数の取得
SizeType
CptOpr2::operand_num() const
{
  return 2;
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

// @brief オペランドの取得
const AstExpr*
CptOpr2::operand(
  SizeType pos
) const
{
  if ( pos >= 2 ) {
    throw std::out_of_range{"pos is out of range"};
  }
  return mOpr[pos];
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
// 三項演算子を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptOpr3::CptOpr3(
  VpiOpType op_type,
  const AstExpr* opr1,
  const AstExpr* opr2,
  const AstExpr* opr3
) : CptOpr(op_type),
    mOpr{opr1, opr2, opr3}
{
  if ( opr1 == nullptr ) {
    throw std::logic_error{"opr1 == nullptr"};
  }
  if ( opr2 == nullptr ) {
    throw std::logic_error{"opr2 == nullptr"};
  }
  if ( opr3 == nullptr ) {
    throw std::logic_error{"opr3 == nullptr"};
  }
}

// デストラクタ
CptOpr3::~CptOpr3()
{
}

// ファイル位置を返す．
FileRegion
CptOpr3::file_region() const
{
  return FileRegion(mOpr[0]->file_region(), mOpr[2]->file_region());
}

// @brief オペランドの数の取得
SizeType
CptOpr3::operand_num() const
{
  return 3;
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

// @brief オペランドの取得
const AstExpr*
CptOpr3::operand(
  SizeType pos
) const
{
  if ( pos >= 3 ) {
    throw std::out_of_range{"pos is out of range"};
  }
  return mOpr[pos];
}


//////////////////////////////////////////////////////////////////////
// concatenation を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptConcat::CptConcat(
  const FileRegion& file_region,
  PtExprArray&& expr_array
) : mFileRegion{file_region},
    mExprArray{std::move(expr_array)}
{
}

// デストラクタ
CptConcat::~CptConcat()
{
}

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
  return mExprArray.size();
}

// @brief 0番目のオペランドの取得
const AstExpr*
CptConcat::operand0() const
{
  if ( operand_num() == 0 ) {
    throw std::logic_error{"Does not have operan0"};
  }
  return mExprArray[0];
}

// @brief 1番目のオペランドの取得
const AstExpr*
CptConcat::operand1() const
{
  if ( operand_num() < 1 ) {
    throw std::logic_error{"Does not have operan1"};
  }
  return mExprArray[1];
}

// @brief 2番目のオペランドの取得
const AstExpr*
CptConcat::operand2() const
{
  if ( operand_num() < 2 ) {
    throw std::logic_error{"Does not have operan2"};
  }
  return mExprArray[2];
}

// @brief オペランドの取得
const AstExpr*
CptConcat::operand(
  SizeType pos
) const
{
  if ( pos >= operand_num() ) {
    throw std::out_of_range{"pos is out of range"};
  }
  return mExprArray[pos];
}


//////////////////////////////////////////////////////////////////////
// multiple concatenation を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptMultiConcat::CptMultiConcat(
  const FileRegion& file_region,
  const AstExpr* rep,
  PtExprArray&& expr_array
) : CptConcat(file_region, std::move(expr_array)),
    mRep{rep}
{
}

// デストラクタ
CptMultiConcat::~CptMultiConcat()
{
}

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
// min/typ/max delay のベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptMinTypMax::CptMinTypMax(
  const AstExpr* val0,
  const AstExpr* val1,
  const AstExpr* val2
) : mValue{val0, val1, val2}
{
  if ( val0 == nullptr ) {
    throw std::logic_error{"val0 == nullptr"};
  }
  if ( val1 == nullptr ) {
    throw std::logic_error{"val1 == nullptr"};
  }
  if ( val2 == nullptr ) {
    throw std::logic_error{"val2 == nullptr"};
  }
}

// デストラクタ
CptMinTypMax::~CptMinTypMax()
{
}

// ファイル位置を返す．
FileRegion
CptMinTypMax::file_region() const
{
  return FileRegion(mValue[0]->file_region(), mValue[2]->file_region());
}

// クラスの型を返す．
AstExpr::Type
CptMinTypMax::type() const
{
  return Opr;
}

// 演算子の種類の取得
VpiOpType
CptMinTypMax::op_type() const
{
  return VpiOpType::MinTypMax;
}

// 子供の数の取得
SizeType
CptMinTypMax::operand_num() const
{
  return 3;
}

// @brief 0番目のオペランドの取得
const AstExpr*
CptMinTypMax::operand0() const
{
  return mValue[0];
}

// @brief 1番目のオペランドの取得
const AstExpr*
CptMinTypMax::operand1() const
{
  return mValue[1];
}

// @brief 2番目のオペランドの取得
const AstExpr*
CptMinTypMax::operand2() const
{
  return mValue[2];
}

// 値(式)を取出す．
const AstExpr*
CptMinTypMax::operand(
  SizeType pos
) const
{
  if ( pos >= 3 ) {
    throw std::out_of_range{"pos is out of range"};
  }
  return mValue[pos];
}


//////////////////////////////////////////////////////////////////////
// function call / system function call に共通の基底クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptFuncCallBase::CptFuncCallBase(
  const FileRegion& file_region,
  const char* name,
  PtExprArray&& arg_array
) : mFileRegion{file_region},
    mName{name},
    mArgArray{std::move(arg_array)}
{
}

// デストラクタ
CptFuncCallBase::~CptFuncCallBase()
{
}

// ファイル位置を返す．
FileRegion
CptFuncCallBase::file_region() const
{
  return mFileRegion;
}

// 末尾の名前を返す．
const char*
CptFuncCallBase::name() const
{
  return mName;
}

// @brief オペランドの数の取得
SizeType
CptFuncCallBase::operand_num() const
{
  return mArgArray.size();
}

// @brief 0番目のオペランドの取得
const AstExpr*
CptFuncCallBase::operand0() const
{
  if ( operand_num() == 0 ) {
    throw std::logic_error{"Does not have operan0"};
  }
  return mArgArray[0];
}

// @brief 1番目のオペランドの取得
const AstExpr*
CptFuncCallBase::operand1() const
{
  if ( operand_num() < 1 ) {
    throw std::logic_error{"Does not have operan1"};
  }
  return mArgArray[1];
}

// @brief 2番目のオペランドの取得
const AstExpr*
CptFuncCallBase::operand2() const
{
  if ( operand_num() < 2 ) {
    throw std::logic_error{"Does not have operan2"};
  }
  return mArgArray[2];
}

// @brief オペランドの取得
const AstExpr*
CptFuncCallBase::operand(
  SizeType pos
) const
{
  if ( pos >= operand_num() ) {
    throw std::out_of_range{"pos is out of range"};
  }
  return mArgArray[pos];
}


//////////////////////////////////////////////////////////////////////
// 階層なし名前を持つ function callを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptFuncCall::CptFuncCall(
  const FileRegion& file_region,
  const char* name,
  PtExprArray&& arg_array
) : CptFuncCallBase(file_region, name, std::move(arg_array))
{
}

// デストラクタ
CptFuncCall::~CptFuncCall()
{
}

// クラスの型を返す．
AstExpr::Type
CptFuncCall::type() const
{
  return FuncCall;
}


//////////////////////////////////////////////////////////////////////
// 階層つき名前を持つ function call を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptFuncCallH::CptFuncCallH(
  const FileRegion& file_region,
  PtNameBranchArray&& nb_array,
  const char* tail_name,
  PtExprArray&& arg_array
) : CptFuncCall(file_region, tail_name, std::move(arg_array)),
    mNbArray{std::move(nb_array)}
{
}

// デストラクタ
CptFuncCallH::~CptFuncCallH()
{
}

// @brief 階層ブランチの要素数の取得
SizeType
CptFuncCallH::namebranch_num() const
{
  return mNbArray.size();
}

// @brief 階層ブランチの取得
const AstNameBranch*
CptFuncCallH::namebranch(
  SizeType pos
) const
{
  return mNbArray[pos];
}


//////////////////////////////////////////////////////////////////////
// system function callを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptSysFuncCall::CptSysFuncCall(
  const FileRegion& file_region,
  const char* name,
  PtExprArray&& arg_array
) : CptFuncCallBase(file_region, name, std::move(arg_array))
{
}

// デストラクタ
CptSysFuncCall::~CptSysFuncCall()
{
}

// クラスの型を返す．
AstExpr::Type
CptSysFuncCall::type() const
{
  return SysFuncCall;
}


//////////////////////////////////////////////////////////////////////
// PtConstant のベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptConstant::CptConstant(
  const FileRegion& file_region
) : mFileRegion{file_region}
{
}

// デストラクタ
CptConstant::~CptConstant()
{
}

// ファイル位置を返す．
FileRegion
CptConstant::file_region() const
{
  return mFileRegion;
}

// クラスの型を返す．
AstExpr::Type
CptConstant::type() const
{
  return Const;
}


//////////////////////////////////////////////////////////////////////
// 整数型の定数の基底クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptIntConstant::CptIntConstant(
  const FileRegion& file_region
) : CptConstant(file_region)
{
}

// デストラクタ
CptIntConstant::~CptIntConstant()
{
}

// 階層名の添字として使える式の時に true を返す．
bool
CptIntConstant::is_index_expr() const
{
  return true;
}

// @brief インデックスの値の取得
int
CptIntConstant::index_value() const
{
  return const_bitvect().to_int();
}

// 整数型の定数のサイズの取得
SizeType
CptIntConstant::const_size() const
{
  return 0;
}


//////////////////////////////////////////////////////////////////////
// 整数型の定数(サイズ/基数の指定なし)
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptIntConstant1::CptIntConstant1(
  const FileRegion& file_region,
  SizeType value
) : CptIntConstant(file_region),
    mValue{value}
{
}

// デストラクタ
CptIntConstant1::~CptIntConstant1()
{
}

// 定数の種類を表す型(vpiIntConst, vpiBinaryConst など) を返す．
VpiConstType
CptIntConstant1::const_type() const
{
  return VpiConstType::Int;
}

// 整数型の値の取得
BitVector
CptIntConstant1::const_bitvect() const
{
  return BitVector(mValue);
}

// 文字列型の値の取得
const char*
CptIntConstant1::const_str() const
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// 整数型の定数(基数のみ指定あり)
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptIntConstant2::CptIntConstant2(
  const FileRegion& file_region,
  VpiConstType const_type,
  const char* value
) : CptIntConstant(file_region),
    mConstType{const_type},
    mValue{value}
{
}

// デストラクタ
CptIntConstant2::~CptIntConstant2()
{
}

// 定数の種類を表す型(vpiIntConst, vpiBinaryConst など) を返す．
VpiConstType
CptIntConstant2::const_type() const
{
  return mConstType;
}

// 整数型の値の取得
BitVector
CptIntConstant2::const_bitvect() const
{
  switch ( mConstType ) {
  case VpiConstType::Dec:
  case VpiConstType::Int:
    return BitVector(0, false, 10, mValue);
  case VpiConstType::SignedDec:
    return BitVector(0, true,  10, mValue);
  case VpiConstType::Binary:
    return BitVector(0, false,  2, mValue);
  case VpiConstType::SignedBinary:
    return BitVector(0, true,   2, mValue);
  case VpiConstType::Oct:
    return BitVector(0, false,  8, mValue);
  case VpiConstType::SignedOct:
    return BitVector(0, true,   8, mValue);
  case VpiConstType::Hex:
    return BitVector(0, false, 16, mValue);
  case VpiConstType::SignedHex:
    return BitVector(0, true,  16, mValue);
  default: break;
  }
  throw std::logic_error{"Should not be reached"};
}

// 文字列型の値の取得
const char*
CptIntConstant2::const_str() const
{
  return mValue;
}


//////////////////////////////////////////////////////////////////////
// 整数型の定数(サイズと基数の指定あり)
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptIntConstant3::CptIntConstant3(
  const FileRegion& file_region,
  SizeType size,
  VpiConstType const_type,
  const char* value
) : CptIntConstant(file_region),
    mConstType{const_type},
    mSize{size},
    mValue{value}
{
}

// デストラクタ
CptIntConstant3::~CptIntConstant3()
{
}

// 定数の種類を表す型(vpiIntConst, vpiBinaryConst など) を返す．
VpiConstType
CptIntConstant3::const_type() const
{
  return mConstType;
}

// 整数型の定数のサイズの取得
SizeType
CptIntConstant3::const_size() const
{
  return mSize;
}

// 整数型の値の取得
BitVector
CptIntConstant3::const_bitvect() const
{
  switch ( mConstType ) {
  case VpiConstType::Dec:
  case VpiConstType::Int:
    return BitVector(mSize, false, 10, mValue);
  case VpiConstType::SignedDec:
    return BitVector(mSize, true,  10, mValue);
  case VpiConstType::Binary:
    return BitVector(mSize, false,  2, mValue);
  case VpiConstType::SignedBinary:
    return BitVector(mSize, true,   2, mValue);
  case VpiConstType::Oct:
    return BitVector(mSize, false,  8, mValue);
  case VpiConstType::SignedOct:
    return BitVector(mSize, true,   8, mValue);
  case VpiConstType::Hex:
    return BitVector(mSize, false, 16, mValue);
  case VpiConstType::SignedHex:
    return BitVector(mSize, true,  16, mValue);
  default: break;
  }
  throw std::logic_error{"Should not be reached"};
}

// 文字列型の値の取得
const char*
CptIntConstant3::const_str() const
{
  return mValue;
}


//////////////////////////////////////////////////////////////////////
// 実数型の定数(中身は PtConstant)
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptRealConstant::CptRealConstant(
  const FileRegion& file_region,
  double value
) : CptConstant(file_region),
    mValue{value}
{
}

// デストラクタ
CptRealConstant::~CptRealConstant()
{
}

// 定数の種類を表す型(vpiRealConst) を返す．
VpiConstType
CptRealConstant::const_type() const
{
  return VpiConstType::Real;
}

// 実数型の値の取得
double
CptRealConstant::const_real() const
{
  return mValue;
}


//////////////////////////////////////////////////////////////////////
// 文字列型の定数(中身は PtConstant)
//////////////////////////////////////////////////////////////////////

// 値を表す文字列を引数にとるコンストラクタ
CptStringConstant::CptStringConstant(
  const FileRegion& file_region,
  const char* value
) : CptConstant(file_region),
    mValue{value}
{
}

// デストラクタ
CptStringConstant::~CptStringConstant()
{
}

// 定数の種類を表す型(vpiStringConst) を返す．
VpiConstType
CptStringConstant::const_type() const
{
  return VpiConstType::String;
}

// 値を表示用の文字列の形で得る．
const char*
CptStringConstant::const_str() const
{
  return mValue;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPart
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptPart::file_region() const
{
  return mFileRegion;
}

// @brief 1番目の式を取り出す．
const AstExpr*
CptPart::left() const
{
  return mLeft;
}

// @brief 2番めの式を取り出す．
const AstExpr*
CptPart::right() const
{
  return mRight;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPartC
//////////////////////////////////////////////////////////////////////

// @brief 範囲指定のモードを返す．
VpiRangeMode
CptPartC::mode() const
{
  return VpiRangeMode::Const;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPartP
//////////////////////////////////////////////////////////////////////

// @brief 範囲指定のモードを返す．
VpiRangeMode
CptPartP::mode() const
{
  return VpiRangeMode::Plus;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPartM
//////////////////////////////////////////////////////////////////////

// @brief 範囲指定のモードを返す．
VpiRangeMode
CptPartM::mode() const
{
  return VpiRangeMode::Minus;
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
  PtExprArray&& expr_array
)
{
  auto p = mAlloc.get_memory(sizeof(CptConcat));
  return new (p) CptConcat(file_region, std::move(expr_array));
}

// multiple concatenation を生成する．
PtExpr*
PtFactory::new_MultiConcat(
  const FileRegion& file_region,
  const AstExpr* rep,
  PtExprArray&& expr_array
)
{
  auto p = mAlloc.get_memory(sizeof(CptMultiConcat));
  return new (p) CptMultiConcat(file_region, rep, std::move(expr_array));
}

// min-typ-max を生成する．
PtExpr*
PtFactory::new_MinTypMax(
  const FileRegion& file_region,
  const AstExpr* val0,
  const AstExpr* val1,
  const AstExpr* val2
)
{
  // 実は file_region は不要
  auto p = mAlloc.get_memory(sizeof(CptMinTypMax));
  return new (p) CptMinTypMax(val0, val1, val2);
}

// function call を生成する．
PtExpr*
PtFactory::new_FuncCall(
  const FileRegion& file_region,
  const char* name,
  PtExprArray&& arg_array
)
{
  auto p = mAlloc.get_memory(sizeof(CptFuncCall));
  return new (p) CptFuncCall(file_region, name, std::move(arg_array));
}

// function call を生成する．
PtExpr*
PtFactory::new_FuncCall(
  const FileRegion& file_region,
  PtHierName* hname,
  PtExprArray&& arg_array
)
{
  auto p = mAlloc.get_memory(sizeof(CptFuncCallH));
  auto tail_name = hname->tail_name();
  return new (p) CptFuncCallH(file_region,
			      hname->name_branch_to_array(mAlloc),
			      tail_name,
			      std::move(arg_array));
}

// system function call を生成する．
PtExpr*
PtFactory::new_SysFuncCall(
  const FileRegion& file_region,
  const char* name,
  PtExprArray&& arg_array
)
{
  auto p = mAlloc.get_memory(sizeof(CptSysFuncCall));
  return new CptSysFuncCall(file_region, name, std::move(arg_array));
}

// 定数を生成する．
PtExpr*
PtFactory::new_IntConst(
  const FileRegion& file_region,
  SizeType value
)
{
  auto p = mAlloc.get_memory(sizeof(CptIntConstant1));
  return new (p) CptIntConstant1(file_region, value);
}

// 定数を生成する．
PtExpr*
PtFactory::new_IntConst(
  const FileRegion& file_region,
  const char* value
)
{
  auto p = mAlloc.get_memory(sizeof(CptIntConstant2));
  return new (p) CptIntConstant2(file_region, VpiConstType::Int, value);
}

// 定数を生成する．
PtExpr*
PtFactory::new_IntConst(
  const FileRegion& file_region,
  VpiConstType const_type,
  const char* value
)
{
  auto p = mAlloc.get_memory(sizeof(CptIntConstant2));
  return new (p) CptIntConstant2(file_region, const_type, value);
}

// 定数を生成する．
PtExpr*
PtFactory::new_IntConst(
  const FileRegion& file_region,
  SizeType size,
  VpiConstType const_type,
  const char* value
)
{
  auto p = mAlloc.get_memory(sizeof(CptIntConstant3));
  return new (p) CptIntConstant3(file_region, size, const_type, value);
}

// 定数を生成する．
PtExpr*
PtFactory::new_RealConst(
  const FileRegion& file_region,
  double value
)
{
  auto p = mAlloc.get_memory(sizeof(CptRealConstant));
  return new (p) CptRealConstant(file_region, value);
}

// 定数を生成する．
PtExpr*
PtFactory::new_StringConst(
  const FileRegion& file_region,
  const char* value
)
{
  auto p = mAlloc.get_memory(sizeof(CptStringConstant));
  return new (p) CptStringConstant(file_region, value);
}

// @brief 範囲指定の生成
PtPart*
PtFactory::new_Part(
  const FileRegion& fr,
  VpiRangeMode mode,
  const AstExpr* expr1,
  const AstExpr* expr2
)
{
  if ( mode == VpiRangeMode::Const ) {
    auto p = mAlloc.get_memory(sizeof(CptPartC));
    return new (p) CptPartC(fr, expr1, expr2);
  }
  if ( mode == VpiRangeMode::Plus ) {
    auto p = mAlloc.get_memory(sizeof(CptPartP));
    return new (p) CptPartP(fr, expr1, expr2);
  }
  if ( mode == VpiRangeMode::Minus ) {
    auto p = mAlloc.get_memory(sizeof(CptPartM));
    return new (p) CptPartM(fr, expr1, expr2);
  }
  throw std::logic_error{"Should not be reached"};
}

END_NAMESPACE_YM_VERILOG
