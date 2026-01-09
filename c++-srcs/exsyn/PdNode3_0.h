#ifndef PDNODE3_0_H
#define PDNODE3_0_H

/// @file PdNode3_0.h
/// @brief PdNode3_0 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PdNode3.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class PdNode3_0 PdNode3_0.h "PdNode3_0.h"
/// @brief 3つのオペランドが固定している PdNode3 の継承クラス
//////////////////////////////////////////////////////////////////////
class PdNode3_0 :
  public PdNode3
{
public:

  /// @brief コンストラクタ
  PdNode3_0(
    SatSolver& solver,   ///< [in] SATソルバ
    SizeType input_num,  ///< [in] 入力数
    SizeType op_id,      ///< [in] 演算ノード番号
    SizeType opr0,       ///< [in] オペランド0
    SizeType opr1,       ///< [in] オペランド1
    SizeType opr2        ///< [in] オペランド2
  );

  /// @brief デストラクタ
  ~PdNode3_0() = default;


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

  /// @brief オペランド0の値を返す．
  SizeType
  opr0_val() const
  {
    return mOpr0;
  }

  /// @brief オペランド1の値を返す．
  SizeType
  opr1_val() const
  {
    return mOpr1;
  }

  /// @brief オペランド2の値を返す．
  SizeType
  opr2_val() const
  {
    return mOpr2;
  }

  /// @brief 直前の演算ノードに対する制約を追加する．
  void
  add_op_order_cond(
    SatSolver& solver, ///< [in] SATソルバ
    const OpNode* op   ///< [in] 直前の演算ノード
  ) const override;

  /// @brief op_id 番目の演算を確定で使用している時 true を返す．
  bool
  use(
    SizeType op_id ///< [in] 演算番号
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

  // オペランド0の割り当て結果
  SizeType mOpr0;

  // オペランド1の割り当て結果
  SizeType mOpr1;

  // オペランド2の割り当て結果
  SizeType mOpr2;

};

END_NAMESPACE_YM_EXSYN

#endif // PDNODE3_0_H
