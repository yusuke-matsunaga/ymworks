#ifndef YM_CLIBTIME_H
#define YM_CLIBTIME_H

/// @file ym/ClibTime.h
/// @brief ClibTime のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2017, 2018, 2019, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"
#include "ym/BinDec.h"
#include "ym/BinEnc.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
/// @ingroup ClibGroup
/// @class ClibTime ClibTime.h "ym/ClibTime.h"
/// @brief 時間を表すクラス
///
/// - 内容的には double そのものだが他の単位との演算が
///   行えないようになっている．
/// - ClibTime 同士の演算も加減算のみ定義されている．
//////////////////////////////////////////////////////////////////////
class ClibTime
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  ///
  /// - 内容は 0.0 に初期化される．
  ClibTime() = default;

  /// @brief double からの変換用コンストラクタ
  explicit
  ClibTime(
    double v ///< [in] 設定する値
  ) : mValue{v}
  {
  }

  /// @brief 無限大の値を作るクラス関数
  static
  ClibTime
  infinity()
  {
    return ClibTime{std::numeric_limits<double>::infinity()};
  }

  /// @brief デストラクタ
  ~ClibTime() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 値を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を得る．
  double
  value() const
  {
    return mValue;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 代入演算子
  //////////////////////////////////////////////////////////////////////

  /// @brief 代入演算子
  /// @return 代入後の自身への参照を返す．
  const ClibTime&
  operator=(
    const ClibTime& src ///< [in] コピー元のオブジェクト
  )
  {
    mValue = src.mValue;
    return *this;
  }

  /// @brief 加算付き代入演算子
  /// @return 代入後の自身への参照を返す．
  const ClibTime&
  operator+=(
    const ClibTime& src ///< [in] オペランド
  )
  {
    mValue += src.mValue;
    return *this;
  }

  /// @brief 減算付き代入演算子
  /// @return 代入後の自身への参照を返す．
  const ClibTime&
  operator-=(
    const ClibTime& src ///< [in] オペランド
  )
  {
    mValue -= src.mValue;
    return *this;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  double mValue{0.0};

};

/// @brief 加算
/// @relates ClibTime
/// @return 演算結果を返す．
inline
ClibTime
operator+(
  const ClibTime& left, ///< [in] 左のオペランド
  const ClibTime& right ///< [in] 右のオペランド
)
{
  return ClibTime(left).operator+=(right);
}

/// @brief 減算
/// @relates ClibTime
/// @return 演算結果を返す．
inline
ClibTime
operator-(
  const ClibTime& left, ///< [in] 左のオペランド
  const ClibTime& right ///< [in] 右のオペランド
)
{
  return ClibTime(left).operator-=(right);
}

/// @brief 等価比較演算子
/// @relates ClibTime
/// @return 演算結果を返す．
inline
bool
operator==(
  const ClibTime& left, ///< [in] 左のオペランド
  const ClibTime& right ///< [in] 右のオペランド
)
{
  return left.value() == right.value();
}

/// @brief 非等価比較演算子
/// @relates ClibTime
/// @return 演算結果を返す．
inline
bool
operator!=(
  const ClibTime& left, ///< [in] 左のオペランド
  const ClibTime& right ///< [in] 右のオペランド
)
{
  return !operator==(left, right);
}

/// @brief 大小比較演算子
/// @relates ClibTime
/// @return 演算結果を返す．
inline
bool
operator<(
  const ClibTime& left, ///< [in] 左のオペランド
  const ClibTime& right ///< [in] 右のオペランド
)
{
  return left.value() < right.value();
}

/// @brief 大小比較演算子
/// @relates ClibTime
/// @return 演算結果を返す．
inline
bool
operator>(
  const ClibTime& left, ///< [in] 左のオペランド
  const ClibTime& right ///< [in] 右のオペランド
)
{
  return operator<(right, left);
}

/// @brief 大小比較演算子
/// @relates ClibTime
/// @return 演算結果を返す．
inline
bool
operator<=(
  const ClibTime& left, ///< [in] 左のオペランド
  const ClibTime& right ///< [in] 右のオペランド
)
{
  return !operator<(right, left);
}

/// @brief 大小比較演算子
/// @relates ClibTime
/// @return 演算結果を返す．
inline
bool
operator>=(
  const ClibTime& left, ///< [in] 左のオペランド
  const ClibTime& right ///< [in] 右のオペランド
)
{
  return !operator<(left, right);
}

/// @brief ストリーム出力
inline
std::ostream&
operator<<(
  std::ostream& s,    ///< [in] 出力先のストリーム
  const ClibTime& val ///< [in] 値
)
{
  return s << val.value();
}

/// @brief ストリーム入力
/// @relates ClibTime
inline
std::istream&
operator>>(
  std::istream& s, ///< [in] 入力元のストリーム
  ClibTime& val    ///< [out] 読み出された値
)
{
  double tmp;
  s >> tmp;
  val = ClibTime{tmp};
  return s;
}

/// @brief バイトストリーム出力
inline
BinEnc&
operator<<(
  BinEnc& s,         ///< [in] 出力先のストリーム
  const ClibTime& val ///< [in] 値
)
{
  return s << val.value();
}

/// @brief バイナリストリーム入力
/// @relates ClibTime
inline
BinDec&
operator>>(
  BinDec& s,   ///< [in] 入力元のストリーム
  ClibTime& val ///< [out] 読み出された値
)
{
  double tmp;
  s >> tmp;
  val = ClibTime{tmp};
  return s;
}

END_NAMESPACE_YM_CLIB

#endif // YM_CLIBTIME_H
