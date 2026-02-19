#ifndef MCPERTURBATOR_H
#define MCPERTURBATOR_H

/// @file McPerturbator.h
/// @brief McPerturbator のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/combopt.h"
#include "ym/MinCov.h"
#include "ym/JsonValue.h"
#include <random>


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class McPerturbator McPerturbator.h "McPerturbator.h"
/// @brief MinCov の別解を求めるクラス
//////////////////////////////////////////////////////////////////////
class McPerturbator
{
public:

  /// @brief コンストラクタ
  McPerturbator(
    const MinCov& mincov,        ///< [in] 最小被覆問題
    const JsonValue& option = {} ///< [in] オプションを表すJSONオブジェクト
  ) : mMinCov{mincov},
      mOption{option}
  {
  }

  /// @brief デストラクタ
  ~McPerturbator() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 別解を求める．
  ///
  /// 具体的には以下の処理を行う．
  /// - ランダムに解に含まれる列を選び，削除する．
  /// - 被覆条件が満たされる様に他の列を選ぶ．
  ///
  /// 別解が存在しない時は空リストを返す．
  std::vector<SizeType>
  operator()(
    const std::vector<SizeType>& solution ///< [in] 初期解
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 被覆問題
  MinCov mMinCov;

  // オプション
  JsonValue mOption;

  // 乱数生成o器
  std::mt19937 mRandGen;

};

END_NAMESPACE_YM_MINCOV

#endif // MCPERTURBATOR_H
