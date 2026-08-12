
/// @file CptPort.cc
/// @brief CptPort の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptPort.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"
#include "parser/PtExpr.h" // for promoting PtExpr -> AstExpr


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptPort
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptPort::file_region() const
{
  return mFileRegion;
}

// 外向の名前(本当のポート名)を取出す
const char*
CptPort::ext_name() const
{
  return mExtName;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPort0
//////////////////////////////////////////////////////////////////////

// @brief 内側のポート結線を表す式の取得
const AstExpr*
CptPort0::expr() const
{
  return nullptr;
}

// @brief 内部のポート結線リストのサイズの取得
SizeType
CptPort0::portref_size() const
{
  return 0;
}

// @brief 内部のポート結線リストの取得
const AstExpr*
CptPort0::portref_elem(
  SizeType pos
) const
{
  throw std::out_of_range{"pos is out of range"};
}

//@brief 内部ポート結線の方向の取得
VpiDir
CptPort0::portref_dir(
  SizeType pos
) const
{
  throw std::out_of_range{"pos is out of range"};
}

// @brief portref の方向を設定する．
void
CptPort0::set_portref_dir(
  SizeType pos,
  VpiDir dir
)
{
  throw std::out_of_range{"pos is out of range"};
}


//////////////////////////////////////////////////////////////////////
// クラス CptPort1
//////////////////////////////////////////////////////////////////////

// @brief 内側のポート結線を表す式の取得
const AstExpr*
CptPort1::expr() const
{
  return mPortRef;
}

// @brief 内部のポート結線リストのサイズの取得
SizeType
CptPort1::portref_size() const
{
  return 1;
}

// @brief 内部のポート結線リストの取得
const AstExpr*
CptPort1::portref_elem(
  SizeType pos
) const
{
  if ( pos >= 1 ) {
    throw std::out_of_range{"pos is out of range"};
  }
  return mPortRef;
}

// @brief 内部ポート結線の方向の取得
VpiDir
CptPort1::portref_dir(
  SizeType pos
) const
{
  return mDir;
}

// @brief portref の方向を設定する．
void
CptPort1::set_portref_dir(
  SizeType pos,
  VpiDir dir
)
{
  if ( pos >= 1 ) {
    throw std::out_of_range{"pos is out of range"};
  }
  mDir = dir;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPort2
//////////////////////////////////////////////////////////////////////

// @brief 内側のポート結線を表す式の取得
const AstExpr*
CptPort2::expr() const
{
  return mExpr;
}

// @brief 内部のポート結線リストのサイズの取得
SizeType
CptPort2::portref_size() const
{
  return mPortRefArray.size();
}

// @brief 内部のポート結線リストの取得
const AstExpr*
CptPort2::portref_elem(
  SizeType pos
) const
{
  return mPortRefArray[pos];
}

// @brief 内部ポート結線の方向の取得
VpiDir
CptPort2::portref_dir(
  SizeType pos
) const
{
  if ( pos >= portref_size() ) {
    throw std::out_of_range{"pos is out of range"};
  }
  return mDirArray[pos];
}

// @brief portref の方向を設定する．
void
CptPort2::set_portref_dir(
  SizeType pos,
  VpiDir dir
)
{
  if ( pos >= portref_size() ) {
    throw std::out_of_range{"pos is out of range"};
  }
  mDirArray[pos] = dir;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// @brief ポートの生成
PtPort*
PtFactory::new_Port(
  const FileRegion& file_region,
  const char* ext_name
)
{
  void* p = mAlloc.get_memory(sizeof(CptPort0));
  return new (p) CptPort0(file_region, ext_name);
}

// ポートの生成
PtPort*
PtFactory::new_Port(
  const FileRegion& file_region,
  const AstExpr* portref,
  const char* ext_name
)
{
  void* p = mAlloc.get_memory(sizeof(CptPort1));
  return new (p) CptPort1(file_region, portref, ext_name);
}

// ポートの生成
PtPort*
PtFactory::new_Port(
  const FileRegion& file_region,
  PtExprArray&& portref_array,
  const char* ext_name
)
{
  auto expr = new_Concat(file_region, PtArray(mAlloc, portref_array));
  void* p = mAlloc.get_memory(sizeof(CptPort2));
  void* q = mAlloc.get_memory(sizeof(VpiDir) * portref_array.size());
  return new (p) CptPort2(file_region, expr,
			  std::move(portref_array),
			  ext_name, q);
}

END_NAMESPACE_YM_VERILOG
