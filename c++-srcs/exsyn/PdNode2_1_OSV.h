#ifndef PDNODE2_1_OSV_H
#define PDNODE2_1_OSV_H

/// @file PdNode2_1_OSV.h
/// @brief PdNode2_1_OSV のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PdNode2_1.h"
#include "ym/SatOrderedSet.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class PdNode2_1_OSV PdNode2_1_OSV.h "PdNode2_1_OSV.h"
/// @brief OSV 方式の PdNode2_1
//////////////////////////////////////////////////////////////////////
class PdNode2_1_OSV :
  public PdNode2_1
{
public:

  /// @brief コンストラクタ
  PdNode2_1_OSV(
    SatSolver& solver,  ///< [in] SATソルバ
    SizeType input_num, ///< [in] 入力数
    SizeType op_id,     ///< [in] 演算ノード番号
    SizeType opr1       ///< [in] オペランド1のノード番号
  );

  /// @brief デストラクタ
  ~PdNode2_1_OSV() = default;


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

  /// @brief SATモデルから入力の選択結果を得る．
  std::vector<SizeType>
  get_opr(
    const SatModel& model ///< [in] SATモデル
  ) const override;

  /// @breif step 番目の入力(演算)を使用している時 true となるリテラルを追加する．
  ///
  /// 入力の場合，入力番号が step になる．
  /// 演算の場合，演算番号 + 入力数が step になる．
  void
  add_use_lit(
    SizeType step,                    ///< [in] ステップ番号
    std::vector<SatLiteral>& lit_list ///< [out] 格納先の変数
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
  SatOrderedSet mSel0;

};

END_NAMESPACE_YM_EXSYN

#endif // PDNODE2_1_OSV_H
