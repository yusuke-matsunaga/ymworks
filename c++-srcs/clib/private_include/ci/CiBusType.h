#ifndef CIBUSTYPE_H
#define CIBUSTYPE_H

/// @file CiBusType.h
/// @brief CiBusType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"
#include "ym/ClibBusType.h"
#include "ym/ShString.h"


BEGIN_NAMESPACE_YM_CLIB

class Serializer;
class Deserializer;

//////////////////////////////////////////////////////////////////////
/// @class CiBusType CiBusType.h "CiBusType.h"
/// @brief ClibBusType の実装クラス
//////////////////////////////////////////////////////////////////////
class CiBusType
{
public:

  /// @brief 空のコンストラクタ
  CiBusType() = default;

  /// @brief 要素を指定したコンストラクタ
  CiBusType(
    const ShString& name,     ///< [in] 名前
    SizeType bit_from,        ///< [in] 開始位置
    SizeType bit_to           ///< [in] 終了位置
  ) : mName{name},
      mBitFrom{bit_from},
      mBitTo{bit_to}
  {
    set_bit_width();
  }

  /// @brief デストラクタ
  ~CiBusType() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  ShString
  name() const
  {
    return mName;
  }

  /// @brief 名前の取得
  ShString
  _name() const
  {
    return mName;
  }

  /// @brief base_type の取得
  ClibBusType::BaseType
  base_type() const
  {
    return ClibBusType::ArrayType;
  }

  /// @brief data_type の取得
  ClibBusType::DataType
  data_type() const
  {
    return ClibBusType::BitType;
  }

  /// @brief ビット幅の取得
  SizeType
  bit_width() const
  {
    return mBitWidth >> 1;
  }

  /// @brief 開始ビットの取得
  SizeType
  bit_from() const
  {
    return mBitFrom;
  }

  /// @brief 終了ビットの取得
  SizeType
  bit_to() const
  {
    return mBitTo;
  }

  /// @brief 向きの取得
  /// @note true の時，降順を表す．
  bool
  downto() const
  {
    return static_cast<bool>(mBitWidth & 1U);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // dump/restore 関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をシリアライズする．
  void
  serialize(
    Serializer& s ///< [in] シリアライザ
  ) const;

  /// @brief 内容をバイナリダンプする．
  void
  dump(
    Serializer& s ///< [in] シリアライザ
  ) const;

  /// @brief 内容を復元する．
  static
  std::unique_ptr<CiBusType>
  restore(
    Deserializer& s   ///< [in] デシリアライザ
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief mBitWidth の計算を行う．
  void
  set_bit_width()
  {
    if ( mBitFrom <= mBitTo ) {
      mBitWidth = (mBitTo - mBitFrom + 1) * 2;
    }
    else {
      mBitWidth = (mBitFrom - mBitTo + 1) * 2 + 1;
    }
  }

  /// @brief restore() の本体
  void
  _restore(
    Deserializer& s
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // ビット幅(最下位ビットは向きを表す)
  SizeType mBitWidth;

  // 開始ビット
  SizeType mBitFrom;

  // 終了ビット
  SizeType mBitTo;

};

END_NAMESPACE_YM_CLIB

#endif // CIBUS_H
