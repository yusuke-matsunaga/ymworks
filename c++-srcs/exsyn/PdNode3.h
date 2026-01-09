#ifndef PDNODE3_H
#define PDNODE3_H

/// @file PdNode3.h
/// @brief PdNode3 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "OpNode3.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class PdNode3 PdNode3.h "PdNode3.h"
/// @brief 三項演算の OpNode
///
/// 三項演算の3つのオペランド情報と演算タイプの情報を持つ．
/// オペランドは固定の場合と複数の候補の中から選択する場合がある．
/// 基本的に関数(演算)はすべて正規化されているので(0, 0, 0)に対する
/// 出力値は 0 となっていて残りの7ビットに自由度がある．
//////////////////////////////////////////////////////////////////////
class PdNode3 :
public OpNode3
{
public:

  /// @brief コンストラクタ
  PdNode3(
    SatSolver& solver,  ///< [in] SATソルバ
    SizeType input_num, ///< [in] 入力数
    SizeType op_id      ///< [in] 演算ノード番号
  ) : OpNode3(solver, input_num, op_id)
  {
  }

  /// @brief デストラクタ
  virtual
  ~PdNode3() = default;

  /// @brief 実際の継承クラスを生成するクラスメソッド
  ///
  /// opr0, opr1, opr2 がそれぞれ -1 の時は入力からの選択
  /// を表す．非負の整数の場合には固定した演算番号を表す．
  static
  OpNode*
  new_op(
    SatSolver& solver,      ///< [in] SATソルバ
    SizeType input_num,     ///< [in] 入力数
    SizeType op_id,         ///< [in] 演算ノード番号
    int opr0,               ///< [in] オペランド0
    int opr1,               ///< [in] オペランド1
    int opr2,               ///< [in] オペランド2
    const JsonValue& option ///< [in] オプション
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

};

END_NAMESPACE_YM_EXSYN

#endif // PDNODE3_H
