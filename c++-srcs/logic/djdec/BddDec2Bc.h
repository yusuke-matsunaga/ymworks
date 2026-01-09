#ifndef BDDDEC2BC_H
#define BDDDEC2BC_H

/// @file BddDec2Bc.h
/// @brief BddDec2Bc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/DjDec.h"
#include "ym/BddVar.h"
#include "ym/BcGraph.h"


BEGIN_NAMESPACE_YM_DJDEC

class BddDecEdge;

//////////////////////////////////////////////////////////////////////
/// @class BddDec2Bc BddDec2Bc.h "BddDec2Bc.h"
/// @brief Decomposition Graph を BcGraph に変換するクラス
//////////////////////////////////////////////////////////////////////
class BddDec2Bc
{
public:

  /// @brief コンストラクタ
  BddDec2Bc(
    const std::vector<BddVar>& sup_list ///< [in] 対象の関数のサポート変数のリスト
  );

  /// @brief デストラクタ
  ~BddDec2Bc() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief BcGraph に変換する．
  BcGraph
  make_bcgraph(
    BddDecEdge root ///< [in] Decomposition Graph の根の枝
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief make_bcgraph の下請け関数
  BcEdge
  convert(
    BddDecEdge edge
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 結果のグラフ
  BcGraph mG;

  // BddVar をキーとして入力番号を格納する辞書
  std::unordered_map<BddVar, SizeType> mVarMap;

};

END_NAMESPACE_YM_DJDEC

#endif // BDDDEC2BC_H
