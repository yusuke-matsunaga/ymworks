
/// @file ExprGen_primary.cc
/// @brief ElbMgr の実装ファイル(式の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ExprGen.h"
#include "ElbEnv.h"

#include "ym/vl/AstModule.h"
#include "ym/vl/AstExpr.h"
#include "ym/vl/AstPart.h"

#include "ym/vl/VlModule.h"
#include "ym/vl/VlDecl.h"
#include "ym/vl/VlDeclArray.h"
#include "ym/vl/VlRange.h"

#include "elaborator/ElbDecl.h"
#include "elaborator/ElbParameter.h"
#include "elaborator/ElbPrimitive.h"
#include "elaborator/ElbExpr.h"
#include "elaborator/ElbGenvar.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief プライマリ式の生成を行う．
//
// プライマリ式は以下の情報を持つ．
// - 識別子名(場合によっては階層名を持つ)
// - インデックス式のリスト
// - 範囲指定
//
// 面倒なのは配列要素のインデックスとビット指定式
// の見かけが同じに見えるということ．
// 識別子に対応する宣言要素の次元から推測する．
//
// 有効な宣言要素のタイプは以下の通り
// - net/reg
//   対象が配列型の場合はインデックスが定数の場合と
//   可変式の場合がある．
// - genvar
//   右辺のみ有効
// - namedevent
//   イベント文のみ有効
ElbExpr*
ExprGen::instantiate_primary(
  const VlScope* parent,
  const ElbEnv& env,
  const AstExpr& ast_expr
)
{
  // 名前に対応したオブジェクトのハンドルを取り出す．
  auto handle = find_handle(parent, env, ast_expr);

  if ( env.is_system_tf_arg() ) {
    // システム関数/タスクの引数の場合
    // 配列そのものやプリミティブ，スコープなどの特別な場合を処理する．
    // 該当しない場合は nullptr を返す．
    auto expr = instantiate_system_tf_arg(handle, parent, ast_expr);
    if ( expr != nullptr ) {
      return expr;
    }
  }

  auto genvar = handle->genvar();
  if ( genvar != nullptr ) {
    // 対象のオブジェクトが genvar の場合
    return instantiate_genvar(parent, ast_expr, genvar->value());
  }

  // 添字には constant/constant function 以外の情報は引き継がない
  ElbEnv index_env;
  if ( ast_expr.is_const_index() ) {
    index_env = ElbConstantEnv();
  }
  else if ( env.inside_constant_function() ) {
    index_env = ElbConstantFunctionEnv(env.constant_function());
  }

  // 宣言要素の参照を求める．
  // ビット指定式が含まれていたら ast_bit に入れる．
  AstExpr ast_bit;
  auto decl_ref = instantiate_decl_ref(handle, parent, index_env,
				       ast_expr, ast_bit);

  // 宣言要素のタイプをチェックする．
  check_decl(env, ast_expr, decl_ref, ast_bit.is_valid());

  auto sel = instantiate_selecter(parent, ast_expr, decl_ref.declbase(),
				  index_env, ast_bit, true);
  return elb_mgr().new_Primary(ast_expr, decl_ref, sel);
}

// @brief イベント式用のプライマリ式の生成を行う．
ElbExpr*
ExprGen::instantiate_event_primary(
  const VlScope* parent,
  const ElbEnv& env,
  const AstExpr& ast_expr
)
{
  ElbEventExprEnv env1(env);
  return instantiate_primary(parent, env1, ast_expr);
}

// @brief genvar に対応した定数を生成する．
ElbExpr*
ExprGen::instantiate_genvar(
  const VlScope* parent,
  const AstExpr& ast_expr,
  int val
)
{
  auto has_range_select = ast_expr.part().is_valid();
  auto index_list = ast_expr.index_list();
  auto isize = index_list.size();
  if (  isize > 1 || (isize == 1 && has_range_select) ) {
    // 配列型ではない．
    log_mgr().error_dimension_mismatch(__FILE__, __LINE__, ast_expr);
  }

  auto has_bit_select = (isize == 1);
  if ( has_bit_select ) {
    auto index1 = evaluate_int(parent, index_list.front());
    val >>= index1;
    val &= 1;
  }
  else if ( has_range_select ) {
    auto ast_part = ast_expr.part();
    auto index1 = evaluate_int(parent, ast_part.left());
    auto index2 = evaluate_int(parent, ast_part.right());
    val >>= index2;
    val &= ((1 << (index1 - index2 + 1)) - 1);
  }

  // genvar の値に対応した定数式を生成
  return elb_mgr().new_GenvarConstant(ast_expr, val);
}

