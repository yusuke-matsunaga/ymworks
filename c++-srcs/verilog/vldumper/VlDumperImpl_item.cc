
/// @file VlDumperImpl.cc
/// @brief VlDumperImpl の実装ファイル (item担当)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "VlDumperImpl.h"
#include "ym/vl/VlMgr.h"
#include "ym/vl/VlUdpDefn.h"
#include "ym/vl/VlTableEntry.h"
#include "ym/vl/VlPrimitive.h"
#include "ym/vl/VlScope.h"
#include "ym/vl/VlTaskFunc.h"
#include "ym/vl/VlContAssign.h"
#include "ym/vl/VlModule.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief primitive array 内容を出力する関数
void
VlDumperImpl::put(
  const char* label,
  const VlPrimArray* primarray
)
{
  const char* nm = nullptr;
  switch ( primarray->type() ) {
  case VpiObjType::GateArray:   nm = "GateArray"; break;
  case VpiObjType::SwitchArray: nm = "SwitchArray"; break;
  case VpiObjType::UdpArray:    nm = "UdpArray"; break;
  default: ASSERT_NOT_REACHED;
  }
  VlDumpHeader x(this, label, nm);

  put("FileRegion", primarray->file_region() );
  put("FullName", primarray->full_name() );
  put("Size", primarray->elem_num() );
  put("LeftRange", primarray->left_range_val() );
  put("RightRange", primarray->right_range_val() );
  put("Delay", primarray->delay() );

  auto n = primarray->elem_num();
  for ( int i = 0; i < n; ++ i ) {
    auto primitive = primarray->elem_by_offset(i);
    put("Primitive", primitive);
  }
}

// primitive の内容を出力する関数
// IEEE 1364-2001 p.646
void
VlDumperImpl::put(
  const char* label,
  const VlPrimitive* prim
)
{
  const char* nm = nullptr;
  switch ( prim->type() ) {
  case VpiObjType::Gate:   nm = "Gate"; break;
  case VpiObjType::Switch: nm = "Switch"; break;
  case VpiObjType::Udp:    nm = "Udp"; break;
  default: ASSERT_NOT_REACHED;
  }
  VlDumpHeader x(this, label, nm);

  put("FileRegion", prim->file_region() );
  put("FullName", prim->full_name() );
  put("DefName", prim->def_name() );
  put("PrimType", prim->prim_type() );
  put("Module", prim->parent_module()->full_name() );
  put("Scope", prim->parent_scope()->full_name() );
  put("Strength0", prim->drive0() );
  put("Strength1", prim->drive1() );
  put("Delay", prim->delay() );

  if ( prim->type() == VpiObjType::Udp ) {
    put("vpiUdpDefn", prim->udp_defn()->def_name() );
  }

  put("port_num", prim->port_num() );

  auto n = prim->port_num();
  for ( int i = 0; i < n; ++ i ) {
    put("PrimTerm", prim->prim_term(i) );
  }
}

// prim term の内容を出力する関数
// IEEE 1364-2001 p.646
void
VlDumperImpl::put(
  const char* label,
  const VlPrimTerm* primterm
)
{
  VlDumpHeader x(this, label, "PrimTerm");

  put("FileRegion", primterm->file_region() );
  put("Primitive", primterm->primitive()->full_name() );
  put("TermIndex", primterm->term_index() );
  put("Direction", primterm->direction() );
  put("Expr", primterm->expr() );
}

// udp defn の内容を出力する関数
// IEEE 1364-2001 p.647
void
VlDumperImpl::put(
  const char* label,
  const VlUdpDefn* udp
)
{
  VlDumpHeader x(this, label, "UdpDefn");

  put("FileRegion", udp->file_region() );

  put("DefName", udp->def_name() );
  put("PrimType", udp->prim_type() );
  put("Protected", udp->is_protected() );
  put("PortNum", udp->port_num() );

  auto n = udp->port_num() - 1;
  for ( int i = 0; i < n; ++ i ) {
    put("IODecl", udp->input(i));
  }
  put("IODecl", udp->output());

  put("Initial", udp->init_val_string());

  {
    VlDumpHeader x(this, "vpiTableEntry", "Iterator");
    auto n = udp->table_size();
    for ( int i = 0; i < n; ++ i ) {
      const VlTableEntry* entry = udp->table_entry(i);
      put("TableEntry", entry->str());
#if 0
      s_vpi_value s_value;
      s_value.format = vpiStringVal;
      handle1.get_value(&s_value);
      string str = s_value.value.str;
      put("TableEntry", str);
#endif
    }
  }
}

// task の内容を出力する関数
// IEEE 1364-2001 p.649
void
VlDumperImpl::put_task(
  const char* label,
  const VlMgr& mgr,
  const VlTaskFunc* task
)
{
  VlDumpHeader x(this, label, "Task");

  put("FileRegion", task->file_region() );
  put("FullName", task->full_name() );

  SizeType n = task->io_num();
  for ( SizeType i = 0; i < n; ++ i ) {
    put("IODecl", task->io(i) );
  }

  put_scope_sub(mgr, task);

  put("Stmt", task->stmt() );
}

// function の内容を出力する関数
// IEEE 1364-2001 p.649
void
VlDumperImpl::put_function(
  const char* label,
  const VlMgr& mgr,
  const VlTaskFunc* func
)
{
  VlDumpHeader x(this, label, "Function");

  put("FileRegion", func->file_region() );
  put("FullName", func->full_name() );
  put("FuncType", func->func_type() );
  put("Signed", func->is_signed() );
  put("Size", func->bit_size() );
  put("LeftRange", func->left_range_val() );
  put("RightRange", func->right_range_val() );

  auto n = func->io_num();
  for ( int i = 0; i < n; ++ i ) {
    put("IODecl", func->io(i) );
  }

  put_scope_sub(mgr, func);

  put("Stmt", func->stmt() );
}

// continuous assignment の内容を出力する関数
// IEEE 1364-2001 p.652
void
VlDumperImpl::put(
  const char* label,
  const VlContAssign* ca
)
{
  VlDumpHeader x(this, label, "ContAssign");

  put("FileRegion", ca->file_region() );
  put("NetDeclAssign", ca->has_net_decl_assign() );
  put("vpiModule", ca->module()->full_name() );
  put("Strength0", ca->drive0() );
  put("Strength1", ca->drive1() );
  put("Delay", ca->delay() );

  put("Lhs", ca->lhs() );
  put("Rhs", ca->rhs() );
}

END_NAMESPACE_YM_VERILOG
