
/// @file ExprGen_primary.cc
/// @brief ElbMgr の実装ファイル(式の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ExprGen.h"
#include "ElbEnv.h"
#include "ErrorGen.h"

#include "ym/vl/AstModule.h"
#include "ym/vl/AstExpr.h"
#include "ym/vl/AstMisc.h"

#include "ym/vl/VlModule.h"
#include "ym/vl/VlDeclArray.h"
#include "ym/vl/VlRange.h"

#include "elaborator/ElbDecl.h"
#include "elaborator/ElbParameter.h"
#include "elaborator/ElbPrimitive.h"
#include "elaborator/ElbExpr.h"
#include "elaborator/ElbGenvar.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief AstPrimary から ElbExpr を生成する．
ElbExpr*
ExprGen::instantiate_primary(
  const VlScope* parent,
  const ElbEnv& env,
  const AstExpr* ast_expr
)
{
  // 識別子の階層
  auto has_hname = (ast_expr->namebranch_list().size() > 0);
  if ( has_hname ) {
    if ( env.is_constant() ) {
      // 階層つき識別子はだめ
      ErrorGen::hname_in_ce(__FILE__, __LINE__, ast_expr);
    }
    if ( env.inside_constant_function() ) {
      // 階層つき識別子はだめ
      ErrorGen::hname_in_cf(__FILE__, __LINE__, ast_expr);
    }
  }

  // 識別子の名前
  auto name = ast_expr->name();

  // 識別子の添字の次元
  auto isize = ast_expr->index_list().size();

  // 名前に対応したオブジェクトのハンドル
  auto handle = (ObjHandle*){nullptr};
  if ( env.is_constant() ) {
    handle = find_const_handle(parent, ast_expr);
  }
  else if ( env.inside_constant_function() ) {
    // まず関数内の識別子を探索する．
    handle = mgr().find_obj_up(parent, ast_expr, env.constant_function());
    if ( handle == nullptr && !env.is_lhs() ) {
      // 右辺ならモジュール内の定数識別子を探索する．
      handle = find_const_handle(parent, ast_expr);
    }
  }
  else {
    // 通常のスコープで探索する．
    // たぶんモジュール内でいいはず．
    handle = mgr().find_obj_up(parent, ast_expr,  parent->parent_module());
    if ( handle == nullptr ) {
      // 見つからなくてもデフォルトネットタイプが kVpiNone でないかぎり
      // 暗黙の1ビットネット宣言を行う．
      // ただし識別子に添字がついていたらだめ
      auto parent_module = parent->parent_module();
      auto def_nettype = parent_module->def_net_type();
      if ( ast_expr->is_simple() &&
	   !has_hname &&
	   isize == 0 &&
	   def_nettype != VpiNetType::None ) {
	auto decl = mgr().new_ImpNet(parent, ast_expr, def_nettype);

	handle = mgr().find_obj(parent, name);
	// 今作ったはずなので絶対見つかるはず．
	if ( handle == nullptr ) {
	  throw std::logic_error{"handle == nullptr"};
	}
      }
    }
    if ( handle == nullptr ) {
      ErrorGen::not_found(__FILE__, __LINE__, ast_expr);
    }
  }


  if ( env.is_system_tf_arg() ) {
    // システム関数/タスクの引数の場合
    if ( isize == 0 ) {
      auto scope = handle->scope();
      if ( scope ) {
	return mgr().new_ArgHandle(ast_expr, scope);
      }

      auto primitive = handle->primitive();
      if ( primitive ) {
	return mgr().new_ArgHandle(ast_expr, primitive);
      }

      auto decl = handle->decl();
      if ( decl ) {
	return mgr().new_Primary(ast_expr, decl);
      }

      auto declarray = handle->declarray();
      if ( declarray ) {
	return mgr().new_ArgHandle(ast_expr, declarray);
      }
    }
    else if ( isize == 1 ) {
      auto index_list = ast_expr->index_list().to_vector();
      auto ast_expr1 = index_list[0];
      auto index = evaluate_int(parent, ast_expr1);
      auto scope = handle->array_elem(index);
      if ( scope ) {
	return mgr().new_ArgHandle(ast_expr, scope);
      }

      auto prim_array = handle->prim_array();
      if ( prim_array ) {
	auto primitive = prim_array->elem_by_index(index);
	if ( primitive ) {
	  return mgr().new_ArgHandle(ast_expr, primitive);
	}
      }
    }
    ErrorGen::illegal_object(__FILE__, __LINE__, ast_expr);
  }

  if ( !env.is_lhs() ) {
    // 対象のオブジェクトが genvar の場合
    auto genvar = handle->genvar();
    if ( genvar ) {
      return instantiate_genvar(parent, ast_expr, genvar->value());
    }
  }

  // 添字には constant/constant function 以外の情報は引き継がない
  ElbEnv index_env;
  if ( ast_expr->is_const_index() ) {
    index_env = ElbConstantEnv();
  }
  else if ( env.inside_constant_function() ) {
    index_env = ElbConstantFunctionEnv(env.constant_function());
  }

  // 対象のオブジェクトが宣言要素だった場合
  bool is_array;
  bool has_range_select;
  bool has_bit_select;
  auto primary = instantiate_primary_sub(handle, parent,
					 index_env, ast_expr,
					 is_array,
					 has_range_select,
					 has_bit_select);

  auto decl_base = primary->decl_base();
  if ( decl_base == nullptr ) {
    throw std::logic_error{"decl_base == nullptr"};
  }
  auto decl_type = decl_base->type();

  // 式をチェックする．
  // 不適切な場合は例外が送出される．
  check_decl(env, ast_expr, decl_type, is_array,
	     has_range_select | has_bit_select);

  if ( has_bit_select ) {
    // ビット指定付きの場合
    auto index_vec = ast_expr->index_list().to_vector();
    auto ast_expr1 = index_vec[isize - 1];
    bool is_const;
    int index_val = evaluate_int_if_const(parent, ast_expr1, is_const);
    if ( is_const ) {
      // 固定インデックスだった．
      SizeType offset;
      auto stat2 = decl_base->calc_bit_offset(index_val, offset);
      if ( !stat2 ) {
	// インデックスが範囲外
	// ただ値が X になるだけでエラーにはならないそうだ．
	put_warning(__FILE__, __LINE__,
		    ast_expr1->file_region(),
		    "ELAB",
		    "Bit-Select index is out of range.");
      }
      return mgr().new_BitSelect(ast_expr, primary, ast_expr1, index_val);
    }
    else {
      // 可変インデックスだった．
      auto index = instantiate_expr(parent, index_env, ast_expr1);
      return mgr().new_BitSelect(ast_expr, primary, index);
    }
  }
  if ( has_range_select ) {
    // 範囲指定付きの場合
    auto ast_part = ast_expr->part();
    switch ( ast_part->mode() ) {
    case VpiRangeMode::Const:
    {
      auto ast_left = ast_part->left();
      auto index1_val = evaluate_int(parent, ast_left);
      auto ast_right = ast_part->right();
      auto index2_val = evaluate_int(parent, ast_right);
      auto big = (index1_val >= index2_val);
      if ( big ^ decl_base->is_big_endian() ) {
	// 範囲の順番が逆
	ErrorGen::range_order(__FILE__, __LINE__, ast_expr);
      }

      SizeType offset;
      auto stat3 = decl_base->calc_bit_offset(index1_val, offset);
      if ( !stat3 ) {
	// 左のインデックスが範囲外
	// ただ値が X になるだけでエラーにはならないそうだ．
	put_warning(__FILE__, __LINE__,
		    ast_left->file_region(),
		    "ELAB",
		    "Left index is out of range.");
      }

      auto stat4 = decl_base->calc_bit_offset(index2_val, offset);
      if ( !stat4 ) {
	// 右のインデックスが範囲外
	// ただ値が X になるだけでエラーにはならないそうだ．
	put_warning(__FILE__, __LINE__,
		    ast_right->file_region(),
		    "ELAB",
		    "Right index is out of range.");
      }

      return mgr().new_PartSelect(ast_expr, primary,
				  ast_left, ast_right,
				  index1_val, index2_val);
    }

    case VpiRangeMode::Plus:
    {
      auto ast_range = ast_part->right();
      auto range_val = evaluate_int(parent, ast_range);
      auto ast_base = ast_part->left();
      bool is_const;
      auto base_val = evaluate_int_if_const(parent, ast_base, is_const);
      if ( is_const ) {
	// 固定インデックスだった．
	int index1_val;
	int index2_val;
	if ( decl_base->is_big_endian() ) {
	  index1_val = base_val + range_val - 1;
	  index2_val = base_val;
	}
	else {
	  index1_val = base_val;
	  index2_val = base_val + range_val - 1;
	}

	SizeType offset;
	auto stat3 = decl_base->calc_bit_offset(index1_val, offset);
	auto stat4 = decl_base->calc_bit_offset(index2_val, offset);
	if ( !stat3 || !stat4 ) {
	  // 左か右のインデックスが範囲外
	  // ただ値が X になるだけでエラーにはならないそうだ．
	  put_warning(__FILE__, __LINE__,
		      ast_expr->file_region(),
		      "ELAB",
		      "Index is out of range.");
	}
	return mgr().new_PartSelect(ast_expr, primary,
				    ast_base, ast_range,
				    index1_val, index2_val);
      }
      else {
	// 可変インデックスだった．
	auto base = instantiate_expr(parent, index_env, ast_base);
	return mgr().new_PlusPartSelect(ast_expr, primary,
					base, ast_range, range_val);
      }
    }

    case VpiRangeMode::Minus:
    {
      auto ast_range = ast_part->right();
      auto range_val = evaluate_int(parent, ast_range);
      auto ast_base = ast_part->left();
      bool is_const;
      auto base_val = evaluate_int_if_const(parent, ast_base, is_const);
      if ( is_const ) {
	// 固定インデックスだった．
	int index1_val;
	int index2_val;
	if ( decl_base->is_big_endian() ) {
	  index1_val = base_val;
	  index2_val = base_val - range_val + 1;
	}
	else {
	  index1_val = base_val - range_val + 1;
	  index2_val = base_val;
	}

	SizeType offset;
	auto stat3 = decl_base->calc_bit_offset(index1_val, offset);
	auto stat4 = decl_base->calc_bit_offset(index2_val, offset);
	if ( !stat3 || !stat4 ) {
	  // 左か右のインデックスが範囲外
	  // ただ値が X になるだけでエラーにはならないそうだ．
	  put_warning(__FILE__, __LINE__,
		      ast_expr->file_region(),
		      "ELAB",
		      "Index is out of range.");
	}
	return mgr().new_PartSelect(ast_expr, primary,
				    ast_base, ast_range,
				    index1_val, index2_val);
      }
      else {
	// 可変インデックスだった．
	auto base = instantiate_expr(parent, index_env, ast_base);
	return mgr().new_MinusPartSelect(ast_expr, primary,
					 base, ast_range, range_val);
      }
    }

    case VpiRangeMode::No:
      throw std::logic_error{"Should not be reached"};
    }
  }
  return primary;
}

