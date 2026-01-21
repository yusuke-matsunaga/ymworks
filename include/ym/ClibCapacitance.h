#ifndef YM_CLIBCAPACITANCE_H
#define YM_CLIBCAPACITANCE_H

/// @file ym/ClibCapacitance.h
/// @brief ClibCapacitance のヘッダファイル
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
/// @class ClibCapacitance ClibCapacitance.h "ym/ClibCapacitance.h"
/// @brief 容量値を表すクラス
///
/// - 内容はただの double だが他の数値型との演算を禁止するために独立
///   したクラスとしている．
//////////////////////////////////////////////////////////////////////
class ClibCapacitance
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  ///
  /// - 内容は 0.0 に初期化される．
  ClibCapacitance() = default;

  /// @brief double からの変換コンストラクタ
  explicit
  ClibCapacitance(
    double v ///< [in] 設定する値
  ) : mValue{v}
  {
  }

  /// @brief 無限大の値を作るクラス関数
  static
  ClibCapacitance
  infinity()
  {
    return ClibCapacitance{std::numeric_limits<double>::infinity()};
  }

  /// @brief デストラクタ
  ~ClibCapacitance() = default;


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
  const ClibCapacitance&
  operator=(
    const ClibCapacitance& src ///< [in] コピー元のオブジェクト
  )
  {
    mValue = src.mValue;
    return *this;
  }

  /// @brief 加算付き代入演算子
  /// @return 代入後の自身への参照を返す．
  const ClibCapacitance&
  operator+=(
    const ClibCapacitance& src ///< [in] オペランド
  )
  {
    mValue += src.mValue;
    return *this;
  }

  /// @brief 減算付き代入演算子
  /// @return 代入後の自身への参照を返す．
  const ClibCapacitance&
  operator-=(
    const ClibCapacitance& src ///< [in] オペランド
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
/// @relates ClibCapacitance
/// @return 演算結果を返す．
inline
ClibCapacitance
operator+(
  const ClibCapacitance& left, ///< [in] 左のオペランド
  const ClibCapacitance& right	///< [in] 右のオペランド
)
{
  return ClibCapacitance(left).operator+=(right);
}

/// @brief 減算
/// @relates ClibCapacitance
/// @return 演算結果を返す．
inline
ClibCapacitance
operator-(
  const ClibCapacitance& left, ///< [in] 左のオペランド
  const ClibCapacitance& right ///< [in] 右のオペランド
)
{
  return ClibCapacitance(left).operator-=(right);
}

/// @brief 等価比較演算子
/// @relates ClibCapacitance
/// @return 演算結果を返す．
inline
bool
operator==(
  const ClibCapacitance& left, ///< [in] 左のオペランド
  const ClibCapacitance& right ///< [in] 右のオペランド
)
{
  return left.value() == right.value();
}

/// @brief 非等価比較演算子
/// @relates ClibCapacitance
/// @return 演算結果を返す．
inline
bool
operator!=(
  const ClibCapacitance& left, ///< [in] 左のオペランド
  const ClibCapacitance& right ///< [in] 右のオペランド
)
{
  return !operator==(left, right);
}

/// @brief 大小比較演算子
/// @relates ClibCapacitance
/// @return 演算結果を返す．
inline
bool
operator<(
  const ClibCapacitance& left, ///< [in] 左のオペランド
  const ClibCapacitance& right ///< [in] 右のオペランド
)
{
  return left.value() < right.value();
}

/// @brief 大小比較演算子
/// @relates ClibCapacitance
/// @return 演算結果を返す．
inline
bool
operator>(
  const ClibCapacitance& left, ///< [in] 左のオペランド
  const ClibCapacitance& right ///< [in] 右のオペランド
)
{
  return operator<(right, left);
}

/// @brief 大小比較演算子
/// @relates ClibCapacitance
/// @return 演算結果を返す．
inline
bool
operator<=(
  const ClibCapacitance& left, ///< [in] 左のオペランド
  const ClibCapacitance& right ///< [in] 右のオペランド
)
{
  return !operator<(right, left);
}

/// @brief 大小比較演算子
/// @relates ClibCapacitance
/// @return 演算結果を返す．
inline
bool
operator>=(
  const ClibCapacitance& left, ///< [in] 左のオペランド
  const ClibCapacitance& right ///< [in] 右のオペランド
)
{
  return !operator<(left, right);
}

/// @brief ストリーム出力
/// @relates ClibCapacitance
inline
std::ostream&
operator<<(
  std::ostream& s,           ///< [in] 出力先のストリーム
  const ClibCapacitance& val ///< [in] 値
)
{
  return s << val.value();
}

/// @brief ストリーム入力
inline
std::istream&
operator>>(
  std::istream& s,     ///< [in] 入力元のストリーム
  ClibCapacitance& val ///< [out] 読み出された値を格納する変数
)
{
  double tmp;
  s >> tmp;
  val = ClibCapacitance{tmp};
  return s;
}

/// @brief バイナリストリーム出力
/// @relates ClibCapacitance
inline
BinEnc&
operator<<(
  BinEnc& s,                ///< [in] 出力先のストリーム
  const ClibCapacitance& val ///< [in] 値
)
{
  return s << val.value();
}

/// @brief ストリーム入力
inline
BinDec&
operator>>(
  BinDec& s,          ///< [in] 入力元のストリーム
  ClibCapacitance& val ///< [out] 読み出された値を格納する変数
)
{
  double tmp;
  s >> tmp;
  val = ClibCapacitance{tmp};
  return s;
}

END_NAMESPACE_YM_CLIB

#endif // YM_CLIBCAPACITANCE_H
