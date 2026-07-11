
/// @file PdNode2.cc
/// @brief PdNode2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PdNode2.h"
#include "PdNode2_0.h"
#include "PdNode2_1_OneHot.h"
#include "PdNode2_1_OSV.h"
#include "PdNode2_1_OSV2.h"
#include "PdNode2_2_SSV.h"
#include "PdNode2_2_MSV.h"
#include "PdNode2_2_OSV.h"
#include "PdNode2_2_OSSV.h"
#include "PdNode2_2_OSV2.h"
#include "get_opt.h"


BEGIN_NAMESPACE_YM_EXSYN

// @brief コンストラクタ
PdNode2::PdNode2(
  SatSolver& solver,
  SizeType input_num,
  SizeType op_id
) : OpNode2(solver, input_num, op_id)
{
}

// @brief 実際の継承クラスを生成するクラスメソッド
OpNode*
PdNode2::new_op(
  SatSolver& solver,
  SizeType input_num,
  SizeType op_id,
  int opr0,
  int opr1,
  const JsonValue& option
)
{
  auto selecter = option.get_string_elem("selecter", "OSV");
  if ( opr0 >= 0 ) {
    // オペランド0は固定
    // ということはオペランド1も固定
    return new PdNode2_0(solver, input_num, op_id, opr0, opr1);
  }
  // opr0 == -1
  else if ( opr1 >= 0 ) {
    // オペランド0は入力からの選択
    // オペランド1は固定
    if ( selecter == "SSV" ) {
      return new PdNode2_1_OneHot(solver, input_num, op_id, opr1);
    }
    if ( selecter == "MSV" ) {
      return new PdNode2_1_OneHot(solver, input_num, op_id, opr1);
    }
    if ( selecter == "OSV" ) {
      return new PdNode2_1_OSV(solver, input_num, op_id, opr1);
    }
    if ( selecter == "OSSV" ) {
      return new PdNode2_1_OSV(solver, input_num, op_id, opr1);
    }
    if ( selecter == "OSV2" ) {
      return new PdNode2_1_OSV2(solver, input_num, op_id, opr1);
    }
  }
  else {
    // オペランド0，オペランド1は入力からの選択
    if ( selecter == "SSV" ) {
      return new PdNode2_2_SSV(solver, input_num, op_id);
    }
    if ( selecter == "MSV" ) {
      return new PdNode2_2_MSV(solver, input_num, op_id);
    }
    if ( selecter == "OSV" ) {
      return new PdNode2_2_OSV(solver, input_num, op_id);
    }
    if ( selecter == "OSSV" ) {
      return new PdNode2_2_OSV(solver, input_num, op_id);
    }
    if ( selecter == "OSV2" ) {
      return new PdNode2_2_OSV2(solver, input_num, op_id);
    }
  }
  std::ostringstream buf;
  buf << selecter << ": Unknown selecter name";
  throw std::invalid_argument{buf.str()};
}

END_NAMESPACE_YM_EXSYN
