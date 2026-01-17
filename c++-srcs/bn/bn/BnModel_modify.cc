
/// @file BnModel.cc
/// @brief BnModel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnModel.h"
#include "ym/BnNode.h"
#include "ym/BnNodeList.h"
#include "ym/BnFunc.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラス BnModel
//////////////////////////////////////////////////////////////////////

// @brief オプション情報をセットする．
void
BnModel::set_option(
  const JsonValue& option
)
{
  auto& model = _model_impl();
  model.set_option(option);
}

// @brief 名前を設定する．
void
BnModel::set_name(
  const std::string& name
)
{
  auto& model = _model_impl();
  model.set_name(name);
}

// @brief コメントを追加する．
void
BnModel::add_comment(
  const std::string& comment
)
{
  auto& model = _model_impl();
  model.add_comment(comment);
}

// @brief 入力名をセットする．
void
BnModel::set_input_name(
  SizeType input_id,
  const std::string& name
)
{
  auto& model = _model_impl();
  model.set_input_name(input_id, name);
}

// @brief 出力名をセットする．
void
BnModel::set_output_name(
  SizeType output_id,
  const std::string& name
)
{
  auto& model = _model_impl();
  model.set_output_name(output_id, name);
}

// @brief DFF名をセットする．
void
BnModel::set_dff_name(
  SizeType dff_id,
  const std::string& name
)
{
  auto& model = _model_impl();
  model.set_dff_name(dff_id, name);
}

// @brief DFFを作る．
BnDff
BnModel::new_dff(
  const std::string& name,
  char reset_val
)
{
  auto& model = _model_impl();
  auto dff_id = model.new_dff(name, reset_val);
  model.new_dff_output(dff_id);
  auto dff = model.dff_impl(dff_id);
  return BnDff(dff);
}

// @brief DFFの入力ノードを設定する．
void
BnModel::set_dff_src(
  const BnDff& dff,
  BnNode src
)
{
  auto& model = _model_impl();
  model.set_dff_src(dff.id(), src.mPtr);
}

// @brief 入力ノードを作る．
BnNode
BnModel::new_input(
  const std::string& name
)
{
  auto& model = _model_impl();
  auto node = model.new_input(name);
  return node;
}

// @brief 出力ノードを作る．
SizeType
BnModel::new_output(
  BnNode src,
  const std::string& name
)
{
  auto& model = _model_impl();
  auto oid = model.new_output(src.mPtr, name);
  return oid;
}

// @brief プリミティブ型の論理ノードを作る．
BnNode
BnModel::new_primitive(
  PrimType primitive_type,
  const std::vector<BnNode>& fanin_list
)
{
  auto& model = _model_impl();
  auto input_num = fanin_list.size();
  auto func = model.reg_primitive(input_num, primitive_type);
  auto ptr_list = make_ptr_list(fanin_list);
  auto node = model.new_logic(func, ptr_list);
  return BnNode(node);
}

// @brief カバー型の論理ノードを登録する．
BnNode
BnModel::new_cover(
  const SopCover& input_cover,
  bool output_inv,
  const std::vector<BnNode>& fanin_list
)
{
  if ( input_cover.variable_num() != fanin_list.size() ) {
    throw std::invalid_argument{"input_cover.variable_num() != fanin_list.size()"};
  }
  auto& model = _model_impl();
  auto func = model.reg_cover(input_cover, output_inv);
  auto ptr_list = make_ptr_list(fanin_list);
  auto node = model.new_logic(func, ptr_list);
  return BnNode(node);
}

// @brief 論理式型の論理ノードを登録する．
BnNode
BnModel::new_expr(
  const Expr& expr,
  const std::vector<BnNode>& fanin_list
)
{
  if ( expr.input_size() != fanin_list.size() ) {
    throw std::invalid_argument{"expr.input_size() != fanin_list.size()"};
  }
  auto& model = _model_impl();
  auto func = model.reg_expr(expr);
  auto ptr_list = make_ptr_list(fanin_list);
  auto node = model.new_logic(func, ptr_list);
  return BnNode(node);
}

// @brief 真理値表型の論理ノードを登録する．
BnNode
BnModel::new_tvfunc(
  const TvFunc& tvfunc,
  const std::vector<BnNode>& fanin_list
)
{
  if ( tvfunc.input_num() != fanin_list.size() ) {
    throw std::invalid_argument{"func.input_num() != fanin_list.size()"};
  }
  auto& model = _model_impl();
  auto func = model.reg_tvfunc(tvfunc);
  auto ptr_list = make_ptr_list(fanin_list);
  auto node = model.new_logic(func, ptr_list);
  return BnNode(node);
}

// @brief BDD型の論理ノードを登録する．
BnNode
BnModel::new_bdd(
  const Bdd& bdd,
  const std::vector<BnNode>& fanin_list
)
{
  if ( bdd.support_size() != fanin_list.size() ) {
    throw std::invalid_argument{"bdd.support_size() != fanin_list.size()"};
  }
  auto& model = _model_impl();
  auto func = model.reg_bdd(bdd);
  auto ptr_list = make_ptr_list(fanin_list);
  auto node = model.new_logic(func, ptr_list);
  return BnNode(node);
}

END_NAMESPACE_YM_BN
