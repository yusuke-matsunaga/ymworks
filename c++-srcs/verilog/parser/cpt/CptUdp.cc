
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

// @brief ポート数を取り出す．
SizeType
CptUdp::port_num() const
{
  return mPortArray.size();
}

// @brief ポートを取り出す．
const AstPort*
CptUdp::port(
  SizeType pos
) const
{
  return mPortArray[pos];
}

// @brief 入出力宣言ヘッダ配列の要素数の取得
SizeType
CptUdp::iohead_num() const
{
  return mIOHeadArray.size();
}

// @brief 入出力宣言ヘッダの取得
const AstIOHead*
CptUdp::iohead(
  SizeType pos
) const
{
  return mIOHeadArray[pos];
}

// 初期値を取出す．
const AstExpr*
CptUdp::init_value() const
{
  return mInitValue;
}

// @brief テーブルの要素数を取り出す．
SizeType
CptUdp::table_num() const
{
  return mTableArray.size();
}

// @brief テーブルの要素を取り出す．
const AstUdpEntry*
CptUdp::table(
  SizeType pos
) const
{
  return mTableArray[pos];
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

// @brief 入力値の配列の要素数を取り出す．
SizeType
CptUdpEntry::input_num() const
{
  return mInputArray.size();
}

// @brief 入力値を取り出す．
const AstUdpValue*
CptUdpEntry::input(
  SizeType pos
) const
{
  return mInputArray[pos];
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
  PtPortArray&& port_array,
  PtIOHeadArray&& iohead_array,
  PtUdpEntryArray&& entry_array
)
{
  void* p = mAlloc.get_memory(sizeof(CptUdp));
  return new (p) CptUdp(file_region, name,
			std::move(port_array),
			std::move(iohead_array),
			false, nullptr,
			std::move(entry_array));
}

// sequential UDP の生成
PtUdp*
PtFactory::new_SeqUdp(
  const FileRegion& file_region,
  const char* name,
  PtPortArray&& port_array,
  PtIOHeadArray&& iohead_array,
  const AstExpr* init_value,
  PtUdpEntryArray&& entry_array
)
{
  void* p = mAlloc.get_memory(sizeof(CptUdp));
  return new (p) CptUdp(file_region, name,
			std::move(port_array),
			std::move(iohead_array),
			true, init_value,
			std::move(entry_array));
}

// combinational UDP 用のテーブルエントリの生成
PtUdpEntry*
PtFactory::new_UdpEntry(
  const FileRegion& file_region,
  PtUdpValueArray&& input_array,
  const AstUdpValue* output
)
{
  void* p = mAlloc.get_memory(sizeof(CptUdpEntry));
  return new (p) CptUdpEntry(file_region,
			     std::move(input_array),
			     output);
}

// sequential UDP 用のテーブルエントリの生成
PtUdpEntry*
PtFactory::new_UdpEntry(
  const FileRegion& file_region,
  PtUdpValueArray&& input_array,
  const AstUdpValue* current,
  const AstUdpValue* output
)
{
  void* p = mAlloc.get_memory(sizeof(CptUdpEntryS));
  return new (p) CptUdpEntryS(file_region,
			      std::move(input_array),
			      current,
			      output);
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
