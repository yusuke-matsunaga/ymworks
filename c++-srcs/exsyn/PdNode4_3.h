#ifndef PDNODE4_3_H
#define PDNODE4_3_H

/// @file PdNode4_3.h
/// @brief PdNode4_3 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PdNode4.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class PdNode4_3 PdNode4_3.h "PdNode4_3.h"
/// @brief オペランド3が確定している PdNode4 の派生クラス
//////////////////////////////////////////////////////////////////////
class PdNode4_3 :
  public PdNode4
{
public:

  /// @brief コンストラクタ
  PdNode4_3(
    SatSolver& solver,   ///< [in] SATソルバ
    SizeType input_num,  ///< [in] 入力数
    SizeType op_id,      ///< [in] 演算ノード番号
    SizeType opr3        ///< [in] オペランド3
  );

  /// @brief デストラクタ
  ~PdNode4_3() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

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

  // オペランド3の割り当て結果
  SizeType mOpr3;

};

END_NAMESPACE_YM_EXSYN

#endif // PDNODE4_3_H
