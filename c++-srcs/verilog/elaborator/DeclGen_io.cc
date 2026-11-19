
/// @file DeclGen_io.cc
/// @brief DeclGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "DeclGen.h"
#include "ElbEnv.h"
#include "elaborator/ElbDeclHead.h"
#include "elaborator/ElbDecl.h"
#include "elaborator/ElbDeclArray.h"
#include "elaborator/ElbDeclRef.h"
#include "elaborator/ElbParameter.h"
#include "elaborator/ElbGenvar.h"
#include "elaborator/ElbModule.h"
#include "elaborator/ElbTaskFunc.h"
#include "elaborator/ElbRange.h"
#include "elaborator/ElbExpr.h"
#include "elaborator/RangeVal.h"
#include "elaborator/ElbError.h"


#define DOUT std::cout

BEGIN_NAMESPACE_YM_VERILOG

BEGIN_NONAMESPACE

inline
const VlScope*
get_scope(
  ElbModule* module,
  ElbTaskFunc* taskfunc
)
{
  if ( module != nullptr ) {
    return module;
  }
  if ( taskfunc != nullptr ) {
    return taskfunc;
  }
  throw std::logic_error{"get_scope(): module == nullptr && task == nullptr"};
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス DeclGen
//////////////////////////////////////////////////////////////////////

// @brief IO宣言要素をインスタンス化する．
void
DeclGen::instantiate_iodecl(
  ElbModule* module,
  ElbTaskFunc* taskfunc,
  const AstIOHeadList& ast_head_list
)
{
  auto scope = get_scope(module, taskfunc);
  for ( auto ast_head: ast_head_list ) {
    auto def_aux_type = ast_head.aux_type();
    auto sign = ast_head.is_signed();
    auto ast_range = ast_head.range();
    auto has_range = ast_range.is_valid();

    // 範囲指定を持っている場合には範囲を計算する．
    RangeVal range;
    if ( has_range ) {
      range = evaluate_range(scope, ast_range);
    }

    // ヘッダ情報の生成
    // ちなみに IOHead は範囲の情報を持たない．
    auto head = instantiate_iohead(module, taskfunc, ast_head);

    for ( auto ast_item: ast_head.item_list() ) {
      // IO定義と変数/ネット定義が一致しているか調べる．
      auto handle = elb_mgr().find_obj(scope, ast_item.name());
      auto decl = (ElbDecl*){nullptr};
      if ( handle != nullptr ) {
	// 同名の要素が見つかった．
	if ( def_aux_type != VpiAuxType::None ) {
	  // なのに IO 宣言の aux_type もある．
	  log_mgr().error_dup_type(__FILE__, __LINE__,
				   ast_item,
				   handle->file_region());
	}
	if ( handle->declarray() != nullptr ) {
	  // 対象が配列だった場合．
	  log_mgr().error_array_in_io_decl(__FILE__, __LINE__,
					   ast_item);
	}
	decl = handle->decl();
	if ( decl != nullptr ) {
	  // 対象が宣言要素だった場合．
	  auto type = decl->type();
	  if ( (module == nullptr || type != VpiObjType::Net) &&
	       type != VpiObjType::Reg &&
	       type != VpiObjType::IntegerVar &&
	       type != VpiObjType::TimeVar) {
	    // ちょっと論理式が分かりにくいが，上の式を否定すると，
	    // - module かつネットは OK
	    // - reg/integer/time は OK
	    // ということになる．
	    // それ以外は nullptr にしておく．
	    // 直後の error_illegal_io_decl() が起動される．
	    decl = nullptr;
	  }
	}
	if ( decl == nullptr ) {
	  // 不適切な型だった場合．
	  auto is_module = module != nullptr;
	  log_mgr().error_illegal_io_decl(__FILE__, __LINE__,
					  ast_item, is_module);
	}

	// ここに来たら decl != nullptr

	// decl と型が一致しているか調べる．
	// IEEE 1364-2001 12.3.3 Port declarations
	if ( decl->has_range() ) {
	  int left_val2 = decl->left_range_val();
	  int right_val2 = decl->right_range_val();
	  if ( !has_range ) {
	    // decl は範囲を持っているが IO は持っていない．
	    // これはエラーにしなくてよいのだろうか？
	    // たぶんコンパイルオプションで制御すべき
	    if ( allow_empty_io_range() ) {
	      range.left = left_val2;
	      range.right = right_val2;
	    }
	    else {
	      log_mgr().error_conflict_io_range(__FILE__, __LINE__, ast_item);
	    }
	  }
	  else if ( range.left != left_val2 || range.right != right_val2 ) {
	    // 範囲が異なっていた．
	    log_mgr().error_conflict_io_range(__FILE__, __LINE__, ast_item, decl);
	  }
	}
	else if ( has_range ) {
	  // decl は範囲を持っていないが IO は持っている．
	  // エラーとする．
	  log_mgr().error_conflict_io_range(__FILE__, __LINE__, ast_item);
	}
	// どちらか一方でも符号付きなら両方符号付きにする．
	// ちょっと ad-hoc な仕様
	if ( !decl->is_signed() && sign ) {
	  decl->set_signed();
	}
      }
      else {
	// 同名の要素が見つからなかったので作る必要がある．
	auto aux_type = def_aux_type;
	if ( aux_type == VpiAuxType::None ) {
	  if ( module ) {
	    // モジュール IO の場合は `default_net_type を参照する．
	    auto net_type = module->def_net_type();
	    if ( net_type == VpiNetType::None ) {
	      log_mgr().error_no_impnet(__FILE__, __LINE__, ast_item);
	    }
	    aux_type = VpiAuxType::Net;
	  }
	  else {
	    // task/function の場合，型指定が無い時は reg 型となる．
	    aux_type = VpiAuxType::Reg;
	  }
	}

	// ヘッダを生成する．
	ElbDeclHead* head = nullptr;
	if ( has_range ) {
	  head = elb_mgr().new_DeclHead(scope, ast_head, aux_type,
					ast_range, range);
	}
	else {
	  head = elb_mgr().new_DeclHead(scope, ast_head, aux_type);
	}

	// 初期値を生成する．
	auto ast_init = ast_item.init_value();
	auto init = (ElbExpr*){nullptr};
	if ( module ) {
	  if ( ast_init.is_valid() ) {
	    // 初期値を持つ場合
	    if ( aux_type == VpiAuxType::Net ) {
	      // net 型の場合(ここに来るのは暗黙宣言のみ)は初期値を持てない．
	      log_mgr().error_impnet_with_init(__FILE__, __LINE__, ast_item);
	    }
	    // これは verilog_grammer.yy の list_of_variable_port_identifiers
	    // に対応するので必ず constant_expression である．
	    init = instantiate_constant_expr(scope, ast_init);
	  }
	}
	else {
	  // task/function の IO 宣言には初期値はない．
	  if ( ast_init.is_valid() ) {
	    throw std::logic_error{"DeclGen::instantiate_iodecl(): ast_init != nullptr"};
	  }
	}

	int tag = 0;
	switch ( aux_type ) {
	case VpiAuxType::Net: tag = vpiNet; break;
	case VpiAuxType::Reg: tag = vpiReg; break;
	case VpiAuxType::Var: tag = vpiVariables; break;
	default:
	  throw std::logic_error{"DeclGen::instantiate_iodecl(): wrong aux_type"};
	}
	decl = elb_mgr().new_Decl(tag, head, ast_item, init);
      }

      auto io_decl = elb_mgr().new_IODecl(head, ast_item, decl);
      if ( module ) {
	module->add_iodecl(io_decl);
      }
      else if ( taskfunc ) {
	taskfunc->add_iodecl(io_decl);
      }

      log_mgr().info_iodecl(__FILE__, __LINE__, ast_item, scope);
    }
  }
}

// @brief IOHead を作る．
ElbIOHead*
DeclGen::instantiate_iohead(
  ElbModule* module,
  ElbTaskFunc* taskfunc,
  const AstIOHead& ast_head
)
{
  if ( module != nullptr ) {
    return elb_mgr().new_IOHead(module, ast_head);
  }
  if ( taskfunc != nullptr ) {
    return elb_mgr().new_IOHead(taskfunc, ast_head);
  }
  throw std::logic_error{"DeclGen::instantiate_iohead(): module == nullptr && taskfunc == nullptr"};
}

END_NAMESPACE_YM_VERILOG
