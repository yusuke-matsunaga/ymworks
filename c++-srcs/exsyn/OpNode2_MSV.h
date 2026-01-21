#ifndef OPNODE2_MSV_H
#define OPNODE2_MSV_H

/// @file OpNode2_MSV.h
/// @brief OpNode2_MSV のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "OpNode2.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class OpNode2_MSV OpNode2_MSV.h "OpNode2_MSV.h"
/// @brief MSV バージョンの OpNode2
//////////////////////////////////////////////////////////////////////
class OpNode2_MSV :
  public OpNode2
{
public:

  /// @brief コンストラクタ
  OpNode2_MSV(
    SatSolver& solver,   ///< [in] SATソルバ
    SizeType input_num,  ///< [in] 入力数
    SizeType op_id,      ///< [in] 演算ノード番号
    SizeType cand_num    ///< [in] オペランドの候補数
  );

  /// @brief デストラクタ
  ~OpNode2_MSV() = default;


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
    const OpNode* op2  ///< [in] 演算ノード
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

  /// @brief op_id 番目の演算を使用している時 true となるリテラルを追加する．
  void
  add_use_lit(
    SizeType op_id,                   ///< [in] 演算番号
    std::vector<SatLiteral>& lit_list ///< [out] 格納先の変数
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

  // MSV 方式の変数選択
  std::vector<SatLiteral> mSel;

};

END_NAMESPACE_YM_EXSYN

#endif // OPNODE2_MSV_H
