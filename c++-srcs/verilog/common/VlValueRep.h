#ifndef VLVALUEREP_H
#define VLVALUEREP_H

/// @file VlValueRep.h
/// @brief VlValueRep (とその派生クラス)のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/VlValue.h"
#include "ym/vl/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlValueRep VlValue.h "ym/VlValue.h"
/// @brief VlValue の実体を表す仮想クラス
///
/// ここではインターフェイスの定義のみを行っている．
//////////////////////////////////////////////////////////////////////
class VlValueRep
{
public:

  VlValueRep() = default;

  virtual
  ~VlValueRep() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlValueRep の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 整数型の時に true を返す．
  virtual
  bool
  is_int() const;

  /// @brief 符号なし整数型の時に true 返す．
  virtual
  bool
  is_uint() const;

  /// @brief 実数型の時に true を返す．
  virtual
  bool
  is_real() const;

  /// @brief スカラー型の時に true を返す．
  virtual
  bool
  is_scalar() const;

  /// @brief time 型の時に true を返す．
  virtual
  bool
  is_time() const;

  /// @brief ビットベクタ型の時に true を返す．
  virtual
  bool
  is_bitvector() const;

  /// @brief エラー状態の時に true を返す．
  virtual
  bool
  is_error() const;

  /// @brief 整数型に変換可能な時に true を返す．
  virtual
  bool
  is_int_compat() const = 0;

  /// @brief SizeType 型に変換可能な時に true を返す．
  virtual
  bool
  is_uint_compat() const = 0;

  /// @brief 実数型に変換可能な時に true を返す．
  virtual
  bool
  is_real_compat() const = 0;

  /// @brief time 型に変換可能な時に true を返す．
  virtual
  bool
  is_time_compat() const = 0;

  /// @brief ビットベクタ型に変換可能な時に true を返す．
  virtual
  bool
  is_bitvector_compat() const = 0;

  /// @brief 符号付きの型の時に true を返す．
  virtual
  bool
  is_signed() const = 0;

  /// @brief ビットサイズを返す．
  virtual
  SizeType
  bit_size() const = 0;

  /// @brief 値の型を返す．
  virtual
  VlValueType
  value_type() const = 0;

  /// @brief 整数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  virtual
  int
  int_value() const = 0;

  /// @brief SizeType 型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  virtual
  SizeType
  uint_value() const = 0;

  /// @brief スカラー型の値を返す．
  virtual
  VlScalarVal
  scalar_value() const = 0;

  /// @brief 論理型の値を返す．
  virtual
  VlScalarVal
  logic_value() const = 0;

  /// @brief 実数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  virtual
  double
  real_value() const = 0;

  /// @brief time 型の値を返す．
  virtual
  VlTime
  time_value() const = 0;

  /// @brief ビットベクタ型の値を返す．
  virtual
  BitVector
  bitvector_value(
    const VlValueType& req_type ///< [in] 要求されるデータの型
  ) const = 0;

