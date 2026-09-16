
/// @file DeclGen.cc
/// @brief DeclGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "DeclGen.h"
#include "ElbEnv.h"

#include "elaborator/ElbDecl.h"
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

//////////////////////////////////////////////////////////////////////
// クラス DeclGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DeclGen::DeclGen(
  Elaborator& elab
) : ElbProxy{elab}
{
}

// @brief デストラクタ
DeclGen::~DeclGen()
{
}

// @brief parameter と genvar を実体化する．
void
DeclGen::phase1_decl(
  const VlScope* scope,
  const AstDeclHeadList& ast_head_list,
  bool force_to_local
)
{
  for ( auto ast_head: ast_head_list ) {
    try {
      switch ( ast_head.type() ) {
      case AstDeclHead::Param:
	instantiate_param_head(scope, ast_head, force_to_local);
	break;

      case AstDeclHead::LocalParam:
	instantiate_param_head(scope, ast_head, true);
	break;

      case AstDeclHead::Genvar:
	instantiate_genvar_head(scope, ast_head);
	break;

      default:
	// それ以外のタイプはここでは無視する．
	break;
      }
    }
    catch ( const ElbError& error ) {
      // エラーは無視して処理を続ける．
    }
  }
}

// @brief IO宣言要素をインスタンス化する．
void
DeclGen::instantiate_iodecl(
  ElbModule* module,
  ElbTaskFunc* taskfunc,
  const AstIOHeadList& ast_head_list
)
{
  auto scope = (const VlScope*){nullptr};
  if ( module ) {
    scope = module;
  }
  else if ( taskfunc ) {
    scope = taskfunc;
  }
  if ( scope == nullptr ) {
    throw std::logic_error{"scope == nullptr"};
  }

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
    auto head = new_IOHead(module, taskfunc, ast_head);

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
	auto head = new_DeclHead(scope, ast_head, aux_type, ast_range, range);

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
	    throw std::logic_error{"ast_init != nullptr"};
	  }
	}

	int tag = 0;
	switch ( aux_type ) {
	case VpiAuxType::Net: tag = vpiNet; break;
	case VpiAuxType::Reg: tag = vpiReg; break;
	case VpiAuxType::Var: tag = vpiVariables; break;
	default:
	  throw std::logic_error{"Should not be reached"};
	}
	decl = elb_mgr().new_Decl(tag, head, ast_item, init);
      }

      if ( module ) {
	module->add_iodecl(head, ast_item, decl);
      }
      else if ( taskfunc ) {
	taskfunc->add_iodecl(head, ast_item, decl);
      }
      else {
	throw std::logic_error{"Should not be reached"};
      }

      log_mgr().info_iodecl(__FILE__, __LINE__, ast_item, scope);
    }
  }
}

// @brief IOヘッダを作る．
ElbIOHead*
DeclGen::new_IOHead(
  const ElbModule* module,
  const ElbTaskFunc* taskfunc,
  const AstIOHead& ast_head
)
{
  if ( module != nullptr ) {
    return elb_mgr().new_IOHead(module, ast_head);
  }
  if ( taskfunc != nullptr ) {
    return elb_mgr().new_IOHead(taskfunc, ast_head);
  }
  throw std::logic_error{"module == nullptr && taskfunc == nullptr"};
}

// @brief 宣言ヘッダを作る．
ElbDeclHead*
DeclGen::new_DeclHead(
  const VlScope* scope,
  const AstIOHead& ast_head,
  VpiAuxType aux_type,
  const AstRange& ast_range,
  const RangeVal& range
)
{
  if ( ast_range.is_valid() ) {
    return elb_mgr().new_DeclHead(scope, ast_head, aux_type,
				  ast_range, range);
  }
  else {
    return elb_mgr().new_DeclHead(scope, ast_head, aux_type);
  }
}