// @brief システム関数/タスクの引数を返す．
ElbExpr*
ExprGen::instantiate_system_tf_arg(
  ObjHandle* handle,
  const VlScope* parent,
  const AstExpr& ast_expr
)
{
  // 識別子の添字の次元
  auto ast_index_list = ast_expr.index_list().to_vector();
  auto isize = ast_index_list.size();

  if ( isize == 0 ) {
    // 添字なし
    // - スコープ
    // - プリミティブ
    // - 宣言配列
    auto scope = handle->scope();
    if ( scope != nullptr ) {
      return elb_mgr().new_ArgHandle(ast_expr, scope);
    }

    auto primitive = handle->primitive();
    if ( primitive != nullptr ) {
      return elb_mgr().new_ArgHandle(ast_expr, primitive);
    }

    auto declarray = handle->declarray();
    if ( declarray != nullptr ) {
      return elb_mgr().new_ArgHandle(ast_expr, declarray);
    }
  }
  else if ( isize == 1 ) {
    // 添字あり
    // - スコープ配列の要素
    // - プリミティブ配列の要素
    auto index_list = ast_expr.index_list().to_vector();
    auto ast_expr1 = index_list[0];
    auto index = evaluate_int(parent, ast_expr1);
    auto scope = handle->array_elem(index);
    if ( scope != nullptr ) {
      return elb_mgr().new_ArgHandle(ast_expr, scope);
    }

    auto prim_array = handle->prim_array();
    if ( prim_array != nullptr ) {
      auto primitive = prim_array->elem_by_index(index);
      if ( primitive != nullptr ) {
	return elb_mgr().new_ArgHandle(ast_expr, primitive);
      }
    }
  }
  // 該当なし．
  return nullptr;
}

