
/// @file EiPrimary.cc
/// @brief EiPrimary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbMgr.h"
#include "EiAuxPrimary.h"
#include "EiScopePrimary.h"
#include "EiPrimitivePrimary.h"
#include "EiDeclArrayPrimary.h"
#include "ym/vl/VlDecl.h"
#include "ym/vl/VlDeclArray.h"
#include "ym/vl/VlPrimitive.h"
#include "ym/vl/VlValue.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス EiFactory
//////////////////////////////////////////////////////////////////////

// @brief プライマリ式を生成する(net decl の初期値用)．
ElbExpr*
ElbMgr::new_Primary(
  const FileRegion& loc,
  const VlDecl* decl
)
{
  return new_expr(
    [&]() -> ElbExpr* {
      return new EiAuxPrimary(loc, decl);
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
// クラス EiAuxPrimary
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiAuxPrimary::type() const
{
  return mObj->type();
}

// @brief ファイル位置を返す．
FileRegion
EiAuxPrimary::file_region() const
{
  return mFileRegion;
}

// @brief 式のタイプを返す．
VlValueType
EiAuxPrimary::value_type() const
{
  return mObj->value_type();
}

// @brief Verilog-HDL の文字列を得る．
std::string
EiAuxPrimary::decompile() const
{
  return mObj->name();
}

// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
const VlDeclBase*
EiAuxPrimary::decl_base() const
{
  return mObj;
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
const VlDecl*
EiAuxPrimary::decl_obj() const
{
  return mObj;
}

// @brief 宣言要素の番号を返す．
SizeType
EiAuxPrimary::decl_id() const
{
  return mObj->decl_id();
}

// @brief 宣言要素に対するビット/部分選択の種類を返す．
VlSelectType
EiAuxPrimary::select_type() const
{
  return VlSelectType::NONE;
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
