
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
  return mPortList.size();
}

// @brief ポートを取り出す．
const AstPort*
CptUdp::port(
  SizeType index
) const
{
  return mPortList[index];
}

// @brief ポートのリストを取り出す．
AstPortVec
CptUdp::port_list() const
{
  return mPortList.to_vector();
}

// @brief 入出力宣言ヘッダ配列の要素数の取得
SizeType
CptUdp::iohead_num() const
{
  return mIOHeadList.size();
}

// @brief 入出力宣言ヘッダの取得
const AstIOHead*
CptUdp::iohead(
  SizeType index
) const
{
  return mIOHeadList[index];
}

// @brief 入出力宣言ヘッダのリストの取得
AstIOHeadVec
CptUdp::iohead_list() const
{
  return mIOHeadList.to_vector();
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
  return mTableList.size();
}

// @brief テーブルを返す．
const AstUdpEntry*
CptUdp::table(
  SizeType index
) const
{
  return mTableList[index];
}

// @brief テーブルのリストを返す．
AstUdpEntryVec
CptUdp::table_list() const
{
  return mTableList.to_vector();
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
  return mInputList.size();
}

// @brief 入力値を取り出す．
const AstUdpValue*
CptUdpEntry::input(
  SizeType index
) const
{
  return mInputList[index];
}

// @brief 入力値のリストを取り出す．
AstUdpValueVec
CptUdpEntry::input_list() const
{
  return mInputList.to_vector();
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
  const std::vector<PtPort*>& port_list,
  const std::vector<PtIOHead*>& iohead_list,
  const AstUdpEntryVec& entry_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptUdp));
  return new (p) CptUdp(file_region, name,
			PtPortArray(mAlloc, port_list),
			PtIOHeadArray(mAlloc, iohead_list),
			false, nullptr,
			PtUdpEntryArray(mAlloc, entry_list));
}

// sequential UDP の生成
PtUdp*
PtFactory::new_SeqUdp(
  const FileRegion& file_region,
  const char* name,
  const std::vector<PtPort*>& port_list,
  const std::vector<PtIOHead*>& iohead_list,
  const AstExpr* init_value,
  const AstUdpEntryVec& entry_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptUdp));
  return new (p) CptUdp(file_region, name,
			PtPortArray(mAlloc, port_list),
			PtIOHeadArray(mAlloc, iohead_list),
			true, init_value,
			PtUdpEntryArray(mAlloc, entry_list));
}

// combinational UDP 用のテーブルエントリの生成
PtUdpEntry*
PtFactory::new_UdpEntry(
  const FileRegion& file_region,
  const AstUdpValueVec& input_list,
  const AstUdpValue* output
)
{
  void* p = mAlloc.get_memory(sizeof(CptUdpEntry));
  return new (p) CptUdpEntry(file_region,
			     PtUdpValueArray(mAlloc, input_list),
			     output);
}

// sequential UDP 用のテーブルエントリの生成
PtUdpEntry*
PtFactory::new_UdpEntry(
  const FileRegion& file_region,
  const AstUdpValueVec& input_list,
  const AstUdpValue* current,
  const AstUdpValue* output
)
{
  void* p = mAlloc.get_memory(sizeof(CptUdpEntryS));
  return new (p) CptUdpEntryS(file_region,
			      PtUdpValueArray(mAlloc, input_list),
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
