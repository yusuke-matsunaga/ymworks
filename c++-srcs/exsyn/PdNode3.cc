
/// @file PdNode3.cc
/// @brief PdNode3 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PdNode3.h"
#include "ym/SatSolver.h"
#include "PdNode3_0.h"
#include "PdNode3_1_OneHot.h"
#include "PdNode3_2_SSV.h"
#include "PdNode3_2_MSV.h"
#include "PdNode3_2_OSV.h"
#include "PdNode3_3_SSV.h"
#include "PdNode3_3_MSV.h"
#include "PdNode3_3_OSV.h"
#include "get_opt.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス PdNode3
//////////////////////////////////////////////////////////////////////

// @brief 実際の継承クラスを生成するクラスメソッド
OpNode*
PdNode3::new_op(
  SatSolver& solver,
  SizeType input_num,
  SizeType op_id,
  int opr0,
  int opr1,
  int opr2,
  const JsonValue& option
)
{
  auto selecter = option.get_string_elem("selecter", "OSV");
  if ( opr0 >= 0 ) {
    // オペランド0は固定
    // ということはすべてのオペランドが固定
    return new PdNode3_0(solver, input_num, op_id, opr0, opr1, opr2);
  }
  // opr0 == -1
  if ( opr1 >= 0 ) {
    // オペランド1は固定
    // ということはオペランド2も固定
    return new PdNode3_1_OneHot(solver, input_num, op_id, opr1, opr2);
  }
  // opr1 == -1
  else if ( opr2 >= 0 ) {
    // オペランド2は固定
    if ( selecter == "SSV" ) {
      return new PdNode3_2_SSV(solver, input_num, op_id, opr2);
    }
    if ( selecter == "MSV" ) {
      return new PdNode3_2_MSV(solver, input_num, op_id, opr2);
    }
    if ( selecter == "OSV" ) {
      return new PdNode3_2_OSV(solver, input_num, op_id, opr2);
    }
  }
  // opr2 == -1
  else {
    // すべてのオペランドが可変
    if ( selecter == "SSV" ) {
      return new PdNode3_3_SSV(solver, input_num, op_id);
    }
    if ( selecter == "MSV" ) {
      return new PdNode3_3_MSV(solver, input_num, op_id);
    }
    if ( selecter == "OSV" ) {
      return new PdNode3_3_OSV(solver, input_num, op_id);
    }
  }
  std::ostringstream buf;
  buf << selecter << ": Unknown selecter name";
  throw std::invalid_argument{buf.str()};
}

END_NAMESPACE_YM_EXSYN