  /// @brief 自身の複製を返す．
  virtual
  VlValueRep*
  duplicate() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VlValueError VlValueRep.h "VlValueRep.h"
/// @brief エラーを表す VlValueRep の派生クラス
//////////////////////////////////////////////////////////////////////
class VlValueError :
  public VlValueRep
{
public:

  /// @brief コンストラクタ
  VlValueError() = default;

  /// @brief デストラクタ
  ~VlValueError() = default;


private:
  //////////////////////////////////////////////////////////////////////
  // VlValueRep の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief エラー状態の時に true を返す．
  bool
  is_error() const override;

  /// @brief 整数型に変換可能な時に true を返す．
  bool
  is_int_compat() const override;

  /// @brief SizeType 型に変換可能な時に true を返す．
  bool
  is_uint_compat() const override;

  /// @brief 実数型に変換可能な時に true を返す．
  bool
  is_real_compat() const override;

  /// @brief time 型に変換可能な時に true を返す．
  bool
  is_time_compat() const override;

  /// @brief ビットベクタ型に変換可能な時に true を返す．
  bool
  is_bitvector_compat() const override;

  /// @brief 符号付きの型の時に true を返す．
  bool
  is_signed() const override;

  /// @brief ビットサイズを返す．
  SizeType
  bit_size() const override;

  /// @brief 値の型を返す．
  VlValueType
  value_type() const override;

  /// @brief 整数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  int
  int_value() const override;

  /// @brief SizeType 型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  SizeType
  uint_value() const override;

  /// @brief スカラー型の値を返す．
  VlScalarVal
  scalar_value() const override;

  /// @brief 論理型の値を返す．
  VlScalarVal
  logic_value() const override;

  /// @brief 実数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  double
  real_value() const override;

  /// @brief time 型の値を返す．
  VlTime
  time_value() const override;

  /// @brief ビットベクタ型の値を返す．
  BitVector
  bitvector_value(
    const VlValueType& req_type ///< [in] 要求されるデータの型
  ) const override;

  /// @brief 自身の複製を返す．
  VlValueRep*
  duplicate() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class VlValueInt VlValueRep.h "VlValueRep.h"
/// @brief 整数型を表す VlValueRep の派生クラス
//////////////////////////////////////////////////////////////////////
class VlValueInt :
  public VlValueRep
{
public:

  /// @brief コンストラクタ
  VlValueInt(
    int val ///< [in] 値
  );

  /// @brief デストラクタ
  ~VlValueInt() = default;


private:
  //////////////////////////////////////////////////////////////////////
  // VlValueRep の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 整数型の時に true を返す．
  bool
  is_int() const override;

  /// @brief 整数型に変換可能な時に true を返す．
  bool
  is_int_compat() const override;

  /// @brief SizeType 型に変換可能な時に true を返す．
  bool
  is_uint_compat() const override;

  /// @brief 実数型に変換可能な時に true を返す．
  bool
  is_real_compat() const override;

  /// @brief time 型に変換可能な時に true を返す．
  bool
  is_time_compat() const override;

  /// @brief ビットベクタ型に変換可能な時に true を返す．
  bool
  is_bitvector_compat() const override;

  /// @brief 符号付きの型の時に true を返す．
  bool
  is_signed() const override;

  /// @brief ビットサイズを返す．
  SizeType
  bit_size() const override;

  /// @brief 値の型を返す．
  VlValueType
  value_type() const override;

  /// @brief 整数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  int
  int_value() const override;

  /// @brief SizeType 型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  SizeType
  uint_value() const override;

  /// @brief スカラー型の値を返す．
  VlScalarVal
  scalar_value() const override;

  /// @brief 論理型の値を返す．
  VlScalarVal
  logic_value() const override;

  /// @brief 実数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  double
  real_value() const override;

  /// @brief time 型の値を返す．
  VlTime
  time_value() const override;

  /// @brief ビットベクタ型の値を返す．
  BitVector
  bitvector_value(
    const VlValueType& req_type ///< [in] 要求されるデータの型
  ) const override;

  /// @brief 自身の複製を返す．
  VlValueRep*
  duplicate() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  int mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class VlValueUint VlValueRep.h "VlValueRep.h"
/// @brief 符号なし整数型を表す VlValueRep の派生クラス
//////////////////////////////////////////////////////////////////////
class VlValueUint :
  public VlValueRep
{
public:

  /// @brief コンストラクタ
  VlValueUint(
    SizeType val ///< [in] 値
  );

  /// @brief デストラクタ
  ~VlValueUint() = default;


private:
  //////////////////////////////////////////////////////////////////////
  // VlValueRep の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号なし整数型の時に true 返す．
  bool
  is_uint() const override;

  /// @brief 整数型に変換可能な時に true を返す．
  bool
  is_int_compat() const override;

  /// @brief SizeType 型に変換可能な時に true を返す．
  bool
  is_uint_compat() const override;

  /// @brief 実数型に変換可能な時に true を返す．
  bool
  is_real_compat() const override;

  /// @brief time 型に変換可能な時に true を返す．
  bool
  is_time_compat() const override;

  /// @brief ビットベクタ型に変換可能な時に true を返す．
  bool
  is_bitvector_compat() const override;

  /// @brief 符号付きの型の時に true を返す．
  bool
  is_signed() const override;

  /// @brief ビットサイズを返す．
  SizeType
  bit_size() const override;

  /// @brief 値の型を返す．
  VlValueType
  value_type() const override;

  /// @brief 整数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  int
  int_value() const override;

  /// @brief SizeType 型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  SizeType
  uint_value() const override;

  /// @brief スカラー型の値を返す．
  VlScalarVal
  scalar_value() const override;

  /// @brief 論理型の値を返す．
  VlScalarVal
  logic_value() const override;

  /// @brief 実数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  double
  real_value() const override;

  /// @brief time 型の値を返す．
  VlTime
  time_value() const override;

  /// @brief ビットベクタ型の値を返す．
  BitVector
  bitvector_value(
    const VlValueType& req_type ///< [in] 要求されるデータの型
  ) const override;

  /// @brief 自身の複製を返す．
  VlValueRep*
  duplicate() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  SizeType mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class VlValueScalar VlValueRep.h "VlValueRep.h"
/// @brief スカラー型を表す VlValueRep の派生クラス
//////////////////////////////////////////////////////////////////////
class VlValueScalar :
  public VlValueRep
{
public:

  /// @brief コンストラクタ
  VlValueScalar(
    const VlScalarVal& val ///< [in] 値
  );

  /// @brief デストラクタ
  ~VlValueScalar() = default;


private:
  //////////////////////////////////////////////////////////////////////
  // VlValueRep の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief スカラー型の時に true を返す．
  bool
  is_scalar() const override;

  /// @brief 整数型に変換可能な時に true を返す．
  bool
  is_int_compat() const override;

  /// @brief SizeType 型に変換可能な時に true を返す．
  bool
  is_uint_compat() const override;

  /// @brief 実数型に変換可能な時に true を返す．
  bool
  is_real_compat() const override;

  /// @brief time 型に変換可能な時に true を返す．
  bool
  is_time_compat() const override;

  /// @brief ビットベクタ型に変換可能な時に true を返す．
  bool
  is_bitvector_compat() const override;

  /// @brief 符号付きの型の時に true を返す．
  bool
  is_signed() const override;

  /// @brief ビットサイズを返す．
  SizeType
  bit_size() const override;

  /// @brief 値の型を返す．
  VlValueType
  value_type() const override;

  /// @brief 整数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  int
  int_value() const override;

  /// @brief SizeType 型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  SizeType
  uint_value() const override;

  /// @brief スカラー型の値を返す．
  VlScalarVal
  scalar_value() const override;

  /// @brief 論理型の値を返す．
  VlScalarVal
  logic_value() const override;

  /// @brief 実数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  double
  real_value() const override;

  /// @brief time 型の値を返す．
  VlTime
  time_value() const override;

  /// @brief ビットベクタ型の値を返す．
  BitVector
  bitvector_value(
    const VlValueType& req_type ///< [in] 要求されるデータの型
  ) const override;

  /// @brief 自身の複製を返す．
  VlValueRep*
  duplicate() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  VlScalarVal mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class VlValueReal VlValueRep.h "VlValueRep.h"
/// @brief 実数型を表す VlValueRep の派生クラス
//////////////////////////////////////////////////////////////////////
class VlValueReal :
  public VlValueRep
{
public:

  /// @brief コンストラクタ
  VlValueReal(
    double val ///< [in] 値
  );

  /// @brief デストラクタ
  ~VlValueReal() = default;


private:
  //////////////////////////////////////////////////////////////////////
  // VlValueRep の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 実数型の時に true を返す．
  bool
  is_real() const override;

  /// @brief 整数型に変換可能な時に true を返す．
  bool
  is_int_compat() const override;

  /// @brief SizeType 型に変換可能な時に true を返す．
  bool
  is_uint_compat() const override;

  /// @brief 実数型に変換可能な時に true を返す．
  bool
  is_real_compat() const override;

  /// @brief time 型に変換可能な時に true を返す．
  bool
  is_time_compat() const override;

  /// @brief ビットベクタ型に変換可能な時に true を返す．
  bool
  is_bitvector_compat() const override;

  /// @brief 符号付きの型の時に true を返す．
  bool
  is_signed() const override;

  /// @brief ビットサイズを返す．
  SizeType
  bit_size() const override;

  /// @brief 値の型を返す．
  VlValueType
  value_type() const override;

  /// @brief 整数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  int
  int_value() const override;

  /// @brief SizeType 型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  SizeType
  uint_value() const override;

  /// @brief スカラー型の値を返す．
  VlScalarVal
  scalar_value() const override;

  /// @brief 論理型の値を返す．
  VlScalarVal
  logic_value() const override;

  /// @brief 実数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  double
  real_value() const override;

  /// @brief time 型の値を返す．
  VlTime
  time_value() const override;

  /// @brief ビットベクタ型の値を返す．
  BitVector
  bitvector_value(
    const VlValueType& req_type ///< [in] 要求されるデータの型
  ) const override;

  /// @brief 自身の複製を返す．
  VlValueRep*
  duplicate() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  double mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class VlValueTime VlValueRep.h "VlValueRep.h"
/// @brief time 型を表す VlValueRep の派生クラス
//////////////////////////////////////////////////////////////////////
class VlValueTime :
  public VlValueRep
{
public:

  /// @brief コンストラクタ
  VlValueTime(
    VlTime val ///< [in] 値
  );

  /// @brief デストラクタ
  ~VlValueTime() = default;


private:
  //////////////////////////////////////////////////////////////////////
  // VlValueRep の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief time 型の時に true を返す．
  bool
  is_time() const override;

  /// @brief 整数型に変換可能な時に true を返す．
  bool
  is_int_compat() const override;

  /// @brief SizeType 型に変換可能な時に true を返す．
  bool
  is_uint_compat() const override;

  /// @brief 実数型に変換可能な時に true を返す．
  bool
  is_real_compat() const override;

  /// @brief time 型に変換可能な時に true を返す．
  bool
  is_time_compat() const override;

  /// @brief ビットベクタ型に変換可能な時に true を返す．
  bool
  is_bitvector_compat() const override;

  /// @brief 符号付きの型の時に true を返す．
  bool
  is_signed() const override;

  /// @brief ビットサイズを返す．
  SizeType
  bit_size() const override;

  /// @brief 値の型を返す．
  VlValueType
  value_type() const override;

  /// @brief 整数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  int
  int_value() const override;

  /// @brief SizeType 型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  SizeType
  uint_value() const override;

  /// @brief スカラー型の値を返す．
  VlScalarVal
  scalar_value() const override;

  /// @brief 論理型の値を返す．
  VlScalarVal
  logic_value() const override;

  /// @brief 実数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  double
  real_value() const override;

  /// @brief time 型の値を返す．
  VlTime
  time_value() const override;

  /// @brief ビットベクタ型の値を返す．
  BitVector
  bitvector_value(
    const VlValueType& req_type ///< [in] 要求されるデータの型
  ) const override;

  /// @brief 自身の複製を返す．
  VlValueRep*
  duplicate() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  VlTime mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class VlValueBitVector VlValueRep.h "VlValueRep.h"
/// @brief ビットベクタ型を表す VlValueRep の派生クラス
//////////////////////////////////////////////////////////////////////
class VlValueBitVector :
  public VlValueRep
{
public:

  /// @brief コンストラクタ
  VlValueBitVector(
    const BitVector& val ///< [in] 値
  );

  /// @brief デストラクタ
  ~VlValueBitVector() = default;


private:
  //////////////////////////////////////////////////////////////////////
  // VlValueRep の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ビットベクタ型の時に true を返す．
  bool
  is_bitvector() const override;

  /// @brief 整数型に変換可能な時に true を返す．
  bool
  is_int_compat() const override;

  /// @brief SizeType 型に変換可能な時に true を返す．
  bool
  is_uint_compat() const override;

  /// @brief 実数型に変換可能な時に true を返す．
  bool
  is_real_compat() const override;

  /// @brief time 型に変換可能な時に true を返す．
  bool
  is_time_compat() const override;

  /// @brief ビットベクタ型に変換可能な時に true を返す．
  bool
  is_bitvector_compat() const override;

  /// @brief 符号付きの型の時に true を返す．
  bool
  is_signed() const override;

  /// @brief ビットサイズを返す．
  SizeType
  bit_size() const override;

  /// @brief 値の型を返す．
  VlValueType
  value_type() const override;

  /// @brief 整数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  int
  int_value() const override;

  /// @brief SizeType 型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  SizeType
  uint_value() const override;

  /// @brief スカラー型の値を返す．
  VlScalarVal
  scalar_value() const override;

  /// @brief 論理型の値を返す．
  VlScalarVal
  logic_value() const override;

  /// @brief 実数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  double
  real_value() const override;

  /// @brief time 型の値を返す．
  VlTime
  time_value() const override;

  /// @brief ビットベクタ型の値を返す．
  BitVector
  bitvector_value(
    const VlValueType& req_type ///< [in] 要求されるデータの型
  ) const override;

  /// @brief 自身の複製を返す．
  VlValueRep*
  duplicate() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  BitVector mVal;

};

END_NAMESPACE_YM_VERILOG

#endif // VLVALUEREP_H
