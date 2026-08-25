
/// @file CptIO.cc
/// @brief CptIO の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptIO.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// IO宣言のヘッダのベース実装クラス
//////////////////////////////////////////////////////////////////////

// @brief ファイル位置の取得
FileRegion
CptIOHBase::file_region() const
{
  return mFileRegion;
}

// @brief 型の取得
VpiDir
CptIOHBase::direction() const
{
  return static_cast<VpiDir>((mAttr & 0xff) >> 1);
}

// @brief 補助的な型の取得
VpiAuxType
CptIOHBase::aux_type() const
{
  return static_cast<VpiAuxType>((mAttr >> 8) & 0xff);
}

// @bief 補助的なネット型の取得
VpiNetType
CptIOHBase::net_type() const
{
  return static_cast<VpiNetType>((mAttr >> 16) & 0xff);
}

// @brief 補助的な変数型の取得
VpiVarType
CptIOHBase::var_type() const
{
  return static_cast<VpiVarType>((mAttr >> 24) & 0xff);
}

// @brief 符号の取得
bool
CptIOHBase::is_signed() const
{
  return static_cast<bool>(mAttr & 1);
}

// @brief 範囲の取得
const AstRange*
CptIOHBase::range() const
{
  return nullptr;
}

// @brief 要素のリストの取得
AstIOItemList
CptIOHBase::item_list() const
{
  return AstIOItemList(mItemTop);
}

// @brief 先頭の要素を返す．
PtIOItem*
CptIOHBase::_item_top() const
{
  return mItemTop;
}


//////////////////////////////////////////////////////////////////////
// ビットベクタ型のIO宣言のヘッダ
//////////////////////////////////////////////////////////////////////

// @brief 範囲の取得
const AstRange*
CptIOHV::range() const
{
  return mRange;
}


//////////////////////////////////////////////////////////////////////
// IO宣言要素を表すクラス
//////////////////////////////////////////////////////////////////////

// @brief ファイル位置の取得
FileRegion
CptIOItem::file_region() const
{
  return mLoc;
}

// @brief 名前の取得
const char*
CptIOItem::name() const
{
  return mName;
}

// @brief 初期値の取得
const AstExpr*
CptIOItem::init_value() const
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// 初期値をもった IO宣言要素の基底クラス
//////////////////////////////////////////////////////////////////////

// @brief ファイル位置の取得
FileRegion
CptIOItemI::file_region() const
{
  return FileRegion(CptIOItem::file_region(), mInitValue->file_region());
}

// @brief 初期値の取得
const AstExpr*
CptIOItemI::init_value() const
{
  return mInitValue;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// @brief 1ビットの IO 宣言のヘッダの生成
PtIOHead*
PtFactory::new_IOHead(
  const FileRegion& file_region,
  VpiDir dir,
  PtIOItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptIOH));
  return new (p) CptIOH(file_region, dir,
			VpiAuxType::None,
			VpiNetType::None,
			VpiVarType::None,
			item_top);
}

// @brief 範囲付きの IO 宣言のヘッダの生成
PtIOHead*
PtFactory::new_IOHead(
  const FileRegion& file_region,
  VpiDir dir,
  bool sign,
  const AstRange* range,
  PtIOItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptIOHV));
  return new (p) CptIOHV(file_region, dir, VpiAuxType::None,
			 VpiNetType::None, sign, range,
			 item_top);
}

// @brief 1ビットの IO 宣言のヘッダの生成 (reg 型)
PtIOHead*
PtFactory::new_RegIOHead(
  const FileRegion& file_region,
  VpiDir dir,
  PtIOItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptIOH));
  return new (p) CptIOH(file_region, dir, VpiAuxType::Reg,
			VpiNetType::None, VpiVarType::None,
			item_top);
}

// @brief 範囲付きの IO 宣言のヘッダの生成 (reg 型)
PtIOHead*
PtFactory::new_RegIOHead(
  const FileRegion& file_region,
  VpiDir dir,
  bool sign,
  const AstRange* range,
  PtIOItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptIOHV));
  return new (p) CptIOHV(file_region, dir, VpiAuxType::Reg,
			 VpiNetType::None, sign, range,
			 item_top);
}

// @brief 1ビットの IO 宣言のヘッダの生成 (ネット型)
PtIOHead*
PtFactory::new_NetIOHead(
  const FileRegion& file_region,
  VpiDir dir,
  VpiNetType net_type,
  PtIOItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptIOH));
  return new (p) CptIOH(file_region, dir, VpiAuxType::Net,
			net_type, VpiVarType::None,
			item_top);
}

// @brief 範囲付きの IO 宣言のヘッダの生成 (ネット型)
PtIOHead*
PtFactory::new_NetIOHead(
  const FileRegion& file_region,
  VpiDir dir,
  VpiNetType net_type,
  bool sign,
  const AstRange* range,
  PtIOItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptIOHV));
  return new (p) CptIOHV(file_region, dir, VpiAuxType::Net,
			 net_type, sign, range,
			 item_top);
}

// @brief IO 宣言のヘッダの生成 (変数型)
PtIOHead*
PtFactory::new_VarIOHead(
  const FileRegion& file_region,
  VpiDir dir,
  VpiVarType var_type,
  PtIOItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptIOH));
  return new (p) CptIOH(file_region, dir, VpiAuxType::Var,
			VpiNetType::None, var_type,
			item_top);
}

// @brief 初期値付き IO 宣言の要素の生成
PtIOItem*
PtFactory::new_IOItem(
  const FileRegion& file_region,
  const char* name
)
{
  void* p = mAlloc.get_memory(sizeof(CptIOItem));
  return new (p) CptIOItem(file_region, name);
}

// @brief 初期値付き IO 宣言の要素の生成
PtIOItem*
PtFactory::new_IOItem(
  const FileRegion& file_region,
  const char* name,
  const AstExpr* init_value
)
{
  void* p = mAlloc.get_memory(sizeof(CptIOItemI));
  return new (p) CptIOItemI(file_region, name, init_value);
}

END_NAMESPACE_YM_VERILOG
