#ifndef CIBUNDLE_H
#define CIBUNDLE_H

/// @file CiBundle.h
/// @brief CiBundle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"
#include "ym/ShString.h"


BEGIN_NAMESPACE_YM_CLIB

class CiPin;
class Serializer;
class Deserializer;

//////////////////////////////////////////////////////////////////////
/// @class CiBundle CiBundle.h "CiBundle.h"
/// @brief ClibBundle の実装クラス
//////////////////////////////////////////////////////////////////////
class CiBundle
{
public:

  /// @brief 空のコンストラクタ
  CiBundle() = default;

  /// @brief コンストラクタ
  CiBundle(
    const ShString& name,                     ///< [in] 名前
    const std::vector<const CiPin*>& pin_list ///< [in] ピンリスト
  ) : mName{name},
      mPinList{pin_list}
  {
  }

  /// @brief デストラクタ
  ~CiBundle() = default;


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

  /// @brief 内容を復元する．
  static
  std::unique_ptr<CiBundle>
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

  // ピンのリスト
  std::vector<const CiPin*> mPinList;

};

END_NAMESPACE_YM_CLIB

#endif // CIBUNDLE_H