// @brief 宣言要素のインスタンス化を行う．
ElbDeclRef
ExprGen::instantiate_decl_ref(
  ObjHandle* handle,
  const VlScope* parent,
  const ElbEnv& index_env,
  const AstExpr& ast_expr,
  AstExpr& ast_bit
)
{
  // プライマリ式の次元 (ビット指定を含んでいる可能性あり)
  auto ast_index_list = ast_expr.index_list().to_vector();
  auto isize = ast_index_list.size();

  // 配列の次元
  SizeType dsize = 0;

  // 範囲指定があるとき true となるフラグ
  auto ast_part = ast_expr.part();
  auto has_range_select = ast_part.is_valid();

  if ( handle->type() == VpiObjType::Parameter ) {
    // パラメータの場合
    if ( isize != dsize && (isize != dsize + 1 || has_range_select) ) {
      // 次元が合わない．
      log_mgr().error_dimension_mismatch(__FILE__, __LINE__, ast_expr);
    }
    if ( isize == dsize + 1 ) {
      ast_bit = ast_index_list.back();
    }
    return ElbDeclRef(handle->parameter());
  }

  auto decl = handle->decl();
  if ( decl != nullptr ) {
    // 単独の宣言要素の場合
    if ( isize != dsize && (isize != dsize + 1 || has_range_select) ) {
      // 次元が合わない．
      log_mgr().error_dimension_mismatch(__FILE__, __LINE__, ast_expr);
    }
    if ( isize == dsize + 1 ) {
      ast_bit = ast_index_list.back();
    }
    return ElbDeclRef(decl);
  }

  auto declarray = handle->declarray();
  if ( declarray != nullptr ) {
    // 配列型の宣言要素の場合

    // 配列の次元
    dsize = declarray->dimension();
    if ( isize != dsize && (isize != dsize + 1 || has_range_select) ) {
      // 次元が合わない．
      log_mgr().error_dimension_mismatch(__FILE__, __LINE__, ast_expr);
    }
    if ( isize == dsize + 1 ) {
      ast_bit = ast_index_list.back();
    }

    // インデックスが定数かどうか調べる．
    auto const_index = true;
    std::vector<int> index_val_list;
    index_val_list.reserve(dsize);
    for ( auto ast_index: ast_index_list ) {
      bool is_const;
      auto index_val = evaluate_int_if_const(parent, ast_index, is_const);
      if ( is_const ) {
	index_val_list.push_back(index_val);
      }
      else {
	const_index = false;
	break;
      }
      if ( index_val_list.size() == dsize ) {
	break;
      }
    }
    if ( const_index ) {
      // 添字が定数ならオフセットを計算する．
      SizeType array_offset;
      if ( !declarray->calc_array_offset(index_val_list, array_offset) ) {
	// インデックスが範囲外
	// 仕様上は値が X になるだけでエラーではない．
	log_mgr().warning_index_out_of_range(__FILE__, __LINE__,
					     ast_expr.file_region());
      }
      return ElbDeclRef(declarray, array_offset);
    }

    // 可変インデックスの場合のインデックス式
    std::vector<const VlExpr*> index_list;
    index_list.reserve(dsize);
    for ( auto ast_index: ast_index_list ) {
      auto expr = instantiate_expr(parent, index_env, ast_index);
      index_list.push_back(expr);
    }
    return ElbDeclRef(declarray, index_list);
  }
  // 適切な型ではなかった．
  log_mgr().error_illegal_object(__FILE__, __LINE__, ast_expr);
  // ダミー
  return ElbDeclRef();
}

// @brief ビット選択/部分選択の情報を実体化する．
ElbSelecter
ExprGen::instantiate_selecter(
  const VlScope* parent,
  const AstExpr& ast_expr,
  const VlDeclBase* declbase,
  const ElbEnv& index_env,
  const AstExpr& ast_bit,
  bool var_index
)
{
  if ( ast_bit.is_valid() ) {
    // ビット選択あり
    return instantiate_bitselect(parent, ast_expr, declbase,
				 index_env, ast_bit, var_index);
  }
  auto ast_part = ast_expr.part();
  if ( ast_part.is_valid() ) {
    // 部分選択あり
    return instantiate_partselect(parent, ast_expr, declbase,
				  index_env, ast_part, var_index);
  }
  // 単純なプライマリ式
  return ElbSelecter::new_NONE(declbase->bit_size());
}

// @brief ビット選択式を作る．
ElbSelecter
ExprGen::instantiate_bitselect(
  const VlScope* parent,
  const AstExpr& ast_expr,
  const VlDeclBase* declbase,
  const ElbEnv& index_env,
  const AstExpr& ast_bit,
  bool var_index
)
{
  if ( declbase->value_type().is_real_type() ) {
    // real type でのビット指定はエラー
    log_mgr().error_select_for_real(__FILE__, __LINE__, ast_expr);
  }
  bool is_const;
  int index_val = evaluate_int_if_const(parent, ast_bit, is_const);
  if ( is_const ) {
    // 固定インデックスだった．
    SizeType bit_offset = 0;
    auto stat = declbase->calc_bit_offset(index_val, bit_offset);
    if ( !stat ) {
      // インデックスが範囲外
      // ただ値が X になるだけでエラーにはならないそうだ．
      log_mgr().warning_index_out_of_range(__FILE__, __LINE__,
					   ast_bit.file_region());
    }
    return ElbSelecter::new_CONST(1, bit_offset);
  }
  if ( var_index ) {
    // 可変インデックスだった．
    auto index = instantiate_expr(parent, index_env, ast_bit);
    return ElbSelecter::new_PLUS(1, index);
  }
  // 固定インデックス以外はエラー
#warning "TODO: エラーメッセージ"
  throw std::logic_error{"Never reached"};
}