// 宣言要素をインスタンス化する．
void
DeclGen::instantiate_decl(
  const VlScope* scope,
  const AstDeclHeadList& ast_head_list
)
{
  for ( auto ast_head: ast_head_list ) {
    try {
      switch ( ast_head.type() ) {
      case AstDeclHead::Param:
      case AstDeclHead::LocalParam:
	// すでに処理済みのはず．
	break;

      case AstDeclHead::Reg:
	instantiate_reg_head(scope, ast_head);
	break;

      case AstDeclHead::Var:
	instantiate_var_head(scope, ast_head);
	break;

      case AstDeclHead::Genvar:
	// すでに処理済みのはず．
	break;

      case AstDeclHead::Net:
	instantiate_net_head(scope, ast_head);
	break;

      case AstDeclHead::Event:
	instantiate_event_head(scope, ast_head);
	break;

      case AstDeclHead::SpecParam:
	// 未対応
	break;

      default:
	ASSERT_NOT_REACHED;
	break;
      }
    }
    catch ( const ElbError& error ) {
      // 無視して処理を続ける．
    }
  }
}

// @brief パラメータ用の instantiate 関数
void
DeclGen::instantiate_param_head(
  const VlScope* scope,
  const AstDeclHead& ast_head,
  bool force_local
)
{
  auto module = scope->parent_module();
  auto ast_range = ast_head.range();
  auto param_head = (ElbParamHead*)nullptr;
  if ( ast_range.is_valid() ) {
    auto range = evaluate_range(scope, ast_range);
    param_head = elb_mgr().new_ParamHead(scope, ast_head,
				     ast_range, range);
  }
  else {
    param_head = elb_mgr().new_ParamHead(scope, ast_head);
  }

  for ( auto ast_item: ast_head.item_list() ) {
    const auto& file_region = ast_item.file_region();
    auto param = elb_mgr().new_Parameter(param_head,
				     ast_item,
				     force_local);
    if ( param == nullptr ) {
      throw std::logic_error{"param == nullptr"};
    }

    // attribute instance の生成
    auto attr_list = attribute_list(ast_head);
    elb_mgr().reg_attr(param, attr_list);

    log_mgr().info_param(__FILE__, __LINE__, param);

    // 右辺の式は constant expression のはずなので今つくる．
    auto ast_init_expr = ast_item.init_value();
    auto value = evaluate_expr(scope, ast_init_expr);
    param->set_init_expr(ast_init_expr, value);

    // ダブっている感じがするけど同じことを表す parameter assign 文
    // をつくってモジュールに追加する．
    auto loc = ast_item.file_region();
    auto pa = elb_mgr().new_NamedParamAssign(module, loc, param,
					     ast_init_expr, value);
  }
}

// @brief net をインスタンス化する．
void
DeclGen::instantiate_net_head(
  const VlScope* scope,
  const AstDeclHead& ast_head
)
{
  auto ast_range = ast_head.range();
  auto ast_delay = ast_head.delay();
  auto has_delay = ast_delay.is_valid();

  auto net_head = (ElbDeclHead*)nullptr;
  if ( ast_range.is_valid() ) {
    auto range = evaluate_range(scope, ast_range);
    net_head = elb_mgr().new_DeclHead(scope, ast_head,
				      ast_range, range,
				      has_delay);
  }
  else {
    net_head = elb_mgr().new_DeclHead(scope, ast_head);
  }
  if ( net_head == nullptr ) {
    throw std::logic_error{"net_head == nullptr"};
  }

  if ( has_delay ) {
    add_phase3stub(make_net_delay_stub(net_head, ast_delay));
  }

  for ( auto ast_item: ast_head.item_list() ) {
    // init_value() が 0 でなければ初期割り当てを持つということ．
    auto ast_init = ast_item.init_value();
    auto dim_list = ast_item.range_list();
    auto dim_size = dim_list.size();
    if ( dim_size > 0 ) {
      // 配列

      // 初期割り当ては構文規則上持てないはず
      if ( ast_init.is_valid() ) {
	throw std::logic_error{"ast_init != nullptr"};
      }

      // 範囲の配列を作る．
      std::vector<ElbRangeSrc> range_src;
      if ( !instantiate_dimension_list(scope, ast_item, range_src) ) {
	continue;
      }

      auto net_array = elb_mgr().new_DeclArray(vpiNetArray, net_head,
					       ast_item, range_src);

      // attribute instance の生成
      auto attr_list = attribute_list(ast_head);
      elb_mgr().reg_attr(net_array, attr_list);

      log_mgr().info_net_array(__FILE__, __LINE__, net_array);
    }
    else {
      // 単一の要素
      auto net = elb_mgr().new_Decl(vpiNet, net_head, ast_item);

      if ( ast_init.is_valid() ) {
	// 初期割り当てつき
	// net の初期割り当ては continuous assignment と同等なので
	// あとで作る．
	add_phase3stub(make_net_assign_stub(net, ast_item));
      }

      // attribute instance の生成
      auto attr_list = attribute_list(ast_head);
      elb_mgr().reg_attr(net, attr_list);

      log_mgr().info_net(__FILE__, __LINE__, net);
    }
  }
}

