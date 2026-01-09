
/// @file Engine_Naive.cc
/// @brief Engine_Naive の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "Engine_Naive.h"
#include "OpNode2_SSV.h"
#include "OpNode2_MSV.h"
#include "OpNode2_OSV.h"
#if 0
#include "OpNode3_SSV.h"
#include "OpNode3_OSV.h"
#endif
#include "get_opt.h"


BEGIN_NAMESPACE_YM_EXSYN

// @brief コンストラクタ
Engine_Naive::Engine_Naive(
  const TruthTable& func,
  SizeType op_num,
  SizeType max_input,
  const JsonValue& option
) : EngineBase(func, op_num, option),
    mMaxInput{max_input}
{
}

// @brief デストラクタ
Engine_Naive::~Engine_Naive()
{
}

// @brief 演算ノードと出力の制約を作る．
void
Engine_Naive::make_op_list()
{
  // 演算ノードを作る．
  auto selecter = get_string(option(), "selecter", "OSV");
  switch ( mMaxInput ) {
  case 2:
    if ( selecter == "SSV" ) {
      for ( SizeType op_id = 0; op_id < op_num(); ++ op_id ) {
	SizeType cand_num = input_num() + op_id;
	auto op = new OpNode2_SSV(solver(), input_num(), op_id, cand_num);
	set_op(op_id, op);
      }
    }
    else if ( selecter == "MSV" ) {
      for ( SizeType op_id = 0; op_id < op_num(); ++ op_id ) {
	SizeType cand_num = input_num() + op_id;
	auto op = new OpNode2_MSV(solver(), input_num(), op_id, cand_num);
	set_op(op_id, op);
      }
    }
    else if ( selecter == "OSV" ) {
      for ( SizeType op_id = 0; op_id < op_num(); ++ op_id ) {
	SizeType cand_num = input_num() + op_id;
	auto op = new OpNode2_OSV(solver(), input_num(), op_id, cand_num);
	set_op(op_id, op);
      }
    }
    else {
      std::ostringstream buf;
      buf << selecter << ": illegal selecter option for "
	  << mMaxInput << " operands";
      throw std::invalid_argument{buf.str()};
    }
    break;
#if 0
  case 3:
    if ( selecter == "SSV" ) {
      for ( SizeType op_id = 0; op_id < op_num(); ++ op_id ) {
	SizeType cand_num = input_num() + op_id;
	auto op = new OpNode3_SSV(solver(), input_num(), op_id, cand_num);
	set_op(op_id, op);
      }
    }
    else if ( selecter == "MSV" ) {
      for ( SizeType op_id = 0; op_id < op_num(); ++ op_id ) {
	SizeType cand_num = input_num() + op_id;
	auto op = new OpNode3_SSV(solver(), input_num(), op_id, cand_num);
	set_op(op_id, op);
      }
    }
    else if ( selecter == "OSV" ) {
      for ( SizeType op_id = 0; op_id < op_num(); ++ op_id ) {
	SizeType cand_num = input_num() + op_id;
	auto op = new OpNode3_OSV(solver(), input_num(), op_id, cand_num);
	set_op(op_id, op);
      }
    }
    else {
      std::ostringstream buf;
      buf << selecter << ": illegal selecter option for "
	  << mMaxInput << " operands";
      throw std::invalid_argument{buf.str()};
    }
    break;
#endif
  }
}

// @brief use_all_inputs 制約を作る．
void
Engine_Naive::add_use_all_inputs()
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
Engine_Naive::add_use_all_steps()
{
  auto end = op_num();
  if ( output_num() == 1 ) {
    // 1出力の場合は最後の演算を必ず使う．
    -- end;
  }
  SizeType node_num = input_num() + op_num();
  for ( SizeType op_id = 0; op_id < end; ++ op_id ) {
    // op_id 番目の演算を使う他の演算 or 出力が存在する．
    std::vector<SatLiteral> tmp_lits;
    tmp_lits.reserve(node_num - op_id + output_num());
    for ( SizeType op_id2 = op_id + 1; op_id2 < op_num(); ++ op_id2 ) {
      auto op2 = get_op(op_id2);
      op2->add_use_lit(op_id + input_num(), tmp_lits);
    }
    if ( output_num() > 1 ) {
      add_ref_cond(op_id, tmp_lits);
    }
    // 使用される条件を追加する．
    solver().add_clause(tmp_lits);
  }
}

// @brief no_reapplication 制約を作る．
void
Engine_Naive::add_no_reapplication()
{
  // i 番目の演算の入力(の一方)と i を入力にする他の演算を禁止する．
  for ( SizeType op1_id = 0; op1_id < op_num(); ++ op1_id ) {
    auto op1 = get_op(op1_id);
    for ( SizeType op2_id = op1_id + 1; op2_id < op_num(); ++ op2_id ) {
      auto op2 = get_op(op2_id);
      op1->add_no_reapplication_cond(solver(), op2);
    }
  }
}

// @brief op_order 制約を作る．
void
Engine_Naive::add_op_order()
{
  // 入力を辞書式順序で比較する．
  if ( op_num() > 1 ) {
    // 入力を辞書式順序で比較する．
    SizeType n = op_num() - 1;
    for ( SizeType op_id = 1; op_id < op_num(); ++ op_id ) {
      auto op1 = get_op(op_id - 1);
      auto op2 = get_op(op_id);
      op1->add_op_order_cond(solver(), op2);
    }
  }
}

END_NAMESPACE_YM_EXSYN
