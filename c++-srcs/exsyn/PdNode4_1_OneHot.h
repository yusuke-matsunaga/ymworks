#ifndef PDNODE4_1_ONEHOT_H
#define PDNODE4_1_ONEHOT_H

/// @file PdNode4_1_OneHot.h
/// @brief PdNode4_1_OneHot のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PdNode4_1.h"
#include "ym/SatOneHotVar.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class PdNode4_1_OneHot PdNode4_1_OneHot.h "PdNode4_1_OneHot.h"
/// @brief one-hot エンコーディングを用いた PdNode4_1
//////////////////////////////////////////////////////////////////////
class PdNode4_1_OneHot :
  public PdNode4_1
{
public:

  /// @brief コンストラクタ
  PdNode4_1_OneHot(
    SatSolver& solver,   ///< [in] SATソルバ
    SizeType input_num,  ///< [in] 入力数
    SizeType op_id,      ///< [in] 演算ノード番号
    SizeType opr1,       ///< [in] オペランド1
    SizeType opr2,       ///< [in] オペランド2
    SizeType opr3        ///< [in] オペランド3
  );

  /// @brief デストラクタ
  ~PdNode4_1_OneHot() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力値に関する制約を追加する．
  /// @return 出力値を表す変数を返す．
  SatLiteral
  add_ovar_cond(
    SatSolver& solver,                        ///< [in] SATソルバ
    const std::vector<bool>& ival_list,       ///< [in] 入力値のリスト
    const std::vector<SatLiteral>& opvar_list ///< [in] 演算ノードの出力の変数のリスト
  ) const override;

  /// @brief 直前の演算ノードに対する制約を追加する．
  void
  add_op_order_cond(
    SatSolver& solver, ///< [in] SATソルバ
    const OpNode* op   ///< [in] 直前の演算ノード
  ) const override;

  /// @brief SATモデルから入力の選択結果を得る．
  std::vector<SizeType>
  get_opr(
    const SatModel& model ///< [in] SATモデル
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オペランド0の選択を表す変数
  SatOneHotVar mSel0;


};

END_NAMESPACE_YM_EXSYN

#endif // PDNODE4_1_ONEHOT_H
