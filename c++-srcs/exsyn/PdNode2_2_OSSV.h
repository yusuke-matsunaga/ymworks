#ifndef PDNODE2_2_OSSV_H
#define PDNODE2_2_OSSV_H

/// @file PdNode2_2_OSSV.h
/// @brief PdNode2_2_OSSV のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PdNode2.h"
#include "ym/SatOrderedSet.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class PdNode2_2_OSSV PdNode2_2_OSSV.h "PdNode2_2_OSSV.h"
/// @brief OSSV バージョンの PdNode(PD用)
///
/// ほぼ OpNode2_SSV と同じだが選択候補が入力のみとなっている．
//////////////////////////////////////////////////////////////////////
class PdNode2_2_OSSV :
  public PdNode2
{
public:

  /// @brief コンストラクタ
  PdNode2_2_OSSV(
    SatSolver& solver,   ///< [in] SATソルバ
    SizeType input_num,  ///< [in] 入力数
    SizeType op_id       ///< [in] 演算ノード番号
  );

  /// @brief デストラクタ
  ~PdNode2_2_OSSV() = default;


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

  /// @brief no_reapplication 制約を追加する．
  void
  add_no_reapplication_cond(
    SatSolver& solver, ///< [in] SATソルバ
    const OpNode* op2  ///< [in] 演算ノードのリスト
  ) const override;

  /// @brief オペランドの組み合わせを禁止する制約を追加する．
  void
  add_ng_operand_cond(
    SatSolver& solver, ///< [in] SATソルバ
    SizeType opr0,     ///< [in] オペランド0
    SizeType opr1      ///< [in] オペランド1
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

  /// @brief opr0, opr1 を選択している時 true になるリテラルを返す．
  SatLiteral
  sel_lit(
    SizeType opr0,
    SizeType opr1
  ) const
  {
    return mLitArray[opr0 * input_num() + opr1];
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // (i1 * input_num()) + i2 の位置に変数を格納した配列
  // i1 < i2 なので未使用の領域がある．
  std::vector<SatLiteral> mLitArray;

  // 変数のリスト
  SatOrderedSet mLitSet;

};

END_NAMESPACE_YM_EXSYN

#endif // PDNODE2_2_OSSV_H
