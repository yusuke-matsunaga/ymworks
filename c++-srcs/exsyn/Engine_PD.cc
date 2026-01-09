
/// @file Engine_PD.cc
/// @brief Engine_PD の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "Engine_PD.h"
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
#include "PdNode3_0.h"
#include "PdNode3_1_OneHot.h"
#include "PdNode3_2_SSV.h"
#include "PdNode3_2_MSV.h"
#include "PdNode3_2_OSV.h"
#include "PdNode3_3_SSV.h"
#include "PdNode3_3_MSV.h"
#include "PdNode3_3_OSV.h"
#include "PdNode4.h"
#include "PdNode2_1.h"
#include "PdNode3_1.h"
#include "PdNode3_2.h"
#include "get_opt.h"


BEGIN_NAMESPACE_YM_EXSYN

// @brief コンストラクタ
Engine_PD::Engine_PD(
  const TruthTable& func,
  const PD& pd,
  const JsonValue& option
) : EngineBase(func, pd.op_num(), option),
    mPD{pd}
{
}

// @brief デストラクタ
Engine_PD::~Engine_PD()
{
}

// @brief 演算ノードと出力の制約を作る．
void
Engine_PD::make_op_list()
{
  // 演算ノードを作る．
  auto selecter = get_string(option(), "selecter", "OSV");
  if ( selecter == "SSV" ) {
    gen_op_list_SSV();
  }
  else if ( selecter == "MSV" ) {
    gen_op_list_MSV();
  }
  else if ( selecter == "OSV" ) {
    gen_op_list_OSV();
  }
  else if ( selecter == "SMART" ) {
    gen_op_list_SMART();
  }
#if 0
  else if ( selecter == "SMART2" ) {
    gen_op_list_SMART2();
  }
#endif
  else {
    std::ostringstream buf;
    buf << selecter << ": Unknown selecter name";
    throw std::invalid_argument{buf.str()};
  }
}

// @brief use_all_inputs 制約を作る．
void
Engine_PD::add_use_all_inputs()
{
  for ( SizeType i = 0; i < input_num(); ++ i ) {
    std::vector<SatLiteral> tmp_lits;
    for ( SizeType op_id = 0; op_id < op_num(); ++ op_id ) {
      auto op = get_op(op_id);
      op->add_use_lit(i, tmp_lits);
    }
    solver().add_clause(tmp_lits);
  }
}

// @brief use_all_steps 制約を作る．
void
Engine_PD::add_use_all_steps()
{
  // 1出力の場合は既に成り立っている．
  if ( output_num() > 1 ) {
    SizeType node_num = input_num() + op_num();
    for ( SizeType op_id = 0; op_id < op_num(); ++ op_id ) {
      // op_id 番目の演算を使う他の演算 or 出力が存在する．
      // 演算に関しては使用する演算は固定
      bool used = false;
      for ( SizeType op_id2 = op_id + 1; op_id2 < op_num(); ++ op_id2 ) {
	auto op2 = get_op(op_id2);
	if ( op2->use(op_id) ) {
	  used = true;
	  break;
	}
      }
      if ( !used ) {
	// 使用される条件を追加する．
	std::vector<SatLiteral> tmp_lits;
	tmp_lits.reserve(output_num());
	add_ref_cond(op_id, tmp_lits);
	solver().add_clause(tmp_lits);
      }
    }
  }
}

// @brief no_reapplication 制約を作る．
void
Engine_PD::add_no_reapplication()
{
  // i 番目の演算の入力(の一方)と i を入力にする他の演算を禁止する．
  for ( SizeType op1_id = 0; op1_id < op_num(); ++ op1_id ) {
    auto op1 = get_op(op1_id);
    for ( SizeType op2_id = op1_id + 1; op2_id < op_num(); ++ op2_id ) {
      auto op2 = get_op(op2_id);
      op2->add_no_reapplication_cond(solver(), op2);
    }
  }
}

// @brief op_order 制約を作る．
void
Engine_PD::add_op_order()
{
  // 入力を辞書式順序で比較する．

  // レベル0 のグループ間の順序関係
  add_op_order_sub(mPD.group2_2());
  add_op_order_sub(mPD.group3_3());

  // レベル1以降のグループ間の順序関係
  for ( auto& group1: mPD.group2_1_list() ) {
    add_op_order_sub(group1);
  }
  for ( auto& group1: mPD.group3_1_list() ) {
    add_op_order_sub(group1);
  }
  for ( auto& group1: mPD.group3_2_list() ) {
    add_op_order_sub(group1);
  }
}

