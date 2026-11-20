
/// @file VlDumperImpl_scope.cc
/// @brief VlDumperImpl の実装ファイル (scope担当)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "VlDumperImpl.h"
#include "ym/vl/VlMgr.h"
#include "ym/vl/VlModule.h"
#include "ym/vl/VlScope.h"
#include "ym/vl/VlTaskFunc.h"


BEGIN_NAMESPACE_YM_VERILOG

// scope の内容を出力する関数
// IEEE1364-2001 p.634
void
VlDumperImpl::put(
  const char* label,
  const VlMgr& mgr,
  const VlScope* scope
)
{
  VlDumpHeader x(this, label, "Scope");

  put("mFileRegion", scope->file_region() );
  put("vpiFullName", scope->full_name() );

  put_scope_sub(mgr, scope);
}

void
VlDumperImpl::put_scope_sub(
  const VlMgr& mgr,
  const VlScope* scope
)
{
  put("Parameter", mgr.find_decl_list(scope, vpiParameter));
  put("ParamAssign", mgr.find_paramassign_list(scope));
  put("DefParam", mgr.find_defparam_list(scope));
  put("SpecParam", mgr.find_decl_list(scope, vpiSpecParam));
  put("Net", mgr.find_decl_list(scope, vpiNet));
  put("NetArray", mgr.find_declarray_list(scope, vpiNetArray));
  put("Reg", mgr.find_decl_list(scope, vpiReg));
  put("RegArray", mgr.find_declarray_list(scope, vpiRegArray));
  put("Variables", mgr.find_decl_list(scope, vpiVariables));
  put("VariablesArray", mgr.find_declarray_list(scope, vpiVariables));
  put("NamedEvent", mgr.find_decl_list(scope, vpiNamedEvent));
  put("NamedEventArray", mgr.find_declarray_list(scope, vpiNamedEventArray));

  auto scope_list = mgr.find_internalscope_list(scope);
  for ( auto obj: scope_list ) {
    put("InternalScope", mgr, obj);
  }

  auto task_list = mgr.find_task_list(scope);
  for ( auto task: task_list ) {
    put_task("Task", mgr, task);
  }

  auto func_list = mgr.find_function_list(scope);
  for ( auto func: func_list ) {
    put_function("Function", mgr, func);
  }

  put("ContAssign", mgr.find_contassign_list(scope));

  auto module_list = mgr.find_module_list(scope);
  // module instance 名前だけ
  for ( auto module: module_list ) {
    put("Module", module->full_name());
  }

  put("ModuleArray", mgr, mgr.find_modulearray_list(scope));

  put("Primitive", mgr.find_primitive_list(scope));
  put("PrimitiveArray", mgr.find_primarray_list(scope));
}

END_NAMESPACE_YM_VERILOG
