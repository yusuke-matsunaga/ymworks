
/// @file AstModule.cc
/// @brief AstModule の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstModule.h"
#include "ym/vl/AstDeclHead.h"
#include "ym/vl/AstIOHead.h"
#include "ym/vl/AstItem.h"
#include "ym/vl/AstPort.h"
#include "parser/PtModule.h"
#include "parser/JsonUtils.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstModule
//////////////////////////////////////////////////////////////////////

// @brief macromodule 情報の取得
bool
AstModule::is_macromodule() const
{
  _check_ptr();
  return mPtr->is_macromodule();
}

// @brief cell 情報の取得
bool
AstModule::is_cell() const
{
  _check_ptr();
  return mPtr->is_cell();
}

// @brief protect 情報の取得
bool
AstModule::is_protected() const
{
  _check_ptr();
  return mPtr->is_protected();
}

// @brief time unit の取得
int
AstModule::time_unit() const
{
  _check_ptr();
  return mPtr->time_unit();
}

// @brief time precision の取得
int
AstModule::time_precision() const
{
  _check_ptr();
  return mPtr->time_precision();
}

// @brief default net type の取得
VpiNetType
AstModule::nettype() const
{
  _check_ptr();
  return mPtr->nettype();
}

// @brief unconnected drive の取得
VpiUnconnDrive
AstModule::unconn_drive() const
{
  _check_ptr();
  return mPtr->unconn_drive();
}

// @brief default delay mode の取得
VpiDefDelayMode
AstModule::delay_mode() const
{
  _check_ptr();
  return mPtr->delay_mode();
}

// @brief default decay time の取得
int
AstModule::decay_time() const
{
  _check_ptr();
  return mPtr->decay_time();
}

// @brief portfaults 情報の取得
bool
AstModule::portfaults() const
{
  _check_ptr();
  return mPtr->portfaults();
}

// @brief suppress_faults 情報の取得
bool
AstModule::suppress_faults() const
{
  _check_ptr();
  return mPtr->suppress_faults();
}

// @brief config 情報の取得
const std::string&
AstModule::config() const
{
  _check_ptr();
  return mPtr->config();
}

// @brief library 情報の取得
const std::string&
AstModule::library() const
{
  _check_ptr();
  return mPtr->library();
}

// @brief cell 情報の取得
const std::string&
AstModule::cell() const
{
  _check_ptr();
  return mPtr->cell();
}

// @brief パラメータポート宣言のリストの取得
AstDeclHeadList
AstModule::paramport_list() const
{
  _check_ptr();
  return AstDeclHeadList(AstDeclHead(mPtr->paramport_top()));
}

// @brief ポートのリストを返す．
AstPortList
AstModule::port_list() const
{
  _check_ptr();
  return AstPortList(AstPort(mPtr->port_top()));
}

// @brief 入出力宣言のヘッダのリストを返す．
AstIOHeadList
AstModule::iohead_list() const
{
  _check_ptr();
  return AstIOHeadList(AstIOHead(mPtr->iohead_top()));
}

// @brief 入出力宣言の要素数の取得
SizeType
AstModule::iodecl_num() const
{
  _check_ptr();
  return mPtr->iodecl_num();
}

// @brief 宣言ヘッダのリストを返す．
AstDeclHeadList
AstModule::declhead_list() const
{
  _check_ptr();
  return AstDeclHeadList(AstDeclHead(mPtr->declhead_top()));
}

// @brief item のリストを返す．
AstItemList
AstModule::item_list() const
{
  _check_ptr();
  return AstItemList(AstItem(mPtr->item_top()));
}

// @brief top_module フラグを下ろす
void
AstModule::clear_topmodule() const
{
  _check_ptr();
  mPtr->clear_topmodule();
}

// @brief top module のチェック
bool
AstModule::is_topmodule() const
{
  _check_ptr();
  return mPtr->is_topmodule();
}

// @brief in_use フラグの設定
void
AstModule::set_in_use() const
{
  _check_ptr();
  mPtr->set_in_use();
}

// @brief in_use フラグの解除
void
AstModule::reset_in_use() const
{
  _check_ptr();
  mPtr->reset_in_use();
}

// @brief in_use フラグの取得
bool
AstModule::is_in_use() const
{
  _check_ptr();
  return mPtr->is_in_use();
}

// @brief ハッシュ値を返す．
SizeType
AstModule::hash() const
{
  return _key();
}

// @brief 適切な値を持っている時 true を返す．
bool
AstModule::is_valid() const
{
  return mPtr != nullptr;
}

// @brief ファイル位置の取得
FileRegion
AstModule::file_region() const
{
  _check_ptr();
  return mPtr->file_region();
}

// @brief 比較用のユニークなキーを返す．
PtrIntType
AstModule::key() const
{
  return _key();
}

// @brief 名前の取得
const char*
AstModule::name() const
{
  _check_ptr();
  return mPtr->name();
}

// @brief json_obj() の下請け関数
void
AstModule::json_sub(
  JsonValue& jobj
) const
{
  AstNamedBase::json_sub(jobj);
  jobj.add("is_macromodule", JsonValue(is_macromodule()));
  jobj.add("is_cell", JsonValue(is_cell()));
  jobj.add("is_protected", JsonValue(is_protected()));
  jobj.add("time_unit", JsonValue(time_unit()));
  jobj.add("time_unit", JsonValue(time_precision()));
  JsonUtils::add(jobj, "nettype", nettype());

  const char* ud_str = nullptr;
  switch ( unconn_drive() ) {
  case VpiUnconnDrive::HighZ: ud_str = "HighZ"; break;
  case VpiUnconnDrive::Pull1: ud_str = "Pull1"; break;
  case VpiUnconnDrive::Pull0: ud_str = "Pull0"; break;
  }
  jobj.add("unconn_drive", JsonValue(ud_str));

  const char* dm_str = nullptr;
  switch ( delay_mode() ) {
  case VpiDefDelayMode::None:    dm_str = nullptr; break;
  case VpiDefDelayMode::Path:    dm_str = "Path"; break;
  case VpiDefDelayMode::Distrib: dm_str = "Distrib"; break;
  case VpiDefDelayMode::Unit:    dm_str = "Unit"; break;
  case VpiDefDelayMode::Zero:    dm_str = "Zero"; break;
  case VpiDefDelayMode::MTM:     dm_str = "MTM"; break;
  }
  JsonUtils::add(jobj, "delay_mode", dm_str);

  jobj.add("decay_time", JsonValue(decay_time()));

  // portfaults, suppress_faults は無視

  jobj.add("config", JsonValue(config()));
  jobj.add("library", JsonValue(library()));
  jobj.add("cell", JsonValue(cell()));

  JsonUtils::add_list(jobj, "paramport_list", paramport_list());
  JsonUtils::add_list(jobj, "port_list", port_list());
  JsonUtils::add_list(jobj, "iohead_list", iohead_list());
  JsonUtils::add_list(jobj, "declhead_list", declhead_list());
  JsonUtils::add_list(jobj, "item_list", item_list());
}

END_NAMESPACE_YM_VERILOG
