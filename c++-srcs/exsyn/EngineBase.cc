
/// @file EngineBase.cc
/// @brief EngineBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EngineBase.h"
#include "OpNode.h"
#include "MakeBcGraph.h"
#include "get_opt.h"


BEGIN_NAMESPACE_YM_EXSYN

// @brief コンストラクタ
EngineBase::EngineBase(
  const TruthTable& func,
  SizeType op_num,
  const JsonValue& option
) : mFunc{func},
    mOption{option},
    mSolver(sat_param(option)),
    mInputNum{func.input_num()},
    mOutputNum{func.output_num()},
    mOpList(op_num),
    mOutSelMatrix{mSolver.new_onehot_matrix(op_num, mOutputNum)},
    mOutInvArray(mOutputNum, SatLiteral::X)
{
  // 出力の選択変数の生成
  if ( output_num() == 1 ) {
    // 1出力の場合は最後の演算が出力になる．
    // ピボットを適切に選んでいるので bipol になることはない．
  }
  else {
    for ( SizeType o = 0; o < output_num(); ++ o ) {
      auto val0 = func.get_value(0, o);
      if ( val0 == Val3::X ) {
	mOutInvArray[o] = mSolver.new_variable(true);
      }
    }
  }
}

// @brief デストラクタ
EngineBase::~EngineBase()
{
}

// @brief 問題を解く
BcGraph
EngineBase::solve()
{
  make_op_list();

  if ( option().get_bool_elem("use_all_inputs", true) ) {
    add_use_all_inputs();
  }

  if ( option().get_bool_elem("use_all_steps", true) ) {
    add_use_all_steps();
  }

  if ( option().get_bool_elem("no_reapplication", true) ) {
    add_no_reapplication();
  }

  if ( option().get_bool_elem("op_order", true) ) {
    add_op_order();
  }

  // すべての入力に対する条件を追加する．
  auto ni_exp = 1 << input_num();
  for ( SizeType b = 1; b < ni_exp; ++ b ) {
    if ( !mSolver.sane() ) {
      // 失敗
      return BcGraph();
    }
    add_cond(b);
  }

  // SAT問題を解く
  auto res = mSolver.solve();
  if ( res == SatBool3::True ) {
    // SAT問題の解から情報を取り出す．
    auto& model = mSolver.model();
    MakeBcGraph make_graph(*this, model);
    return make_graph();
  }
  return BcGraph();
}

// @brief op_id の演算を出力が参照している条件を求める．
void
EngineBase::add_ref_cond(
  SizeType op_id,
  std::vector<SatLiteral>& dst_lits
)
{
  dst_lits.reserve(dst_lits.size() + mOutputNum);
  for ( SizeType o = 0; o < mOutputNum; ++ o ) {
    auto var = mOutSelMatrix.var(o, op_id);
    dst_lits.push_back(var);
  }
}

// @brief 入力値に対応する条件を追加する．
void
EngineBase::add_cond(
  SizeType input_index
)
{
  // そもそもこの入力に対してドントケア以外の値を持つ出力が
  // あるかチェックする．
  bool found = false;
  for ( SizeType o = 0; o < mOutputNum; ++ o ) {
    auto oval = mFunc.get_value(input_index, o);
    if ( oval != Val3::X ) {
      found = true;
      break;
    }
  }
  if ( !found ) {
    // すべての出力の値がドントケアだったので条件を追加する必要がない．
    return;
  }

  // 入力値のリストを作る．
  auto ival_list = mFunc.decode_index(input_index);

  // 各演算ノードの出力値を表す変数
  std::vector<SatLiteral> opvar_list;
  opvar_list.reserve(op_num());
  // 演算ノードに関する条件
  for ( auto& op: mOpList ) {
    auto ovar = op->add_ovar_cond(mSolver, ival_list, opvar_list);
    opvar_list.push_back(ovar);
    if ( !mSolver.sane() ) {
      return;
    }
  }
  // 出力値に関する条件
  if ( mOutputNum == 1 ) {
    auto last_op_id = op_num() - 1;
    auto ovar = opvar_list[last_op_id];
    auto oval = mFunc.get_value(input_index, 0);
    if ( oval == Val3::ZERO ) {
      mSolver.add_clause(~ovar);
    }
    else {
      mSolver.add_clause( ovar);
    }
  }
  else {
    for ( SizeType o = 0; o < mOutputNum; ++ o ) {
      auto oval = mFunc.get_value(input_index, o);
      if ( oval == Val3::X ) {
	// ドントケアは無視
	continue;
      }
      for ( SizeType op_id = 0; op_id < op_num(); ++ op_id ) {
	// op_id の演算ノードの出力の値
	auto ovar = opvar_list[op_id];
	// 出力 o が op_id を選ぶ時 true になる変数
	auto svar = mOutSelMatrix.var(o, op_id);
	auto _ = mSolver.condblock(svar);
	// 反転属性
	auto ivar = mOutInvArray[o];
	if ( ivar == SatLiteral::X ) {
	  // 極性固定
	  if ( oval == Val3::ZERO ) {
	    mSolver.add_clause(~ovar);
	  }
	  else {
	    mSolver.add_clause( ovar);
	  }
	}
	else {
	  // ivar に従って極性反転する場合
	  if ( oval == Val3::ONE ) {
	    mSolver.add_clause(~ivar, ~ovar);
	    mSolver.add_clause( ivar,  ovar);
	  }
	  else {
	    mSolver.add_clause(~ivar,  ovar);
	    mSolver.add_clause( ivar, ~ovar);
	  }
	}
      }
    }
  }
}

// @brief 出力の選択結果を返す．
SizeType
EngineBase::output_sel(
  SizeType o_id,
  const SatModel& model
) const
{
  return mOutSelMatrix.val(model, o_id);
}

// @brief 出力の反転属性結果を返す．
bool
EngineBase::output_inv(
  SizeType o_id,
  const SatModel& model
) const
{
  auto ivar = mOutInvArray[o_id];
  if ( ivar == SatLiteral::X ) {
    return false;
  }
  return model[ivar] == SatBool3::True;
}

// @brief 演算ノードを設定する．
void
EngineBase::set_op(
  SizeType op_id,
  const OpNode* op
)
{
  if ( op_id >= mOpList.size() ) {
    throw std::out_of_range{"op_id is out of range"};
  }
  mOpList[op_id] = std::unique_ptr<const OpNode>{op};
}

END_NAMESPACE_YM_EXSYN
