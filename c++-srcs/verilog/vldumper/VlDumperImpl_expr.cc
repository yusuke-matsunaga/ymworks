
/// @file VlDumperImpl_expr.cc
/// @brief VlDumperImpl の実装ファイル (expr担当)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "VlDumperImpl.h"
#include "ym/vl/VlMgr.h"
#include "ym/vl/VlDecl.h"
#include "ym/vl/VlDeclArray.h"
#include "ym/vl/VlDelay.h"
#include "ym/vl/VlExpr.h"
#include "ym/vl/VlPrimitive.h"
#include "ym/vl/VlRange.h"
#include "ym/vl/VlScope.h"
#include "ym/vl/VlTaskFunc.h"
#include "ym/vl/VlUserSystf.h"
#include "ym/vl/VlValue.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// 式の内容の出力
//////////////////////////////////////////////////////////////////////

// @brief 式の内容を出力する関数
void
VlDumperImpl::put(
  const char* label,
  const VlExpr* expr
)
{
  if ( expr == nullptr ) {
    if ( !nullptr_suppress_mode() ) {
      VlDumpHeader x(this, label, "null-pointer", false);
    }
    return;
  }

  VlDumpHeader x(this, label, "Expr");
  put("FileRegion", expr->file_region());
  put("ValueType", expr->value_type());
  put("ReqType", expr->req_type());
  put("ExprType", expr->expr_type());
  switch ( expr->expr_type() ) {
  case VlExprType::CONST:
    put("vpiConstType", expr->constant_type());
    put("vpiDecompile", expr->decompile()); // <= 手抜き
    break;
  case VlExprType::PRIMARY:
    put("PrimaryType", expr->primary_type());
    switch ( expr->primary_type() ) {
    case VlPrimaryType::NONE:
      break;
    case VlPrimaryType::DECL:
      put("Decl", expr->decl_obj()->name());
      break;
    case VlPrimaryType::DECLARRAY:
      put("DeclArray", expr->declarray_obj()->name());
      break;
    case VlPrimaryType::CONST_DECL_ELEM:
      put("DeclArray", expr->declarray_obj()->name());
      put("DeclArrayOffset", expr->declarray_offset());
      break;
    case VlPrimaryType::VAR_DECL_ELEM:
      put("DeclArray", expr->declarray_obj()->name());
      put("DeclArrayIndex", expr->declarray_index_list());
      break;
    case VlPrimaryType::SCOPE:
      put("Scope", expr->scope_obj()->name());
      break;
    case VlPrimaryType::PRIMITIVE:
      put("Primitive", expr->primitive_obj()->name());
      break;
    }
    put("SelectType", expr->select_type());
    switch ( expr->select_type() ) {
    case VlSelectType::NONE:
      break;
    case VlSelectType::CONST:
      put("BitOffset", expr->bit_offset());
      break;
    case VlSelectType::PLUS:
    case VlSelectType::MINUS:
      put("BitIndex", expr->bit_index());
      break;
    }
    break;
  case VlExprType::OPERATION:
    put("OpType", expr->op_type());
    put("Operand", expr->operand_list());
    break;
  case VlExprType::FUNCCALL:
    put("Function", expr->function()->full_name());
    put("Argument", expr->argument_list());
    break;
  case VlExprType::SYSFUNCCALL:
    put("UserSysTf", expr->user_systf()->name());
    put("Argument", expr->argument_list());
    break;
  case VlExprType::LHS:
    put("LhsElem", expr->lhs_elem_list());
    break;

  default:
    break;
  }
}

// @brief 遅延値を出力する関数
void
VlDumperImpl::put(
  const char* label,
  const VlDelay* delay
)
{
  if ( delay == nullptr ) {
    if ( !nullptr_suppress_mode() ) {
      VlDumpHeader x(this, label, "null-pointer", false);
    }
    return;
  }

  VlDumpHeader x(this, label, "Delay");

  put("expr", delay->decompile() );
}

// range の内容を出力する関数
// IEEE 1364-2001 p.644
void
VlDumperImpl::put(
  const char* label,
  const VlRange* range
)
{
  if ( range == nullptr ) {
    if ( !nullptr_suppress_mode() ) {
      VlDumpHeader x(this, label, "null-pointer", false);
    }
    return;
  }

  VlDumpHeader x(this, label, "Range");

  put("FileRegion", range->file_region() );

  put("vpiSize", range->size() );

  put("vpiLeftRange", range->left_range_val() );
  put("vpiRightRange", range->right_range_val() );
}

END_NAMESPACE_YM_VERILOG
