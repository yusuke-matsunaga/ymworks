#ifndef EXSYN_H
#define EXSYN_H

/// @file ExSyn.h
/// @brief ExSyn のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include "ym/TruthTable.h"
#include "ym/BcGraph.h"
#include "ym/JsonValue.h"


//////////////////////////////////////////////////////////////////////
// exsyn 用の名前空間の定義用マクロ
//////////////////////////////////////////////////////////////////////

/// @brief exsyn 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_EXSYN \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsExSyn)

/// @brief exsyn 用の名前空間の終了
#define END_NAMESPACE_YM_EXSYN \
END_NAMESPACE(nsExSyn) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class ExSyn ExSyn.h "ExSyn.h"
/// @brief exact synth を行うクラス
///
/// 実はクラスメソッドしか持たない．
//////////////////////////////////////////////////////////////////////
class ExSyn
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 2項演算で演算数が最小となる実装を求める．
  /// @sa BcGraph, TruthTable, JsonValue
  static
  BcGraph
  gen_boolchain(
    const TruthTable& tt,   ///< [in] 真理値表
    const JsonValue& option ///< [in] オプション
  );

  /// @brief BcGraph が正しいかチェックする．
  /// @return 正しければ true を返す．
  /// @sa TruthTable, BcGraph
  static
  bool
  check_boolchain(
    const TruthTable& tt, ///< [in] 真理値表
    const BcGraph& graph  ///< [in] Bool chain を表すグラフ
  );

};

END_NAMESPACE_YM_EXSYN

BEGIN_NAMESPACE_YM

using nsExSyn::ExSyn;

END_NAMESPACE_YM

#endif // EXSYN_H
