
/// @file PdNode2_1.cc
/// @brief PdNode2_1 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PdNode2_1.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス PdNode2_1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PdNode2_1::PdNode2_1(
  SatSolver& solver,
  SizeType input_num,
  SizeType op_id,
  SizeType opr1
) : PdNode2(solver, input_num, op_id),
    mOpr1{opr1}
{
}

// @brief op_id 番目の演算を確定で使用している時 true を返す．
bool
PdNode2_1::use(
  SizeType op_id
) const
{
  return opr1_val() == op_id;
}

END_NAMESPACE_YM_EXSYN
