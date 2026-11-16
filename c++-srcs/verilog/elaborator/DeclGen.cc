
/// @file DeclGen.cc
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
