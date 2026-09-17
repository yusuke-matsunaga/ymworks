
/// @file EiTableEntry.cc
/// @brief EiTableEntry の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiTableEntry.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// クラス EiTableEntry
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiTableEntry::type() const
{
  return VpiObjType::TableEntry;
}

// @brief ファイル位置を返す．
FileRegion
EiTableEntry::file_region() const
{
  return mAstUdpEntry.file_region();
}

// @brief 一行の要素数を返す．
SizeType
EiTableEntry::size() const
{
  SizeType row_size = mUdp->port_num();
  if ( mUdp->prim_type() == VpiPrimType::Seq ) {
    ++ row_size;
  }
  return row_size;
}

// @brief pos 番目の位置の値を返す．
VlUdpVal
EiTableEntry::val(
  SizeType pos
) const
{
  if ( pos >= size() ) {
    throw std::out_of_range{"pos is out of range"};
  }
  return mValArray[pos];
}

// @brief 一行文の内容を表す文字列をつくる．
std::string
EiTableEntry::str() const
{
  auto n = size();
  auto in = n - 1; // 出力変数の分を減らす
  if ( mUdp->prim_type() == VpiPrimType::Seq ) {
    -- in; // さらに状態変数の分を減らす．
  }
  auto in1 = in - 1;
  auto n1 = n - 1;
  std::string s;
  for ( int pos = 0; pos < n; ++ pos ) {
    s += val(pos).to_string();
    if ( pos < in1 ) {
      s += " ";
    }
    else if ( pos < n1 ) {
      s += " : ";
    }
  }
  return s;
}

END_NAMESPACE_YM_VERILOG
