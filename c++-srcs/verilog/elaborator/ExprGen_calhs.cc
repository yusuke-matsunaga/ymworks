
/// @file ExprGen_calhs.cc
/// @brief ExprGen の実装ファイル(継続的代入文の左辺)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ExprGen.h"
#include "ym/vl/BitVector.h"
#include "ym/vl/AstItem.h"
#include "ym/vl/AstExpr.h"
#include "elaborator/ElbDeclRef.h"
#include "elaborator/ElbExpr.h"
#include "elaborator/ElbError.h"
#include "ElbEnv.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ExprGen
//////////////////////////////////////////////////////////////////////

// @brief AstExpr から継続的代入文の左辺式を生成する
ElbExpr*
ExprGen::instantiate_net_lhs(
  const VlScope* parent,
  const AstExpr& ast_expr
)
{
  switch ( ast_expr.type() ) {
  case AstExpr::Opr:
    // 左辺では concatination しか適当でない．
    if ( ast_expr.op_type() == VpiOpType::Concat ) {
      auto ast_opr_list = ast_expr.operand_list().to_vector();
      auto opr_size = ast_opr_list.size();
      std::vector<ElbExpr*> opr_list(opr_size);
      for ( SizeType i = 0; i < opr_size; ++ i ) {
	// 現れた順は上位ビットからなので位置が逆になる．
	SizeType pos = opr_size - i - 1;
	auto ast_expr1 = ast_opr_list[pos];
	auto expr1 = instantiate_net_lhs(parent, ast_expr1);
	opr_list[pos] = expr1;
      }
      auto expr = elb_mgr().new_ConcatOp(ast_expr, opr_list);
      expr->set_selfsize();

      // attribute instance の生成
      auto attr_list = attribute_list(ast_expr);
      elb_mgr().reg_attr(expr, attr_list);

      return expr;
    }
    // それ以外の演算子はエラー
    log_mgr().error_illegal_operator_in_lhs(__FILE__, __LINE__, ast_expr);
    break;

  case AstExpr::Primary:
    return instantiate_net_primary(parent, ast_expr);

  case AstExpr::Const:
    log_mgr().error_illegal_constant_in_lhs(__FILE__, __LINE__, ast_expr);

  case AstExpr::FuncCall:
    log_mgr().error_illegal_funccall_in_lhs(__FILE__, __LINE__, ast_expr);

  case AstExpr::SysFuncCall:
    log_mgr().error_illegal_sysfunccall_in_lhs(__FILE__, __LINE__, ast_expr);

  default:
    break;
  }

  throw std::logic_error{"Should not be reached"};
}

