#ifndef DJDEC_H
#define DJDEC_H

/// @file djdec.h
/// @brief djdec のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include "ym/Bdd.h"
#include "ym/TvFunc.h"
#include "ym/BcGraph.h"


/// @brief djdec 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_DJDEC \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsDjdec)

/// @brief trusyn 用の名前空間の終了
#define END_NAMESPACE_YM_DJDEC \
END_NAMESPACE(nsDjdec) \
END_NAMESPACE_YM

#define DJDEC_NAMESPACE YM_NAMESPACE::nsDjdec

BEGIN_NAMESPACE_YM_DJDEC

//////////////////////////////////////////////////////////////////////
/// @class DjDec DjDec.h "ym/DjDec.h"
/// @ingroup LogicGroup
/// @brief Disjoint Decomposition を行うクラス
///
/// クラスメソッドしか持たない
//////////////////////////////////////////////////////////////////////
class DjDec
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 与えられた関数を分解する．
  /// @sa Bdd, BcGraph
  ///
  /// 結果は BcGraph で表す．
  static
  BcGraph
  decompose(
    const Bdd& func ///< [in] 分解を行う関数
  );

  /// @brief 与えられた関数を分解する．
  /// @ TvFunc, BcGraph
  ///
  /// 結果は BcGraph で表す．
  static
  BcGraph
  decompose(
    const TvFunc& func ///< [in] 分解を行う関数
  );

};

END_NAMESPACE_YM_DJDEC

BEGIN_NAMESPACE_YM

using nsDjdec::DjDec;

END_NAMESPACE_YM

#endif // DJDEC_H
