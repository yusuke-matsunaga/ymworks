#ifndef BCSIM_H
#define BCSIM_H

/// @file BcSim.h
/// @brief BcSim のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bc.h"


BEGIN_NAMESPACE_YM_BC

//////////////////////////////////////////////////////////////////////
/// @class BcSim BcSim.h "BcSim.h"
/// @brief BcGraph のシミュレータ
//////////////////////////////////////////////////////////////////////
class BcSim
{
public:

  /// @brief コンストラクタ
  BcSim(
    const BcGraph& g,                   ///< [in] 対象の Boolean Chain
    const std::vector<bool>& input_vals ///< [in] 入力ベクタ
  );

  /// @brief デストラクタ
  ~BcSim() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief シミュレーションを行う．
  std::vector<bool>
  simulate();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算ノードの出力の値を計算する．
  static
  bool
  calc_op(
    const BcOp& op,                    ///< [in] 演算
    const std::vector<bool>& ival_list ///< [in] 入力値のリスト
  );

  /// @brief 枝に対する値を求める．
  bool
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
  const std::vector<bool>& mInputVals;

  // 演算ノードの出力値
  std::vector<bool> mOpVals;

};

END_NAMESPACE_YM_BC

#endif // BCSIM_H
