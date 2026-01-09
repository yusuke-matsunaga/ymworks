
/// @file Engine_BD.cc
/// @brief Engine_BD の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "Engine_BD.h"
#include "TemplNode.h"
#include "BdNode.h"
#include "get_opt.h"


BEGIN_NAMESPACE_YM_EXSYN

BEGIN_NONAMESPACE

inline
SizeType
get_node_num(
  const std::vector<BD>& bd_list
)
{
  if ( bd_list.empty() ) {
    throw std::invalid_argument{"bd_list.empty()"};
  }
  auto& bd0 = bd_list.front();
  return bd0.op_num();
}

END_NONAMESPACE

// @brief コンストラクタ
Engine_BD::Engine_BD(
  const TruthTable& func,
  const std::vector<BD>& bd_list,
  const JsonValue& option
) : mFunc{func},
    mOption{option},
    mSolver(sat_param(option)),
    mInputNum{func.input_num()},
    mBD_List{bd_list},
    mPdNodeNum{get_node_num(mBD_List)},
    mTemplateList(mPdNodeNum),
    mNodeGroupList(mPdNodeNum),
    mBdSel{mSolver.new_onehot_var(mBD_List.size())}
{
  if ( func.output_num() > 1 ) {
    throw std::invalid_argument{"func.output_num() > 1"};
  }
}

// @brief デストラクタ
Engine_BD::~Engine_BD()
{
}

// @brief 問題を解く
BcGraph
Engine_BD::solve()
{
  make_op_list();

  // すべての入力に対する条件を追加する．
  auto ni_exp = 1 << input_num();
  for ( SizeType b = 1; b < ni_exp; ++ b ) {
    if ( !solver().sane() ) {
      // 失敗
      return BcGraph();
    }
    add_cond(b);
  }

  // SAT問題を解く
  auto res = solver().solve();
  if ( res == SatBool3::True ) {
    // SAT問題の解から情報を取り出す．
    auto& model = solver().model();
    return make_graph(model);
  }
  return BcGraph();
}

BEGIN_NONAMESPACE

inline
std::string
make_signature(
  const std::vector<int>& opr_list,
  const std::unordered_map<SizeType, SizeType>& local_map
)
{
  std::ostringstream buf;
  for ( auto x: opr_list ) {
    auto opr = local_map.at(x);
    buf << ":" << opr;
  }
  return buf.str();
}

END_NONAMESPACE

// @brief 演算ノードと出力の制約を作る．
void
Engine_BD::make_op_list()
{
  // TemplNode を作る．
  auto& bd0 = mBD_List.front();
  for ( SizeType i = 0; i < mPdNodeNum; ++ i ) {
    auto& opr = bd0.opr(i);
    auto nop = opr.size();
    mTemplateList[i] = std::unique_ptr<const TemplNode>{TemplNode::new_obj(solver(), nop)};
  }

  // BdNode を作る．
  // シグネチャをキーとして bd_id を保持する辞書
  std::unordered_map<std::string, SizeType> bd_map;
  mNodeGroupList.resize(mPdNodeNum);
  // mNodeList の内容を仮に入れておくリスト
  // サイズがわからないので大きめな領域を確保しておく．
  std::vector<const BdNode*> node_list;
  node_list.reserve(mPdNodeNum * mBD_List.size());
  for ( auto& bd: mBD_List ) {
    // この bd に関する pd_id と bd_id の辞書
    std::unordered_map<SizeType, SizeType> local_map;
    // 入力番号は同じ
    for ( SizeType i = 0; i < input_num(); ++ i ) {
      local_map.emplace(i, i);
    }
    for ( SizeType pd_op_id = 0; pd_op_id < mPdNodeNum; ++ pd_op_id ) {
      auto& opr = bd.opr(pd_op_id);
      auto sig = make_signature(opr, local_map);
      SizeType bd_id;
      if ( bd_map.count(sig) > 0 ) {
	bd_id = bd_map.at(sig);
      }
      else {
	auto templ_node = mTemplateList[pd_op_id].get();
	// opr からこの bd 用のオペランド番号を作る．
	std::vector<SizeType> local_opr;
	local_opr.reserve(opr.size());
	for ( auto x: opr ) {
	  auto y = local_map.at(x);
	  local_opr.push_back(y);
	}
	bd_id = node_list.size();
	auto bd_node = BdNode::new_obj(templ_node, input_num(), bd_id, local_opr);
	node_list.push_back(bd_node);
	mNodeGroupList[pd_op_id].push_back(bd_id);
	bd_id += input_num();
	bd_map.emplace(sig, bd_id);
      }
      local_map.emplace(pd_op_id + input_num(), bd_id);
    }
  }
  mNodeList.reserve(node_list.size());
  for ( auto node: node_list ) {
    mNodeList.push_back(std::unique_ptr<const BdNode>{node});
  }

  // 出力の選択変数の生成
  mBdSel = solver().new_onehot_var(mNodeGroupList.back().size());
}

