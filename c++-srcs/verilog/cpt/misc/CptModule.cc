
/// @file CptModule.cc
/// @brief CptModule の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptModule.h"
#include "alloc/Alloc.h"
#include "parser/PtPort.h"
#include "parser/PtIOHead.h"
#include "parser/PtDeclHead.h"
#include "parser/PtItem.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// module を表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptModule::CptModule(
  const FileRegion& file_region,
  const char* name,
  bool macro,
  bool is_cell,
  int time_unit,
  int time_precision,
  VpiNetType net_type,
  VpiUnconnDrive unconn,
  VpiDefDelayMode delay,
  int decay,
  const PtDeclHead* paramport_top,
  const PtPort* port_top,
  const PtIOHead* iohead_top,
  const PtDeclHead* declhead_top,
  const PtItem* item_top
) : mFileRegion{file_region},
    mName{name},
    mDefDecayTime{decay},
    mParamPortTop{paramport_top},
    mPortTop{port_top},
    mIOHeadTop{iohead_top},
    mDeclHeadTop{declhead_top},
    mItemTop{item_top}
{
  mFlags =
    static_cast<std::uint32_t>(is_cell)
    | (static_cast<std::uint32_t>(time_precision + 16) << 2)
    | (static_cast<std::uint32_t>(time_unit + 16) << 7)
    | (static_cast<std::uint32_t>(net_type) << 12)
    | (static_cast<std::uint32_t>(unconn) << 16)
    | (static_cast<std::uint32_t>(delay) << 18)
    | (static_cast<std::uint32_t>(macro) << 21)
    ;

  mIODeclNum = 0;
  for ( auto head = iohead_top; head != nullptr; head = head->link() ) {
    mIODeclNum += AstIOItemList(AstIOItem(head->item_top())).size();
  }
}

// デストラクタ
CptModule::~CptModule()
{
}

// ファイル位置を返す．
FileRegion
CptModule::file_region() const
{
  return mFileRegion;
}

// 名前を取り出す．
const char*
CptModule::name() const
{
  return mName;
}

// @brief パラメータポート宣言のリストの取得
const PtDeclHead*
CptModule::paramport_top() const
{
  return mParamPortTop;
}

// @brief ポートのリストを返す．
const PtPort*
CptModule::port_top() const
{
  return mPortTop;
}

// @brief 入出力宣言のヘッダのリストを返す．
const PtIOHead*
CptModule::iohead_top() const
{
  return mIOHeadTop;
}

// @brief 入出力宣言の要素数の取得
SizeType
CptModule::iodecl_num() const
{
  return mIODeclNum;
}

// @brief 宣言ヘッダのリストを返す．
const PtDeclHead*
CptModule::declhead_top() const
{
  return mDeclHeadTop;
}

// @brief item のリストを返す．
const PtItem*
CptModule::item_top() const
{
  return mItemTop;
}

// macromodule の時 true を返す．
bool
CptModule::is_macromodule() const
{
  return static_cast<bool>((mFlags >> 21) & 1);
}

// cell の時 true を返す．
bool
CptModule::is_cell() const
{
  return static_cast<bool>(mFlags & 1);
}

// time unit を返す．
int
CptModule::time_unit() const
{
  return ((mFlags >> 7) & 0x1f) - 16;
}

// time precision を返す．
int
CptModule::time_precision() const
{
  return ((mFlags >> 2) & 0x1f) - 16;
}

// default net type を返す．
VpiNetType
CptModule::nettype() const
{
  return static_cast<VpiNetType>((mFlags >> 12) & 0xf);
}

// unconnected drive を返す．
VpiUnconnDrive
CptModule::unconn_drive() const
{
  return static_cast<VpiUnconnDrive>((mFlags >> 16) & 0x3);
}

// default delay mode を返す．
VpiDefDelayMode
CptModule::delay_mode() const
{
  return static_cast<VpiDefDelayMode>((mFlags >> 18) & 0x7);
}

// default decay time を返す．
int
CptModule::decay_time() const
{
  return mDefDecayTime;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// モジュールの生成
PtModule*
PtFactory::new_Module(
  const FileRegion& file_region,
  const char* name,
  bool macro,
  bool is_cell,
  int time_unit,
  int time_precision,
  VpiNetType net_type,
  VpiUnconnDrive unconn,
  VpiDefDelayMode delay,
  int decay,
  const PtDeclHead* paramport_top,
  const PtPort* port_top,
  const PtIOHead* iohead_top,
  const PtDeclHead* decl_top,
  const PtItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptModule));
  return new (p) CptModule(file_region, name,
			   macro, is_cell,
			   time_unit, time_precision,
			   net_type, unconn,
			   delay, decay,
			   paramport_top,
			   port_top,
			   iohead_top,
			   decl_top,
			   item_top);
}

END_NAMESPACE_YM_VERILOG
