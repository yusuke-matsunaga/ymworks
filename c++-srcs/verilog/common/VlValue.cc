
/// @file VlValue.cc
/// @brief VlValue の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/VlValue.h"
#include "VlValueRep.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス VlValue
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
VlValue::VlValue() :
  mRep{new VlValueError{}}
{
}

// @brief コピーコンストラクタ
VlValue::VlValue(
  const VlValue& src
) : mRep{src.mRep}
{
}

// @brief ムーブコンストラクタ
VlValue::VlValue(
  VlValue&& src
) : mRep{std::move(src.mRep)}
{
}

// @brief 整数値からのコンストラクタ
VlValue::VlValue(
  int val
) : mRep{new VlValueInt{val}}
{
}

// @brief std::uint32_t からのコンストラクタ
VlValue::VlValue(
  std::uint32_t val
) : mRep{new VlValueUint{val}}
{
}

// @brief スカラー値からのコンストラクタ
VlValue::VlValue(
  const VlScalarVal& val
) : mRep{new VlValueScalar{val}}
{
}

// @brief time からのコンストラクタ
VlValue::VlValue(
  VlTime val
) : mRep{new VlValueTime{val}}
{
}

// @brief 実数からのコンストラクタ
VlValue::VlValue(
  double val
) : mRep{new VlValueReal(val)}
{
}

// @brief ビットベクタからのコンストラクタ
VlValue::VlValue(
  const BitVector& val
) : mRep{new VlValueBitVector{val}}
{
}

// @brief 型変換を伴うコンストラクタ
VlValue::VlValue(
  const VlValue& src,
  const VlValueType& value_type
)
{
  if ( value_type.is_int_type() ) {
    mRep = std::shared_ptr<VlValueRep>{new VlValueInt{src.int_value()}};
  }
  else if ( value_type.is_real_type() ) {
    mRep = std::shared_ptr<VlValueRep>{new VlValueReal{src.real_value()}};
  }
  else if ( value_type.is_time_type() ) {
    mRep = std::shared_ptr<VlValueRep>{new VlValueTime{src.time_value()}};
  }
  else if ( value_type.is_no_type() ) {
    mRep = src.mRep;
  }
  else if ( value_type.is_bitvector_type() ) {
    auto src_bv = src.bitvector_value();
    auto value_p = new VlValueBitVector{BitVector{src_bv,
						  value_type.size(),
						  value_type.is_sized(),
						  value_type.is_signed(),
						  src_bv.base()}};
    mRep = std::shared_ptr<VlValueRep>{value_p};
  }
  else {
    ASSERT_NOT_REACHED;
  }
}

// @brief 型変換を伴うムーブコンストラクタ
VlValue::VlValue(
  VlValue&& src,
  const VlValueType& value_type
)
{
  if ( this->value_type() == value_type || value_type.is_no_type() ) {
    // 型が同じ場合だけムーブを使う．
    mRep = std::move(src.mRep);
  }
  else {
    if ( value_type.is_int_type() ) {
      mRep = std::shared_ptr<VlValueRep>{new VlValueInt{src.int_value()}};
    }
    else if ( value_type.is_real_type() ) {
      mRep = std::shared_ptr<VlValueRep>{new VlValueReal{src.real_value()}};
    }
    else if ( value_type.is_time_type() ) {
      mRep = std::shared_ptr<VlValueRep>{new VlValueTime{src.time_value()}};
    }
    else if ( value_type.is_bitvector_type() ) {
      auto src_bv = src.bitvector_value();
      auto value_p = new VlValueBitVector{BitVector{src_bv,
						    value_type.size(),
						    value_type.is_sized(),
						    value_type.is_signed(),
						    src_bv.base()}};
      mRep = std::shared_ptr<VlValueRep>{value_p};
    }
    else {
      ASSERT_NOT_REACHED;
    }
  }
}

// @brief コピー代入演算子
VlValue&
VlValue::operator=(
  const VlValue& src
)
{
  mRep = src.mRep;
  return *this;
}

// @brief ムーブ代入演算子
VlValue&
VlValue::operator=(
  VlValue&& src
)
{
  mRep = std::move(src.mRep);
  return *this;
}

// @brief デストラクタ
VlValue::~VlValue()
{
}

// @brief 整数値を設定する．
void
VlValue::set(
  int val
)
{
  mRep = std::shared_ptr<VlValueRep>{new VlValueInt{val}};
}

// @brief unsigned int の値をセットする．
void
VlValue::set(
  unsigned int val
)
{
  mRep = std::shared_ptr<VlValueRep>{new VlValueUint{val}};
}