void
Engine_PD::gen_op_list_SSV()
{
  for ( auto op_id: mPD.group2_0() ) {
    auto& opr = mPD.opr(op_id);
    auto opr0 = opr[0];
    auto opr1 = opr[1];
    auto op = new PdNode2_0(solver(), input_num(), op_id, opr0, opr1);
    set_op(op_id, op);
  }
  for ( auto& group: mPD.group2_1_list() ) {
    for ( auto op_id: group ) {
      auto& opr = mPD.opr(op_id);
      auto opr1 = opr[1];
      auto op = new PdNode2_1_OneHot(solver(), input_num(), op_id, opr1);
      set_op(op_id, op);
    }
  }
  for ( auto op_id: mPD.group2_2() ) {
    auto op = new PdNode2_2_SSV(solver(), input_num(), op_id);
    set_op(op_id, op);
  }
  for ( auto op_id: mPD.group3_0() ) {
    auto& opr = mPD.opr(op_id);
    auto opr0 = opr[0];
    auto opr1 = opr[1];
    auto opr2 = opr[2];
    auto op = new PdNode3_0(solver(), input_num(), op_id, opr0, opr1, opr2);
    set_op(op_id, op);
  }
  for ( auto& group: mPD.group3_1_list() ) {
    for ( auto op_id: group ) {
      auto& opr = mPD.opr(op_id);
      auto opr1 = opr[1];
      auto opr2 = opr[2];
      auto op = new PdNode3_1_OneHot(solver(), input_num(), op_id, opr1, opr2);
      set_op(op_id, op);
    }
  }
  for ( auto& group: mPD.group3_2_list() ) {
    for ( auto op_id: group ) {
      auto& opr = mPD.opr(op_id);
      auto opr2 = opr[2];
      auto op = new PdNode3_2_SSV(solver(), input_num(), op_id, opr2);
      set_op(op_id, op);
    }
  }
  for ( auto op_id: mPD.group3_3() ) {
    auto op = new PdNode3_3_SSV(solver(), input_num(), op_id);
    set_op(op_id, op);
  }
}

void
Engine_PD::gen_op_list_MSV()
{
  for ( auto op_id: mPD.group2_0() ) {
    auto& opr = mPD.opr(op_id);
    auto opr0 = opr[0];
    auto opr1 = opr[1];
    auto op = new PdNode2_0(solver(), input_num(), op_id, opr0, opr1);
    set_op(op_id, op);
  }
  for ( auto& group: mPD.group2_1_list() ) {
    for ( auto op_id: group ) {
      auto& opr = mPD.opr(op_id);
      auto opr1 = opr[1];
      auto op = new PdNode2_1_OneHot(solver(), input_num(), op_id, opr1);
      set_op(op_id, op);
    }
  }
  for ( auto op_id: mPD.group2_2() ) {
    auto op = new PdNode2_2_MSV(solver(), input_num(), op_id);
    set_op(op_id, op);
  }
  for ( auto op_id: mPD.group3_0() ) {
    auto& opr = mPD.opr(op_id);
    auto opr0 = opr[0];
    auto opr1 = opr[1];
    auto opr2 = opr[2];
    auto op = new PdNode3_0(solver(), input_num(), op_id, opr0, opr1, opr2);
    set_op(op_id, op);
  }
  for ( auto& group: mPD.group3_1_list() ) {
    for ( auto op_id: group ) {
      auto& opr = mPD.opr(op_id);
      auto opr1 = opr[1];
      auto opr2 = opr[2];
      auto op = new PdNode3_1_OneHot(solver(), input_num(), op_id, opr1, opr2);
      set_op(op_id, op);
    }
  }
  for ( auto& group: mPD.group3_2_list() ) {
    for ( auto op_id: group ) {
      auto& opr = mPD.opr(op_id);
      auto opr2 = opr[2];
      auto op = new PdNode3_2_MSV(solver(), input_num(), op_id, opr2);
      set_op(op_id, op);
    }
  }
  for ( auto op_id: mPD.group3_3() ) {
    auto op = new PdNode3_3_MSV(solver(), input_num(), op_id);
    set_op(op_id, op);
  }
}