// @brief link_net_delay() を起動するスタブを作る．
ElbStub*
DeclGen::make_net_delay_stub(
  ElbDeclHead* net_head,
  const AstDelay& ast_delay
)
{
  return make_stub<DeclGen,
		   ElbDeclHead*,
		   const AstDelay&>(this, &DeclGen::link_net_delay,
				    net_head, ast_delay);
}

// @brief net の遅延値を生成する．
void
DeclGen::link_net_delay(
  ElbDeclHead* net_head,
  const AstDelay& ast_delay
)
{
  auto scope = net_head->parent_scope();
  auto delay = instantiate_delay(scope, ast_delay);
  net_head->set_delay(delay);
}

// @brief link_net_delay() を起動するスタブを作る．
ElbStub*
DeclGen::make_net_assign_stub(
  ElbDecl* net,
  const AstDeclItem& ast_item
)
{
  return make_stub<DeclGen,
		   ElbDecl*,
		   const AstDeclItem&>(this, &DeclGen::link_net_assign,
				       net, ast_item);
}

// @brief net の初期値を生成する．
void
DeclGen::link_net_assign(
  ElbDecl* net,
  const AstDeclItem& ast_item
)
{
  // 実体は左辺が net の代入文を作る．
  auto lhs = elb_mgr().new_Primary(ast_item, net);
  auto scope = net->parent_scope();
  auto ast_init = ast_item.init_value();
  auto rhs = instantiate_rhs(scope, ElbEnv(), ast_init, lhs);
  if ( !rhs ) {
    return;
  }

  net->set_init(rhs);

  // 対応する continuous assign 文を作る．
  auto module = scope->parent_module();
  auto loc = ast_item.file_region();
  auto ca = elb_mgr().new_ContAssign(module, loc, lhs, rhs);
}

// @brief reg をインスタンス化する．
void
DeclGen::instantiate_reg_head(
  const VlScope* scope,
  const AstDeclHead& ast_head
)
{
  auto ast_range = ast_head.range();

  auto reg_head = (ElbDeclHead*)nullptr;
  if ( ast_range.is_valid() ) {
    auto range = evaluate_range(scope, ast_range);
    reg_head = elb_mgr().new_DeclHead(scope, ast_head,
				  ast_range, range);
  }
  else {
    reg_head = elb_mgr().new_DeclHead(scope, ast_head);
  }
  if ( reg_head == nullptr ) {
    throw std::logic_error{"reg_head == nullptr"};
  }

  for ( auto ast_item: ast_head.item_list() ) {
    auto ast_init = ast_item.init_value();
    auto dim_size = ast_item.range_list().size();
    if ( dim_size > 0 ) {
      // 配列の場合

      // 配列は初期値を持たない．
      if ( ast_init.is_valid() ) {
	throw std::logic_error{"ast_init != nullptr"};
      }

      // 範囲の配列を作る．
      std::vector<ElbRangeSrc> range_src;
      if ( !instantiate_dimension_list(scope, ast_item, range_src) ) {
	continue;
      }

      auto reg_array = elb_mgr().new_DeclArray(vpiRegArray, reg_head,
					       ast_item, range_src);

      // attribute instance の生成
      auto attr_list = attribute_list(ast_head);
      elb_mgr().reg_attr(reg_array, attr_list);

      log_mgr().info_reg_array(__FILE__, __LINE__, reg_array);
    }
    else {
      // 単独の要素
      auto init = (const VlExpr*){nullptr};
      if ( ast_init.is_valid() ) {
	// 初期値を持つ場合
	// 初期値は constant_expression なので今作る．
	init = instantiate_constant_expr(scope, ast_init);
      }

      auto reg = elb_mgr().new_Decl(vpiReg, reg_head, ast_item, init);

      // attribute instance の生成
      auto attr_list = attribute_list(ast_head);
      elb_mgr().reg_attr(reg, attr_list);

      log_mgr().info_reg(__FILE__, __LINE__, reg);
    }
  }
}