// @brief スカラー値をセットする．
void
VlValue::set(
  const VlScalarVal& val
)
{
  mRep = std::shared_ptr<VlValueRep>{new VlValueScalar{val}};
}

// @brief time の値をセットする．
void
VlValue::set(
  VlTime val
)
{
  mRep = std::shared_ptr<VlValueRep>{new VlValueTime{val}};
}

// @brief 実数値をセットする．
void
VlValue::set(
  double val
)
{
  mRep = std::shared_ptr<VlValueRep>{new VlValueReal{val}};
}

// @brief ビットベクタの値をセットする．
void
VlValue::set(
  const BitVector& val
)
{
  mRep = std::shared_ptr<VlValueRep>{new VlValueBitVector{val}};
}

// @brief 整数型の時に true を返す．
bool
VlValue::is_int() const
{
  return mRep->is_int();
}

// @brief 符号なし整数型の時に true 返す．
bool
VlValue::is_uint() const {
  return mRep->is_uint();
}

// @brief 実数型の時に true を返す．
bool
VlValue::is_real() const {
  return mRep->is_real();
}

// @brief スカラー型の時に true を返す．
bool
VlValue::is_scalar() const {
  return mRep->is_scalar();
}

// @brief time 型の時に true を返す．
bool
VlValue::is_time() const {
  return mRep->is_time();
}

// @brief ビットベクタ型の時に true を返す．
bool
VlValue::is_bitvector() const {
  return mRep->is_bitvector();
}

// @brief エラー状態の時に true を返す．
bool
VlValue::is_error() const {
  return mRep->is_error();
}

// @brief 整数型に変換可能な時に true を返す．
bool
VlValue::is_int_compat() const {
  return mRep->is_int_compat();
}

// @brief unsigned uint 型に変換可能な時に true を返す．
bool
VlValue::is_uint_compat() const {
  return mRep->is_uint_compat();
}

// @brief 実数型に変換可能な時に true を返す．
bool
VlValue::is_real_compat() const {
  return mRep->is_real_compat();
}

// @brief time 型に変換可能な時に true を返す．
bool
VlValue::is_time_compat() const {
  return mRep->is_time_compat();
}

// @brief ビットベクタ型に変換可能な時に true を返す．
bool
VlValue::is_bitvector_compat() const {
  return mRep->is_bitvector_compat();
}

// @brief 符号付きの型の時に true を返す．
bool
VlValue::is_signed() const
{
  return mRep->is_signed();
}

// @brief ビットサイズを返す．
SizeType
VlValue::bit_size() const
{
  return mRep->bit_size();
}

// @brief 値の型を返す．
VlValueType
VlValue::value_type() const
{
  return mRep->value_type();
}

// @brief 整数型の値を返す．
int
VlValue::int_value() const
{
  return mRep->int_value();
}

// @brief unsigned int 型の値を返す．
unsigned int
VlValue::uint_value() const
{
  return mRep->uint_value();
}

// @brief スカラー型の値を返す．
VlScalarVal
VlValue::scalar_value() const
{
  return mRep->scalar_value();
}

// @brief 論理型の値を返す．
VlScalarVal
VlValue::logic_value() const
{
  return mRep->logic_value();
}

// @brief 実数型の値を返す．
double
VlValue::real_value() const
{
  return mRep->real_value();
}

// @brief time 型の値を返す．
VlTime
VlValue::time_value() const
{
  return mRep->time_value();
}

// @brief ビットベクタ型の値を返す．
BitVector
VlValue::bitvector_value(
  const VlValueType& req_type
) const
{
  return mRep->bitvector_value(req_type);
}

// @relates VlValue
VlValue
operator-(
  const VlValue& src
)
{
  if ( src.is_int() ) {
    return VlValue{ - src.int_value() };
  }
  if ( src.is_uint() || src.is_scalar() ) {
    return VlValue{ static_cast<unsigned int>(- src.int_value()) };
  }
  if ( src.is_real() || src.is_bitvector() ) {
    return VlValue{ - src.real_value() };
  }
  if ( src.is_time() ) {
    return VlValue{ - src.bitvector_value() };
  }
  if ( src.is_error() ) {
    return src;
  }
  ASSERT_NOT_REACHED;
  return VlValue();
}

