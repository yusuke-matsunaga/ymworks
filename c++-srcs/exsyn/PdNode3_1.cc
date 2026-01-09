
/// @file PdNode3_1.cc
/// @brief PdNode3_1 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PdNode3_1.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス PdNode3_1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PdNode3_1::PdNode3_1(
  SatSolver& solver,
  SizeType input_num,
  SizeType op_id,
  SizeType opr1,
  SizeType opr2
) : PdNode3(solver, input_num, op_id),
    mOpr1{opr1},
    mOpr2{opr2}
{
}

// @brief op_id 番目の演算を確定で使用している時 true を返す．
bool
PdNode3_1::use(
  SizeType op_id
) const
{
  return opr1_val() == op_id || opr2_val() == op_id;
}

END_NAMESPACE_YM_EXSYN