// @brief AstPrimary から ElbExpr を生成する．
ElbExpr*
ExprGen::instantiate_net_primary(
  const VlScope* parent,
  const AstExpr& ast_expr
)
{
  if ( ast_expr.namebranch_list().size() > 0 ) {
    // 階層付き識別子はエラー
    throw std::logic_error{"Net LHS should not have hierarchical name"};
  }

  // 名前に対応したオブジェクトのハンドルを取り出す．
  auto env = ElbEnv();
  auto handle = find_handle(parent, env, ast_expr);

#if 0
  // 識別子の名前
  auto name = ast_expr.name();

  // 識別子の添字の次元
  auto isize = ast_expr.index_list().size();

  // 名前に対応したオブジェクトのハンドルを通常のスコープで探索する．
  // たぶんモジュール内でいいはず．
  auto handle = elb_mgr().find_obj_up(parent, ast_expr,  parent->parent_module());
  if ( handle == nullptr ) {
    // 見つからなくてもデフォルトネットタイプが kVpiNone でないかぎり
    // 暗黙の1ビットネット宣言を行う．
    // ただし識別子に添字がついていたらだめ
    auto parent_module = parent->parent_module();
    auto def_nettype = parent_module->def_net_type();
    if ( ast_expr.is_simple() &&
	 isize == 0 &&
	 def_nettype != VpiNetType::None ) {
      auto decl = elb_mgr().new_ImpNet(parent, ast_expr, def_nettype);

      handle = elb_mgr().find_obj(parent, name);
      // 今作ったはずなので絶対見つかるはず．
      if ( handle == nullptr ) {
	throw std::logic_error{"handle == nullptr"};
      }
    }
  }
  if ( handle == nullptr ) {
    log_mgr().error_not_found(__FILE__, __LINE__,
			      ast_expr.file_region(),
			      ast_expr.name());
  }
#endif

  auto index_env = ElbConstantEnv();
  AstExpr ast_bit;
  auto decl_ref = instantiate_decl_ref(handle, parent, index_env,
				       ast_expr, ast_bit);
  if ( ast_bit.is_valid() ) {
    // ビット選択あり
    return instantiate_bitselect(parent, ast_expr, decl_ref,
				 index_env, ast_bit, false);
  }
  auto ast_part = ast_expr.part();
  if ( ast_part.is_valid() ) {
    // 部分選択あり
    return instantiate_partselect(parent, ast_expr, decl_ref,
				  index_env, ast_part, false);
  }
  // 単純なプライマリ式
  return elb_mgr().new_Primary(ast_expr, decl_ref);
#if 0
  bool is_array;
  bool has_range_select;
  bool has_bit_select;
  // 必ず固定インデックスになるはず．
  ElbConstantEnv index_env;
  auto primary = instantiate_ca_primary_sub(handle, parent,
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
    auto index_vec = ast_expr.index_list().to_vector();
    auto ast_expr1 = index_vec[isize - 1];
    bool is_const;
    int index_val = evaluate_int_if_const(parent, ast_expr1, is_const);
    if ( !is_const ) {
      // ネット左辺(継続的代入文)では可変インデックスは使えない．
#warning "TODO: エラーメッセージ"
    }
    SizeType offset;
    auto stat2 = decl_base->calc_bit_offset(index_val, offset);
    if ( !stat2 ) {
      // インデックスが範囲外
      // ただ値が X になるだけでエラーにはならないそうだ．
      log_mgr().warning_index_out_of_range(__FILE__, __LINE__,
					   ast_expr1.file_region());
    }
    return elb_mgr().new_BitSelect(ast_expr, primary, ast_expr1, index_val);
  }
  if ( has_range_select ) {
    // 範囲指定付きの場合
    auto ast_part = ast_expr.part();
    switch ( ast_part.mode() ) {
    case VpiRangeMode::Const:
    {
      auto ast_left = ast_part.left();
      auto index1_val = evaluate_int(parent, ast_left);
      auto ast_right = ast_part.right();
      auto index2_val = evaluate_int(parent, ast_right);
      auto big = (index1_val >= index2_val);
      if ( big ^ decl_base->is_big_endian() ) {
	// 範囲の順番が逆
	log_mgr().error_range_order(__FILE__, __LINE__, ast_expr);
      }

      SizeType offset;
      auto stat3 = decl_base->calc_bit_offset(index1_val, offset);
      if ( !stat3 ) {
	// 左のインデックスが範囲外
	// ただ値が X になるだけでエラーにはならないそうだ．
	log_mgr().warning_left_index_out_of_range(__FILE__, __LINE__,
						  ast_left.file_region());
      }

      auto stat4 = decl_base->calc_bit_offset(index2_val, offset);
      if ( !stat4 ) {
	// 右のインデックスが範囲外
	// ただ値が X になるだけでエラーにはならないそうだ．
	log_mgr().warning_right_index_out_of_range(__FILE__, __LINE__,
						   ast_right.file_region());
      }

      return elb_mgr().new_PartSelect(ast_expr, primary,
				  ast_left, ast_right,
				  index1_val, index2_val);
    }

    case VpiRangeMode::Plus:
    {
      auto ast_range = ast_part.right();
      auto range_val = evaluate_int(parent, ast_range);
      auto ast_base = ast_part.left();
      bool is_const;
      auto base_val = evaluate_int_if_const(parent, ast_base, is_const);
      if ( !is_const ) {
	// ネット左辺(継続的代入文)では可変インデックスは使えない．
#warning "TODO: エラーメッセージ"
      }
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
	log_mgr().warning_index_out_of_range(__FILE__, __LINE__,
					     ast_expr.file_region());
      }
      return elb_mgr().new_PartSelect(ast_expr, primary,
				      ast_base, ast_range,
				      index1_val, index2_val);
    }

    case VpiRangeMode::Minus:
    {
      auto ast_range = ast_part.right();
      auto range_val = evaluate_int(parent, ast_range);
      auto ast_base = ast_part.left();
      bool is_const;
      auto base_val = evaluate_int_if_const(parent, ast_base, is_const);
      if ( !is_const ) {
	// ネット左辺(継続的代入文)では可変インデックスは使えない．
#warning "TODO: エラーメッセージ"
      }
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
	log_mgr().warning_index_out_of_range(__FILE__, __LINE__,
					     ast_expr.file_region());
      }
      return elb_mgr().new_PartSelect(ast_expr, primary,
				      ast_base, ast_range,
				      index1_val, index2_val);
    }

    case VpiRangeMode::No:
      throw std::logic_error{"Should not be reached"};
    }
  }
  return primary;
