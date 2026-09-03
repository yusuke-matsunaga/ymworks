
/// @file EiGenBlockScope.cc
/// @brief EiGenBlockScope の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiGenBlockScope.h"
#include "ym/vl/AstItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief generate block を生成する．
const VlScope*
EiFactory::new_GenBlock(
  const VlScope* parent,
  const AstItem& ast_item
)
{
  return new EiGenBlockScope(parent, ast_item);
}

// @brief generate for block を生成する．
const VlScope*
EiFactory::new_GfBlock(
  const VlScope* parent,
  const AstItem& ast_item,
  int gvi
)
{
  return new EiGfBlockScope(parent, ast_item, gvi);
}


//////////////////////////////////////////////////////////////////////
// クラス EiGenBlockScope
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiGenBlockScope::EiGenBlockScope(
  const VlScope* parent,
  const AstItem& ast_item
) : EiScope(parent),
    mAstItem{ast_item}
{
}

// @brief デストラクタ
EiGenBlockScope::~EiGenBlockScope()
{
}

// @brief ファイル位置の取得
FileRegion
EiGenBlockScope::file_region() const
{
  return mAstItem.file_region();
}

// @brief 名前の取得
std::string
EiGenBlockScope::name() const
{
  return mAstItem.name();
}


//////////////////////////////////////////////////////////////////////
// クラス EiGfBlockScope
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiGfBlockScope::EiGfBlockScope(
  const VlScope* parent,
  const AstItem& ast_item,
  int index
) : EiGenBlockScope(parent, ast_item),
    mIndex{index}
{
}

// @brief デストラクタ
EiGfBlockScope::~EiGfBlockScope()
{
}

// @brief 名前の取得
std::string
EiGfBlockScope::name() const
{
  std::ostringstream buf;
  buf << EiGenBlockScope::name() << "[" << mIndex << "]";
  return buf.str();
}

END_NAMESPACE_YM_VERILOG