// @relates VlValue
VlValue
operator+(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( src1.is_uint() ) {
    if ( src2.is_uint() ) {
      return VlValue{ src1.uint_value() + src2.uint_value() };
    }
    if ( src2.is_int() ) {
      return VlValue{ src1.int_value() + src2.int_value() };
    }
  }
  if ( src1.is_int() ) {
    if ( src2.is_int() || src2.is_int() ) {
      return VlValue{ src1.int_value() + src2.int_value() };
    }
  }
  if ( src1.is_real() || src2.is_real() ) {
    return VlValue{ src1.real_value() + src2.real_value() };
  }
  if ( src1.is_time() && src2.is_time() ) {
    return VlValue{ src1.time_value() + src2.time_value() };
  }
  return VlValue{ src1.bitvector_value() + src2.bitvector_value() };
}

// @relates VlValue
VlValue
operator-(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( src1.is_uint() ) {
    if ( src2.is_uint() ) {
      return VlValue{ src1.uint_value() - src2.uint_value() };
    }
    if ( src2.is_int() ) {
      return VlValue{ src1.int_value() - src2.int_value() };
    }
  }
  if ( src1.is_int() ) {
    if ( src2.is_int() || src2.is_int() ) {
      return VlValue{ src1.int_value() - src2.int_value() };
    }
  }
  if ( src1.is_real() || src2.is_real() ) {
    return VlValue{ src1.real_value() - src2.real_value() };
  }
  return VlValue{ src1.bitvector_value() - src2.bitvector_value() };
}

// @relates VlValue
VlValue
operator*(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( src1.is_uint() ) {
    if ( src2.is_uint() ) {
      return VlValue{ src1.uint_value() * src2.uint_value() };
    }
    if ( src2.is_int() ) {
      return VlValue{ src1.int_value() * src2.int_value() };
    }
  }
  if ( src1.is_int() ) {
    if ( src2.is_int() || src2.is_int() ) {
      return VlValue{ src1.int_value() * src2.int_value() };
    }
  }
  if ( src1.is_real() || src2.is_real() ) {
    return VlValue{ src1.real_value() * src2.real_value() };
  }
  return VlValue{ src1.bitvector_value() * src2.bitvector_value() };
}

// @relates VlValue
VlValue
operator/(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( src1.is_uint() ) {
    if ( src2.is_uint() ) {
      return VlValue{ src1.uint_value() / src2.uint_value() };
    }
    if ( src2.is_int() ) {
      return VlValue{ src1.int_value() / src2.int_value() };
    }
  }
  if ( src1.is_int() ) {
    if ( src2.is_int() || src2.is_int() ) {
      return VlValue{ src1.int_value() / src2.int_value() };
    }
  }
  if ( src1.is_real() || src2.is_real() ) {
    return VlValue{ src1.real_value() / src2.real_value() };
  }
  return VlValue{ src1.bitvector_value() / src2.bitvector_value() };
}

// @relates VlValue
VlValue
operator%(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( src1.is_uint() ) {
    if ( src2.is_uint() ) {
      return VlValue{ src1.uint_value() % src2.uint_value() };
    }
    if ( src2.is_int() ) {
      return VlValue{ src1.int_value() % src2.int_value() };
    }
  }
  if ( src1.is_int() ) {
    if ( src2.is_int() || src2.is_int() ) {
      return VlValue{ src1.int_value() % src2.int_value() };
    }
  }
  if ( src1.is_real() || src2.is_real() ) {
    return VlValue{};
  }
  return VlValue{ src1.bitvector_value() % src2.bitvector_value() };
}

// @relates VlValue
VlValue
power(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( src1.is_real() || src1.is_int() ||
       src2.is_real() || src2.is_int() ) {
    double v1 = src1.real_value();
    double v2 = src2.real_value();
    if ( (v1 == 0.0 && v2 <= 0.0) || (v1 < 0.0 && rint(v2) != v2) ) {
      return VlValue{VlScalarVal::x()};
    }
    return VlValue{ pow(v1, v2) };
  }
  return VlValue{ power(src1.bitvector_value(), src2.bitvector_value()) };
}

// @relates VlValue
VlValue
lt(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( src1.is_int() ) {
    if ( src2.is_int() || src2.is_uint() ) {
      if ( src1.int_value() < src2.int_value() ) {
	return VlValue{VlScalarVal::one()};
      }
      else {
	return VlValue{VlScalarVal::zero()};
      }
    }
  }
  else if ( src1.is_uint() ) {
    if ( src2.is_int() ) {
      if ( src1.int_value() < src2.int_value() ) {
	return VlValue{VlScalarVal::one()};
      }
      else {
	return VlValue{VlScalarVal::zero()};
      }
    }
    else if ( src2.is_uint() ) {
      if ( src1.uint_value() < src2.uint_value() ) {
	return VlValue{VlScalarVal::one()};
      }
      else {
	return VlValue{VlScalarVal::zero()};
      }
    }
  }
  else if ( src1.is_real() || src2.is_real() ) {
    if ( src1.real_value() < src2.real_value() ) {
      return VlValue{VlScalarVal::one()};
    }
    else {
      return VlValue{VlScalarVal::zero()};
    }
  }
  return VlValue{ lt(src1.bitvector_value(), src2.bitvector_value()) };
}