void
Engine_PD::gen_op_list_OSV()
{
  for ( auto op_id: mPD.group2_0() ) {
    auto& opr = mPD.opr(op_id);
    auto opr0 = opr[0];
    auto opr1 = opr[1];
    auto op = new PdNode2_0(solver(), input_num(), op_id, opr0, opr1);
    set_op(op_id, op);
  }
  for ( auto& group: mPD.group2_1_list() ) {
    for ( auto op_id: group ) {
      auto& opr = mPD.opr(op_id);
      auto opr1 = opr[1];
      auto op = new PdNode2_1_OneHot(solver(), input_num(), op_id, opr1);
      set_op(op_id, op);
    }
  }
  for ( auto op_id: mPD.group2_2() ) {
    auto op = new PdNode2_2_OSV(solver(), input_num(), op_id);
    set_op(op_id, op);
  }
  for ( auto op_id: mPD.group3_0() ) {
    auto& opr = mPD.opr(op_id);
    auto opr0 = opr[0];
    auto opr1 = opr[1];
    auto opr2 = opr[2];
    auto op = new PdNode3_0(solver(), input_num(), op_id, opr0, opr1, opr2);
    set_op(op_id, op);
  }
  for ( auto& group: mPD.group3_1_list() ) {
    for ( auto op_id: group ) {
      auto& opr = mPD.opr(op_id);
      auto opr1 = opr[1];
      auto opr2 = opr[2];
      auto op = new PdNode3_1_OneHot(solver(), input_num(), op_id, opr1, opr2);
      set_op(op_id, op);
    }
  }
  for ( auto& group: mPD.group3_2_list() ) {
    for ( auto op_id: group ) {
      auto& opr = mPD.opr(op_id);
      auto opr2 = opr[2];
      auto op = new PdNode3_2_OSV(solver(), input_num(), op_id, opr2);
      set_op(op_id, op);
    }
  }
  for ( auto op_id: mPD.group3_3() ) {
    auto op = new PdNode3_3_OSV(solver(), input_num(), op_id);
    set_op(op_id, op);
  }
}

void
Engine_PD::gen_op_list_SMART()
{
  for ( auto op_id: mPD.group2_0() ) {
    auto& opr = mPD.opr(op_id);
    auto opr0 = opr[0];
    auto opr1 = opr[1];
    auto op = new PdNode2_0(solver(), input_num(), op_id, opr0, opr1);
    set_op(op_id, op);
  }
  for ( auto& group: mPD.group2_1_list() ) {
    for ( auto op_id: group ) {
      auto& opr = mPD.opr(op_id);
      auto opr1 = opr[1];
      auto op = new PdNode2_1_OneHot(solver(), input_num(), op_id, opr1);
      set_op(op_id, op);
    }
  }
  if ( mPD.group2_2().size() >= 2 ) {
    for ( auto op_id: mPD.group2_2() ) {
      auto op = new PdNode2_2_OSV(solver(), input_num(), op_id);
      set_op(op_id, op);
    }
  }
  else {
    for ( auto op_id: mPD.group2_2() ) {
      auto op = new PdNode2_2_MSV(solver(), input_num(), op_id);
      set_op(op_id, op);
    }
  }
  for ( auto op_id: mPD.group3_0() ) {
    auto& opr = mPD.opr(op_id);
    auto opr0 = opr[0];
    auto opr1 = opr[1];
    auto opr2 = opr[2];
    auto op = new PdNode3_0(solver(), input_num(), op_id, opr0, opr1, opr2);
    set_op(op_id, op);
  }
  for ( auto& group: mPD.group3_1_list() ) {
    for ( auto op_id: group ) {
      auto& opr = mPD.opr(op_id);
      auto opr1 = opr[1];
      auto opr2 = opr[2];
      auto op = new PdNode3_1_OneHot(solver(), input_num(), op_id, opr1, opr2);
      set_op(op_id, op);
    }
  }
  if ( mPD.group3_2_list().size() >= 2 ) {
    for ( auto& group: mPD.group3_2_list() ) {
      for ( auto op_id: group ) {
	auto& opr = mPD.opr(op_id);
	auto opr2 = opr[2];
	auto op = new PdNode3_2_OSV(solver(), input_num(), op_id, opr2);
	set_op(op_id, op);
      }
    }
  }
  else {
    for ( auto& group: mPD.group3_2_list() ) {
      for ( auto op_id: group ) {
	auto& opr = mPD.opr(op_id);
	auto opr2 = opr[2];
	auto op = new PdNode3_2_MSV(solver(), input_num(), op_id, opr2);
	set_op(op_id, op);
      }
    }
  }
  if ( mPD.group3_3().size() >= 2 ) {
    for ( auto op_id: mPD.group3_3() ) {
      auto op = new PdNode3_3_OSV(solver(), input_num(), op_id);
      set_op(op_id, op);
    }
  }
  else {
    for ( auto op_id: mPD.group3_3() ) {
      auto op = new PdNode3_3_MSV(solver(), input_num(), op_id);
      set_op(op_id, op);
    }
  }
}

