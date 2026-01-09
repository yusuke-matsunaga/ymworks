#ifndef CIBUS_H
#define CIBUS_H

/// @file CiBus.h
/// @brief CiBus のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023, 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"
#include "ym/ShString.h"


BEGIN_NAMESPACE_YM_CLIB

class CiBusType;
class CiPin;
class Serializer;
class Deserializer;

//////////////////////////////////////////////////////////////////////
/// @class CiBus CiBus.h "CiBus.h"
/// @brief ClibBus の実装クラス
//////////////////////////////////////////////////////////////////////
class CiBus
{
public:

  /// @brief 空のコンストラクタ
  CiBus() = default;

  /// @brief コンストラクタ
  CiBus(
    const ShString& name,                     ///< [in] 名前
    const CiBusType* bus_type,                ///< [in] バスタイプ
    const std::vector<const CiPin*>& pin_list ///< [in] ピンリスト
  ) : mName{name},
      mBusType{bus_type},
      mPinList{pin_list}
  {
  }

  /// @brief デストラクタ
  ~CiBus() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  std::string
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

  /// @brief バスの型の取得
  const CiBusType*
  bus_type() const
  {
    return mBusType;
  }

  /// @brief ピン数の取得
  SizeType
  pin_num() const
  {
    return mPinList.size();
  }

  /// @brief ピンの取得
  const CiPin*
  pin(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < pin_num() )
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < pin_num() );
    return mPinList[pos];
  }

  /// @brief ピンのリストの取得
  const std::vector<const CiPin*>&
  pin_list() const
  {
    return mPinList;
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

  /// @brief 内容を読み込む．
  static
  std::unique_ptr<CiBus>
  restore(
    Deserializer& s ///< [in] デシリアライザ
  );


protected:

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

  // バスの型
  const CiBusType* mBusType{nullptr};

  // ピンのリスト
  std::vector<const CiPin*> mPinList;

};

END_NAMESPACE_YM_CLIB

#endif // CIBUS_H