// @brief AstExpr(primary) から named_event を生成する．
ElbExpr*
ExprGen::instantiate_namedevent(
  const VlScope* parent,
  const AstExpr* ast_expr
)
{
  if ( ast_expr->type() != AstExpr::Primary ) {
    throw std::logic_error{"ast_expr->type() != AstExpr::Primary"};
  }
  if ( ast_expr->part() != nullptr ) {
    throw std::logic_error{"ast_expr->part() != nullptr"};
  }

  // 名前に対応したオブジェクトのハンドルを求める．
  auto handle = mgr().find_obj_up(parent, ast_expr, nullptr);
  if ( handle == nullptr ) {
    // 見つからなかった．
    ErrorGen::not_found(__FILE__, __LINE__, ast_expr);
  }

  // 配列要素などの処理を行う．
  ElbEnv env0;
  if ( ast_expr->is_const_index() ) {
    env0 = ElbConstantEnv();
  }

  bool is_array;
  bool has_range_select;
  bool has_bit_select;
  auto primary = instantiate_primary_sub(handle, parent, env0, ast_expr,
					 is_array,
					 has_range_select,
					 has_bit_select);

  auto decl_base = primary->decl_base();
  if ( decl_base == nullptr ) {
    throw std::logic_error{"decl_base == nullptr"};
  }
  auto decl_type = decl_base->type();
  if ( decl_type != VpiObjType::NamedEvent ) {
    // 型が違う
    ErrorGen::not_a_namedevent(__FILE__, __LINE__, ast_expr);
  }
  if ( has_range_select || has_bit_select ) {
    // 部分選択，ビット選択は使えない．
    ErrorGen::select_for_namedevent(__FILE__, __LINE__, ast_expr);
  }

  return primary;
}