// @brief 入力値に対応する条件を追加する．
void
Engine_BD::add_cond(
  SizeType input_index
)
{
  // そもそもこの入力に対してドントケア以外の値を持つ出力が
  // あるかチェックする．
  auto oval = mFunc.get_value(input_index, 0);
  if ( oval == Val3::X ) {
    // ドントケアの入力値は無視
    return;
  }

  // 入力値のリストを作る．
  auto ival_list = mFunc.decode_index(input_index);

  // 各演算ノードの出力値を表す変数
  std::vector<SatLiteral> opvar_list;
  opvar_list.reserve(mNodeList.size());
  // 演算ノードに関する条件
  for ( auto& op: mNodeList ) {
    auto ovar = op->add_ovar_cond(mSolver, ival_list, opvar_list);
    opvar_list.push_back(ovar);
    if ( !mSolver.sane() ) {
      return;
    }
  }
  // 出力値に関する条件
  // 1出力の場合は最後の演算ノードが出力となっている．
  auto& group = mNodeGroupList.back();
  auto group_num = group.size();
  for ( SizeType i = 0; i < group_num; ++ i ) {
    auto op_id = group[i];
    auto svar = mBdSel.var(i);
    auto _ = mSolver.condblock(svar);
    auto ovar = opvar_list[op_id];
    if ( oval == Val3::ZERO ) {
      mSolver.add_clause(~ovar);
    }
    else {
      mSolver.add_clause( ovar);
    }
  }
}

// @brief SATの解から BcGraph を作る．
BcGraph
Engine_BD::make_graph(
  const SatModel& model
)
{
  BcGraph g(mInputNum);
  auto bd_sel = mBdSel.val(model);
  // 1出力の場合は常に最後の演算ノードが出力に対応している．
  auto op_id = mNodeGroupList.back()[bd_sel];
  std::unordered_map<SizeType, SizeType> op_dict;
  auto bd_op_id = make_op(model, op_id, g, op_dict);
  auto edge = BcEdge::op(bd_op_id);
  g.add_output(edge);
  return g;
}

// @brief 演算ノードを作る．
SizeType
Engine_BD::make_op(
  const SatModel& model,
  SizeType op_id,
  BcGraph& g,
  std::unordered_map<SizeType, SizeType>& op_dict
)
{
  if ( op_dict.count(op_id) > 0 ) {
    return op_dict.at(op_id);
  }
  auto& op = mNodeList[op_id];
  auto map_info = op->get_map_info(model);
  auto type = map_info.op_type;
  auto nop = op->opr_num();
  std::vector<BcEdge> edge_list(nop);
  for ( SizeType i = 0; i < nop; ++ i ) {
    auto src = op->opr(i);
    auto inv = map_info.inv[i];
    if ( src < mInputNum ) {
      edge_list[i] = BcEdge::input(src, inv);
    }
    else {
      auto op_id = src - mInputNum;
      auto bc_op_id = make_op(model, op_id, g, op_dict);
      edge_list[i] = BcEdge::op(bc_op_id, inv);
    }
  }
  SizeType bc_op_id;
  if ( type == BcOpType::TV ) {
    bc_op_id = g.add_tvop(map_info.tv, edge_list);
  }
  else {
    bc_op_id = g.add_op(type, edge_list);
  }
  op_dict.emplace(op_id, bc_op_id);
  return bc_op_id;
}

END_NAMESPACE_YM_EXSYN
