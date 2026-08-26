
/// @file CptCondStmt.cc
/// @brief CptCondStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptCondStmt.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG


//////////////////////////////////////////////////////////////////////
// クラス CptIf
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptIf::type() const
{
  return If;
}

// 条件式を返す．
const AstExpr*
CptIf::expr() const
{
  return mCond;
}

// 成り立ったとき実行されるステートメント
const AstStmt*
CptIf::body() const
{
  return mThen;
}


//////////////////////////////////////////////////////////////////////
// クラス CptIfElse
//////////////////////////////////////////////////////////////////////

// 成り立たなかったとき実行されるステートメント
const AstStmt*
CptIfElse::else_body() const
{
  return mElse;
}


//////////////////////////////////////////////////////////////////////
// クラス CptCase
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptCase::type() const
{
  return Case;
}

// 比較される式を返す．
const AstExpr*
CptCase::expr() const
{
  return mExpr;
}

// @brief case item のリストの取得
AstCaseItemList
CptCase::caseitem_list() const
{
  return AstCaseItemList(mCaseItemTop);
}


//////////////////////////////////////////////////////////////////////
// クラス CptCaseX
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptCaseX::type() const
{
  return CaseX;
}


//////////////////////////////////////////////////////////////////////
// クラス CptCaseZ
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptCaseZ::type() const
{
  return CaseZ;
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
AstExprList
CptCaseItem::label_list() const
{
  return AstExprList(mLabelTop);
}

// 本体のステートメント得る．
const AstStmt*
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
  const AstExpr* expr,
  const AstStmt* then_body
)
{
  void* p = mAlloc.get_memory(sizeof(CptIf));
  return new (p) CptIf(file_region, expr, then_body);
}

// if-else 文を生成する．
PtStmt*
PtFactory::new_IfElse(
  const FileRegion& file_region,
  const AstExpr* expr,
  const AstStmt* then_body,
  const AstStmt* else_body
)
{
  void* p = mAlloc.get_memory(sizeof(CptIfElse));
  return new (p) CptIfElse(file_region, expr,
			   then_body, else_body);
}

// case 文を生成する．
PtStmt*
PtFactory::new_Case(
  const FileRegion& file_region,
  const AstExpr* expr,
  PtCaseItem* caseitem_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptCase));
  return new (p) CptCase(file_region, expr, caseitem_top);
}

// casex 文を生成する．
PtStmt*
PtFactory::new_CaseX(
  const FileRegion& file_region,
  const AstExpr* expr,
  PtCaseItem* caseitem_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptCaseX));
  return new (p) CptCaseX(file_region, expr, caseitem_top);
}

// casez 文を生成する．
PtStmt*
PtFactory::new_CaseZ(
  const FileRegion& file_region,
  const AstExpr* expr,
  PtCaseItem* caseitem_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptCaseZ));
  return new (p) CptCaseZ(file_region, expr, caseitem_top);
}

// case item を生成する．
PtCaseItem*
PtFactory::new_CaseItem(
  const FileRegion& file_region,
  PtExpr* label_top,
  const AstStmt* body
)
{
  void* p = mAlloc.get_memory(sizeof(CptCaseItem));
  return new (p) CptCaseItem(file_region, label_top, body);
}

END_NAMESPACE_YM_VERILOG
