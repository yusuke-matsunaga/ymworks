
/// @file VlValueRep.cc
/// @brief VlValueRep の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "VlValueRep.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス VlValueRep
//////////////////////////////////////////////////////////////////////

// @brief 整数型の時に true を返す．
bool
VlValueRep::is_int() const
{
  return false;
}

// @brief 符号なし整数型の時に true 返す．
bool
VlValueRep::is_uint() const
{
  return false;
}

// @brief 実数型の時に true を返す．
bool
VlValueRep::is_real() const
{
  return false;
}

// @brief スカラー型の時に true を返す．
bool
VlValueRep::is_scalar() const
{
  return false;
}

// @brief time 型の時に true を返す．
bool
VlValueRep::is_time() const
{
  return false;
}

// @brief ビットベクタ型の時に true を返す．
bool
VlValueRep::is_bitvector() const
{
  return false;
}

// @brief エラー状態の時に true を返す．
bool
VlValueRep::is_error() const
{
  return false;
}


//////////////////////////////////////////////////////////////////////
// クラス VlValueError
//////////////////////////////////////////////////////////////////////

// @brief エラー状態の時に true を返す．
bool
VlValueError::is_error() const
{
  return true;
}

// @brief 整数型に変換可能な時に true を返す．
bool
VlValueError::is_int_compat() const
{
  return false;
}

// @brief SizeType 型に変換可能な時に true を返す．
bool
VlValueError::is_uint_compat() const
{
  return false;
}

// @brief 実数型に変換可能な時に true を返す．
bool
VlValueError::is_real_compat() const
{
  return false;
}

// @brief time 型に変換可能な時に true を返す．
bool
VlValueError::is_time_compat() const
{
  return false;
}

// @brief ビットベクタ型に変換可能な時に true を返す．
bool
VlValueError::is_bitvector_compat() const
{
  return false;
}

// @brief 符号付きの型の時に true を返す．
bool
VlValueError::is_signed() const
{
  return false;
}

// @brief ビットサイズを返す．
SizeType
VlValueError::bit_size() const
{
  return 0;
}

// @brief 値の型を返す．
VlValueType
VlValueError::value_type() const
{
  return VlValueType();
}

// @brief 整数型の値を返す．
int
VlValueError::int_value() const
{
  return 0;
}

// @brief SizeType 型の値を返す．
SizeType
VlValueError::uint_value() const
{
  return 0;
}

// @brief スカラー型の値を返す．
VlScalarVal
VlValueError::scalar_value() const
{
  return VlScalarVal::x();
}

// @brief 論理型の値を返す．
VlScalarVal
VlValueError::logic_value() const
{
  return VlScalarVal::x();
}

// @brief 実数型の値を返す．
double
VlValueError::real_value() const
{
  return 0.0;
}

// @brief time 型の値を返す．
VlTime
VlValueError::time_value() const
{
  return VlTime();
}

// @brief ビットベクタ型の値を返す．
BitVector
VlValueError::bitvector_value(
  const VlValueType& req
) const
{
  return BitVector();
}

// @brief 自身の複製を返す．
VlValueRep*
VlValueError::duplicate() const
{
  return new VlValueError;
}


//////////////////////////////////////////////////////////////////////
// クラス VlValueInt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VlValueInt::VlValueInt(
  int val
) : mVal{val}
{
}

// @brief 整数型の時に true を返す．
bool
VlValueInt::is_int() const
{
  return true;
}

// @brief 整数型に変換可能な時に true を返す．
bool
VlValueInt::is_int_compat() const
{
  return true;
}

// @brief SizeType 型に変換可能な時に true を返す．
bool
VlValueInt::is_uint_compat() const
{
  return true;
}

// @brief 実数型に変換可能な時に true を返す．
bool
VlValueInt::is_real_compat() const
{
  return true;
}

// @brief time 型に変換可能な時に true を返す．
bool
VlValueInt::is_time_compat() const
{
  return true;
}

// @brief ビットベクタ型に変換可能な時に true を返す．
bool
VlValueInt::is_bitvector_compat() const
{
  return true;
}

// @brief 符号付きの型の時に true を返す．
bool
VlValueInt::is_signed() const
{
  return true;
}

// @brief ビットサイズを返す．
SizeType
VlValueInt::bit_size() const
{
  return kVpiSizeInteger;
}

// @brief 値の型を返す．
VlValueType
VlValueInt::value_type() const
{
  return VlValueType::int_type();
}

// @brief 整数型の値を返す．
int
VlValueInt::int_value() const
{
  return mVal;
}

