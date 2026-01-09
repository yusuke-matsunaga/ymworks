
/// @file AigMgr_bn.cc
/// @brief AigMgr_bn の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AigMgr.h"
#include "Bn2Aig.h"
#include "ym/BnNode.h"
#include "ym/BnNodeList.h"
#include "ym/BnDff.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス AigMgr
//////////////////////////////////////////////////////////////////////

// @brief BnModel からの変換
AigMgr
AigMgr::from_bn(
  const BnModel& bn_model
)
{
  AigMgr mgr;

  Bn2Aig bn2aig(bn_model, mgr);

  return mgr;
}


//////////////////////////////////////////////////////////////////////
// クラス Bn2Aig
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Bn2Aig::Bn2Aig(
  const BnModel& model,
  AigMgr& mgr
)
{
  // 外部入力をコピーする．
  for ( auto bn_node: model.input_list() ) {
    auto name = model.input_name(bn_node.input_id());
    auto aig = mgr.new_input(name);
    mAigMap.emplace(bn_node.id(), aig);
  }
  // DFF出力をコピーする．
  for ( auto bn_dff: model.dff_list() ) {
    auto name = bn_dff.name();
    auto dff_id = mgr.new_dff(name);
    auto aig = mgr.dff_output(dff_id);
    mAigMap.emplace(bn_dff.output().id(), aig);
  }
  // 論理ノードをコピーする．
  for ( auto bn_node: model.logic_list() ) {
    // ファンインの枝を取ってくる．
    auto nfi = bn_node.fanin_num();
    std::vector<AigLiteral> fanin_list;
    fanin_list.reserve(nfi);
    for ( auto bn_inode: bn_node.fanin_list() ) {
      auto iaig = node2aig(bn_inode);
      fanin_list.push_back(iaig);
    }
    // 関数に応じた操作を行う．
    auto func = bn_node.func();
    auto aig = AigLiteral::zero();
    switch ( func.type() ) {
    case BnFunc::PRIMITIVE:
      aig = mgr.new_primitive(func.primitive_type(), fanin_list);
      break;
    case BnFunc::COVER:
      aig = mgr.from_cover(func.input_cover(), fanin_list) * func.output_inv();
      break;
    case BnFunc::EXPR:
      aig = mgr.from_expr(func.expr(), fanin_list);
      break;
    default:
      throw std::invalid_argument{"wrong function type"};
    }
    mAigMap.emplace(bn_node.id(), aig);
  }
  // 外部出力をセットする．
  auto no = model.output_num();
  for ( SizeType i = 0; i < no; ++ i ) {
    auto bn_node = model.output(i);
    auto aig = node2aig(bn_node);
    auto name = model.output_name(i);
    mgr.new_output(aig, name);
  }
  // DFF入力をセットする．
  auto nf = model.dff_num();
  for ( SizeType i = 0; i < nf; ++ i ) {
    auto bn_dff = model.dff(i);
    auto bn_node = bn_dff.input();
    auto aig = node2aig(bn_node);
    mgr.set_dff_input(i, aig);
  }
}

// @brief BnNode に対応する AigLiteral を取り出す．
AigLiteral
Bn2Aig::node2aig(
  const BnNode& node
) const
{
  auto id = node.id();
  if ( mAigMap.count(id) == 0 ) {
    std::ostringstream buf;
    buf << "Node#" << id << " is not registered";
    throw std::logic_error{buf.str()};
  }
  return mAigMap.at(id);
}

END_NAMESPACE_YM_AIG
