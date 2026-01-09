#ifndef CLIBPINMAP_H
#define CLIBPINMAP_H

/// @file ClibPinMap.h
/// @brief ClibPinMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"
#include "ym/BinDec.h"
#include "ym/BinEnc.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
/// @class ClibPinMap ClibPinMap.h "ClibPinMap.h"
/// @brief ピンの割当を表すクラス
///
/// 具体的にはピン番号を極性を持つ．
//////////////////////////////////////////////////////////////////////
class ClibPinMap
{
public:

  /// @brief 空のコンストラクタ
  ClibPinMap() = default;

  /// @brief コンストラクタ
  explicit
  ClibPinMap(
    SizeType id,     ///< [in] ピン番号
    bool inv = false ///< [in] 反転フラグ
  )
  {
    mBody = static_cast<std::uint8_t>(id) << 1 | static_cast<std::uint8_t>(inv);
  }

  /// @brief デストラクタ
  ~ClibPinMap() = default;

  /// @brief 不正な値を返すクラス関数
  static
  ClibPinMap
  invalid()
  {
    // 実は空のコンストラクタが不正値を作る．
    return ClibPinMap{};
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ピン番号を返す．
  SizeType
  id() const
  {
    return static_cast<SizeType>(mBody >> 1);
  }

  /// @brief 反転属性を返す．
  bool
  inv() const
  {
    return static_cast<bool>(mBody & 1U);
  }

  /// @brief 適正な値の時に true を返す．
  bool
  is_valid() const
  {
    return mBody != 0xFFU;
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const ClibPinMap& right
  ) const
  {
    return mBody == right.mBody;
  }

  /// @brief 非等価演算子
  bool
  operator!=(
    const ClibPinMap& right
  ) const
  {
    return !operator==(right);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // バイナリダンプ用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief バイナリダンプを行う．
  void
  dump(
    BinEnc& bos ///< [in] 出力先のストリーム
  ) const;

  /// @brief バイナリダンプされた内容を読み込む．
  void
  restore(
    BinDec& bis ///< [in] 入力元のストリーム
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 番号と反転属性をパックしたもの
  std::uint8_t mBody{0xFFU}; // 不正な値で初期化する．

};

/// @relates ClibPinMap
/// @brief ClibPinMap のストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s,         ///< [in] 出力先のストリーム
  const ClibPinMap& pinmap ///< [in] ピンのマッピング
)
{
  s << pinmap.id();
  if ( pinmap.inv() ) {
    s << "~";
  }
  return s;
}

END_NAMESPACE_YM_CLIB

#endif // CLIBPINMAP_H
