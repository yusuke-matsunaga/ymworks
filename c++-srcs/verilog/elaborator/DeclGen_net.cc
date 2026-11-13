
/// @file DeclGen_net.cc
/// @brief DeclGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "DeclGen.h"
#include "ElbEnv.h"
#include "ym/vl/VlTerm.h"
#include "elaborator/ElbDeclHead.h"
#include "elaborator/ElbDecl.h"
#include "elaborator/ElbDeclArray.h"
#include "elaborator/ElbDeclRef.h"
//#include "elaborator/ElbParameter.h"
//#include "elaborator/ElbGenvar.h"
#include "elaborator/ElbModule.h"
//#include "elaborator/ElbTaskFunc.h"
#include "elaborator/ElbRange.h"
#include "elaborator/ElbExpr.h"
#include "elaborator/RangeVal.h"
#include "elaborator/ElbError.h"


#define DOUT std::cout

BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス DeclGen
//////////////////////////////////////////////////////////////////////

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
		   AstDelay,
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
		   AstDeclItem,
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
  auto expr = elb_mgr().new_Primary(ast_item, net);
  auto lhs = elb_mgr().new_Term(expr);
  auto scope = net->parent_scope();
  auto ast_init = ast_item.init_value();
  auto rhs = instantiate_rhs(scope, ElbEnv(), ast_init, lhs->value_type());
  if ( !rhs ) {
    return;
  }

  net->set_init(rhs);

  // 対応する continuous assign 文を作る．
  auto vl_module = scope->parent_module();
  auto loc = ast_item.file_region();
  auto ca = elb_mgr().new_ContAssign(vl_module, loc, lhs, rhs);
}

END_NAMESPACE_YM_VERILOG