// @brief SizeType 型の値を返す．
SizeType
VlValueInt::uint_value() const
{
  return static_cast<SizeType>(mVal);
}

// @brief スカラー型の値を返す．
VlScalarVal
VlValueInt::scalar_value() const
{
  return VlScalarVal(mVal);
}

// @brief 論理型の値を返す．
VlScalarVal
VlValueInt::logic_value() const
{
  if ( mVal != 0 ) {
    return VlScalarVal::one();
  }
  return VlScalarVal::zero();
}

// @brief 実数型の値を返す．
double
VlValueInt::real_value() const
{
  return static_cast<double>(mVal);
}

// @brief time 型の値を返す．
VlTime
VlValueInt::time_value() const
{
  return VlTime(uint_value());
}

// @brief ビットベクタ型の値を返す．
BitVector
VlValueInt::bitvector_value(
  const VlValueType& req
) const
{
  return BitVector(mVal).coerce(req);
}

// @brief 自身の複製を返す．
VlValueRep*
VlValueInt::duplicate() const
{
  return new VlValueInt(mVal);
}


//////////////////////////////////////////////////////////////////////
// クラス VlValueUint
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VlValueUint::VlValueUint(
  SizeType val
) : mVal{val}
{
}

// @brief 符号なし整数型の時に true 返す．
bool
VlValueUint::is_uint() const
{
  return true;
}

// @brief 整数型に変換可能な時に true を返す．
bool
VlValueUint::is_int_compat() const
{
  return true;
}

// @brief SizeType 型に変換可能な時に true を返す．
bool
VlValueUint::is_uint_compat() const
{
  return true;
}

// @brief 実数型に変換可能な時に true を返す．
bool
VlValueUint::is_real_compat() const
{
  return true;
}

// @brief time 型に変換可能な時に true を返す．
bool
VlValueUint::is_time_compat() const
{
  return true;
}

// @brief ビットベクタ型に変換可能な時に true を返す．
bool
VlValueUint::is_bitvector_compat() const
{
  return true;
}

// @brief 符号付きの型の時に true を返す．
bool
VlValueUint::is_signed() const
{
  return false;
}

// @brief ビットサイズを返す．
SizeType
VlValueUint::bit_size() const
{
  return kVpiSizeInteger;
}

// @brief 値の型を返す．
VlValueType
VlValueUint::value_type() const
{
  return VlValueType(false, true, kVpiSizeInteger);
}

// @brief 整数型の値を返す．
int
VlValueUint::int_value() const
{
  return static_cast<int>(mVal);
}

// @brief SizeType 型の値を返す．
SizeType
VlValueUint::uint_value() const
{
  return mVal;
}

// @brief スカラー型の値を返す．
VlScalarVal
VlValueUint::scalar_value() const
{
  return VlScalarVal(mVal);
}

// @brief 論理型の値を返す．
VlScalarVal
VlValueUint::logic_value() const
{
  if ( mVal != 0 ) {
    return VlScalarVal::one();
  }
  return VlScalarVal::zero();
}

// @brief 実数型の値を返す．
double
VlValueUint::real_value() const
{
  return static_cast<double>(mVal);
}

// @brief time 型の値を返す．
VlTime
VlValueUint::time_value() const
{
  return VlTime(mVal);
}

// @brief ビットベクタ型の値を返す．
BitVector
VlValueUint::bitvector_value(
  const VlValueType& req
) const
{
  return BitVector(mVal).coerce(req);
}

// @brief 自身の複製を返す．
VlValueRep*
VlValueUint::duplicate() const
{
  return new VlValueUint(mVal);
}


//////////////////////////////////////////////////////////////////////
// クラス VlValueScalar
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VlValueScalar::VlValueScalar(
  const VlScalarVal& val
) : mVal{val}
{
}

// @brief スカラー型の時に true を返す．
bool
VlValueScalar::is_scalar() const
{
  return true;
}

// @brief 整数型に変換可能な時に true を返す．
bool
VlValueScalar::is_int_compat() const
{
  return !mVal.is_xz();
}

// @brief SizeType 型に変換可能な時に true を返す．
bool
VlValueScalar::is_uint_compat() const
{
  return is_int_compat();
}

// @brief 実数型に変換可能な時に true を返す．
bool
VlValueScalar::is_real_compat() const
{
  return is_int_compat();
}

// @brief time 型に変換可能な時に true を返す．
bool
VlValueScalar::is_time_compat() const
{
  return is_int_compat();
}

// @brief ビットベクタ型に変換可能な時に true を返す．
bool
VlValueScalar::is_bitvector_compat() const
{
  return true;
}

