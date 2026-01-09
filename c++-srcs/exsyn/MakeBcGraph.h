#ifndef MAKEBCGRAPH_H
#define MAKEBCGRAPH_H

/// @file MakeBcGraph.h
/// @brief MakeBcGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ExSyn.h"
#include "ym/SatModel.h"
#include "ym/BcGraph.h"


BEGIN_NAMESPACE_YM_EXSYN

class EngineBase;

//////////////////////////////////////////////////////////////////////
/// @class MakeBcGraph MakeBcGraph.h "MakeBcGraph.h"
/// @brief SAT モデルから BcGraph を生成するクラス
//////////////////////////////////////////////////////////////////////
class MakeBcGraph
{
public:

  /// @brief コンストラクタ
  MakeBcGraph(
    const EngineBase& engine, ///< [in] 対象のエンジン
    const SatModel& model     ///< [in] SAT問題のモデル
  );

  /// @brief デストラクタ
  ~MakeBcGraph();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief SATの解から BcGraph を作る．
  BcGraph
  operator()();


private:
  //////////////////////////////////////////////////////////////////////
  // 派生クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算ノードを作る．
  /// @return BcGraph のノード番号を返す．
  SizeType
  make_op(
    SizeType op_id ///< [in] 演算ノード番号
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // エンジン
  const EngineBase& mEngine;

  // SATモデル
  const SatModel& mModel;

  // 結果のグラフ
  BcGraph mGraph;

  // 演算番号をキーとして BcGraph のノード番号を格納した辞書
  std::unordered_map<SizeType, SizeType> mIdDict;

};

END_NAMESPACE_YM_EXSYN

#endif // MAKEBCGRAPH_H
