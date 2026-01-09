
/// @file PdNode4_3.cc
/// @brief PdNode4_3 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PdNode4_3.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス PdNode4_3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PdNode4_3::PdNode4_3(
  SatSolver& solver,
  SizeType input_num,
  SizeType op_id,
  SizeType opr3
) : PdNode4(solver, input_num, op_id),
    mOpr3{opr3}
{
}

// @brief op_id 番目の演算を確定で使用している時 true を返す．
bool
PdNode4_3::use(
  SizeType op_id ///< [in] 演算番号
) const
{
  return opr3_val() == op_id;
}

END_NAMESPACE_YM_EXSYN
