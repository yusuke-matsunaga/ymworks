#ifndef PDNODE4_H
#define PDNODE4_H

/// @file PdNode4.h
/// @brief PdNode4 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "OpNode4.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class PdNode4 PdNode4.h "PdNode4.h"
/// @brief 4項演算の OpNode
///
/// 4項演算の4つのオペランド情報と演算タイプの情報を持つ．
/// オペランドは固定の場合と複数の候補の中から選択する場合がある．
/// 基本的に関数(演算)はすべて正規化されているので(0, 0, 0, 0)に対する
/// 出力値は 0 となっていて残りの15ビットに自由度がある．
//////////////////////////////////////////////////////////////////////
class PdNode4 :
public OpNode4
{
public:

  /// @brief コンストラクタ
  PdNode4(
    SatSolver& solver,  ///< [in] SATソルバ
    SizeType input_num, ///< [in] 入力数
    SizeType op_id      ///< [in] 演算ノード番号
  ) : OpNode4(solver, input_num, op_id)
  {
  }

  /// @brief デストラクタ
  virtual
  ~PdNode4() = default;

  /// @brief 実際の継承クラスを生成するクラスメソッド
  ///
  /// opr0, opr1, opr2, opr3 がそれぞれ -1 の時は入力から
  /// の選択を表す．
  /// 非負の整数の場合には固定した演算番号を表す．
  static
  OpNode*
  new_op(
    SatSolver& solver,      ///< [in] SATソルバ
    SizeType input_num,     ///< [in] 入力数
    SizeType op_id,         ///< [in] 演算ノード番号
    int opr0,               ///< [in] オペランド0
    int opr1,               ///< [in] オペランド1
    int opr2,               ///< [in] オペランド2
    int opr3,               ///< [in] オペランド3
    const JsonValue& option ///< [in] オプション
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

};

END_NAMESPACE_YM_EXSYN

#endif // PDNODE4_H
