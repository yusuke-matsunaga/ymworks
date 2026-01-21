#ifndef YM_RANDPERMGEN_H
#define YM_RANDPERMGEN_H

/// @file ym/RandPermGen.h
/// @brief RandPermGen のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include <random>


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class RandPermGen RandGen.h "RandGen.h"
/// @brief ランダムな順列を作るクラス
//////////////////////////////////////////////////////////////////////
class RandPermGen
{
public:

  /// @brief コンストラクタ
  RandPermGen(
    SizeType n ///< [in] 要素数
  ) : mArray(n)
  {
  }

  /// @brief デストラクタ
  ~RandPermGen() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数を返す．
  SizeType
  num() const { return mArray.size(); }

  /// @brief ランダムな順列を生成する．
  template<class URNG>
  void
  generate(
    URNG& randgen ///< [in] 乱数発生器
  )
  {
    std::vector<SizeType> src_array(num());
    for ( SizeType i = 0; i < num(); ++ i ) {
      src_array[i] = i;
    }
    auto n = num() - 1;
    for ( SizeType i = 0; i < num(); ++ i ) {
      std::uniform_int_distribution<SizeType> rd(0, n);
      auto r = rd(randgen);
      mArray[i] = src_array[r];
      src_array[r] = src_array[n];
      -- n;
    }
  }

  /// @brief 順列の要素を取り出す．
  /// @exception std::out_of_range 範囲外のアクセス
  SizeType
  elem(
    SizeType pos ///< [in] 要素の位置番号 ( 0 <= pos < num() )
  ) const
  {
    if ( pos >= num() ) {
      throw std::out_of_range{"'pos' is out of range"};
    }
    return mArray[pos];
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 現在の順列
  std::vector<SizeType> mArray;

};

END_NAMESPACE_YM

#endif // YM_RANDPERMGEN_H
