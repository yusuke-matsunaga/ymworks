
/// @file SptModule.cc
/// @brief SptModule の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "SptModule.h"
#include "parser/SptFactory.h"

#include "ym/pt/PtItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// module を表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptModule::SptModule(
  const FileRegion& file_region,
  const char* name,
  bool macro,
  bool is_cell,
  bool is_protected,
  int time_unit,
  int time_precision,
  VpiNetType net_type,
  VpiUnconnDrive unconn,
  VpiDefDelayMode delay,
  int decay,
  bool explicit_name,
  bool portfaults,
  bool suppress_faults,
  const std::string& config,
  const std::string& library,
  const std::string& cell,
  PtiDeclHeadArray&& paramport_array,
  PtiPortArray&& port_array,
  PtiIOHeadArray&& iohead_array,
  PtiDeclHeadArray&& decl_array,
  PtiItemArray&& item_array
) : mFileRegion{file_region},
    mName{name},
    mDefDecayTime{decay},
    mConfig{config},
    mLibrary{library},
    mCell{cell},
    mParamPortArray{std::move(paramport_array)},
    mPortArray{std::move(port_array)},
    mIOHeadArray{std::move(iohead_array)},
    mDeclHeadArray{std::move(decl_array)},
    mItemArray{std::move(item_array)}
{
  mFlags =
    static_cast<std::uint32_t>(is_cell)
    | (static_cast<std::uint32_t>(is_protected) << 1)
    | (static_cast<std::uint32_t>(time_precision + 16) << 2)
    | (static_cast<std::uint32_t>(time_unit + 16) << 7)
    | (static_cast<std::uint32_t>(net_type) << 12)
    | (static_cast<std::uint32_t>(unconn) << 16)
    | (static_cast<std::uint32_t>(delay) << 18)
    | (static_cast<std::uint32_t>(macro) << 21)
    | (static_cast<std::uint32_t>(explicit_name) << 22)
    | (1U << 23) // top_module
    | (static_cast<std::uint32_t>(portfaults) << 25)
    | (static_cast<std::uint32_t>(suppress_faults) << 26)
    ;

  mIODeclNum = 0;
  for ( auto head: mIOHeadArray ) {
    mIODeclNum += head->item_num();
  }
}

// デストラクタ
SptModule::~SptModule()
{
}

// ファイル位置の取得
FileRegion
SptModule::file_region() const
{
  return mFileRegion;
}

// 名前の取得
const char*
SptModule::name() const
{
  return mName;
}

// @brief パラメータポート宣言配列の要素数の取得
SizeType
SptModule::paramport_num() const
{
  return mParamPortArray.size();
}

// @brief パラメータポート宣言の取得
const PtDeclHead*
SptModule::paramport(
  SizeType pos
) const
{
  return mParamPortArray[pos];
}

// @brief ポート数を取り出す．
SizeType
SptModule::port_num() const
{
  return mPortArray.size();
}

// @brief ポートを取り出す．
const PtPort*
SptModule::port(
  SizeType pos
) const
{
  return mPortArray[pos];
}

// @brief 入出力宣言ヘッダ配列の要素数の取得
SizeType
SptModule::iohead_num() const
{
  return mIOHeadArray.size();
}

// @brief 入出力宣言の取得
const PtIOHead*
SptModule::iohead(
  SizeType pos
) const
{
  return mIOHeadArray[pos];
}

// @brief 入出力宣言の要素数の取得
SizeType
SptModule::iodecl_num() const
{
  return mIODeclNum;
}

// @brief 宣言ヘッダ配列の要素数の取得
SizeType
SptModule::declhead_num() const
{
  return mDeclHeadArray.size();
}

// @brief 宣言ヘッダの取得
const PtDeclHead*
SptModule::declhead(
  SizeType pos
) const
{
  return mDeclHeadArray[pos];
}

// @brief item 配列の要素数の取得
SizeType
SptModule::item_num() const
{
  return mItemArray.size();
}

// @brief item の取得
const PtItem*
SptModule::item(
  SizeType pos
) const
{
  return mItemArray[pos];
}

// macromodule 情報の取得
bool
SptModule::is_macromodule() const
{
  return (mFlags >> 21) & 1;
}

// cell 情報の取得
bool
SptModule::is_cell() const
{
  return mFlags & 1;
}

// protect 情報の取得
bool
SptModule::is_protected() const
{
  return (mFlags >> 1) & 1;
}

// time unit の取得
int
SptModule::time_unit() const
{
  return ((mFlags >> 7) & 0x1f) - 16;
}

// time precision の取得
int
SptModule::time_precision() const
{
  return ((mFlags >> 2) & 0x1f) - 16;
}

// default net type の取得
VpiNetType
SptModule::nettype() const
{
  return static_cast<VpiNetType>((mFlags >> 12) & 0xf);
}

// unconnected drive の取得
VpiUnconnDrive
SptModule::unconn_drive() const
{
  return static_cast<VpiUnconnDrive>((mFlags >> 16) & 0x3);
}

// default delay mode の取得
VpiDefDelayMode
SptModule::delay_mode() const
{
  return static_cast<VpiDefDelayMode>((mFlags >> 18) & 0x7);
}

// default decay time の取得
int
SptModule::decay_time() const
{
  return mDefDecayTime;
}

// top module のチェック
bool
SptModule::is_topmodule() const
{
  return static_cast<bool>((mFlags >> 23) & 1);
}

