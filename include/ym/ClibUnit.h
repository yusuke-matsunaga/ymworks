#ifndef CLIBUNIT_H
#define CLIBUNIT_H

/// @file ClibUnit.h
/// @brief ClibUnit のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
/// @class ClibUnitStr ClibUnit.h "ym/ClibUnit.h"
/// @brief 文字列で単位を表すクラス
/// @ingroup ClibGroup
//////////////////////////////////////////////////////////////////////
class ClibUnitStr
{
public:

  /// @brief コンストラクタ
  ClibUnitStr(
    std::string val = {} ///< [in] 単位を表す文字列
  ) : mVal{val}
  {
  }

  /// @brief デストラクタ
  ~ClibUnitStr() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を返す．
  const std::string&
  val() const
  {
    return mVal;
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const ClibUnitStr& right ///< [in] オペランド
  ) const
  {
    return mVal == right.mVal;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const ClibUnitStr& right ///< [in] オペランド
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  std::string mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class ClibTimeUnit ClibUnit.h "ym/ClibUnit.h"
/// @brief 時間単位を表すクラス
/// @ingroup ClibGroup
//////////////////////////////////////////////////////////////////////
class ClibTimeUnit :
  public ClibUnitStr
{
public:

  /// @brief コンストラクタ
  ClibTimeUnit(
    std::string val = {} ///< [in] 単位を表す文字列
  ) : ClibUnitStr(val)
  {
  }

  /// @brief デストラクタ
  ~ClibTimeUnit() = default;

};


//////////////////////////////////////////////////////////////////////
/// @class ClibVoltageUnit ClibUnit.h "ym/ClibUnit.h"
/// @brief 電圧単位を表すクラス
/// @ingroup ClibGroup
//////////////////////////////////////////////////////////////////////
class ClibVoltageUnit :
  public ClibUnitStr
{
public:

  /// @brief コンストラクタ
  ClibVoltageUnit(
    std::string val = {} ///< [in] 単位を表す文字列
  ) : ClibUnitStr(val)
  {
  }

  /// @brief デストラクタ
  ~ClibVoltageUnit() = default;

};


//////////////////////////////////////////////////////////////////////
/// @class ClibCurrentUnit ClibUnit.h "ym/ClibUnit.h"
/// @brief 電流単位を表すクラス
/// @ingroup ClibGroup
//////////////////////////////////////////////////////////////////////
class ClibCurrentUnit :
  public ClibUnitStr
{
public:

  /// @brief コンストラクタ
  ClibCurrentUnit(
    std::string val = {} ///< [in] 単位を表す文字列
  ) : ClibUnitStr(val)
  {
  }

  /// @brief デストラクタ
  ~ClibCurrentUnit() = default;

};


//////////////////////////////////////////////////////////////////////
/// @class ClibResistanceUnit ClibUnit.h "ym/ClibUnit.h"
/// @brief 抵抗単位を表すクラス
/// @ingroup ClibGroup
//////////////////////////////////////////////////////////////////////
class ClibResistanceUnit :
  public ClibUnitStr
{
public:

  /// @brief コンストラクタ
  ClibResistanceUnit(
    std::string val = {} ///< [in] 単位を表す文字列
  ) : ClibUnitStr(val)
  {
  }

  /// @brief デストラクタ
  ~ClibResistanceUnit() = default;

};


//////////////////////////////////////////////////////////////////////
/// @class ClibPowerUnit ClibUnit.h "ym/ClibUnit.h"
/// @brief 電力単位を表すクラス
/// @ingroup ClibGroup
//////////////////////////////////////////////////////////////////////
class ClibPowerUnit :
  public ClibUnitStr
{
public:

  /// @brief コンストラクタ
  ClibPowerUnit(
    std::string val = {} ///< [in] 単位を表す文字列
  ) : ClibUnitStr(val)
  {
  }

  /// @brief デストラクタ
  ~ClibPowerUnit() = default;

};


//////////////////////////////////////////////////////////////////////
/// @class ClibCapacitanceUnit ClibUnit.h "ym/ClibUnit.h"
/// @brief 容量単位を表すクラス
/// @ingroup ClibGroup
//////////////////////////////////////////////////////////////////////
class ClibCapacitanceUnit
{
public:

  /// @brief コンストラクタ
  ClibCapacitanceUnit(
    double float_val = 0.0,  ///< [in] 単位を表す数値
    std::string str_val = {} ///< [in] 単位を表す文字列
  ) : mFloatVal{float_val},
      mStrVal{str_val}
  {
  }

  /// @brief デストラクタ
  ~ClibCapacitanceUnit() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 数値を返す．
  double
  float_val() const
  {
    return mFloatVal;
  }

  /// @brief 文字列を返す．
  const std::string&
  str_val() const
  {
    return mStrVal;
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const ClibCapacitanceUnit& right ///< [in] オペランド
  ) const
  {
    return mFloatVal == right.mFloatVal && mStrVal == right.mStrVal;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 単位を表す数値
  double mFloatVal;

  // 単位を表す文字列
  std::string mStrVal;

};


/// @brief ストリーム出演算子
/// @relates ClibUnitStr
inline
std::ostream&
operator<<(
  std::ostream& s,        ///< [in] 出力ストリーム
  const ClibUnitStr& unit ///< [in] 単位
)
{
  s << unit.val();
  return s;
}

/// @brief ストリーム出演算子
/// @relates ClibCapacitanceUnit
inline
std::ostream&
operator<<(
  std::ostream& s,                 ///< [in] 出力ストリーム
  const ClibCapacitanceUnit& unit ///< [in] 単位
)
{
  s << unit.float_val() << unit.str_val();
  return s;
}

END_NAMESPACE_YM_CLIB

#endif // CLIBUNIT_H