// @relates VlValue
VlValue
gt(
  const VlValue& src1,
  const VlValue& src2
)
{
  return lt(src2, src1);
}

// @relates VlValue
VlValue
le(
  const VlValue& src1,
  const VlValue& src2
)
{
  return log_not(lt(src2, src1));
}

// @relates VlValue
VlValue
ge(
  const VlValue& src1,
  const VlValue& src2
)
{
  return log_not(lt(src1, src2));
}

// @relates VlValue
VlValue
eq(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( (src1.is_int() || src1.is_uint()) &&
       (src2.is_int() || src2.is_uint()) ) {
    if ( src1.int_value() == src2.int_value() ) {
      return VlValue{VlScalarVal::one()};
    }
    else {
      return VlValue{VlScalarVal::zero()};
    }
  }
  if ( src1.is_real() || src2.is_real() ) {
    if ( src1.real_value() == src2.real_value() ) {
      return VlValue{VlScalarVal::one()};
    }
    else {
      return VlValue{VlScalarVal::zero()};
    }
  }
  return VlValue{ eq(src1.bitvector_value(), src2.bitvector_value()) };
}

// @relates VlValue
VlValue
eq_with_x(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( (src1.is_int() || src1.is_uint()) &&
       (src2.is_int() || src2.is_uint()) ) {
    if ( src1.int_value() == src2.int_value() ) {
      return VlValue{VlScalarVal::one()};
    }
    else {
      return VlValue{VlScalarVal::zero()};
    }
  }
  if ( src1.is_real() || src2.is_real() ) {
    if ( src1.real_value() == src2.real_value() ) {
      return VlValue{VlScalarVal::one()};
    }
    else {
      return VlValue{VlScalarVal::zero()};
    }
  }
  return VlValue{ eq_with_x(src1.bitvector_value(), src2.bitvector_value()) };
}

// @relates VlValue
VlValue
eq_with_xz(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( (src1.is_int() || src1.is_uint()) &&
       (src2.is_int() || src2.is_uint()) ) {
    if ( src1.int_value() == src2.int_value() ) {
      return VlValue{VlScalarVal::one()};
    }
    else {
      return VlValue{VlScalarVal::zero()};
    }
  }
  if ( src1.is_real() || src2.is_real() ) {
    if ( src1.real_value() == src2.real_value() ) {
      return VlValue{VlScalarVal::one()};
    }
    else {
      return VlValue{VlScalarVal::zero()};
    }
  }
  return VlValue{ eq_with_xz(src1.bitvector_value(), src2.bitvector_value()) };
}

// @relates VlValue
VlValue
ne(
  const VlValue& src1,
  const VlValue& src2
)
{
  return log_not(eq(src1, src2));
}

// @relates VlValue
VlValue
log_not(
  const VlValue& src
)
{
  if ( src.is_error() ) {
    return VlValue{};
  }
  return VlValue{ !src.logic_value() };
}

// @relates VlValue
VlValue
log_and(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  return VlValue{ src1.logic_value() && src2.logic_value() };
}

// @relates VlValue
VlValue
log_or(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  return VlValue{ src1.logic_value() || src2.logic_value() };
}

// @relates VlValue
VlValue
bit_negate(
  const VlValue& src
)
{
  if ( src.is_error() ) {
    return VlValue{};
  }
  if ( !src.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ ~src.bitvector_value() };
}

// @relates VlValue
VlValue
bit_and(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( !src1.is_bitvector_compat() || !src2.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ src1.bitvector_value() & src2.bitvector_value() };
}

// @relates VlValue
VlValue
bit_or(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( !src1.is_bitvector_compat() || !src2.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ src1.bitvector_value() | src2.bitvector_value() };
}

// @relates VlValue
VlValue
bit_xor(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( !src1.is_bitvector_compat() || !src2.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ src1.bitvector_value() ^ src2.bitvector_value() };
}

// @relates VlValue
VlValue
bit_xnor(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( !src1.is_bitvector_compat() || !src2.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ ~(src1.bitvector_value() ^ src2.bitvector_value()) };
}

// @brief リダクションAND
VlValue
reduction_and(
  const VlValue& src
)
{
  if ( src.is_error() ) {
    return VlValue{};
  }
  if ( !src.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ src.bitvector_value().reduction_and() };
}