// top_module フラグを下ろす．
void
SptModule::clear_topmodule() const
{
  mFlags &= ~(1 << 23);
}

// @brief in_use フラグの設定
void
SptModule::set_in_use() const
{
  mFlags |= (1 << 24);
}

// @brief in_use フラグの解除
void
SptModule::reset_in_use() const
{
  mFlags &= ~(1 << 24);
}

// @brief in_use フラグの取得
bool
SptModule::is_in_use() const
{
  return static_cast<bool>((mFlags >> 24) & 1);
}

// portfaults 情報の取得
bool
SptModule::portfaults() const
{
  return static_cast<bool>((mFlags >> 25) & 1);
}

// suppress_faults 情報の取得
bool
SptModule::suppress_faults() const
{
  return static_cast<bool>((mFlags >> 26) & 1);
}

// config 情報の取得
const std::string&
SptModule::config() const
{
  return mConfig;
}

// library 情報の取得
const std::string&
SptModule::library() const
{
  return mLibrary;
}

// cell 情報の取得
const std::string&
SptModule::cell() const
{
  return mCell;
}

// 名無しのポートを持つことを記録する．
void
SptModule::set_named_port()
{
  mFlags |= (1 << 22);
}

// すべてのポートが外部名を持っているときに true を返す．
bool
SptModule::explicit_name() const
{
  return (mFlags >> 22) & 1;
}


//////////////////////////////////////////////////////////////////////
// port を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptPort::SptPort(
  const FileRegion& file_region,
  const PtExpr* portref,
  PtiExprArray&& portref_array,
  const char* ext_name
) : mFileRegion{file_region},
    mExtName{ext_name},
    mPortRef{portref},
    mPortRefArray{std::move(portref_array)}
{
}

// デストラクタ
SptPort::~SptPort()
{
}

// ファイル位置の取得
FileRegion
SptPort::file_region() const
{
  return mFileRegion;
}

// 外向の名前の取得
const char*
SptPort::ext_name() const
{
  return mExtName;
}

// @brief 内側のポート結線を表す式の取得
const PtExpr*
SptPort::portref() const
{
  return mPortRef;
}

// 内部のポート結線リストのサイズの取得
SizeType
SptPort::portref_size() const
{
  return mPortRefArray.size();
}

// 内部のポート結線の取得
const PtExpr*
SptPort::portref_elem(
  int pos
) const
{
  return mPortRefArray[pos];
}

// @brief 内部のポート結線の報告の取得
VpiDir
SptPort::portref_dir(
  int pos
) const
{
  return mDirArray[pos];
}

// @brief portref の方向を設定する．
void
SptPort::_set_portref_dir(
  int pos,
  VpiDir dir
)
{
  mDirArray[pos] = dir;
}


//////////////////////////////////////////////////////////////////////
// モジュール関係のオブジェクトを生成する関数
//////////////////////////////////////////////////////////////////////

// @brief モジュールの生成
const PtModule*
SptFactory::new_Module(
  const FileRegion& file_region,
  const char* name,
  bool macro,
  bool is_cell,
  bool is_protected,
  int time_unit,
  int time_precision,
  VpiNetType net_type,
  VpiUnconnDrive unconn,
  VpiDefDelayMode delay,
  int decay,
  bool explicit_name,
  bool portfaults,
  bool suppress_faults,
  const std::string& config,
  const std::string& library,
  const std::string& cell,
  const std::vector<const PtDeclHead*>& paramport_array,
  const std::vector<const PtPort*>& port_array,
  const std::vector<const PtIOHead*>& iohead_array,
  const std::vector<const PtDeclHead*>& declhead_array,
  const std::vector<const PtItem*>& item_array
)
{
  auto node = new SptModule(file_region, name,
			    macro, is_cell, is_protected,
			    time_unit, time_precision,
			    net_type, unconn,
			    delay, decay,
			    explicit_name,
			    portfaults, suppress_faults,
			    config, library, cell,
			    PtiDeclHeadArray(mAlloc, paramport_array),
			    PtiPortArray(mAlloc, port_array),
			    PtiIOHeadArray(mAlloc, iohead_array),
			    PtiDeclHeadArray(mAlloc, declhead_array),
			    PtiItemArray(mAlloc, item_array));
  return node;
}


//////////////////////////////////////////////////////////////////////
// ポート関係のオブジェクトを生成する関数
//////////////////////////////////////////////////////////////////////

// @brief ポートの生成
PtiPort*
SptFactory::new_Port(
  const FileRegion& file_region,
  const char* ext_name
)
{
  auto node = new SptPort(file_region,
			  nullptr,
			  PtiExprArray(),
			  ext_name);
  return node;
}

// @brief ポートの生成
PtiPort*
SptFactory::new_Port(
  const FileRegion& file_region,
  const PtExpr* portref,
  const char* ext_name
)
{
  auto node = new SptPort(file_region,
			  portref,
			  PtiExprArray(),
			  ext_name);
  return node;
}

// @brief ポートの生成
PtiPort*
SptFactory::new_Port(
  const FileRegion& file_region,
  const PtExpr* portref,
  const std::vector<const PtExpr*>& portref_array,
  const char* ext_name
)
{
  auto node = new SptPort(file_region,
			  portref,
			  PtiExprArray(mAlloc, portref_array),
			  ext_name);
  return node;
}

END_NAMESPACE_YM_VERILOG
