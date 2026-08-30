
/// @file EiGfRoot.cc
/// @brief EiGfRoot の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiGfRoot.h"

#include "ym/vl/AstItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief GfBlock 検索用の親の名前付きオブジェクトを作る．
ElbGfRoot*
EiFactory::new_GfRoot(
  const VlScope* parent,
  const AstItem* ast_item
)
{
  return new EiGfRoot(parent, ast_item);
}


//////////////////////////////////////////////////////////////////////
// クラス EiGfRoot
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiGfRoot::EiGfRoot(
  const VlScope* parent,
  const AstItem* ast_item
) : mParent{parent},
    mAstItem{ast_item}
{
}

// @brief デストラクタ
EiGfRoot::~EiGfRoot()
{
}

// @brief 型の取得
VpiObjType
EiGfRoot::type() const
{
  return VpiObjType::Scope;
}

// @brief ファイル位置の取得
FileRegion
EiGfRoot::file_region() const
{
  return mAstItem->file_region();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlScope*
EiGfRoot::parent_scope() const
{
  return mParent;
}

// @brief 名前の取得
std::string
EiGfRoot::name() const
{
  return mAstItem->name();
}

// @brief 子供のスコープを追加する．
void
EiGfRoot::add(
  SizeType index,
  const VlScope* block
)
{
  mTable.emplace(index, block);
}

// @brief 子供のスコープを取り出す．
const VlScope*
EiGfRoot::elem_by_index(
  SizeType index
)
{
  if ( mTable.count(index) ) {
    return mTable.at(index);
  }
  else {
    return nullptr;
  }
}

END_NAMESPACE_YM_VERILOG
