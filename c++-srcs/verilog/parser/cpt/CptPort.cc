
/// @file CptPort.cc
/// @brief CptPort の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptPort.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


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

// @brief 内側のポート結線を表す式の取得
const AstExpr*
CptPort::expr() const
{
  return nullptr;
}

// @brief 内部のポート結線リストのサイズの取得
SizeType
CptPort::portref_size() const
{
  return 0;
}

// @brief 内部のポート結線の取得
const AstExpr*
CptPort::portref(
  SizeType index
) const
{
  throw std::out_of_range{"index is out of range"};
}

// @brief 内部のポート結線のリストの取得
AstExprVec
CptPort::portref_list() const
{
  return {};
}

// @brief 内部のポート結線の向きの取得
VpiDir
CptPort::portref_dir(
  SizeType index
) const
{
  throw std::out_of_range{"index is out of range"};
}

// @brief 内部のポート結線の向きを設定する．
void
CptPort::set_portref_dir(
  SizeType index,
  VpiDir dir
)
{
  throw std::out_of_range{"index is out of range"};
}


//////////////////////////////////////////////////////////////////////
// クラス CptPort1
//////////////////////////////////////////////////////////////////////

// @brief 内側のポート結線を表す式の取得
const AstExpr*
CptPort1::expr() const
{
  return mExpr;
}

// @brief 内部のポート結線リストのサイズの取得
SizeType
CptPort1::portref_size() const
{
  return 1;
}

// @brief 内部のポート結線の取得
const AstExpr*
CptPort1::portref(
  SizeType index
) const
{
  _check_index(index);
  return mExpr;
}

// @brief 内部のポート結線のリストの取得
AstExprVec
CptPort1::portref_list() const
{
  return {mExpr};
}

// @brief 内部のポート結線の向きの取得
VpiDir
CptPort1::portref_dir(
  SizeType index
) const
{
  _check_index(index);
  return mDir;
}

// @brief 内部のポート結線の向きを設定する．
void
CptPort1::set_portref_dir(
  SizeType index,
  VpiDir dir
)
{
  _check_index(index);
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
  return mPortRefList.size();
}

// @brief 内部のポート結線の取得
const AstExpr*
CptPort2::portref(
  SizeType index
) const
{
  return mPortRefList[index];
}

// @brief 内部のポート結線のリストの取得
AstExprVec
CptPort2::portref_list() const
{
  return mPortRefList.to_vector();
}

// @brief 内部のポート結線の向きの取得
VpiDir
CptPort2::portref_dir(
  SizeType index
) const
{
  _check_index(index);
  return mDirArray[index];
}

// @brief 内部のポート結線の向きを設定する．
void
CptPort2::set_portref_dir(
  SizeType index,
  VpiDir dir
)
{
  _check_index(index);
  mDirArray[index] = dir;
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
  void* p = mAlloc.get_memory(sizeof(CptPort));
  return new (p) CptPort(file_region, ext_name);
}

// ポートの生成
PtPort*
PtFactory::new_Port(
  const FileRegion& file_region,
  const char* ext_name,
  const AstExpr* expr
)
{
  void* p = mAlloc.get_memory(sizeof(CptPort1));
  return new (p) CptPort1(file_region, ext_name, expr);
}

// ポートの生成
PtPort*
PtFactory::new_Port(
  const FileRegion& file_region,
  const char* ext_name,
  const AstExpr* expr,
  PtExprList* portref_list
)
{
  auto n = portref_list->size();
  void* p = mAlloc.get_memory(sizeof(CptPort2));
  auto dir_array = mAlloc.get_array<VpiDir>(n);
  return new (p) CptPort2(file_region, ext_name,
			  expr,
			  portref_list->to_array(mAlloc),
			  dir_array);
}

END_NAMESPACE_YM_VERILOG
