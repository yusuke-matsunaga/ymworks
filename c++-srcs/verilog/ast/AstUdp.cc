
/// @file AstUdp.cc
/// @brief AstUdp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstUdp.h"
#include "ym/vl/AstExpr.h"
#include "ym/vl/AstIOHead.h"
#include "ym/vl/AstPort.h"
#include "ym/vl/AstUdpEntry.h"
#include "ym/vl/AstUdpValue.h"
#include "parser/PtUdp.h"
#include "parser/JsonUtils.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstUdp
//////////////////////////////////////////////////////////////////////

// @brief primitive type を返す．
VpiPrimType
AstUdp::prim_type() const
{
  _check_ptr();
  return mPtr->prim_type();
}

// @brief ポートのリストを取り出す．
AstPortList
AstUdp::port_list() const
{
  _check_ptr();
  return AstPortList(AstPort(mPtr->port_top()));
}

// @brief 入出力宣言ヘッダのリストの取得
AstIOHeadList
AstUdp::iohead_list() const
{
  _check_ptr();
  return AstIOHeadList(AstIOHead(mPtr->iohead_top()));
}

// @brief 初期値を取出す．
AstExpr
AstUdp::init_value() const
{
  _check_ptr();
  return AstExpr(mPtr->init_value());
}

// @brief テーブルのリストを返す．
AstUdpEntryList
AstUdp::table_list() const
{
  _check_ptr();
  return AstUdpEntryList(AstUdpEntry(mPtr->table_top()));
}

// @brief 適切な値を持っている時 true を返す．
bool
AstUdp::is_valid() const
{
  return mPtr != nullptr;
}

// @brief ファイル位置の取得
FileRegion
AstUdp::file_region() const
{
  _check_ptr();
  return mPtr->file_region();
}

// @brief 比較用のユニークなキーを返す．
PtrIntType
AstUdp::key() const
{
  return _key();
}

// @brief 名前の取得
const char*
AstUdp::name() const
{
  _check_ptr();
  return mPtr->name();
}

// @brief json_obj() の下請け関数
void
AstUdp::json_sub(
  JsonValue& jobj
) const
{
  AstNamedBase::json_sub(jobj);
  const char* type_str = nullptr;
  switch ( prim_type() ) {
  case VpiPrimType::Seq: type_str =  "Seq"; break;
  case VpiPrimType::Comb: type_str = "Comb"; break;
  default: break;
  }
  jobj.add("prim_type", JsonValue(type_str));
  jobj.add("port_list", port_list().json_obj());
  jobj.add("iohead_list", iohead_list().json_obj());
  JsonUtils::add(jobj, "init_value", init_value());
  jobj.add("table_list", table_list().json_obj());
}

END_NAMESPACE_YM_VERILOG
