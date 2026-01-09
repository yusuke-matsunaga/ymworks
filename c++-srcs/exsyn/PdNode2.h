#ifndef PDNODE2_H
#define PDNODE2_H

/// @file PdNode2.h
/// @brief PdNode2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "OpNode2.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class PdNode2 PdNode2.h "PdNode2.h"
/// @brief PD 用の OpNode2
///
/// 実際には特別なメンバを持たない．
/// new_op() のために存在する名前空間のようなクラス
//////////////////////////////////////////////////////////////////////
class PdNode2 :
  public OpNode2
{
public:

  /// @brief コンストラクタ
  PdNode2(
    SatSolver& solver,   ///< [in] SATソルバ
    SizeType input_num,  ///< [in] 入力数
    SizeType op_id       ///< [in] 演算ノード番号
  );

  /// @brief デストラクタ
  ~PdNode2() = default;

  /// @brief 実際の継承クラスを生成するクラスメソッド
  ///
  /// opr0, opr1 がそれぞれ -1 の時は入力からの選択を表す．
  /// 非負の整数の場合には固定した演算番号を表す．
  static
  OpNode*
  new_op(
    SatSolver& solver,      ///< [in] SATソルバ
    SizeType input_num,     ///< [in] 入力数
    SizeType op_id,         ///< [in] 演算ノード番号
    int opr0,               ///< [in] オペランド0
    int opr1,               ///< [in] オペランド1
    const JsonValue& option ///< [in] オプション
  );

};

END_NAMESPACE_YM_EXSYN

#endif // PDNODE2_H
