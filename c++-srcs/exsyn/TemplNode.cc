
/// @file TemplNode.cc
/// @brief TemplNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "TemplNode.h"
#include "TemplNode2.h"
#include "TemplNode3.h"
#include "TemplNode4.h"


BEGIN_NAMESPACE_YM_EXSYN

// @brief 実際の継承クラスを作る．
TemplNode*
TemplNode::new_obj(
  SatSolver& solver,
  SizeType opr_num
)
{
  switch ( opr_num ) {
  case 2: return new TemplNode2(solver);
  case 3: return new TemplNode3(solver);
  case 4: return new TemplNode4(solver);
  }
  std::ostringstream buf;
  buf << "opr_num is too BIG";
  throw std::invalid_argument{buf.str()};
}

END_NAMESPACE_YM_EXSYN
