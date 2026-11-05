
/// @file EiCondStmt.cc
/// @brief EiCondStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbMgr.h"
#include "EiCondStmt.h"
#include "elaborator/ElbExpr.h"
#include "ym/vl/BitVector.h"
#include "ym/vl/AstStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbMgr
//////////////////////////////////////////////////////////////////////

// @brief while 文を生成する．
const VlStmt*
ElbMgr::new_WhileStmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt& ast_stmt,
  const VlExpr* cond,
  const VlStmt* body
)
{
  return new_stmt(
    [&]() -> const VlStmt* {
      return new EiWhileStmt(parent, process, ast_stmt,
			     cond, body);
    });
}

// @brief repeat 文を生成する．
const VlStmt*
ElbMgr::new_RepeatStmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt& ast_stmt,
  const VlExpr* cond,
  const VlStmt* body
)
{
  return new_stmt(
    [&]() -> const VlStmt* {
      return new EiRepeatStmt(parent, process, ast_stmt,
			      cond, body);
    });
}

// @brief wait 文を生成する．
const VlStmt*
ElbMgr::new_WaitStmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt& ast_stmt,
  const VlExpr* cond,
  const VlStmt* body
)
{
  return new_stmt(
    [&]() -> const VlStmt* {
      return new EiWaitStmt(parent, process, ast_stmt,
			      cond, body);
    });
}

// @brief for 文を生成する．
const VlStmt*
ElbMgr::new_ForStmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt& ast_stmt,
  const VlExpr* cond,
  const VlStmt* init_stmt,
  const VlStmt* inc_stmt,
  const VlStmt* body
)
{
  return new_stmt(
    [&]() -> const VlStmt* {
      return new EiForStmt(parent, process, ast_stmt,
			   cond, init_stmt, inc_stmt,
			   body);
    });
}

// @brief forever 文を生成する．
const VlStmt*
ElbMgr::new_ForeverStmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt& ast_stmt,
  const VlStmt* body
)
{
  return new_stmt(
    [&]() -> const VlStmt* {
      return new EiForeverStmt(parent, process, ast_stmt,
			       body);
    });
}

// @brief if 文を生成する．
const VlStmt*
ElbMgr::new_IfStmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt& ast_stmt,
  const VlExpr* cond,
  const VlStmt* then_stmt,
  const VlStmt* else_stmt
)
{
  return new_stmt(
    [&]() -> const VlStmt* {
      if ( else_stmt != nullptr ) {
	return new EiIfElseStmt(parent, process, ast_stmt,
				cond, then_stmt, else_stmt);
      }
      else {
	return new EiIfStmt(parent, process, ast_stmt,
			    cond, then_stmt);
      }
    });
}

// @brief case 文を生成する．
const VlStmt*
ElbMgr::new_CaseStmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt& ast_stmt,
  const VlExpr* expr,
  const std::vector<const VlCaseItem*>& caseitem_list
)
{
  return new_stmt(
    [&]() -> const VlStmt* {
      return new EiCaseStmt(parent, process, ast_stmt,
			      expr, caseitem_list);
    });
}

// @brief caseitem を生成する．
const VlCaseItem*
ElbMgr::new_CaseItem(
  const AstCaseItem& ast_item,
  const std::vector<ElbExpr*>& label_list,
  const VlStmt* body
)
{
  auto caseitem = new EiCaseItem(ast_item, label_list, body);
  reg_obj(caseitem);
  return caseitem;
}


//////////////////////////////////////////////////////////////////////
// クラス EiLoopStmt
//////////////////////////////////////////////////////////////////////

// @brief 条件式を返す．
const VlExpr*
EiLoopStmt::expr() const
{
  return mCondition;
}

// @brief 本体のステートメントを返す．
const VlStmt*
EiLoopStmt::body_stmt() const
{
  return mBodyStmt;
}


//////////////////////////////////////////////////////////////////////
// クラス EiWhileStmt
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiWhileStmt::type() const
{
  return VpiObjType::While;
}


