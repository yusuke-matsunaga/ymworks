#ifndef PDNODE4_2_H
#define PDNODE4_2_H

/// @file PdNode4_2.h
/// @brief PdNode4_2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PdNode4.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class PdNode4_2 PdNode4_2.h "PdNode4_2.h"
/// @brief オペランド2が確定している PdNode4 の派生クラス
//////////////////////////////////////////////////////////////////////
class PdNode4_2 :
  public PdNode4
{
public:

  /// @brief コンストラクタ
  PdNode4_2(
    SatSolver& solver,   ///< [in] SATソルバ
    SizeType input_num,  ///< [in] 入力数
    SizeType op_id,      ///< [in] 演算ノード番号
    SizeType opr2,       ///< [in] オペランド2
    SizeType opr3        ///< [in] オペランド3
  );

  /// @brief デストラクタ
  ~PdNode4_2() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief オペランド2の値を返す．
  SizeType
  opr2_val() const
  {
    return mOpr2;
  }

  /// @brief オペランド3の値を返す．
  SizeType
  opr3_val() const
  {
    return mOpr3;
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

  // オペランド2の割り当て結果
  SizeType mOpr2;

  // オペランド3の割り当て結果
  SizeType mOpr3;

};

END_NAMESPACE_YM_EXSYN

#endif // PDNODE4_2_H
