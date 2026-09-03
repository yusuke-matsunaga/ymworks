
/// @file EiStmtBlockScope.cc
/// @brief EiStmtBlockScope の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiStmtBlockScope.h"
#include "ym/vl/AstStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief statement block を生成する．
const VlScope*
EiFactory::new_StmtBlockScope(
  const VlScope* parent,
  const AstStmt& ast_stmt
)
{
  auto scope = new EiStmtBlockScope(parent, ast_stmt);
  return scope;
}

//////////////////////////////////////////////////////////////////////
// クラス EiStmtBlockScope
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiStmtBlockScope::EiStmtBlockScope(
  const VlScope* parent,
  const AstStmt& ast_stmt
) : EiScope{parent},
    mAstStmt{ast_stmt}
{
}

// @brief デストラクタ
EiStmtBlockScope::~EiStmtBlockScope()
{
}

// @brief ファイル位置の取得
FileRegion
EiStmtBlockScope::file_region() const
{
  return mAstStmt.file_region();
}

// @brief 名前の取得
std::string
EiStmtBlockScope::name() const
{
  return mAstStmt.name();
}

END_NAMESPACE_YM_VERILOG
