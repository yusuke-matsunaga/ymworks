
/// @file PdNode4.cc
/// @brief PdNode4 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PdNode4.h"
#include "PdNode4_0.h"
#include "PdNode4_1_OneHot.h"
#include "PdNode4_2_SSV.h"
#include "PdNode4_2_MSV.h"
#include "PdNode4_2_OSV.h"
#include "PdNode4_3_SSV.h"
#include "PdNode4_3_MSV.h"
#include "PdNode4_3_OSV.h"
#include "PdNode4_4_SSV.h"
#include "PdNode4_4_MSV.h"
#include "PdNode4_4_OSV.h"
#include "ym/SatSolver.h"
#include "get_opt.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス PdNode4
//////////////////////////////////////////////////////////////////////

// @brief 実際の継承クラスを生成するクラスメソッド
OpNode*
PdNode4::new_op(
  SatSolver& solver,
  SizeType input_num,
  SizeType op_id,
  int opr0,
  int opr1,
  int opr2,
  int opr3,
  const JsonValue& option
)
{
  auto selecter = get_string(option, "selecter", "OSV");
  if ( opr0 >= 0 ) {
    // オペランド0は固定
    // ということはすべてのオペランドが固定
    return new PdNode4_0(solver, input_num, op_id, opr0, opr1, opr2, opr3);
  }
  // opr0 == -1
  if ( opr1 >= 0 ) {
    // オペランド1は固定
    // ということはオペランド2，オペランド3も固定
    return new PdNode4_1_OneHot(solver, input_num, op_id, opr1, opr2, opr3);
  }
  // opr1 == -1
  else if ( opr2 >= 0 ) {
    // オペランド2，オペランド3は固定
    if ( selecter == "SSV" ) {
      return new PdNode4_2_SSV(solver, input_num, op_id, opr2, opr3);
    }
    if ( selecter == "MSV" ) {
      return new PdNode4_2_MSV(solver, input_num, op_id, opr2, opr3);
    }
    if ( selecter == "OSV" ) {
      return new PdNode4_2_OSV(solver, input_num, op_id, opr2, opr3);
    }
  }
  // opr2 == -1
  else if ( opr3 >= 0 ) {
    // オペランド3は固定
    if ( selecter == "SSV" ) {
      return new PdNode4_3_SSV(solver, input_num, op_id, opr3);
    }
    if ( selecter == "MSV" ) {
      return new PdNode4_3_MSV(solver, input_num, op_id, opr3);
    }
    if ( selecter == "OSV" ) {
      return new PdNode4_3_OSV(solver, input_num, op_id, opr3);
    }
  }
  // opr3 == -1
  else {
    // すべてのオペランドが可変
    if ( selecter == "SSV" ) {
      return new PdNode4_4_SSV(solver, input_num, op_id);
    }
    if ( selecter == "MSV" ) {
      return new PdNode4_4_MSV(solver, input_num, op_id);
    }
    if ( selecter == "OSV" ) {
      return new PdNode4_4_OSV(solver, input_num, op_id);
    }
  }
  std::ostringstream buf;
  buf << selecter << ": Unknown selecter name";
  throw std::invalid_argument{buf.str()};
}

END_NAMESPACE_YM_EXSYN