// @brief 範囲選択式を作る．
ElbSelecter
ExprGen::instantiate_partselect(
  const VlScope* parent,
  const AstExpr& ast_expr,
  const VlDeclBase* declbase,
  const ElbEnv& index_env,
  const AstPart& ast_part,
  bool var_index
)
{
  if ( declbase->value_type().is_real_type() ) {
    // real type での範囲指定はエラー
    log_mgr().error_select_for_real(__FILE__, __LINE__, ast_expr);
  }
  switch ( ast_part.mode() ) {
  case VpiRangeMode::Const:
    return instantiate_const_partselect(parent, ast_expr, declbase,
					ast_part.left(), ast_part.right());
  case VpiRangeMode::Plus:
    return instantiate_plus_partselect(parent, ast_expr, declbase, index_env,
				       ast_part.left(), ast_part.right(),
				       var_index);
  case VpiRangeMode::Minus:
    return instantiate_minus_partselect(parent, ast_expr, declbase, index_env,
					ast_part.left(), ast_part.right(),
					var_index);
  default:
    throw std::logic_error{"never reached"};
  }
}

// @brief 固定型の範囲選択式を作る．
ElbSelecter
ExprGen::instantiate_const_partselect(
  const VlScope* parent,
  const AstExpr& ast_expr,
  const VlDeclBase* declbase,
  const AstExpr& ast_left,
  const AstExpr& ast_right
)
{
  auto index1_val = evaluate_int(parent, ast_left);
  auto index2_val = evaluate_int(parent, ast_right);
  auto big = (index1_val <= index2_val);
  if ( big ^ declbase->is_big_endian() ) {
    // 範囲の順番が逆
    log_mgr().error_range_order(__FILE__, __LINE__, ast_expr);
  }

  SizeType offset1;
  auto stat1 = declbase->calc_bit_offset(index1_val, offset1);
  if ( !stat1 ) {
    // 左のインデックスが範囲外
    // ただ値が X になるだけでエラーにはならないそうだ．
    log_mgr().warning_left_index_out_of_range(__FILE__, __LINE__,
					      ast_left.file_region());
  }

  SizeType offset2;
  auto stat2 = declbase->calc_bit_offset(index2_val, offset2);
  if ( !stat2 ) {
    // 右のインデックスが範囲外
    // ただ値が X になるだけでエラーにはならないそうだ．
    log_mgr().warning_right_index_out_of_range(__FILE__, __LINE__,
					       ast_right.file_region());
  }

  SizeType offset;
  SizeType width;
  if ( offset1 <= offset2 ) {
    offset = offset1;
    width = offset2 - offset1 + 1;
  }
  else {
    offset = offset2;
    width = offset1 - offset2 + 1;
  }
  return ElbSelecter::new_CONST(width, offset);
}

// @brief プラス型の範囲選択式を作る．
ElbSelecter
ExprGen::instantiate_plus_partselect(
  const VlScope* parent,
  const AstExpr& ast_expr,
  const VlDeclBase* declbase,
  const ElbEnv& index_env,
  const AstExpr& ast_left,
  const AstExpr& ast_right,
  bool var_index
)
{
  auto range_val = evaluate_int(parent, ast_right);
  bool is_const;
  auto base_val = evaluate_int_if_const(parent, ast_left, is_const);
  if ( is_const ) {
    // 固定インデックスだった．
    int index_val;
    if ( declbase->is_big_endian() ) {
      index_val = base_val + range_val - 1;
    }
    else {
      index_val = base_val;
    }

    SizeType offset;
    auto stat = declbase->calc_bit_offset(index_val, offset);
    if ( !stat ) {
      // 左か右のインデックスが範囲外
      // ただ値が X になるだけでエラーにはならないそうだ．
      log_mgr().warning_index_out_of_range(__FILE__, __LINE__,
					   ast_expr.file_region());
    }
    return ElbSelecter::new_CONST(range_val, offset);
  }
  if ( var_index ) {
    // 可変インデックスだった
    auto base = instantiate_expr(parent, index_env, ast_left);
    return ElbSelecter::new_PLUS(range_val, base);
  }
  // 可変インデックスはエラー
#warning "TODO: エラーメッセージ"
  throw std::logic_error{"Never reached"};
}

