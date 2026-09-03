
/// @file CptStmt_Cond.cc
/// @brief CptStmt_Cond の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptStmt_Cond.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptStmt_If
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptStmt_If::type() const
{
  return AstStmt::If;
}

// 条件式を返す．
const PtExpr*
CptStmt_If::expr() const
{
  return mCond;
}

// 成り立ったとき実行されるステートメント
const PtStmt*
CptStmt_If::body() const
{
  return mThen;
}

// 成り立たなかったとき実行されるステートメント
const PtStmt*
CptStmt_If::else_body() const
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_IfElse
//////////////////////////////////////////////////////////////////////

// 成り立たなかったとき実行されるステートメント
const PtStmt*
CptStmt_IfElse::else_body() const
{
  return mElse;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_Case
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptStmt_Case::type() const
{
  return AstStmt::Case;
}

// 比較される式を返す．
const PtExpr*
CptStmt_Case::expr() const
{
  return mExpr;
}

// @brief case item のリストの取得
const PtCaseItem*
CptStmt_Case::caseitem_top() const
{
  return mCaseItemTop;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_CaseX
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptStmt_CaseX::type() const
{
  return AstStmt::CaseX;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_CaseZ
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptStmt_CaseZ::type() const
{
  return AstStmt::CaseZ;
}


//////////////////////////////////////////////////////////////////////
// クラス CptCaseItem
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptCaseItem::file_region() const
{
  return mFileRegion;
}

// @brief ラベルリストの取得
const PtExpr*
CptCaseItem::label_top() const
{
  return mLabelTop;
}

// 本体のステートメント得る．
const PtStmt*
CptCaseItem::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// if 文を生成する．
PtStmt*
PtFactory::new_If(
  const FileRegion& file_region,
  const PtExpr* expr,
  const PtStmt* then_body
)
{
  void* p = mAlloc.get_memory(sizeof(CptStmt_If));
  return new (p) CptStmt_If(file_region, expr, then_body);
}

// if-else 文を生成する．
PtStmt*
PtFactory::new_IfElse(
  const FileRegion& file_region,
  const PtExpr* expr,
  const PtStmt* then_body,
  const PtStmt* else_body
)
{
  void* p = mAlloc.get_memory(sizeof(CptStmt_IfElse));
  return new (p) CptStmt_IfElse(file_region, expr,
				then_body, else_body);
}

// case 文を生成する．
PtStmt*
PtFactory::new_Case(
  const FileRegion& file_region,
  const PtExpr* expr,
  const PtCaseItem* caseitem_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptStmt_Case));
  return new (p) CptStmt_Case(file_region, expr, caseitem_top);
}

// casex 文を生成する．
PtStmt*
PtFactory::new_CaseX(
  const FileRegion& file_region,
  const PtExpr* expr,
  const PtCaseItem* caseitem_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptStmt_CaseX));
  return new (p) CptStmt_CaseX(file_region, expr, caseitem_top);
}

// casez 文を生成する．
PtStmt*
PtFactory::new_CaseZ(
  const FileRegion& file_region,
  const PtExpr* expr,
  const PtCaseItem* caseitem_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptStmt_CaseZ));
  return new (p) CptStmt_CaseZ(file_region, expr, caseitem_top);
}

// case item を生成する．
PtCaseItem*
PtFactory::new_CaseItem(
  const FileRegion& file_region,
  const PtExpr* label_top,
  const PtStmt* body
)
{
  void* p = mAlloc.get_memory(sizeof(CptCaseItem));
  return new (p) CptCaseItem(file_region, label_top, body);
}

END_NAMESPACE_YM_VERILOG