// @brief 符号付きの型の時に true を返す．
bool
VlValueScalar::is_signed() const
{
  return false;
}

// @brief ビットサイズを返す．
SizeType
VlValueScalar::bit_size() const
{
  return 1;
}

// @brief 値の型を返す．
VlValueType
VlValueScalar::value_type() const
{
  return VlValueType(false, true, 1);
}

// @brief 整数型の値を返す．
int
VlValueScalar::int_value() const
{
  return mVal.to_int();
}

// @brief SizeType 型の値を返す．
SizeType
VlValueScalar::uint_value() const
{
  return static_cast<SizeType>(int_value());
}

// @brief スカラー型の値を返す．
VlScalarVal
VlValueScalar::scalar_value() const
{
  return mVal;
}

// @brief 論理型の値を返す．
VlScalarVal
VlValueScalar::logic_value() const
{
  if ( mVal.is_z() ) {
    return VlScalarVal::x();
  }
  return mVal;
}

// @brief 実数型の値を返す．
double
VlValueScalar::real_value() const
{
  return static_cast<double>(int_value());
}

// @brief time 型の値を返す．
VlTime
VlValueScalar::time_value() const
{
  return VlTime(uint_value());
}

// @brief ビットベクタ型の値を返す．
BitVector
VlValueScalar::bitvector_value(
  const VlValueType& req
) const
{
  return BitVector(mVal).coerce(req);
}

// @brief 自身の複製を返す．
VlValueRep*
VlValueScalar::duplicate() const
{
  return new VlValueScalar(mVal);
}


//////////////////////////////////////////////////////////////////////
// クラス VlValueReal
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VlValueReal::VlValueReal(
  double val
) : mVal{val}
{
}

// @brief 実数型の時に true を返す．
bool
VlValueReal::is_real() const
{
  return true;
}

// @brief 整数型に変換可能な時に true を返す．
bool
VlValueReal::is_int_compat() const
{
  return true;
}

// @brief SizeType 型に変換可能な時に true を返す．
bool
VlValueReal::is_uint_compat() const
{
  return true;
}

// @brief 実数型に変換可能な時に true を返す．
bool
VlValueReal::is_real_compat() const
{
  return true;
}

// @brief time 型に変換可能な時に true を返す．
bool
VlValueReal::is_time_compat() const
{
  return true;
}

// @brief ビットベクタ型に変換可能な時に true を返す．
bool
VlValueReal::is_bitvector_compat() const
{
  return false;
}

// @brief 符号付きの型の時に true を返す．
bool
VlValueReal::is_signed() const
{
  return true;
}

// @brief ビットサイズを返す．
SizeType
VlValueReal::bit_size() const
{
  return kVpiSizeReal;
}

// @brief 値の型を返す．
VlValueType
VlValueReal::value_type() const
{
  return VlValueType::real_type();
}

// @brief 整数型の値を返す．
int
VlValueReal::int_value() const
{
  return static_cast<int>(mVal);
}

// @brief SizeType 型の値を返す．
SizeType
VlValueReal::uint_value() const
{
  return static_cast<SizeType>(mVal);
}

// @brief スカラー型の値を返す．
VlScalarVal
VlValueReal::scalar_value() const
{
  return VlScalarVal(mVal);
}

// @brief 論理型の値を返す．
VlScalarVal
VlValueReal::logic_value() const
{
  if ( mVal != 0.0 ) {
    return VlScalarVal::one();
  }
  return VlScalarVal::zero();
}

// @brief 実数型の値を返す．
double
VlValueReal::real_value() const
{
  return mVal;
}

// @brief time 型の値を返す．
VlTime
VlValueReal::time_value() const
{
  return VlTime(mVal);
}

// @brief ビットベクタ型の値を返す．
BitVector
VlValueReal::bitvector_value(
  const VlValueType& req
) const
{
  return BitVector{};
}

// @brief 自身の複製を返す．
VlValueRep*
VlValueReal::duplicate() const
{
  return new VlValueReal(mVal);
}


//////////////////////////////////////////////////////////////////////
// クラス VlValueTime
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VlValueTime::VlValueTime(
  VlTime val
) : mVal{val}
{
}

// @brief time 型の時に true を返す．
bool
VlValueTime::is_time() const
{
  return true;
}

// @brief 整数型に変換可能な時に true を返す．
bool
VlValueTime::is_int_compat() const
{
  return mVal.value() <= 0x7FFFFFFFUL;
}

// @brief SizeType 型に変換可能な時に true を返す．
bool
VlValueTime::is_uint_compat() const
{
  return mVal.value() <= 0xFFFFFFFFUL;
}

