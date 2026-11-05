
/// @file EiPrimary.cc
/// @brief EiPrimary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbMgr.h"
#include "EiPrimary.h"
#include "elaborator/ElbParameter.h"
#include "ym/vl/VlDecl.h"
#include "ym/vl/VlDeclArray.h"
#include "ym/vl/VlPrimitive.h"
#include "ym/vl/VlValue.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス EiFactory
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素に対応するプライマリ式を生成する．
ElbExpr*
ElbMgr::new_Primary(
  const AstIOItem& ast_decl,
  const VlDecl* decl
)
{
  return new_expr(
    [&]() -> ElbExpr* {
      return new EiIOPrimary(ast_decl, decl);
    });
}

// @brief プライマリ式を生成する(net decl の初期値用)．
ElbExpr*
ElbMgr::new_Primary(
  const AstDeclItem& ast_item,
  const VlDecl* decl
)
{
  return new_expr(
    [&]() -> ElbExpr* {
      return new EiDeclItemPrimary(ast_item, decl);
    });
}

// @brief システム関数/システムタスクの引数を生成する．
ElbExpr*
ElbMgr::new_ArgHandle(
  const AstExpr& ast_expr,
  const VlScope* arg
)
{
  return new_expr(
    [&]() -> ElbExpr* {
      return new EiScopePrimary(ast_expr, arg);
    });
}

// @brief システム関数/システムタスクの引数を生成する．
ElbExpr*
ElbMgr::new_ArgHandle(
  const AstExpr& ast_expr,
  const VlPrimitive* arg
)
{
  return new_expr(
    [&]() -> ElbExpr* {
      return new EiPrimitivePrimary(ast_expr, arg);
    });
}

// @brief システム関数/システムタスクの引数を生成する．
ElbExpr*
ElbMgr::new_ArgHandle(
  const AstExpr& ast_expr,
  const VlDeclArray* arg
)
{
  return new_expr(
    [&]() -> ElbExpr* {
      return new EiDeclArrayPrimary(ast_expr, arg);
    });
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimaryBase
//////////////////////////////////////////////////////////////////////

// @brief 式のタイプを返す．
VlExprType
EiPrimaryBase::expr_type() const
{
  return VlExprType::PRIMARY;
}

// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
bool
EiPrimaryBase::is_primary() const
{
  return true;
}

// @brief 要求される式の型を計算してセットする．
void
EiPrimaryBase::_set_reqsize(
  const VlValueType& type
)
{
  // なにもしない．
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimaryExprBase
//////////////////////////////////////////////////////////////////////

// @brief 式のタイプを返す．
VlExprType
EiPrimaryExprBase::expr_type() const
{
  return VlExprType::PRIMARY;
}

// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
bool
EiPrimaryExprBase::is_primary() const
{
  return true;
}

// @brief 要求される式の型を計算してセットする．
void
EiPrimaryExprBase::_set_reqsize(
  const VlValueType& type
)
{
  // なにもしない．
}


//////////////////////////////////////////////////////////////////////
// クラス EiIOPrimary
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiIOPrimary::type() const
{
  return mObj->type();
}

// @brief ファイル位置を返す．
FileRegion
EiIOPrimary::file_region() const
{
  return mAstObj.file_region();
}

// @brief 式のタイプを返す．
VlValueType
EiIOPrimary::value_type() const
{
  return mObj->value_type();
}

// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
const VlDeclBase*
EiIOPrimary::decl_base() const
{
  return mObj;;
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
const VlDecl*
EiIOPrimary::decl_obj() const
{
  return mObj;
}

// @brief Verilog-HDL の文字列を得る．
std::string
EiIOPrimary::decompile() const
{
  return mAstObj.name();
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclItemPrimary
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiDeclItemPrimary::type() const
{
  return mObj->type();
}

// @brief ファイル位置を返す．
FileRegion
EiDeclItemPrimary::file_region() const
{
  return mAstObj.file_region();
}

// @brief 式のタイプを返す．
VlValueType
EiDeclItemPrimary::value_type() const
{
  return mObj->value_type();
}

// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
const VlDeclBase*
EiDeclItemPrimary::decl_base() const
{
  return mObj;;
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
const VlDecl*
EiDeclItemPrimary::decl_obj() const
{
  return mObj;
}

// @brief Verilog-HDL の文字列を得る．
std::string
EiDeclItemPrimary::decompile() const
{
  return mAstObj.name();
}


//////////////////////////////////////////////////////////////////////
// クラス EiScopePrimary
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiScopePrimary::type() const
{
  return VpiObjType::Scope;
}

// @brief 式のタイプを返す．
VlValueType
EiScopePrimary::value_type() const
{
  return VlValueType();
}

// @brief 対象のオブジェクトを返す．
const VlScope*
EiScopePrimary::scope_obj() const
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimitivePrimary
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiPrimitivePrimary::type() const
{
  return mObj->type();
}

// @brief 式のタイプを返す．
VlValueType
EiPrimitivePrimary::value_type() const
{
  return VlValueType();
}

// @brief 対象のオブジェクトを返す．
const VlPrimitive*
EiPrimitivePrimary::primitive_obj() const
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclArrayPrimary
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiDeclArrayPrimary::type() const
{
  return mObj->type();
}

// @brief 式のタイプを返す．
VlValueType
EiDeclArrayPrimary::value_type() const
{
  return mObj->value_type();
}

// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
const VlDeclBase*
EiDeclArrayPrimary::decl_base() const
{
  return mObj;
}

// @brief 宣言要素の配列への参照の場合，対象のオブジェクトを返す．
const VlDeclArray*
EiDeclArrayPrimary::declarray_obj() const
{
  return mObj;
}

END_NAMESPACE_YM_VERILOG
