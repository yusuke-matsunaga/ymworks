
/// @file ExSynEngine_fence.cc
/// @brief ExSynEngine_fence の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ExSynEngine_fence.h"
#include "OpNode2.h"
#include "get_opt.h"


BEGIN_NAMESPACE_YM_EXSYN

// @brief コンストラクタ
ExSynEngine_fence::ExSynEngine_fence(
  const TruthTable& tt,
  const Fence& fence,
  const JsonValue& option
) : mTruthTable{tt},
    mFence{fence},
    mSolver(sat_param(option))
{
  if ( option.is_object() ) {
    // オプションの解析
    mFlags[1] = get_bool(option, "cegar", false);
    mFlags[2] = get_bool(option, "use_all_steps", false);
    mFlags[3] = get_bool(option, "no_reapplication", false);
    mFlags[4] = get_bool(option, "ordered_steps", false);
  }

  SizeType ni = tt.input_num();
  SizeType no = tt.output_num();
  SizeType ni_exp = 1 << ni;

  // 演算ノードに関する割り当ての生成
  SizeType l = fence.max_level();
  SizeType k = fence.total_num();
  mOpList.reserve(k);
  for ( SizeType i = 0; i < l; ++ i ) {
    SizeType n = fence.num(i);
    SizeType top = fence.top(i);
    for ( SizeType j = 0; j < n; ++ j ) {
      SizeType id = mOpList.size();
      auto op = OpNode2::new_obj(mSolver, ni, id, top, option);
      mOpList.push_back(std::unique_ptr<OpNode>{op});
    }
  }

  // 出力の選択変数の生成
  // 同時に定数チェックとnormal制約も作る．
  SizeType N = ni + k;
  mOutputList.reserve(no);
  for ( SizeType o = 0; o < no; ++ o ) {
    bool has0 = false;
    bool has1 = false;
    for ( SizeType b = 0; b < ni_exp; ++ b ) {
      auto oval = tt.get_value(b, o);
      if ( oval == Val3::ZERO ) {
	has0 = true;
      }
      else if ( oval == Val3::ONE ) {
	has1 = true;
      }
    }
    if ( !has1 ) {
      mOutputList.push_back(Output::new_zero());
    }
    else if ( !has0 ) {
      mOutputList.push_back(Output::new_one());
    }
    else {
      mOutputList.push_back(Output(mSolver, N));
      auto& output = mOutputList.back();
      auto oval = mTruthTable.get_value(0, o);
      auto inv_lit = output.inv;
      if ( oval == Val3::ZERO ) {
	mSolver.add_clause(~inv_lit);
      }
      else if ( oval == Val3::ONE ) {
	mSolver.add_clause( inv_lit);
      }
    }
  }

  if ( use_all_steps() ) {
    // use_all_steps 制約
    for ( SizeType i = 0; i < k; ++ i ) {
      // i 番目の演算を使う他の演算 or 出力が存在する．
      std::vector<SatLiteral> tmp_lits;
      tmp_lits.reserve(N + no - i);
      for ( SizeType j = i + 1; j < k; ++ j ) {
	auto& op = mOpList[j];
	auto lits = op->use_lit(i + ni);
	tmp_lits.insert(tmp_lits.end(), lits.begin(), lits.end());
      }
      for ( auto& output: mOutputList ) {
	if ( output.zero || output.one ) {
	  continue;
	}
	auto lit = output.sel.var(i + ni);
	tmp_lits.push_back(lit);
      }
      mSolver.add_clause(tmp_lits);
    }
  }

  if ( no_reapplication() ) {
    // no_reapplication 制約
    // i 番目の演算の入力(の一方)と i を入力にする他の演算を禁止する．
    for ( auto& op: mOpList ) {
      op->add_no_reapplication_cond(mSolver, mOpList);
    }
  }

  if ( ordered_steps() ) {
    // 演算間の順序制約
    // 入力を辞書式順序で比較する．
    for ( SizeType i = 0; i < l; ++ i ) {
      SizeType begin = mFence.top(i);
      SizeType end = mFence.top(i + 1);
      SizeType n = end - begin;
      if ( n > 0 ) {
	for ( SizeType i = 0; i < n - 1; ++ i ) {
	  auto op1 = mOpList[i + begin + 1].get();
	  auto op2 = mOpList[i + begin + 0].get();
	  op1->add_order_cond(mSolver, op2);
	}
      }
    }
  }
}

// @brief デストラクタ
ExSynEngine_fence::~ExSynEngine_fence()
{
}

// @brief コンストラクタ
ExSynEngine_fence::Output::Output(
  SatSolver& solver,
  SizeType cand_size
)
{
  sel = solver.new_onehot_var(cand_size);
  inv = solver.new_variable(true);
}

// @brief 問題を解く
BcGraph
ExSynEngine_fence::solve()
{
  if ( cegar() ) {
    return solve_cegar();
  }
  auto ni = mTruthTable.input_num();
  auto ni_exp = 1 << ni;

  // すべての入力に対する条件を追加する．
  for ( SizeType b = 0; b < ni_exp; ++ b ) {
    if ( !add_cond(b) ) {
      // 失敗
      return BcGraph();
    }
  }

  // SAT問題を解く
  auto res = mSolver.solve();
  if ( res == SatBool3::True ) {
    // SAT問題の解から情報を取り出す．
    auto& model = mSolver.model();
    return make_graph(model);
  }
  return BcGraph();
}

