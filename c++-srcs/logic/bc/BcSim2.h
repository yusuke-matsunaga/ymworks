#ifndef BCSIM2_H
#define BCSIM2_H

/// @file BcSim2.h
/// @brief BcSim2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bc.h"


BEGIN_NAMESPACE_YM_BC

//////////////////////////////////////////////////////////////////////
/// @class BcSim2 BcSim2.h "BcSim2.h"
/// @brief BcGraph のシミュレータ(BcBvTypeバージョン)
//////////////////////////////////////////////////////////////////////
class BcSim2
{
public:

  /// @brief コンストラクタ
  BcSim2(
    const BcGraph& g,                       ///< [in] 対象の Boolean Chain
    const std::vector<BcBvType>& input_vals ///< [in] 入力ベクタ
  );

  /// @brief デストラクタ
  ~BcSim2() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief シミュレーションを行う．
  std::vector<BcBvType>
  simulate();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算ノードの出力の値を計算する．
  static
  BcBvType
  calc_op(
    const BcOp& op,                        ///< [in] 演算
    const std::vector<BcBvType>& ival_list ///< [in] 入力値のリスト
  );

  /// @brief 枝に対する値を求める．
  BcBvType
  value(
    const BcEdge& edge ///< [in] 枝
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のグラフ
  const BcGraph& mG;

  // 入力ベクタ
  const std::vector<BcBvType>& mInputVals;

  // 演算ノードの出力値
  std::vector<BcBvType> mOpVals;

};

END_NAMESPACE_YM_BC

#endif // BCSIM_H