#if 0
void
Engine_PD::gen_op_list_SMART2()
{
  for ( auto op_id: mPD.group2_0() ) {
    auto& opr = mPD.opr(op_id);
    auto opr0 = opr[0];
    auto opr1 = opr[1];
    auto op = new PdNode2_0(solver(), input_num(), op_id, opr0, opr1);
    set_op(op_id, op);
  }
  for ( auto& group: mPD.group2_1_list() ) {
    for ( auto op_id: group ) {
      auto& opr = mPD.opr(op_id);
      auto opr1 = opr[1];
      auto op = new PdNode2_1_OneHot(solver(), input_num(), op_id, opr1);
      set_op(op_id, op);
    }
  }
  if ( mPD.group2_2().size() >= 2 ) {
    for ( auto op_id: mPD.group2_2() ) {
      auto op = new PdNode2_2_OSV(solver(), input_num(), op_id);
      set_op(op_id, op);
    }
  }
  else {
    for ( auto op_id: mPD.group2_2() ) {
      auto op = new PdNode2_2_SSV(solver(), input_num(), op_id);
      set_op(op_id, op);
    }
  }
  for ( auto op_id: mPD.group3_0() ) {
    auto& opr = mPD.opr(op_id);
    auto opr0 = opr[0];
    auto opr1 = opr[1];
    auto opr2 = opr[2];
    auto op = new PdNode3_0(solver(), input_num(), op_id, opr0, opr1, opr2);
    set_op(op_id, op);
  }
  for ( auto& group: mPD.group3_1_list() ) {
    for ( auto op_id: group ) {
      auto& opr = mPD.opr(op_id);
      auto opr1 = opr[1];
      auto opr2 = opr[2];
      auto op = new PdNode3_1_OneHot(solver(), input_num(), op_id, opr1, opr2);
      set_op(op_id, op);
    }
  }
  if ( mPD.group3_2_list().size() >= 2 ) {
    for ( auto& group: mPD.group3_2_list() ) {
      for ( auto op_id: group ) {
	auto& opr = mPD.opr(op_id);
	auto opr2 = opr[2];
	auto op = new PdNode3_2_OSV(solver(), input_num(), op_id, opr2);
	set_op(op_id, op);
      }
    }
  }
  else {
    for ( auto& group: mPD.group3_2_list() ) {
      for ( auto op_id: group ) {
	auto& opr = mPD.opr(op_id);
	auto opr2 = opr[2];
	auto op = new PdNode3_2_SSV(solver(), input_num(), op_id, opr2);
	set_op(op_id, op);
      }
    }
  }
  if ( mPD.group3_3().size() >= 2 ) {
    for ( auto op_id: mPD.group3_3() ) {
      auto op = new PdNode3_3_OSV(solver(), input_num(), op_id);
      set_op(op_id, op);
    }
  }
  else {
    for ( auto op_id: mPD.group3_3() ) {
      auto op = new PdNode3_3_SSV(solver(), input_num(), op_id);
      set_op(op_id, op);
    }
  }
}
#endif

void
Engine_PD::add_op_order_sub(
  const std::vector<SizeType>& group
)
{
  auto k = group.size();
  if ( k > 1 ) {
    for ( SizeType i = 0; i < k - 1; ++ i ) {
      auto op0 = get_op(group[i + 0]);
      auto op1 = get_op(group[i + 1]);
      op0->add_op_order_cond(solver(), op1);
    }
  }
}

END_NAMESPACE_YM_EXSYN