// @brief 問題を解く(CEGAR版)
BcGraph
ExSynEngine_fence::solve_cegar()
{
  while ( true ) {
    // SAT問題を解く
    auto res = mSolver.solve();
    if ( res == SatBool3::True ) {
      // SAT問題の解から情報を取り出す．
      auto& model = mSolver.model();
      auto g = make_graph(model);
      // 全ての入力に対して正しいか調べる．
      auto cevect = check_graph(mTruthTable, g);
      if ( cevect.empty() ) {
	//std::cout << "Found!" << std::endl;
	// 正解が求まった．
	return g;
      }
      // 今の反例に対する条件を追加する．
      auto index = mTruthTable.encode_index(cevect);
      if ( !add_cond(index) ) {
	//std::cout << "UNSAT" << std::endl;
	return BcGraph();
      }
      //std::cout << "Retry" << std::endl;
    }
    else {
      //std::cout << "Failed" << std::endl;
      break;
    }
  }
  return BcGraph();
}

// @brief BcGraph が正しいかチェックする．
std::vector<bool>
ExSynEngine_fence::check_graph(
  const TruthTable& tt,
  const BcGraph& graph
)
{
  SizeType ni = tt.input_num();
  SizeType ni_exp = 1 << ni;
  SizeType no = tt.output_num();
  for ( SizeType b = 0; b < ni_exp; ++ b ) {
    auto input_vals = tt.decode_index(b);
    auto output_vals = graph.simulate(input_vals);
    for ( SizeType o = 0; o < no; ++ o ) {
      auto val = tt.get_value(b, o);
      if ( val == Val3::X ) {
	continue;
      }
      auto b_val = (val == Val3::ONE);
      if ( b_val != output_vals[o] ) {
	return input_vals;
      }
    }
  }
  return {};
}

// @brief 入力値に対応する条件を追加する．
bool
ExSynEngine_fence::add_cond(
  SizeType input_index
)
{
  auto k = mOpList.size();
  auto ni = mTruthTable.input_num();
  auto no = mTruthTable.output_num();
  SizeType N = ni + k;
  // 入力と各演算ノードの出力値を表す変数
  std::vector<SatLiteral> opvar_list;
  opvar_list.reserve(N);
  // 入力の値に関する条件
  auto input_vals = mTruthTable.decode_index(input_index);
  for ( SizeType i = 0; i < ni; ++ i ) {
    auto ivar = mSolver.new_variable(false);
    if ( input_vals[i] ) {
      mSolver.add_clause( ivar);
    }
    else {
      mSolver.add_clause(~ivar);
    }
    opvar_list.push_back(ivar);
  }
  // 演算ノードに関する条件
  for ( SizeType i = 0; i < k; ++ i ) {
    auto& op = mOpList[i];
    auto ovar = op->add_ovar_cond(mSolver, opvar_list);
    opvar_list.push_back(ovar);
  }
  // 出力値に関する条件
  for ( SizeType o = 0; o < no; ++ o ) {
    auto oval = mTruthTable.get_value(input_index, o);
    if ( oval == Val3::X ) {
      // ドントケアは無視
      continue;
    }
    auto& output = mOutputList[o];
    if ( output.zero || output.one ) {
      continue;
    }
    auto inv_lit = output.inv;
    for ( SizeType j = 0; j < N; ++ j ) {
      auto svar = output.sel.var(j);
      auto ovar = opvar_list[j];
      if ( oval == Val3::ONE ) {
	mSolver.add_clause(~svar,  ovar,  inv_lit);
	mSolver.add_clause(~svar, ~ovar, ~inv_lit);
      }
      else {
	mSolver.add_clause(~svar,  ovar, ~inv_lit);
	mSolver.add_clause(~svar, ~ovar,  inv_lit);
      }
    }
  }
  return mSolver.sane();
}

// @brief SATの解から BcGraph を作る．
BcGraph
ExSynEngine_fence::make_graph(
  const SatModel& model
)
{
  auto ni = mTruthTable.input_num();
  auto no = mTruthTable.output_num();
  auto k = mOpList.size();
  BcGraph g(ni);
  for ( SizeType i = 0; i < k; ++ i ) {
    auto& op = mOpList[i];
    std::uint8_t tpat = 0;
    if ( op->get_tval01(model) ) {
      tpat |= 1U;
    }
    if ( op->get_tval10(model) ) {
      tpat |= 2U;
    }
    if ( op->get_tval11(model) ) {
      tpat |= 4U;
    }
    auto type = BcOpType::NONE;
    bool inv0 = false;
    bool inv1 = false;
    switch ( tpat ) {
    case 4: type = BcOpType::AND; break;
    case 7: type = BcOpType::OR;  break;
    case 3: type = BcOpType::XOR; break;
    case 2: type = BcOpType::AND; inv1 = true; break;
    case 1: type = BcOpType::AND; inv0 = true; break;
    default: throw std::runtime_error{"invalid tval"};
    }
    SizeType src0;
    SizeType src1;
    op->get_opr(model, src0, src1);
    auto opr0 = make_edge(src0, inv0, g);
    auto opr1 = make_edge(src1, inv1, g);
    g.add_op(type, opr0, opr1);
  }
  for ( SizeType i = 0; i < no; ++ i ) {
    auto& output = mOutputList[i];
    if ( output.zero ) {
      g.add_zero_output();
    }
    else if ( output.one ) {
      g.add_one_output();
    }
    else {
      auto src = output.sel.val(model);
      auto inv = (model[output.inv] == SatBool3::True);
      auto e = make_edge(src, inv, g);
      g.add_output(e);
    }
  }
  return g;
}

// @brief ソースの枝を作る．
BcEdge
ExSynEngine_fence::make_edge(
  SizeType src,
  bool inv,
  BcGraph& g
)
{
  SizeType ni = mTruthTable.input_num();
  if ( src < ni ) {
    auto input_id = src;
    return BcEdge::input(input_id, inv);
  }
  else {
    auto op_id = src - ni;
    return BcEdge::op(op_id, inv);
  }
}

END_NAMESPACE_YM_EXSYN
