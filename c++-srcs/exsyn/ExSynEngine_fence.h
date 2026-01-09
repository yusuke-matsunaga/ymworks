#ifndef EXSYNENGINE_FENCE_H
#define EXSYNENGINE_FENCE_H

/// @file ExSynEngine_fence.h
/// @brief ExSynEngine_fence のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ExSyn.h"
#include "ym/SatSolver.h"
#include "ym/SatLiteral.h"
#include "ym/SatOneHotVar.h"
#include "ym/SatOrderedSet.h"
#include "ym/JsonValue.h"
#include "ym/bc.h"


BEGIN_NAMESPACE_YM_EXSYN

class Fence;
class OpNode;

//////////////////////////////////////////////////////////////////////
/// @class ExSynEngine ExSynEngine.h "ExSynEngine.h"
/// @brief ExSyn 用のエンコーダーの基底クラス
//////////////////////////////////////////////////////////////////////
class ExSynEngine_fence
{
public:

  /// @brief コンストラクタ
  ExSynEngine_fence(
    const TruthTable& tt,   ///< [in] 対象の真理値表
    const Fence& fence,     ///< [in] 各レベルのノード数
    const JsonValue& option ///< [in] オプション
  );

  /// @brief デストラクタ
  ~ExSynEngine_fence();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 問題を解く
  ///
  /// 解けなかった場合は空のグラフを返す．
  BcGraph
  solve();

  /// @brief 問題を解く(CEGAR版)
  ///
  /// 解けなかった場合は空のグラフを返す．
  BcGraph
  solve_cegar();

  /// @brief BcGraph が正しいかチェックする．
  /// @retval 正しければ空のベクタを返す．
  /// @retval 異なっていれば反例の入力ベクタを返す．
  static
  std::vector<bool>
  check_graph(
    const TruthTable& tt, ///< [in] 真理値表
    const BcGraph& graph  ///< [in] Bool chain を表すグラフ
  );

  /// @brief cegar メソッドを用いる時 true とするフラグ
  bool
  cegar() const
  {
    return mFlags[1];
  }

  /// @brief use_all_steps 制約を用いる時 true とするフラグ
  bool
  use_all_steps() const
  {
    return mFlags[2];
  }

  /// @brief no_reapplication 制約を用いる時 true とするフラグ
  bool
  no_reapplication() const
  {
    return mFlags[3];
  }

  /// @brief ordered_steps 制約を用いる時 true とするフラグ
  bool
  ordered_steps() const
  {
    return mFlags[4];
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // 出力に関する情報
  struct Output {
    SatOneHotVar sel;
    SatLiteral inv;
    bool zero{false};
    bool one{false};

    /// @brief 空のコンストラクタ
    Output() = default;

    /// @brief コンストラクタ
    Output(
      SatSolver& solver, ///< [in] SATソルバ
      SizeType cand_size ///< [in] 入力の候補数
    );

    /// @brief 定数0を表すオブジェクトを返す．
    static
    Output
    new_zero()
    {
      Output output;
      output.zero = true;
      return output;
    }

    /// @brief 定数1を表すオブジェクトを返す．
    static
    Output
    new_one()
    {
      Output output;
      output.one = true;
      return output;
    }
  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力値に対応する条件を追加する．
  bool
  add_cond(
    SizeType input_index ///< [in] 入力インデックス
  );

  /// @brief SATの解から BcGraph を作る．
  BcGraph
  make_graph(
    const SatModel& model
  );

  /// @brief ソースの枝を作る．
  BcEdge
  make_edge(
    SizeType src, ///< [in] ソース番号
    bool inv,     ///< [in] 反転属性
    BcGraph& g    ///< [in] 対象のグラフ
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の真理値表
  TruthTable mTruthTable;

  // 対象の fence
  Fence mFence;

  // SATソルバ
  SatSolver mSolver;

  // 演算ノード用の変数のリスト
  // サイズは mOpNodeNum;
  std::vector<std::unique_ptr<OpNode>> mOpList;

  // 出力の選択用変数のリスト
  // サイズは output_num()
  std::vector<Output> mOutputList;

  // 種々のフラグ
  // 1: cegar
  std::bitset<6> mFlags{0};

};

END_NAMESPACE_YM_EXSYN

#endif // EXSYNENGINE_FENCE_H
