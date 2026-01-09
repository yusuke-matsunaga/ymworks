#ifndef PDNODE3_1_H
#define PDNODE3_1_H

/// @file PdNode3_1.h
/// @brief PdNode3_1 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PdNode3.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class PdNode3_1 PdNode3_1.h "PdNode3_1.h"
/// @brief オペランド1とオペランド2が確定している PdNode3 の派生クラス
//////////////////////////////////////////////////////////////////////
class PdNode3_1 :
  public PdNode3
{
public:

  /// @brief コンストラクタ
  PdNode3_1(
    SatSolver& solver,   ///< [in] SATソルバ
    SizeType input_num,  ///< [in] 入力数
    SizeType op_id,      ///< [in] 演算ノード番号
    SizeType opr1,       ///< [in] オペランド1
    SizeType opr2        ///< [in] オペランド2
  );

  /// @brief デストラクタ
  ~PdNode3_1() = default;


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

  /// @brief オペランド2の値を返す．
  SizeType
  opr2_val() const
  {
    return mOpr2;
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

  // オペランド2の割り当て結果
  SizeType mOpr2;


};

END_NAMESPACE_YM_EXSYN

#endif // PDNODE3_1_H
