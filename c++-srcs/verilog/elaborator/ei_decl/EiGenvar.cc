
/// @file EiGenvar.cc
/// @brief EiGenvar の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiGenvar.h"

#include "ym/vl/AstDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief genvar を生成する．
ElbGenvar*
EiFactory::new_Genvar(
  const VlScope* parent,
  const AstDeclItem* ast_item,
  int val
)
{
  return new EiGenvar(parent, ast_item, val);
}


//////////////////////////////////////////////////////////////////////
// genvar を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
EiGenvar::EiGenvar(
  const VlScope* parent,
  const AstDeclItem* ast_item,
  int val
) : mParent{parent},
    mAstItem{ast_item},
    mInUse{false},
    mValue{val}
{
}

// デストラクタ
EiGenvar::~EiGenvar()
{
}

// @brief 型の取得
VpiObjType
EiGenvar::type() const
{
  return VpiObjType::Constant;
}

// @brief ファイル位置の取得
FileRegion
EiGenvar::file_region() const
{
  return mAstItem->file_region();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlScope*
EiGenvar::parent_scope() const
{
  return mParent;
}

// @brief 名前の取得
std::string
EiGenvar::name() const
{
  return mAstItem->name();
}

// 使用中の時 true を返す．
bool
EiGenvar::is_inuse() const
{
  return mInUse;
}

// 使用中にする．
void
EiGenvar::set_inuse()
{
  mInUse = true;
}

// 使用を終える．
void
EiGenvar::reset_inuse()
{
  mInUse = false;
}

// 現在の値を返す．
int
EiGenvar::value() const
{
  return mValue;
}

// 値を設定する．
void
EiGenvar::set_value(
  int value
)
{
  mValue = value;
}

// @brief 元となったパース木の定義要素を返す．
const AstDeclItem*
EiGenvar::ast_item() const
{
  return mAstItem;
}

END_NAMESPACE_YM_VERILOG
