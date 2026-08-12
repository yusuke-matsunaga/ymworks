
/// @file AttrGen.cc
/// @brief AttrGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "AttrGen.h"
#include "elaborator/ElbExpr.h"
#include "ym/vl/AstMisc.h"
#include "ym/vl/VlAttribute.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AttrGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AttrGen::AttrGen(
  Elaborator& elab,
  ElbMgr& elb_mgr
) : ElbProxy(elab, elb_mgr)
{
}

// @brief デストラクタ
AttrGen::~AttrGen()
{
}

// @brief AstAttr から ElbAttr を生成する．
void
AttrGen::instantiate_attribute(
  const PtAttrInfo& attr_info
)
{
  auto ast_obj = attr_info.obj();
  if ( mHash.count(ast_obj) == 0 ) {
    // また未生成なので作る．
    auto ast_attr_list = attr_info.attr_list();
    bool def = attr_info.def();
    std::vector<const VlAttribute*> attr_list;
    for ( auto ast_ai: ast_attr_list ) {
      auto ast_as_list = ast_ai->attrspec_list();
      for ( auto ast_as: ast_as_list ) {
	auto expr = instantiate_constant_expr(nullptr, ast_as->expr());
	if ( !expr ) {
	  // エラー．たぶん expr() が constant_expression ではなかった．
	  // でも無視する．
	}
	// attr_list に ast_as, expr, def を追加
	auto attr = mgr().new_Attribute(ast_as, expr, def);
	attr_list.push_back(attr);
      }
    }
    // attr_list が空でも処理済みの意味で追加する．
    mHash.emplace(ast_obj, attr_list);
  }
  mHash.at(ast_obj);
}

// @brief 構文木要素に対応する属性リストを返す．
const std::vector<const VlAttribute*>&
AttrGen::attribute_list(
  const AstBase* ast_obj
)
{
  if ( mHash.count(ast_obj) > 0 ) {
    return mHash.at(ast_obj);
  }
  return mEmptyList;
}

END_NAMESPACE_YM_VERILOG
