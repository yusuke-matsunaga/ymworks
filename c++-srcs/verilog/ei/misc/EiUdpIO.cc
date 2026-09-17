
/// @file EiUdpIO.cc
/// @brief EiUdpIO の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiUdpIO.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// クラス EiUdpIO
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiUdpIO::type() const
{
  return VpiObjType::IODecl;
}

// ファイル位置を返す．
FileRegion
EiUdpIO::file_region() const
{
  return mAstItem.file_region();
}

// @brief 名前を返す．
std::string
EiUdpIO::name() const
{
  return mAstItem.name();
}

// @brief 方向を返す．
VpiDir
EiUdpIO::direction() const
{
  if ( mAstHeader.direction() == VpiDir::Inout ) {
    throw std::logic_error{"mAstHeader->direction() == VpiDir::Inout"};
  }
  return mAstHeader.direction();
}

// @brief 符号の属性の取得
bool
EiUdpIO::is_signed() const
{
  return false;
}

// @brief 範囲指定を持つとき true を返す．
bool
EiUdpIO::has_range() const
{
  return false;
}

// @brief MSB の値を返す．
int
EiUdpIO::left_range_val() const
{
  return 0;
}

// @brief LSB の値を返す．
int
EiUdpIO::right_range_val() const
{
  return 0;
}

// @brief 範囲のMSBを表す文字列の取得
std::string
EiUdpIO::left_range_string() const
{
  return {};
}

// @brief 範囲のLSBを表す文字列の取得
std::string
EiUdpIO::right_range_string() const
{
  return {};
}

// @brief サイズを返す．
SizeType
EiUdpIO::bit_size() const
{
  return 1;
}

// @brief 対応する宣言要素を返す．
const VlDecl*
EiUdpIO::decl() const
{
  return nullptr;
}

// @brief 親のモジュールの取得
const VlModule*
EiUdpIO::module() const
{
  return nullptr;
}

// @brief 親のUDP定義を返す．
const VlUdpDefn*
EiUdpIO::udp_defn() const
{
  return mUdp;
}

// @brief 親のタスク/の取得
const VlTaskFunc*
EiUdpIO::task() const
{
  return nullptr;
}

// @brief 親の関数の取得
const VlTaskFunc*
EiUdpIO::function() const
{
  return nullptr;
}

END_NAMESPACE_YM_VERILOG
