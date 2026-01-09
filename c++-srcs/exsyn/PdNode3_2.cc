
/// @file PdNode3_2.cc
/// @brief PdNode3_2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PdNode3_2.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス PdNode3_2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PdNode3_2::PdNode3_2(
  SatSolver& solver,
  SizeType input_num,
  SizeType op_id,
  SizeType opr2
) : PdNode3(solver, input_num, op_id),
    mOpr2{opr2}
{
}

// @brief op_id 番目の演算を確定で使用している時 true を返す．
bool
PdNode3_2::use(
  SizeType op_id ///< [in] 演算番号
) const
{
  return opr2_val() == op_id;
}

END_NAMESPACE_YM_EXSYN
