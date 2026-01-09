#ifndef PDNODE2_1_H
#define PDNODE2_1_H

/// @file PdNode2_1.h
/// @brief PdNode2_1 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PdNode2.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class PdNode2_1 PdNode2_1.h "PdNode2_1.h"
/// @brief オペランド1が固定している PdNode2 の派生クラス
//////////////////////////////////////////////////////////////////////
class PdNode2_1 :
  public PdNode2
{
public:

  /// @brief コンストラクタ
  PdNode2_1(
    SatSolver& solver,   ///< [in] SATソルバ
    SizeType input_num,  ///< [in] 入力数
    SizeType op_id,      ///< [in] 演算ノード番号
    SizeType opr1        ///< [in] オペランド1
  );

  /// @brief デストラクタ
  ~PdNode2_1() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief オペランド1の値を返す．
  SizeType
  opr1_val() const
  {
    return mOpr1;
  }

  /// @brief op_id 番目の演算を確定で使用している時 true を返す．
  bool
  use(
    SizeType op_id ///< [in] 演算番号
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オペランド1の割り当て結果
  SizeType mOpr1;

};

END_NAMESPACE_YM_EXSYN

#endif // PDNODE2_1_H