// @brief マイナス型の範囲選択式を作る．
ElbSelecter
ExprGen::instantiate_minus_partselect(
  const VlScope* parent,
  const AstExpr& ast_expr,
  const VlDeclBase* declbase,
  const ElbEnv& index_env,
  const AstExpr& ast_left,
  const AstExpr& ast_right,
  bool var_index
)
{
  auto range_val = evaluate_int(parent, ast_right);
  bool is_const;
  auto base_val = evaluate_int_if_const(parent, ast_left, is_const);
  if ( is_const ) {
    // 固定インデックスだった．
    int index_val;
    if ( declbase->is_big_endian() ) {
      index_val = base_val;
    }
    else {
      index_val = base_val - range_val + 1;
    }

    SizeType offset;
    auto stat = declbase->calc_bit_offset(index_val, offset);
    if ( !stat ) {
      // 左か右のインデックスが範囲外
      // ただ値が X になるだけでエラーにはならないそうだ．
      log_mgr().warning_index_out_of_range(__FILE__, __LINE__,
					   ast_expr.file_region());
    }
    return ElbSelecter::new_CONST(range_val, offset);
  }
  if ( var_index ) {
    // 可変インデックスだった
    auto base = instantiate_expr(parent, index_env, ast_left);
    return ElbSelecter::new_MINUS(range_val, base);
  }
  // 可変インデックスはエラー
#warning "TODO: エラーメッセージ"
  throw std::logic_error{"Never reached"};
}

// @brief プライマリに対応するハンドルを求める．
ObjHandle*
ExprGen::find_handle(
  const VlScope* parent,
  const ElbEnv& env,
  const AstExpr& ast_expr
)
{
  // 階層付き識別子の時 true
  auto has_hname = (ast_expr.namebranch_list().size() > 0);
  if ( has_hname ) {
    env.check_hname(ast_expr, log_mgr());
  }

  if ( env.inside_constant_function() ) {
    // まず関数内の識別子を探索する．
    auto handle = elb_mgr().find_obj_up(parent, ast_expr,
					env.constant_function());
    if ( handle != nullptr ) {
      return handle;
    }
    if ( env.is_lhs() ) {
      // 左辺ならこれ以外の探索はなし．
      log_mgr().error_not_found(__FILE__, __LINE__,
				ast_expr.file_region(),
				ast_expr.name());
    }
  }

  // 通常のスコープで探索する．
  // たぶんモジュール内でいいはず．
  auto handle = elb_mgr().find_obj_up(parent, ast_expr,
				      parent->parent_module());
  if ( handle != nullptr ) {
    if ( env.is_constant() || env.inside_constant_function() ) {
      // 定数型の識別子でなければならない．
      if ( handle->genvar() == nullptr &&
	   handle->parameter() == nullptr ) {
	//log_mgr().error_not_a_constant(__FILE__, __LINE__, ast_expr);
#warning "TODO: エラーメッセージ"
      }
    }
    return handle;
  }

  // 見つからなくてもデフォルトネットタイプが kVpiNone でないかぎり
  // 暗黙の1ビットネット宣言を行う．
  // ただし識別子に添字がついていたらだめ
  auto isize = ast_expr.index_list().size();
  auto parent_module = parent->parent_module();
  auto def_nettype = parent_module->def_net_type();
  if ( ast_expr.is_simple() &&
       !has_hname &&
       isize == 0 &&
       def_nettype != VpiNetType::None ) {
    auto decl = elb_mgr().new_ImpNet(parent, ast_expr, def_nettype);

    auto name = ast_expr.name();
    handle = elb_mgr().find_obj(parent, name);
    // 今作ったはずなので絶対見つかるはず．
    if ( handle == nullptr ) {
      throw std::logic_error{"handle == nullptr"};
    }
    return handle;
  }

  log_mgr().error_not_found(__FILE__, __LINE__,
			    ast_expr.file_region(),
			    ast_expr.name());
  return nullptr;
}