// @brief variable header をインスタンス化する．
void
DeclGen::instantiate_var_head(
  const VlScope* scope,
  const AstDeclHead& ast_head
)
{
  if ( ast_head.data_type() == VpiVarType::None ) {
    throw std::logic_error{"ast_head->data_type() == VpiVarType::None"};
  }

  auto var_head = elb_mgr().new_DeclHead(scope, ast_head);
  for ( auto ast_item: ast_head.item_list() ) {
    auto ast_init = ast_item.init_value();
    auto dim_size = ast_item.range_list().size();
    if ( dim_size > 0 ) {
      // 配列の場合

      // 配列は初期値を持たない．
      if ( ast_init.is_valid() ) {
	throw std::logic_error{"ast_init != nullptr"};
      }

      // 範囲の配列を作る．
      std::vector<ElbRangeSrc> range_src;
      if ( !instantiate_dimension_list(scope, ast_item, range_src) ) {
	continue;
      }

      auto var_array = elb_mgr().new_DeclArray(vpiVariables, var_head,
					   ast_item, range_src);

      // attribute instance の生成
      auto attr_list = attribute_list(ast_head);
      elb_mgr().reg_attr(var_array, attr_list);

      log_mgr().info_var_array(__FILE__, __LINE__, var_array);
    }
    else {
      // 単独の変数
      auto init = (const VlExpr*){nullptr};
      if ( ast_init.is_valid() ) {
	// 初期値を持つ場合
	// 初期値は constant_expression なので今作る．
	init = instantiate_constant_expr(scope, ast_init);
      }

      auto var = elb_mgr().new_Decl(vpiVariables, var_head, ast_item, init);

      // attribute instance の生成
      auto attr_list = attribute_list(ast_head);
      elb_mgr().reg_attr(var, attr_list);

      log_mgr().info_var(__FILE__, __LINE__, var);
    }
  }
}

// @brief named_event をインスタンス化する．
void
DeclGen::instantiate_event_head(
  const VlScope* scope,
  const AstDeclHead& ast_head
)
{
  auto event_head = elb_mgr().new_DeclHead(scope, ast_head);
  for ( auto ast_item: ast_head.item_list() ) {
    auto dim_size = ast_item.range_list().size();
    if ( dim_size > 0 ) {
      // 配列

      // 範囲の配列を作る．
      std::vector<ElbRangeSrc> range_src;
      if ( !instantiate_dimension_list(scope, ast_item, range_src) ) {
	continue;
      }

      auto ne_array = elb_mgr().new_DeclArray(vpiNamedEventArray, event_head,
					  ast_item, range_src);

      // attribute instance の生成
      auto attr_list = attribute_list(ast_head);
      elb_mgr().reg_attr(ne_array, attr_list);

      log_mgr().info_event_array(__FILE__, __LINE__, ne_array);
    }
    else {
      // 単一の要素
      auto named_event = elb_mgr().new_Decl(vpiNamedEvent, event_head, ast_item);

      // attribute instance の生成
      auto attr_list = attribute_list(ast_head);
      elb_mgr().reg_attr(named_event, attr_list);

      log_mgr().info_event(__FILE__, __LINE__, named_event);
    }
  }
}

// @brief genvar をインスタンス化する．
void
DeclGen::instantiate_genvar_head(
  const VlScope* scope,
  const AstDeclHead& ast_head
)
{
  for ( auto ast_item: ast_head.item_list() ) {
    auto genvar = elb_mgr().new_Genvar(scope, ast_item, 0);

    log_mgr().info_genvar(__FILE__, __LINE__, genvar);
  }
}

// @brief 配列の次元リストを生成する．
bool
DeclGen::instantiate_dimension_list(
  const VlScope* scope,
  const AstDeclItem& ast_item,
  std::vector<ElbRangeSrc>& range_src
)
{
  auto n = ast_item.range_list().size();
  range_src.reserve(n);

  for ( auto ast_range: ast_item.range_list() ) {
    auto range = evaluate_range(scope, ast_range);
    range_src.push_back(ElbRangeSrc(ast_range,
				    ast_range.left(), ast_range.right(),
				    range.left, range.right));
  }

  return true;
}

END_NAMESPACE_YM_VERILOG
