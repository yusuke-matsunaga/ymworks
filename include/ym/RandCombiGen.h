#ifndef YM_RANDCOMBIGEN_H
#define YM_RANDCOMBIGEN_H

/// @file ym/RandCombiGen.h
/// @brief RandCombiGen のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include <random>


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class RandCombiGen RandGen.h "ym/RandGen.h"
/// @brief ランダムな組み合わせを作るクラス
//////////////////////////////////////////////////////////////////////
class RandCombiGen
{
public:

  /// @brief コンストラクタ
  RandCombiGen(
    SizeType n, ///< [in] 全要素数
    SizeType k  ///< [in] 組み合わせの要素数
  ) : mNum{n},
      mCombiNum{k},
      mArray(k)
  {
  }

  /// @brief デストラクタ
  ~RandCombiGen() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 全要素数を返す．
  SizeType
  num() const { return mNum; }

  /// @brief 組み合わせの要素数を返す．
  SizeType
  combi_num() const { return mCombiNum; }

  /// @brief ランダムな組み合わせを生成する．
  template <class URNG>
  void
  generate(
    URNG& randgen ///< [in] 乱数発生器
  )
  {
    vector<SizeType> src_array(num());
    for ( SizeType i = 0; i < num(); ++ i ) {
      src_array[i] = i;
    }
    auto n = num() - 1;
    for ( SizeType i = 0; i < combi_num(); ++ i ) {
      std::uniform_int_distribution<SizeType> rd(0, n);
      auto r = rd(randgen);
      mArray[i] = src_array[r];
      src_array[r] = src_array[n];
      -- n;
    }
  }

  /// @brief 組み合わせの要素を取り出す．
  /// @exception std::out_of_range 範囲外のアクセス
  SizeType
  elem(
    SizeType pos ///< [in] 要素の位置番号 ( 0 <= pos < combi_num() )
  ) const
  {
    if ( pos >= combi_num() ) {
      throw std::out_of_range{"'pos' is out of range"};
    }
    return mArray[pos];
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 全要素数
  SizeType mNum;

  // 組み合わせの要素数
  SizeType mCombiNum;

  // 結果を収める配列
  std::vector<int> mArray;

};

END_NAMESPACE_YM

#endif // YM_RANDCOMBIGEN_H
