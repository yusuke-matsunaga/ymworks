#ifndef ENGINE_BD_H
#define ENGINE_BD_H

/// @file Engine_BD.h
/// @brief Engine_BD のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EngineBase.h"
#include "BD.h"
#include "BdNode.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class Engine_BD Engine_BD.h "Engine_BD.h"
/// @brief
//////////////////////////////////////////////////////////////////////
class Engine_BD
{
public:

  /// @brief コンストラクタ
  Engine_BD(
    const TruthTable& tt,           ///< [in] 対象の論理関数
    const std::vector<BD>& bd_list, ///< [in] Bound DAG のリスト
    const JsonValue& option         ///< [in] オプション
  );

  /// @brief デストラクタ
  ~Engine_BD();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 問題を解く
  ///
  /// 解けなかった場合は空のグラフを返す．
  BcGraph
  solve();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 対象の論理関数を返す．
  const TruthTable&
  func() const
  {
    return mFunc;
  }

  /// @brief オプションを返す．
  const JsonValue&
  option() const
  {
    return mOption;
  }

  /// @brief SATソルバを返す．
  SatSolver&
  solver()
  {
    return mSolver;
  }

  /// @brief 入力数を返す．
  SizeType
  input_num() const
  {
    return mInputNum;
  }

  /// @brief 出力数を返す．
  SizeType
  output_num() const
  {
    return 1;
  }

  /// @brief 演算ノードと出力の制約を作る．
  void
  make_op_list();

  /// @brief 入力値に対応する条件を追加する．
  void
  add_cond(
    SizeType input_index ///< [in] 入力インデックス
  );

  /// @brief SATの解から BcGraph を作る．
  BcGraph
  make_graph(
    const SatModel& model ///< [in] SAT問題のモデル
  );

  /// @brief 演算ノードを作る．
  SizeType
  make_op(
    const SatModel& model,                          ///< [in] SAT問題のモデル
    SizeType op_id,                                 ///< [in] 演算ノード番号
    BcGraph& g,                                     ///< [in] 対象のグラフ
    std::unordered_map<SizeType, SizeType>& op_dict ///< [in] 演算番号の辞書
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の論理関数
  TruthTable mFunc;

  // オプション
  JsonValue mOption;

  // SATソルバ
  SatSolver mSolver;

  // 入力数
  SizeType mInputNum;

  // Bound DAG のリスト
  const std::vector<BD>& mBD_List;

  // Partial DAG のノード数
  SizeType mPdNodeNum;

  // TemplNode のリスト
  // サイズは mPdNodeNum
  std::vector<std::unique_ptr<const TemplNode>> mTemplateList;

  // BdNode のリスト
  std::vector<std::unique_ptr<const BdNode>> mNodeList;

  // TemplNodeに対応する BdNode のグループのリスト
  std::vector<std::vector<SizeType>> mNodeGroupList;

  // 出力の BdNode の選択用変数
  SatOneHotVar mBdSel;

};

END_NAMESPACE_YM_EXSYN

#endif // ENGINE_BD_H
