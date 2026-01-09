
/// @file MakeBcGraph.cc
/// @brief MakeBcGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "MakeBcGraph.h"
#include "EngineBase.h"
#include "OpNode.h"


BEGIN_NAMESPACE_YM_EXSYN

// @brief コンストラクタ
MakeBcGraph::MakeBcGraph(
  const EngineBase& engine,
  const SatModel& model
) : mEngine{engine},
    mModel{model},
    mGraph(mEngine.input_num())
{
}

// @brief デストラクタ
MakeBcGraph::~MakeBcGraph()
{
}

// @brief SATの解から BcGraph を作る．
BcGraph
MakeBcGraph::operator()()
{
  if ( mEngine.output_num() == 1 ) {
    auto last_op_id = mEngine.op_num() - 1;
    auto bc_op_id = make_op(last_op_id);
    auto edge = BcEdge::op(bc_op_id);
    mGraph.add_output(edge);
  }
  else {
    for ( SizeType o = 0; o < mEngine.output_num(); ++ o ) {
      auto op_id = mEngine.output_sel(o, mModel);
      auto bc_op_id = make_op(op_id);
      auto inv = mEngine.output_inv(o, mModel);
      auto edge = BcEdge::op(bc_op_id, inv);
      mGraph.add_output(edge);
    }
  }
  return mGraph;
}

// @brief 演算ノードを作る．
SizeType
MakeBcGraph::make_op(
  SizeType op_id
)
{
  if ( mIdDict.count(op_id) > 0 ) {
    return mIdDict.at(op_id);
  }
  auto op = mEngine.get_op(op_id);
  auto map_info = op->get_map_info(mModel);
  auto type = map_info.op_type;
  auto src_list = op->get_opr(mModel);
  auto nop = src_list.size();
  std::vector<BcEdge> edge_list(nop);
  for ( SizeType i = 0; i < nop; ++ i ) {
    auto src = src_list[i];
    auto inv = map_info.inv[i];
    if ( src < mEngine.input_num() ) {
      edge_list[i] = BcEdge::input(src, inv);
    }
    else {
      auto op_id = src - mEngine.input_num();
      auto bc_op_id = make_op(op_id);
      edge_list[i] = BcEdge::op(bc_op_id, inv);
    }
  }
  SizeType bc_op_id;
  if ( type == BcOpType::TV ) {
    bc_op_id = mGraph.add_tvop(map_info.tv, edge_list);
  }
  else {
    bc_op_id = mGraph.add_op(type, edge_list);
  }
  mIdDict.emplace(op_id, bc_op_id);
  return bc_op_id;
}

END_NAMESPACE_YM_EXSYN
