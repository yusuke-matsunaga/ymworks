
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

// @brief 内容をクリアする．
void
BnModel::clear()
{
  _model_impl().clear();
}

// @brief 設定情報を確定する．
void
BnModel::wrap_up()
{
  _model_impl().make_logic_list();
}

// @brief オプション情報をセットする．
void
BnModel::set_option(
  const JsonValue& option
)
{
  _model_impl().set_option(option);
}

// @brief DFFを作る．
BnDff
BnModel::new_dff(
  const std::string& name,
  char reset_val
)
{
  auto dff_id = _model_impl().new_dff(name, reset_val);
  _model_impl().new_dff_output(dff_id);
  return _id2dff(dff_id);
}

// @brief DFFの入力ノードを設定する．
void
BnModel::set_dff_src(
  const BnDff& dff,
  BnNode src
)
{
  _check_dff(dff);
  _check_node(src);
  _model_impl().set_dff_src(dff.id(), src.id());
}

// @brief 入力ノードを作る．
BnNode
BnModel::new_input(
  const std::string& name
)
{
  auto id = _model_impl().new_input(name);
  auto node = _id2node(id);
  return node;
}

// @brief 出力ノードを作る．
SizeType
BnModel::new_output(
  BnNode src,
  const std::string& name
)
{
  _check_node(src);
  auto oid = _model_impl().new_output(src.id(), name);
  return oid;
}

// @brief プリミティブ型の論理ノードを作る．
BnNode
BnModel::new_primitive(
  PrimType primitive_type,
  const BnNodeList& fanin_list
)
{
  auto input_num = fanin_list.size();
  auto func_id = _model_impl().reg_primitive(input_num, primitive_type);
  auto id = _model_impl().new_logic(func_id, fanin_list.id_list());
  return _id2node(id);
}

// @brief カバー型の論理ノードを登録する．
BnNode
BnModel::new_cover(
  const SopCover& input_cover,
  bool output_inv,
  const BnNodeList& fanin_list
)
{
  if ( input_cover.variable_num() != fanin_list.size() ) {
    throw std::invalid_argument{"input_cover.variable_num() != fanin_list.size()"};
  }
  auto func_id = _model_impl().reg_cover(input_cover, output_inv);
  auto id = _model_impl().new_logic(func_id, fanin_list.id_list());
  return _id2node(id);
}

// @brief 論理式型の論理ノードを登録する．
BnNode
BnModel::new_expr(
  const Expr& expr,
  const BnNodeList& fanin_list
)
{
  if ( expr.input_size() != fanin_list.size() ) {
    throw std::invalid_argument{"expr.input_size() != fanin_list.size()"};
  }
  auto func_id = _model_impl().reg_expr(expr);
  auto id = _model_impl().new_logic(func_id, fanin_list.id_list());
  return _id2node(id);
}

// @brief 真理値表型の論理ノードを登録する．
BnNode
BnModel::new_tvfunc(
  const TvFunc& func,
  const BnNodeList& fanin_list
)
{
  if ( func.input_num() != fanin_list.size() ) {
    throw std::invalid_argument{"func.input_num() != fanin_list.size()"};
  }
  auto func_id = _model_impl().reg_tvfunc(func);
  auto id = _model_impl().new_logic(func_id, fanin_list.id_list());
  return _id2node(id);
}

// @brief BDD型の論理ノードを登録する．
BnNode
BnModel::new_bdd(
  const Bdd& bdd,
  const BnNodeList& fanin_list
)
{
  if ( bdd.support_size() != fanin_list.size() ) {
    throw std::invalid_argument{"bdd.support_size() != fanin_list.size()"};
  }
  auto func_id = _model_impl().reg_bdd(bdd);
  auto id = _model_impl().new_logic(func_id, fanin_list.id_list());
  return _id2node(id);
}

END_NAMESPACE_YM_BN