// @brief 定数識別子を探す．
ObjHandle*
ExprGen::find_const_handle(
  const VlScope* parent,
  const AstExpr* ast_expr
)
{
  // モジュール内の識別子を探索する．
  auto handle = mgr().find_obj_up(parent, ast_expr, parent->parent_module());
  if ( handle == nullptr ) {
    // 見つからなかった．
    ErrorGen::not_found(__FILE__, __LINE__, ast_expr);
  }

  // handle が持つオブジェクトは genvar か parameter でなければならない．
  if ( handle->genvar() == nullptr && handle->parameter() == nullptr ) {
    ErrorGen::not_a_parameter(__FILE__, __LINE__, ast_expr);
  }

  return handle;
}

// @brief genvar に対応した定数を生成する．
ElbExpr*
ExprGen::instantiate_genvar(
  const VlScope* parent,
  const AstExpr* ast_expr,
  int val
)
{
  auto has_range_select = (ast_expr->part() != nullptr);
  auto index_list = ast_expr->index_list();
  auto isize = index_list.size();
  if (  isize > 1 || (isize == 1 && has_range_select) ) {
    // 配列型ではない．
    ErrorGen::dimension_mismatch(__FILE__, __LINE__, ast_expr);
  }

  auto has_bit_select = (isize == 1);
  if ( has_bit_select ) {
    auto index1 = evaluate_int(parent, index_list.front());
    val >>= index1;
    val &= 1;
  }
  else if ( has_range_select ) {
    auto ast_part = ast_expr->part();
    auto index1 = evaluate_int(parent, ast_part->left());
    auto index2 = evaluate_int(parent, ast_part->right());
    val >>= index2;
    val &= ((1 << (index1 - index2 + 1)) - 1);
  }

  // genvar の値に対応した定数式を生成
  return mgr().new_GenvarConstant(ast_expr, val);
}

