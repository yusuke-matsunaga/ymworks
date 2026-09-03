
/// @file EiParamAssign.cc
/// @brief EiParamAssign の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiParamAssign.h"

#include "elaborator/ElbParameter.h"
#include "elaborator/ElbExpr.h"

#include "ym/vl/AstItem.h"
#include "ym/vl/AstExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief パラメータ割り当て文を生成する．
const VlParamAssign*
EiFactory::new_ParamAssign(
  const VlModule* module,
  const AstBase& ast_obj,
  ElbParameter* param,
  const AstExpr& rhs_expr,
  const VlValue& rhs_value
)
{
  return new EiParamAssign(module, ast_obj, param, rhs_expr, rhs_value);
}

// @brief 名前によるパラメータ割り当て文を生成する．
const VlParamAssign*
EiFactory::new_NamedParamAssign(
  const VlModule* module,
  const AstBase& ast_obj,
  ElbParameter* param,
  const AstExpr& rhs_expr,
  const VlValue& rhs_value
)
{
  return new EiParamAssign2(module, ast_obj, param, rhs_expr, rhs_value);
}

// @brief defparam 文を生成する．
const VlDefParam*
EiFactory::new_DefParam(
  const VlModule* module,
  const AstItem& ast_header,
  const AstDefParam& ast_defparam,
  ElbParameter* param,
  const AstExpr& rhs_expr,
  const VlValue& rhs_value
)
{
  return new EiDefParam(module, ast_header, ast_defparam,
			param, rhs_expr, rhs_value);
}


//////////////////////////////////////////////////////////////////////
// クラス EiParamAssign
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiParamAssign::EiParamAssign(
  const VlModule* parent,
  const AstBase& ast_obj,
  ElbParameter* param,
  const AstExpr& rhs_expr,
  const VlValue& rhs_value
) : mModule{parent},
    mAstObj{ast_obj},
    mLhs{param},
    mRhsExpr{rhs_expr},
    mRhsValue{rhs_value}
{
}

// @brief デストラクタ
EiParamAssign::~EiParamAssign()
{
}

// @brief 型の取得
VpiObjType
EiParamAssign::type() const
{
  return VpiObjType::ParamAssign;
}

// @brief ファイル位置を返す．
FileRegion
EiParamAssign::file_region() const
{
  return mAstObj.file_region();
}

// @brief 親のモジュールを返す．
const VlModule*
EiParamAssign::parent_module() const
{
  return mModule;
}

// @brief 左辺値を返す．
const VlDecl*
EiParamAssign::lhs() const
{
  return mLhs;
}

// @brief 右辺値を返す．
VlValue
EiParamAssign::rhs_value() const
{
  return mRhsValue;
}

// @brief 右辺の式を表す文字列を返す．
std::string
EiParamAssign::rhs_string() const
{
  return mRhsExpr.decompile();
}

// @brief 名前による接続の場合に true を返す．
bool
EiParamAssign::is_conn_by_name() const
{
  return false;
}


//////////////////////////////////////////////////////////////////////
// クラス EiParamAssign2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiParamAssign2::EiParamAssign2(
  const VlModule* parent,
  const AstBase& ast_obj,
  ElbParameter* param,
  const AstExpr& rhs_expr,
  const VlValue& rhs_value
) : EiParamAssign(parent, ast_obj, param, rhs_expr, rhs_value)
{
}

// @brief デストラクタ
EiParamAssign2::~EiParamAssign2()
{
}

// @brief 名前による接続の場合に true を返す．
bool
EiParamAssign2::is_conn_by_name() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDefParam
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiDefParam::EiDefParam(
  const VlModule* parent,
  const AstItem& ast_header,
  const AstDefParam& ast_defparam,
  ElbParameter* param,
  const AstExpr& rhs_expr,
  const VlValue& rhs_value
) : mModule{parent},
    mAstHead{ast_header},
    mAstDefParam{ast_defparam},
    mLhs{param},
    mRhsExpr{rhs_expr},
    mRhsValue{rhs_value}
{
}

// @brief デストラクタ
EiDefParam::~EiDefParam()
{
}

// @brief 型の取得
VpiObjType
EiDefParam::type() const
{
  return VpiObjType::DefParam;
}

// @brief ファイル位置を返す．
FileRegion
EiDefParam::file_region() const
{
  return mAstDefParam.file_region();
}

// @brief 親のモジュールを返す．
const VlModule*
EiDefParam::parent_module() const
{
  return mModule;
}

// @brief 左辺値を返す．
const VlDecl*
EiDefParam::lhs() const
{
  return mLhs;
}

// @brief 右辺値を返す．
VlValue
EiDefParam::rhs_value() const
{
  return mRhsValue;
}

// @brief 右辺の式を表す文字列を返す．
std::string
EiDefParam::rhs_string() const
{
  return mRhsExpr.decompile();
}

END_NAMESPACE_YM_VERILOG
