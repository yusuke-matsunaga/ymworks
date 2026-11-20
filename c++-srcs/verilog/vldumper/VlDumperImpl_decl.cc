
/// @file VlDumperImpl_decl.cc
/// @brief VlDumper の実装ファイル (decl担当)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "VlDumperImpl.h"
#include "ym/vl/VlMgr.h"
#include "ym/vl/VlValue.h"
#include "ym/vl/VlIODecl.h"
#include "ym/vl/VlDecl.h"
#include "ym/vl/VlDeclArray.h"
#include "ym/vl/VlParamAssign.h"
#include "ym/vl/VlModule.h"
#include "ym/vl/VlScope.h"
#include "ym/vl/VlUdpDefn.h"
#include "ym/vl/VlTaskFunc.h"


BEGIN_NAMESPACE_YM_VERILOG

// io decl の内容を出力する関数
// IEEE 1364-2001 p.636
void
VlDumperImpl::put(
  const char* label,
  const VlIODecl* iodecl
)
{
  if ( iodecl == nullptr ) {
    if ( !nullptr_suppress_mode() ) {
      VlDumpHeader x(this, label, "null-pointer", false);
    }
    return;
  }

  const char* nm = nullptr;
  switch ( iodecl->direction() ) {
  case VpiDir::Input:  nm = "Input"; break;
  case VpiDir::Output: nm = "Output"; break;
  case VpiDir::Inout:  nm = "Inout"; break;
  default: ASSERT_NOT_REACHED;
  }
  VlDumpHeader x(this, label, nm);

  put("FileRegion", iodecl->file_region() );
  put("Name", iodecl->name() );
  put("Signed", iodecl->is_signed() );
  put("Size",   iodecl->bit_size() );

  auto module = iodecl->module();
  if ( module != nullptr ) {
    put("Module", module->full_name() );
  }
  auto udp = iodecl->udp_defn();
  if ( udp != nullptr) {
    put("UdpDefn", udp->def_name() );
  }
  auto task = iodecl->task();
  if ( task != nullptr ) {
    put("Task", task->full_name() );
  }
  auto func = iodecl->function();
  if ( func != nullptr ) {
    put("Function", func->full_name() );
  }
  put("LeftRange", iodecl->left_range_val() );
  put("RightRange", iodecl->right_range_val() );
}

// 宣言要素の内容を出力する関数
// IEEE 1364-2001 p.638
void
VlDumperImpl::put(
  const char* label,
  const VlDecl* decl
)
{
  if ( decl == nullptr ) {
    if ( !nullptr_suppress_mode() ) {
      VlDumpHeader x(this, label, "null-pointer", false);
    }
    return;
  }

  const char* nm = nullptr;
  switch ( decl->type() ) {
  case VpiObjType::Net:             nm = "Net"; break;
  case VpiObjType::Reg:             nm = "Reg"; break;
  case VpiObjType::IntegerVar:      nm = "IntegerVar"; break;
  case VpiObjType::RealVar:         nm = "RealVar"; break;
  case VpiObjType::TimeVar:         nm = "TimeVar"; break;
  case VpiObjType::VarSelect:       nm = "VarSelect"; break;
  case VpiObjType::NamedEvent:      nm = "NamedEvent"; break;
  case VpiObjType::Parameter:       nm = "Parameter"; break;
  case VpiObjType::SpecParam:       nm = "SpecParam"; break;
  default: ASSERT_NOT_REACHED;
  }
  VlDumpHeader x(this, label, nm);

  put("FileRegion", decl->file_region() );
  put("FullName", decl->full_name() );
  put("decl_id", decl->decl_id());
  put("Signed", decl->is_signed() );
  put("Size", decl->bit_size() );
  put("Module", decl->parent_module()->full_name() );
  put("Scope", decl->parent_scope()->full_name() );

  if ( decl->type() == VpiObjType::Net ) {
    put("Strength0", decl->drive0() );
    put("Strength1", decl->drive1() );
    put("ChargeStrength",decl->charge() );
    put("Delay", decl->delay() );
  }

  put("LeftRange", decl->left_range_val());
  put("RightRange", decl->right_range_val());
}

// 宣言要素の内容を出力する関数
// IEEE 1364-2001 p.638
void
VlDumperImpl::put(
  const char* label,
  const VlDeclArray* decl
)
{
  if ( decl == nullptr ) {
    if ( !nullptr_suppress_mode() ) {
      VlDumpHeader x(this, label, "null-pointer", false);
    }
    return;
  }

  const char* nm = nullptr;
  switch ( decl->type() ) {
  case VpiObjType::NetArray:        nm = "NetArray"; break;
  case VpiObjType::RegArray:        nm = "RegArray"; break;
  case VpiObjType::IntegerVar:      nm = "IntegerVar"; break;
  case VpiObjType::RealVar:         nm = "RealVar"; break;
  case VpiObjType::TimeVar:         nm = "TimeVar"; break;
  case VpiObjType::NamedEventArray: nm = "NamedEventArray"; break;
  default: ASSERT_NOT_REACHED;
  }
  VlDumpHeader x(this, label, nm);

  put("FileRegion", decl->file_region() );
  put("FullName", decl->full_name() );
  put("decl_id(0)", decl->decl_id(0));
  put("Signed", decl->is_signed() );
  put("Size", decl->bit_size() );

  put("Module", decl->parent_module()->full_name() );
  put("Scope", decl->parent_scope()->full_name() );

  put("MultiArray", decl->is_multi_array());

  auto n = decl->dimension();
  for ( int i = 0; i < n; ++ i ) {
    put("Range", decl->range(i) );
  }

  if ( decl->type() == VpiObjType::NetArray ) {
    put("Strength0", decl->drive0() );
    put("Strength1", decl->drive1() );
    put("ChargeStrength",decl->charge() );
    put("Delay", decl->delay() );
  }

  put("LeftRange", decl->left_range_val());
  put("RightRange", decl->right_range_val());
}

// @brief def param の内容を出力する関数
void
VlDumperImpl::put(
  const char* label,
  const VlDefParam* defparam
)
{
  VlDumpHeader x(this, label, "DefParam");

  put("FileRegion", defparam->file_region() );
  put("Module", defparam->parent_module()->full_name() );
  put("Lhs", defparam->lhs()->full_name() );
  put("Rhs", defparam->rhs_value());
}

// @brief param assign の内容を出力する関数
void
VlDumperImpl::put(
  const char* label,
  const VlParamAssign* paramassign
)
{
  VlDumpHeader x(this, label, "ParamAssign");

  put("FileRegion", paramassign->file_region() );
  put("Module", paramassign->parent_module()->full_name() );
  put("ConnByName", paramassign->is_conn_by_name() );
  put("Lhs", paramassign->lhs()->full_name() );
  put("Rhs", paramassign->rhs_value());
}

END_NAMESPACE_YM_VERILOG