// @brief 宣言要素のインスタンス化を行う．
ElbExpr*
ExprGen::instantiate_primary_sub(
  ObjHandle* handle,
  const VlScope* parent,
  const ElbEnv& env,
  const AstExpr* ast_expr,
  bool& is_array,
  bool& has_range_select,
  bool& has_bit_select
)
{
  // 配列の次元
  SizeType dsize = 0;
  // プライマリ式の次元 (ビット指定を含んでいる可能性あり)
  auto isize = ast_expr->index_list().size();

  // 範囲指定があるとき true となるフラグ
  has_range_select = (ast_expr->part() != nullptr);

  // 答え
  auto primary = (ElbExpr*){nullptr};
  VlValueType value_type;
  if ( handle->type() == VpiObjType::Parameter ) {
    // パラメータの場合
    auto param = handle->parameter();
    primary = mgr().new_Primary(ast_expr, param);
    is_array = false;
    value_type = param->value_type();
  }
  else {
    // それ以外の宣言要素の場合
    auto decl = handle->decl();
    auto declarray = handle->declarray();
    if ( decl != nullptr ) {
      primary = mgr().new_Primary(ast_expr, decl);
      is_array = false;
      value_type = decl->value_type();
    }
    else if ( declarray != nullptr ) {
      // 配列の次元
      dsize = declarray->dimension();
      if ( isize != dsize && (isize != dsize + 1 || has_range_select) ) {
	// 次元が合わない．
	ErrorGen::dimension_mismatch(__FILE__, __LINE__, ast_expr);
      }

      is_array = true;
      value_type = declarray->value_type();

      // 添字が定数ならオフセットを計算する．
      SizeType offset = 0;
      SizeType mlt = 1;
      auto const_index = true;
      auto ast_index_list = ast_expr->index_list().to_vector();
      for ( SizeType i = 0; i < dsize; ++ i ) {
	auto j = dsize - i - 1;
	auto ast_expr1 = ast_index_list[j];
	bool is_const;
	auto index_val = evaluate_int_if_const(parent, ast_expr1, is_const);
	if ( is_const ) {
	  offset += index_val * mlt;
	  mlt *= declarray->range(j)->size();
	}
	else {
	  const_index = false;
	}
      }
      if ( const_index ) {
	primary = mgr().new_Primary(ast_expr, declarray, offset);
      }
      else {
	// 添字の式を生成する．
	std::vector<ElbExpr*> index_list;
	index_list.reserve(dsize);
	for ( SizeType i = 0; i < dsize; ++ i ) {
	  auto ast_expr1 = ast_index_list[i];
	  auto expr1 = instantiate_expr(parent, env, ast_expr1);
	  index_list.push_back(expr1);
	}

	primary = mgr().new_Primary(ast_expr, declarray, index_list);
      }
    }
  }
  if ( primary == nullptr ) {
    // 適切な型ではなかった．
    ErrorGen::illegal_object(__FILE__, __LINE__, ast_expr);
  }

  // ビット指定があるとき true となるフラグ
  has_bit_select = false;

  if ( isize == dsize + 1 && !has_range_select ) {
    // 識別子の添字の次元と配列の次元が 1 違いで
    // 範囲がなければ識別子の最後の添字はビット指定と見なす．
    has_bit_select = true;
    -- isize;
  }

  if ( isize != dsize ) {
    // 次元が会わない．
    ErrorGen::dimension_mismatch(__FILE__, __LINE__, ast_expr);
  }

  if ( has_range_select || has_bit_select ) {
    if ( value_type.is_real_type() ) {
      ErrorGen::select_for_real(__FILE__, __LINE__, ast_expr);
    }
  }

  return primary;
}