//////////////////////////////////////////////////////////////////////
// クラス EiRepeatStmt
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiRepeatStmt::type() const
{
  return VpiObjType::Repeat;
}


//////////////////////////////////////////////////////////////////////
// クラス EiWaitStmt
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiWaitStmt::type() const
{
  return VpiObjType::Wait;
}


//////////////////////////////////////////////////////////////////////
// クラス EiForStmt
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiForStmt::type() const
{
  return VpiObjType::For;
}

// 初期化文を返す．
const VlStmt*
EiForStmt::init_stmt() const
{
  return mInitStmt;
}

// 繰り返し文を返す．
const VlStmt*
EiForStmt::inc_stmt() const
{
  return mIncStmt;
}


//////////////////////////////////////////////////////////////////////
// クラス EiForeverStmt
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiForeverStmt::type() const
{
  return VpiObjType::Forever;
}

// @brief 本体のステートメントを返す．
const VlStmt*
EiForeverStmt::body_stmt() const
{
  return mBodyStmt;
}


//////////////////////////////////////////////////////////////////////
// クラス IfStmt
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiIfStmt::type() const
{
  return VpiObjType::If;
}

// @brief 条件式を返す．
const VlExpr*
EiIfStmt::expr() const
{
  return mCondition;
}

// @brief 本体のステートメントを返す．
const VlStmt*
EiIfStmt::body_stmt() const
{
  return mBodyStmt;
}


//////////////////////////////////////////////////////////////////////
// クラス EiIfElseStmt
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiIfElseStmt::type() const
{
  return VpiObjType::IfElse;
}

// @brief else ステートメントを返す．
const VlStmt*
EiIfElseStmt::else_stmt() const
{
  return mElseStmt;
}


//////////////////////////////////////////////////////////////////////
// クラス EiCaseItem
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiCaseItem::type() const
{
  return VpiObjType::CaseItem;
}

// @brief ファイル位置を返す．
FileRegion
EiCaseItem::file_region() const
{
  return mAstCaseItem.file_region();
}

// @brief 条件式のリストの要素数を返す．
SizeType
EiCaseItem::expr_num() const
{
  return mExprList.size();
}

// @brief 条件式を返す．
const VlExpr*
EiCaseItem::expr(
  SizeType pos
) const
{
  if ( pos >= expr_num() ) {
    throw std::out_of_range{"pos is out of range"};
  }
  return mExprList[pos];
}

// @brief 条件式のリストを返す．
std::vector<const VlExpr*>
EiCaseItem::expr_list() const
{
  return std::vector<const VlExpr*>{mExprList.begin(), mExprList.end()};
}

// @brief 本体のステートメントを返す．
const VlStmt*
EiCaseItem::body_stmt() const
{
  return mBodyStmt;
}


//////////////////////////////////////////////////////////////////////
// クラス EiCase
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiCaseStmt::type() const
{
  return VpiObjType::Case;
}

// @brief case type を返す．
VpiCaseType
EiCaseStmt::case_type() const
{
  switch ( ast_stmt().type() ) {
  case AstStmt::Case:  return VpiCaseType::Exact;
  case AstStmt::CaseX: return VpiCaseType::X;
  case AstStmt::CaseZ: return VpiCaseType::Z;
  default: break;
  }
  throw std::logic_error{"Should not be reached"};
}

// @brief 条件式を返す．
const VlExpr*
EiCaseStmt::expr() const
{
  return mCondition;
}

// @brief case item のリストの要素数を返す．
SizeType
EiCaseStmt::caseitem_num() const
{
  return mCaseItemList.size();
}

// @brief case item を返す．
const VlCaseItem*
EiCaseStmt::caseitem(
  SizeType pos
) const
{
  if ( pos >= caseitem_num() ) {
    throw std::out_of_range{"pos is out of range"};
  }
  return mCaseItemList[pos];
}

// @brief case item のリストを返す．
std::vector<const VlCaseItem*>
EiCaseStmt::caseitem_list() const
{
  return mCaseItemList;
}

END_NAMESPACE_YM_VERILOG