// @brief リダクションOR
VlValue
reduction_or(
  const VlValue& src
)
{
  if ( src.is_error() ) {
    return VlValue{};
  }
  if ( !src.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ src.bitvector_value().reduction_or() };
}

// @brief リダクションXOR
VlValue
reduction_xor(
  const VlValue& src
)
{
  if ( src.is_error() ) {
    return VlValue{};
  }
  if ( !src.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ src.bitvector_value().reduction_xor() };
}

// @brief リダクションNAND
VlValue
reduction_nand(
  const VlValue& src
)
{
  if ( src.is_error() ) {
    return VlValue{};
  }
  if ( !src.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ src.bitvector_value().reduction_nand() };
}

// @brief リダクションNOR
VlValue
reduction_nor(
  const VlValue& src
)
{
  if ( src.is_error() ) {
    return VlValue{};
  }
  if ( !src.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ src.bitvector_value().reduction_nor() };
}

// @brief リダクションXNOR
VlValue
reduction_xnor(
  const VlValue& src
)
{
  if ( src.is_error() ) {
    return VlValue{};
  }
  if ( !src.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ src.bitvector_value().reduction_xnor() };
}

// @relates VlValue
VlValue
operator<<(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( !src1.is_bitvector_compat() || !src2.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ src1.bitvector_value() << src2.bitvector_value() };
}

// @relates VlValue
VlValue
operator<<(
  const VlValue& src1,
  std::uint32_t src2
)
{
  if ( src1.is_error() ) {
    return VlValue{};
  }
  if ( !src1.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ src1.bitvector_value() << src2 };
}

// @relates VlValue
VlValue
operator>>(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( !src1.is_bitvector_compat() || !src2.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ src1.bitvector_value() >> src2.bitvector_value() };
}

// @relates VlValue
VlValue
operator>>(
  const VlValue& src1,
  std::uint32_t src2
)
{
  if ( src1.is_error() ) {
    return VlValue{};
  }
  if ( !src1.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ src1.bitvector_value() >> src2 };
}

// @relates VlValue
VlValue
alshift(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( !src1.is_bitvector_compat() || !src2.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ alshift(src1.bitvector_value(), src2.bitvector_value()) };
}

// @relates VlValue
VlValue
alshift(
  const VlValue& src1,
  std::uint32_t src2
)
{
  if ( src1.is_error() ) {
    return VlValue{};
  }
  if ( !src1.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ alshift(src1.bitvector_value(), src2) };
}

// @relates VlValue
VlValue
arshift(
  const VlValue& src1,
  const VlValue& src2
)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue{};
  }
  if ( !src1.is_bitvector_compat() || !src2.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ arshift(src1.bitvector_value(), src2.bitvector_value()) };
}

// @relates VlValue
VlValue
arshift(
  const VlValue& src1,
  std::uint32_t src2
)
{
  if ( src1.is_error() ) {
    return VlValue{};
  }
  if ( !src1.is_bitvector_compat() ) {
    return VlValue{};
  }
  return VlValue{ arshift(src1.bitvector_value(), src2) };
}

// @relates VlValue
VlValue
ite(
  const VlValue& src1,
  const VlValue& src2,
  const VlValue& src3
)
{
  if ( src1.logic_value().to_bool() ) {
    return src2;
  }
  else {
    return src3;
  }
}

// @relates VlValue
VlValue
ite(
  const VlScalarVal& src1,
  const VlValue& src2,
  const VlValue& src3
)
{
  if ( src1.to_bool() ) {
    return src2;
  }
  else {
    return src3;
  }
}

// @relates VlValue
VlValue
concat(
  const std::vector<VlValue>& src_list
)
{
  std::vector<BitVector> bv_array;
  bv_array.reserve(src_list.size());
  for ( const auto& v: src_list ) {
    if ( !v.is_bitvector_compat() ) {
      return VlValue{};
    }
    bv_array.push_back(v.bitvector_value());
  }
  return VlValue{ concat(bv_array) };
}

// @relates VlValue
VlValue
multi_concat(
  const std::vector<VlValue>& src_list
)
{
  std::vector<BitVector> bv_array;
  bv_array.reserve(src_list.size());
  auto p = src_list.begin();
  const auto& v0 = *p;
  if ( !v0.is_bitvector_compat() ) {
    return VlValue{};
  }
  for (++ p; p != src_list.end(); ++ p) {
    const auto& v = *p;
    if ( v.is_bitvector_compat() ) {
      return VlValue{};
    }
    bv_array.push_back(v.bitvector_value());
  }
  return VlValue{ multi_concat(v0.bitvector_value(), bv_array) };
}

END_NAMESPACE_YM_VERILOG