// @brief decl の型が適切がチェックする．
void
ExprGen::check_decl(
  const ElbEnv& env,
  const AstExpr* ast_expr,
  VpiObjType decl_type,
  bool is_array,
  bool has_select
)
{
  if ( env.is_pca_lhs() ) {
    // procedural continuous assignment 文の左辺式
    if ( is_array ) {
      // 配列要素はダメ
      ErrorGen::array_in_pca(__FILE__, __LINE__, ast_expr);
    }
    if ( has_select ) {
      // 部分指定はダメ
      ErrorGen::select_in_pca(__FILE__, __LINE__, ast_expr);
    }
    if ( decl_type != VpiObjType::Reg &&
	 decl_type != VpiObjType::IntegerVar &&
	 decl_type != VpiObjType::RealVar &&
	 decl_type != VpiObjType::TimeVar) {
      // reg/変数以外はダメ
      ErrorGen::illegal_object(__FILE__, __LINE__, ast_expr);
    }
  }
  else if ( env.is_force_lhs() ) {
    // force 文の左辺式
    if ( is_array ) {
      // 配列要素はダメ
      ErrorGen::array_in_force(__FILE__, __LINE__, ast_expr);
    }
    if ( has_select ) {
      // 部分指定はダメ
      ErrorGen::select_in_force(__FILE__, __LINE__, ast_expr);
    }
    if ( decl_type != VpiObjType::Net &&
	 decl_type != VpiObjType::Reg &&
	 decl_type != VpiObjType::IntegerVar &&
	 decl_type != VpiObjType::RealVar &&
	 decl_type != VpiObjType::TimeVar) {
      // net/reg/変数以外はダメ
      ErrorGen::illegal_object(__FILE__, __LINE__, ast_expr);
    }
  }
  else if ( env.is_net_lhs() ) {
    if ( decl_type != VpiObjType::Net &&
	 (decl_type != VpiObjType::NetArray || !is_array) ) {
      // net 以外はダメ
      ErrorGen::illegal_object(__FILE__, __LINE__, ast_expr);
    }
  }
  else if ( env.is_var_lhs() ) {
    if ( decl_type != VpiObjType::Reg &&
	 (decl_type != VpiObjType::RegArray || !is_array) &&
	 decl_type != VpiObjType::IntegerVar &&
	 decl_type != VpiObjType::RealVar &&
	 decl_type != VpiObjType::TimeVar &&
	 decl_type != VpiObjType::VarSelect ) {
      // reg/変数以外はダメ
      ErrorGen::illegal_object(__FILE__, __LINE__, ast_expr);
    }
  }
  else {
    // 右辺系の環境
    if ( env.is_constant() ) {
      // 定数式
      if ( decl_type != VpiObjType::Parameter &&
	   decl_type != VpiObjType::SpecParam ) {
	// 定数(parameter)でないのでダメ
	ErrorGen::illegal_object(__FILE__, __LINE__, ast_expr);
      }
    }

    // あとは個別の型ごとにチェックする．
    if ( decl_type == VpiObjType::RealVar && has_select ) {
      // real の部分選択は無効
      ErrorGen::select_for_real(__FILE__, __LINE__, ast_expr);
    }
    if ( decl_type == VpiObjType::NamedEvent && !env.is_event_expr() ) {
      // イベント式以外では名前つきイベントは使えない．
      ErrorGen::illegal_object(__FILE__, __LINE__, ast_expr);
    }
  }
}

END_NAMESPACE_YM_VERILOG
