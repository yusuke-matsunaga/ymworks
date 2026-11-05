
/// @file EiParamAssign.cc
/// @brief EiParamAssign の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbMgr.h"
#include "EiParamAssign.h"

#include "elaborator/ElbParameter.h"
#include "elaborator/ElbExpr.h"

#include "ym/vl/AstItem.h"
#include "ym/vl/AstExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbMgr
//////////////////////////////////////////////////////////////////////

// @brief パラメータ割り当て文を生成する．
const VlParamAssign*
ElbMgr::new_ParamAssign(
  const VlModule* module,
  const FileRegion& loc,
  ElbParameter* param,
  const AstExpr& rhs_expr,
  const VlValue& rhs_value
)
{
  auto pa = new EiParamAssign(module, loc, param,
			      rhs_expr, rhs_value);
  reg_obj(pa);
  mTagDict.add_paramassign(pa);
  return pa;
}

// @brief 名前によるパラメータ割り当て文を生成する．
const VlParamAssign*
ElbMgr::new_NamedParamAssign(
  const VlModule* module,
  const FileRegion& loc,
  ElbParameter* param,
  const AstExpr& rhs_expr,
  const VlValue& rhs_value
)
{
  auto paramassign = new EiParamAssign2(module, loc, param,
					rhs_expr, rhs_value);
  reg_obj(paramassign);
  mTagDict.add_paramassign(paramassign);
  return paramassign;
}

// @brief defparam 文を生成する．
const VlDefParam*
ElbMgr::new_DefParam(
  const VlModule* module,
  const AstItem& ast_header,
  const AstDefParam& ast_defparam,
  ElbParameter* param,
  const AstExpr& rhs_expr,
  const VlValue& rhs_value
)
{
  auto defparam = new EiDefParam(module, ast_header, ast_defparam,
				 param, rhs_expr, rhs_value);
  reg_obj(defparam);
  mTagDict.add_defparam(defparam);
  return defparam;
}


//////////////////////////////////////////////////////////////////////
// クラス EiParamAssign
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiParamAssign::EiParamAssign(
  const VlModule* parent,
  const FileRegion& loc,
  ElbParameter* param,
  const AstExpr& rhs_expr,
  const VlValue& rhs_value
) : mModule{parent},
    mFileRegion{loc},
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
  return mFileRegion;
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
  const FileRegion& loc,
  ElbParameter* param,
  const AstExpr& rhs_expr,
  const VlValue& rhs_value
) : EiParamAssign(parent, loc, param, rhs_expr, rhs_value)
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
