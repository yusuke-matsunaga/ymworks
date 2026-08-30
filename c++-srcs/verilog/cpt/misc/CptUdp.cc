
/// @file CptUdp.cc
/// @brief CptUdp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptUdp.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"
#include "parser/PtPort.h"
#include "parser/PtDecl.h"
#include "parser/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptUdp
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptUdp::file_region() const
{
  return mFileRegion;
}

// primitivie type を返す．
VpiPrimType
CptUdp::prim_type() const
{
  if ( mSeq ) {
    return VpiPrimType::Seq;
  }
  else {
    return VpiPrimType::Comb;
  }
}

// 名前を返す．
const char*
CptUdp::name() const
{
  return mName;
}

// @brief ポートのリストを取り出す．
AstPortList
CptUdp::port_list() const
{
  return AstPortList(mPortTop);
}

// @brief 入出力宣言ヘッダのリストの取得
AstIOHeadList
CptUdp::iohead_list() const
{
  return AstIOHeadList(mIOHeadTop);
}

// 初期値を取出す．
const AstExpr*
CptUdp::init_value() const
{
  return mInitValue;
}

// @brief テーブルのリストを返す．
AstUdpEntryList
CptUdp::table_list() const
{
  return AstUdpEntryList(mTableTop);
}


//////////////////////////////////////////////////////////////////////
// クラス CptUdpEntry
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptUdpEntry::file_region() const
{
  return mFileRegion;
}

// @brief 入力値のリストを取り出す．
AstUdpValueList
CptUdpEntry::input_list() const
{
  return AstUdpValueList(mInputTop);
}

// @brief 現状態の値を取り出す．
const AstUdpValue*
CptUdpEntry::current() const
{
  return nullptr;
}

// 出力の値を取り出す．
const AstUdpValue*
CptUdpEntry::output() const
{
  return mOutput;
}


//////////////////////////////////////////////////////////////////////
// クラス CptUdpEntryS
//////////////////////////////////////////////////////////////////////

// 現状態の値を取り出す．
const AstUdpValue*
CptUdpEntryS::current() const
{
  return mCurrent;
}


//////////////////////////////////////////////////////////////////////
// クラス CptUdpValue
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptUdpValue::file_region() const
{
  return mFileRegion;
}

// シンボルを取り出す．
VlUdpVal
CptUdpValue::symbol() const
{
  return mSymbol;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// combinational UDP の生成
PtUdp*
PtFactory::new_CmbUdp(
  const FileRegion& file_region,
  const char* name,
  PtPort* port_top,
  PtIOHead* iohead_top,
  PtUdpEntry* entry_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptUdp));
  return new (p) CptUdp(file_region, name,
			port_top,
			iohead_top,
			false, nullptr,
			entry_top);
}

// sequential UDP の生成
PtUdp*
PtFactory::new_SeqUdp(
  const FileRegion& file_region,
  const char* name,
  PtPort* port_top,
  PtIOHead* iohead_top,
  const AstExpr* init_value,
  PtUdpEntry* entry_top

)
{
  void* p = mAlloc.get_memory(sizeof(CptUdp));
  return new (p) CptUdp(file_region, name,
			port_top,
			iohead_top,
			true, init_value,
			entry_top);
}

// combinational UDP 用のテーブルエントリの生成
PtUdpEntry*
PtFactory::new_UdpEntry(
  const FileRegion& file_region,
  PtUdpValue* input_top,
  const AstUdpValue* output
)
{
  void* p = mAlloc.get_memory(sizeof(CptUdpEntry));
  return new (p) CptUdpEntry(file_region,
			     input_top,
			     output);
}

// sequential UDP 用のテーブルエントリの生成
PtUdpEntry*
PtFactory::new_UdpEntry(
  const FileRegion& file_region,
  PtUdpValue* input_top,
  const AstUdpValue* current,
  const AstUdpValue* output
)
{
  void* p = mAlloc.get_memory(sizeof(CptUdpEntryS));
  return new (p) CptUdpEntryS(file_region,
			      input_top,
			      current, output);
}

// UDP のテーブルエントリの要素の値の生成 (1つの値)
PtUdpValue*
PtFactory::new_UdpValue(
  const FileRegion& file_region,
  char symbol
)
{
  void* p = mAlloc.get_memory(sizeof(CptUdpValue));
  return new (p) CptUdpValue(file_region, symbol);
}

// @brief UDP のテーブルエントリの要素の値の生成
PtUdpValue*
PtFactory::new_UdpValue(
  const FileRegion& file_region,
  char symbol1,
  char symbol2
)
{
  void* p = mAlloc.get_memory(sizeof(CptUdpValue));
  return new (p) CptUdpValue(file_region, symbol1, symbol2);
}

END_NAMESPACE_YM_VERILOG