// @brief 実数型に変換可能な時に true を返す．
bool
VlValueTime::is_real_compat() const
{
  return true;
}

// @brief time 型に変換可能な時に true を返す．
bool
VlValueTime::is_time_compat() const
{
  return true;
}

// @brief ビットベクタ型に変換可能な時に true を返す．
bool
VlValueTime::is_bitvector_compat() const
{
  return true;
}

// @brief 符号付きの型の時に true を返す．
bool
VlValueTime::is_signed() const
{
  return false;
}

// @brief ビットサイズを返す．
SizeType
VlValueTime::bit_size() const
{
  return kVpiSizeTime;
}

// @brief 値の型を返す．
VlValueType
VlValueTime::value_type() const
{
  return VlValueType::time_type();
}

// @brief 整数型の値を返す．
int
VlValueTime::int_value() const
{
  return static_cast<int>(mVal.to_uint());
}

// @brief SizeType 型の値を返す．
SizeType
VlValueTime::uint_value() const
{
  return mVal.to_uint();
}

// @brief スカラー型の値を返す．
VlScalarVal
VlValueTime::scalar_value() const
{
  return VlScalarVal(static_cast<SizeType>(mVal.low()));
}

// @brief 論理型の値を返す．
VlScalarVal
VlValueTime::logic_value() const
{
  if ( mVal.value() != 0UL ) {
    return VlScalarVal::one();
  }
  return VlScalarVal::zero();
}

// @brief 実数型の値を返す．
double
VlValueTime::real_value() const
{
  return mVal.to_real();
}

// @brief time 型の値を返す．
VlTime
VlValueTime::time_value() const
{
  return mVal;
}

// @brief ビットベクタ型の値を返す．
BitVector
VlValueTime::bitvector_value(
  const VlValueType& req
) const
{
  return BitVector(mVal).coerce(req);
}

// @brief 自身の複製を返す．
VlValueRep*
VlValueTime::duplicate() const
{
  return new VlValueTime(mVal);
}


//////////////////////////////////////////////////////////////////////
// クラス VlValueBitVector
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VlValueBitVector::VlValueBitVector(
  const BitVector& val
) : mVal{val}
{
}

// @brief ビットベクタ型の時に true を返す．
bool
VlValueBitVector::is_bitvector() const
{
  return true;
}

// @brief 整数型に変換可能な時に true を返す．
bool
VlValueBitVector::is_int_compat() const
{
  return mVal.is_int();
}

// @brief SizeType 型に変換可能な時に true を返す．
bool
VlValueBitVector::is_uint_compat() const
{
  return mVal.is_uint32();
}

// @brief 実数型に変換可能な時に true を返す．
bool
VlValueBitVector::is_real_compat() const
{
  return true;
}

// @brief time 型に変換可能な時に true を返す．
bool
VlValueBitVector::is_time_compat() const
{
  return mVal.is_time();
}

// @brief ビットベクタ型に変換可能な時に true を返す．
bool
VlValueBitVector::is_bitvector_compat() const
{
  return true;
}

// @brief 符号付きの型の時に true を返す．
bool
VlValueBitVector::is_signed() const
{
  return mVal.is_signed();
}

// @brief ビットサイズを返す．
SizeType
VlValueBitVector::bit_size() const
{
  return mVal.size();
}

// @brief 値の型を返す．
VlValueType
VlValueBitVector::value_type() const
{
  return mVal.value_type();
}

// @brief 整数型の値を返す．
int
VlValueBitVector::int_value() const
{
  return mVal.to_int();
}

// @brief SizeType 型の値を返す．
SizeType
VlValueBitVector::uint_value() const
{
  return mVal.to_uint32();
}

// @brief スカラー型の値を返す．
VlScalarVal
VlValueBitVector::scalar_value() const
{
  return mVal.to_scalar();
}

// @brief 論理型の値を返す．
VlScalarVal
VlValueBitVector::logic_value() const
{
  return mVal.to_logic();
}

// @brief 実数型の値を返す．
double
VlValueBitVector::real_value() const
{
  return mVal.to_real();
}

// @brief time 型の値を返す．
VlTime
VlValueBitVector::time_value() const
{
  return mVal.to_time();
}

// @brief ビットベクタ型の値を返す．
BitVector
VlValueBitVector::bitvector_value(
  const VlValueType& req
) const
{
  return BitVector(mVal).coerce(req);
}

// @brief 自身の複製を返す．
VlValueRep*
VlValueBitVector::duplicate() const
{
  return new VlValueBitVector(mVal);
}

END_NAMESPACE_YM_VERILOG