#endif
}

#if 0
// @brief 宣言要素のインスタンス化を行う．
ElbExpr*
ExprGen::instantiate_ca_primary_sub(
  ObjHandle* handle,
  const VlScope* parent,
  const AstExpr& ast_expr,
  bool& is_array,
  bool& has_range_select,
  bool& has_bit_select
)
{
  // 配列の次元
  SizeType dsize = 0;
  // プライマリ式の次元 (ビット指定を含んでいる可能性あり)
  auto isize = ast_expr.index_list().size();

  // 範囲指定があるとき true となるフラグ
  has_range_select = ast_expr.part().is_valid();

  // 答え
  auto primary = (ElbExpr*){nullptr};
  VlValueType value_type;
  auto decl = handle->decl();
  auto declarray = handle->declarray();
  if ( decl != nullptr ) {
    primary = elb_mgr().new_Primary(ast_expr, decl);
    is_array = false;
    value_type = decl->value_type();
  }
  else if ( declarray != nullptr ) {
    // 配列の次元
    dsize = declarray->dimension();
    if ( isize != dsize && (isize != dsize + 1 || has_range_select) ) {
      // 次元が合わない．
      log_mgr().error_dimension_mismatch(__FILE__, __LINE__, ast_expr);
    }

    is_array = true;
    value_type = declarray->value_type();

    // 添字が定数ならオフセットを計算する．
    SizeType offset = 0;
    SizeType mlt = 1;
    auto ast_index_list = ast_expr.index_list().to_vector();
    for ( SizeType i = 0; i < dsize; ++ i ) {
      auto j = dsize - i - 1;
      auto ast_expr1 = ast_index_list[j];
      bool is_const;
      auto index_val = evaluate_int_if_const(parent, ast_expr1, is_const);
      if ( !is_const ) {
	// 可変インデックスは使えない．
#warning "TODO: エラーメッセージ"
      }
    }
    primary = elb_mgr().new_Primary(ast_expr, declarray, offset);
  }
  if ( primary == nullptr ) {
    // 適切な型ではなかった．
    log_mgr().error_illegal_object(__FILE__, __LINE__, ast_expr);
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
    log_mgr().error_dimension_mismatch(__FILE__, __LINE__, ast_expr);
  }

  if ( has_range_select || has_bit_select ) {
    if ( value_type.is_real_type() ) {
      log_mgr().error_select_for_real(__FILE__, __LINE__, ast_expr);
    }
  }

  return primary;
}
#endif

END_NAMESPACE_YM_VERILOG
