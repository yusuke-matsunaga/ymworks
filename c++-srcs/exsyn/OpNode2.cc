
/// @file OpNode2.cc
/// @brief OpNode2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "OpNode2.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス OpNode2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
OpNode2::OpNode2(
  SatSolver& solver,
  SizeType input_num,
  SizeType op_id
) : OpNode(input_num, op_id),
    mTemplate(solver)
{
}

// @brief SATモデルから演算タイプを得る．
OpNode::MapInfo
OpNode2::get_map_info(
  const SatModel& model
) const
{
  return mTemplate.get_map_info(model);
}

END_NAMESPACE_YM_EXSYN
