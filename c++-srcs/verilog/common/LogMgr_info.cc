
/// @file LogMgr.cc
/// @brief LogMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "common/LogMgr.h"

#include "elaborator/ElbGenvar.h"
#include "elaborator/ElbParameter.h"
#include "elaborator/ElbPrimitive.h"
#include "elaborator/RangeVal.h"
#include "ym/vl/VlContAssign.h"
#include "ym/vl/VlExpr.h"
#include "ym/vl/VlModule.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief モジュール配列のインスタンス生成
void
LogMgr::info_module_array(
  const char* file,
  int line,
  const AstItem& ast_head,
  const AstInst& ast_inst,
  const RangeVal& range
)
{
  std::ostringstream buf;
  buf << "Instantiating module array \"" << ast_inst.name() << "\" of \""
      << ast_head.name() << "\" [" << range.left << " : " << range.right << "].";
  put_info(file, line,
	   ast_inst.file_region(),
	   "ELAB_MODULE_ARRAY_INSTANTIATE",
	   buf.str());
}

// @brief モジュールのインスタンス生成
void
LogMgr::info_module(
  const char* file,
  int line,
  const VlModule* module
)
{
  std::ostringstream buf;
  buf << "\"" << module->full_name() << "\" has been created.";
  put_info(file, line,
	   module->file_region(),
	   "ELAB_MODULE_INSTANTIATE",
	   buf.str());
}

// @brief IO宣言のインスタンス生成
void
LogMgr::info_iodecl(
  const char* file,
  int line,
  const AstIOItem& ast_item,
  const VlScope* scope
)
{
  std::ostringstream buf;
  buf << "IODecl(" << ast_item.name() << ")@"
      << scope->full_name() << " created.";
  put_info(file, line,
	   ast_item.file_region(),
	   "INFO_IODECL",
	   buf.str());
}

// @brief パラメータのインスタンス生成
void
LogMgr::info_param(
  const char* file,
  int line,
  const VlDecl* decl
)
{
  std::ostringstream buf;
  buf << "Parameter(" << decl->full_name() << ") created.";
  put_info(file, line,
	   decl->file_region(),
	   "INFO_PARAM",
	   buf.str());
}

// @brief ネット配列のインスタンス生成
void
LogMgr::info_net_array(
  const char* file,
  int line,
  const VlDeclArray* decl_array
)
{
  std::ostringstream buf;
  buf << "NetArray(" << decl_array->full_name() << ") created.";
  put_info(file, line,
	   decl_array->file_region(),
	   "INFO_NET_ARRAY",
	   buf.str());
}

// @brief ネットのインスタンス生成
void
LogMgr::info_net(
  const char* file,
  int line,
  const VlDecl* decl
)
{
  std::ostringstream buf;
  buf << "Net(" << decl->full_name() << ") created.";
  put_info(file, line,
	   decl->file_region(),
	   "INFO_NET",
	   buf.str());
}

// @brief Reg配列のインスタンス生成
void
LogMgr::info_reg_array(
  const char* file,
  int line,
  const VlDeclArray* decl_array
)
{
  std::ostringstream buf;
  buf << "RegArray(" << decl_array->full_name() << ") created.";
  put_info(file, line,
	   decl_array->file_region(),
	   "INFO_REG_ARRAY",
	   buf.str());
}

// @brief Regのインスタンス生成
void
LogMgr::info_reg(
  const char* file,
  int line,
  const VlDecl* decl
)
{
  std::ostringstream buf;
  buf << "Reg(" << decl->full_name() << ") created.";
  put_info(file, line,
	   decl->file_region(),
	   "INFO_REG",
	   buf.str());
}

// @brief Var配列のインスタンス生成
void
LogMgr::info_var_array(
  const char* file,
  int line,
  const VlDeclArray* decl_array
)
{
  std::ostringstream buf;
  buf << "VarArray(" << decl_array->full_name() << ") created.";
  put_info(file, line,
	   decl_array->file_region(),
	   "INFO_VAR_ARRAY",
	   buf.str());
}

// @brief Varのインスタンス生成
void
LogMgr::info_var(
  const char* file,
  int line,
  const VlDecl* decl
)
{
  std::ostringstream buf;
  buf << "Var(" << decl->full_name() << ") created.";
  put_info(file, line,
	   decl->file_region(),
	   "INFO_VAR",
	   buf.str());
}

// @brief イベント配列のインスタンス生成
void
LogMgr::info_event_array(
  const char* file,
  int line,
  const VlDeclArray* decl_array
)
{
  std::ostringstream buf;
  buf << "EventArray(" << decl_array->full_name() << ") created.";
  put_info(file, line,
	   decl_array->file_region(),
	   "INFO_EVENT_ARRAY",
	   buf.str());
}

// @brief イベントのインスタンス生成
void
LogMgr::info_event(
  const char* file,
  int line,
  const VlDecl* decl
)
{
  std::ostringstream buf;
  buf << "Event(" << decl->full_name() << ") created.";
  put_info(file, line,
	   decl->file_region(),
	   "INFO_EVENT",
	   buf.str());
}

// @brief genvarのインスタンス生成
void
LogMgr::info_genvar(
  const char* file,
  int line,
  ElbGenvar* genvar
)
{
  std::ostringstream buf;
  buf << "Genvar(" << genvar->full_name() << ") created.";
  put_info(file, line,
	   genvar->file_region(),
	   "INFO_GENVER",
	   buf.str());
}

// @brief defparam の生成
void
LogMgr::info_defparam(
  const char* file,
  int line,
  const FileRegion& file_region,
  ElbParameter* param,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "DefParam("
      << param->full_name()
      << " = "
      << ast_expr.decompile()
      << ")";
  put_info(file, line,
	   file_region,
	   "INFO_DEFPARAM",
	   buf.str());
}

// @brief continuous assign の生成
void
LogMgr::info_contassign(
  const char* file,
  int line,
  const VlContAssign* cont_assign
)
{
  std::ostringstream buf;
  buf << "ContAssign("
      << cont_assign->lhs()->decompile()
      << " = "
      << cont_assign->rhs()->decompile()
      << ")";
  put_info(file, line,
	   cont_assign->file_region(),
	   "INFO_CONTASSIGN",
	   buf.str());
}

// @brief プリミティブ配列インスタンスの生成
void
LogMgr::info_prim_array(
  const char* file,
  int line,
  ElbPrimArray* prim_array
)
{
  std::ostringstream buf;
  buf << "PrimArray("
      << prim_array->full_name()
      << ")";
  put_info(file, line,
	   prim_array->file_region(),
	   "INFO_PRIMARRAY",
	   buf.str());
}

// @brief プリミティブインスタンスの生成
void
LogMgr::info_primitive(
  const char* file,
  int line,
  ElbPrimitive* prim
)
{
  std::ostringstream buf;
  buf << "Primitive("
      << prim->full_name()
      << ")";
  put_info(file, line,
	   prim->file_region(),
	   "INFO_PRIMITIVE",
	   buf.str());
}

END_NAMESPACE_YM_VERILOG
