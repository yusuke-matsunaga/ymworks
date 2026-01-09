#ifndef PDNODE4_3_SSV_H
#define PDNODE4_3_SSV_H

/// @file PdNode4_3_SSV.h
/// @brief PdNode4_3_SSV のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PdNode4_3.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class PdNode4_3_SSV PdNode4_3_SSV.h "PdNode4_3_SSV.h"
/// @brief
//////////////////////////////////////////////////////////////////////
class PdNode4_3_SSV :
  public PdNode4_3
{
public:

  /// @brief コンストラクタ
  PdNode4_3_SSV(
    SatSolver& solver,   ///< [in] SATソルバ
    SizeType input_num,  ///< [in] 入力数
    SizeType op_id,      ///< [in] 演算ノード番号
    SizeType opr3        ///< [in] オペランド3
  );

  /// @brief デストラクタ
  ~PdNode4_3_SSV() = default;


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

  /// @brief opr0, opr1, opr2 を選択している時 true になるリテラルを返す．
  SatLiteral
  sel_lit(
    SizeType opr0,
    SizeType opr1,
    SizeType opr2
  ) const
  {
    return mLitArray[(opr0 * input_num() + opr1) * input_num() + opr2];
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // (i1 * input_num() * input_num()) + (i2 * input_num()) + i3
  // の位置に変数を格納した配列
  // i1 < i2 < i3 なので未使用の領域がある．
  std::vector<SatLiteral> mLitArray;

  // 変数のリスト
  std::vector<SatLiteral> mLitList;

};

END_NAMESPACE_YM_EXSYN

#endif // PDNODE4_3_SSV_H