// @brief decl の型が適切がチェックする．
void
ExprGen::check_decl(
  const ElbEnv& env,
  const AstExpr& ast_expr,
  const ElbDeclRef& decl_ref,
  bool has_bit_select
)
{
  bool has_index = ast_expr.index_list().size() > 0;
  bool has_select = ast_expr.part().is_valid() || has_bit_select;
  auto decl_type = decl_ref.declbase()->type();
  if ( env.is_bitvect() ) {
    if ( decl_type != VpiObjType::Reg &&
	 decl_type != VpiObjType::Net ) {
      // ビットベクタ型ではない．
      log_mgr().error_illegal_object(__FILE__, __LINE__, ast_expr);
    }
  }
  if ( env.is_pca_lhs() ) {
    // procedural continuous assignment 文の左辺式
    if ( has_index ) {
      // 配列要素はダメ
      log_mgr().error_array_in_pca(__FILE__, __LINE__, ast_expr);
    }
    if ( has_select ) {
      // 部分指定はダメ
      log_mgr().error_select_in_pca(__FILE__, __LINE__, ast_expr);
    }
    if ( decl_type != VpiObjType::Reg &&
	 decl_type != VpiObjType::IntegerVar &&
	 decl_type != VpiObjType::RealVar &&
	 decl_type != VpiObjType::TimeVar) {
      // reg/変数以外はダメ
      log_mgr().error_illegal_object(__FILE__, __LINE__, ast_expr);
    }
  }
  else if ( env.is_force_lhs() ) {
    // force 文の左辺式
    if ( has_index ) {
      // 配列要素はダメ
      log_mgr().error_array_in_force(__FILE__, __LINE__, ast_expr);
    }
    if ( has_select ) {
      // 部分指定はダメ
      log_mgr().error_select_in_force(__FILE__, __LINE__, ast_expr);
    }
    if ( decl_type != VpiObjType::Net &&
	 decl_type != VpiObjType::Reg &&
	 decl_type != VpiObjType::IntegerVar &&
	 decl_type != VpiObjType::RealVar &&
	 decl_type != VpiObjType::TimeVar) {
      // net/reg/変数以外はダメ
      log_mgr().error_illegal_object(__FILE__, __LINE__, ast_expr);
    }
  }
  else if ( env.is_net_lhs() ) {
    if ( decl_type != VpiObjType::Net &&
	 (decl_type != VpiObjType::NetArray || !has_index) ) {
      // net 以外はダメ
      log_mgr().error_illegal_object(__FILE__, __LINE__, ast_expr);
    }
  }
  else if ( env.is_var_lhs() ) {
    if ( decl_type != VpiObjType::Reg &&
	 (decl_type != VpiObjType::RegArray || !has_index) &&
	 decl_type != VpiObjType::IntegerVar &&
	 decl_type != VpiObjType::RealVar &&
	 decl_type != VpiObjType::TimeVar &&
	 decl_type != VpiObjType::VarSelect ) {
      // reg/変数以外はダメ
      log_mgr().error_illegal_object(__FILE__, __LINE__, ast_expr);
    }
  }
  else {
    // 右辺系の環境
    if ( env.is_constant() ) {
      // 定数式
      if ( decl_type != VpiObjType::Parameter &&
	   decl_type != VpiObjType::SpecParam ) {
	// 定数(parameter)でないのでダメ
	log_mgr().error_illegal_object(__FILE__, __LINE__, ast_expr);
      }
    }

    // あとは個別の型ごとにチェックする．
    if ( decl_type == VpiObjType::RealVar && has_select ) {
      // real の部分選択は無効
      log_mgr().error_select_for_real(__FILE__, __LINE__, ast_expr);
    }
    if ( decl_type == VpiObjType::NamedEvent && !env.is_event_expr() ) {
      // イベント式以外では名前つきイベントは使えない．
      log_mgr().error_illegal_object(__FILE__, __LINE__, ast_expr);
    }
  }
}

END_NAMESPACE_YM_VERILOG
