
/// @file VlDumperImpl_module.cc
/// @brief VlDumperImpl の実装ファイル (module担当)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "VlDumperImpl.h"
#include "ym/vl/VlMgr.h"
#include "ym/vl/VlModule.h"
#include "ym/vl/VlPort.h"


BEGIN_NAMESPACE_YM_VERILOG

BEGIN_NONAMESPACE

// time unit を出力する関数
std::string
unit2str(
  int scale
)
{
  std::string ans;
  scale += 15;
  int a = scale % 3;
  int b = scale / 3;
  switch ( a ) {
  case 2: ans = "100"; break;
  case 1: ans = "10"; break;
  case 0: ans = "1"; break;
  }
  switch ( b ) {
  case 5: ans += "s"; break;
  case 4: ans += "ms"; break;
  case 3: ans += "us"; break;
  case 2: ans += "ns"; break;
  case 1: ans += "ps"; break;
  case 0: ans += "fs"; break;
  }
  return ans;
}

END_NONAMESPACE


// module の内容を出力する関数
// IEEE1364-2001 p.634
void
VlDumperImpl::put(
  const char* label,
  const VlMgr& mgr,
  const VlModule* module
)
{
  VlDumpHeader x(this, label, "Module");

  put("FileRegion", module->file_region() );

  put("DefName", module->def_name() );
  put("FullName", module->full_name() );
  put("DefFile",  module->def_file_region() );

  bool array = (module->module_array() != nullptr);
  put("Array", array);
  if ( array ) {
    put("ModuleArray", module->module_array()->full_name() );
    put("Index", module->index() );
  }
  put("TopModule", module->is_top_module() );
  put("CellInstance", module->is_cell_instance() );
  put("Protected", module->is_protected() );

  if ( module->time_unit() != -16 ) {
    put("TimeUnit", unit2str(module->time_unit()) );
    put("TimePrecision", unit2str(module->time_precision()) );
  }

  put("DefNetType", module->def_net_type() );
  put("UnconnDrive", module->unconn_drive() );
  put("DefDelayMode", module->def_delay_mode() );
  put("DefDecayTime", module->def_decay_time() );
  put("Config", module->config() );
  put("Library", module->library() );
  put("Cell", module->cell() );

#if 0 // VERIFAULT
  {
    VlDumpHeader x20(s, "portfaults");
    if ( handle.portfaults() ) {
      s << "enabled";
    }
    else {
      s << "disabled";
    }
  }
  {
    VlDumpHeader x21(s, "suppress_faults");
    if ( handle.suppress_faults() ) {
      s << "true";
    }
    else {
      s << "false";
    }
  }
#endif

  for ( int i = 0; i < module->port_num(); ++ i ) {
    put("Port", module->port(i));
  }
  for ( int i = 0; i < module->io_num(); ++ i ) {
    put("IODecl", module->io(i));
  }

  put_scope_sub(mgr, module);

  auto process_list = mgr.find_process_list(module);
  put("Process", process_list);
}

// module array の内容を出力する関数
// IEEE 1364-2001 p.635
void
VlDumperImpl::put(
  const char* label,
  const VlMgr& mgr,
  const VlModuleArray* module_array
)
{
  VlDumpHeader x(this, label, "ModuleArray");

  put("FileRegion", module_array->file_region() );
  put("FullName", module_array->full_name() );
  put("Size", module_array->elem_num() );
  put("LeftRange", module_array->left_range_val() );
  put("RightRange", module_array->right_range_val() );

  // 中身は出力しない．
}

// port の内容を出力する関数
// IEEE 1364-2001 p.637
void
VlDumperImpl::put(
  const char* label,
  const VlPort* port
)
{
  VlDumpHeader x(this, label, "Port");

  put("FileRegion", port->file_region() );

  put("Module", port->module()->full_name() );
  put("ConnByName", port->is_conn_by_name() );
  put("Direction", port->direction() );
  put("ExplicitName", port->has_explicit_name() );
  put("PortIndex", port->port_index() );
  if ( port->name() != std::string{} ) {
    put("Name", port->name() );
  }
  put("Size", port->bit_size() );

  put("HighConn", port->high_conn());
  put("LowConn",  port->low_conn() );
}

END_NAMESPACE_YM_VERILOG
