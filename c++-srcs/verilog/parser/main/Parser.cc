
/// @file Parser.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "parser/Parser.h"
#include "scanner/Lex.h"
#include "parser/PtFactory.h"
#include "parser/PtArray.h"
#include "parser/AstMgr.h"
#include "parser/PtHierName.h"
#include "parser/PtModule.h"
#include "parser/PtUdp.h"
#include "parser/PtItem.h"
#include "parser/PtStmt.h"
#include "parser/PtMisc.h"
#include "ym/MsgMgr.h"


const int debug = 0;
#define dout cout


BEGIN_NAMESPACE_YM_VERILOG

#include "verilog_grammer.hh"

// メモリリークのチェックを行うとき 1 にする．
const int check_memory_leak = 0;

//////////////////////////////////////////////////////////////////////
// Verilog-HDL のパーサークラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Parser::Parser(
  AstMgr& astmgr
) : mAlloc{astmgr.alloc()},
    mAstMgr{astmgr},
    mFactory(mAlloc),
    mLex{new Lex}
{
}

// @brief デストラクタ
Parser::~Parser()
{
  // mLex は unique_ptr のデストラクタで破壊される．
}

// @brief ファイルを読み込む．
bool
Parser::read_file(
  const std::string& filename,
  const PathList& searchpath,
  const std::vector<VlLineWatcher*>& watcher_list
)
{
  // YACC の生成するパーズ関数
  extern int yyparse(Parser&);

  lex().set_searchpath(searchpath);

  for ( auto watcher: watcher_list ) {
    lex().reg_watcher(watcher);
  }

  if ( !lex().open_file(filename) ) {
    std::ostringstream buf;
    buf << filename << " : No such file.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    FileRegion(),
		    MsgType::Failure,
		    "VLPARSER",
		    buf.str());
    return false;
  }

  int stat = yyparse(*this);

  return (stat == 0);
}

// @brief yylex とのインターフェイス
int
Parser::yylex(
  YYSTYPE& lval,
  FileRegion& lloc
)
{
  int id = lex().get_token();

  switch ( id ) {
  case IDENTIFIER:
  case SYS_IDENTIFIER:
  case STRING:
  case UNUMBER:
  case UNUM_BIG:
    // 文字列領域は PtMgr が管理する．
    lval.strtype = mAstMgr.save_string(lex().cur_string());
    break;

  case UNUM_INT:
    lval.uinttype = lex().cur_uint();
    break;

  case RNUMBER:
    lval.dbltype = lex().cur_rnumber();
    break;

  default:
    break;
  }
  lloc = lex().cur_token_loc();
  return id;
}

// @brief 使用されているモジュール名を登録する．
void
Parser::reg_defname(
  const char* name
)
{
  mAstMgr.reg_defname(name);
}

// @brief attribute instance を登録する．
void
Parser::reg_attrinst(
  const AstBase* obj,
  PtAttrInstList* attr_list,
  bool def
)
{
  mAstMgr.reg_attrinst(obj, attr_list, def);
}

// 関数内で使えるステートメントかどうかのチェック
bool
Parser::check_function_statement(
  const AstStmt* stmt
)
{
  switch ( stmt->type() ) {
  case AstStmt::Disable:
  case AstStmt::SysEnable:
  case AstStmt::Null:
    return true;

  case AstStmt::Assign:
    if ( stmt->control() == nullptr) return true;
    break;

  case AstStmt::If:
    if ( stmt->body() ) {
      if ( !check_function_statement(stmt->body()) ) {
	return false;
      }
    }
    if ( stmt->else_body() ) {
      if ( !check_function_statement(stmt->else_body()) ) {
	return false;
      }
    }
    return true;

  case AstStmt::Case:
  case AstStmt::CaseX:
  case AstStmt::CaseZ:
    for ( auto item: stmt->caseitem_list() ) {
      if ( !check_function_statement(item->body()) ) {
	return false;
      }
    }
    return true;

  case AstStmt::Forever:
  case AstStmt::Repeat:
  case AstStmt::While:
  case AstStmt::For:
    if ( stmt->init_stmt() ) {
      if ( !check_function_statement(stmt->init_stmt()) ) {
	return false;
      }
    }
    if ( stmt->next_stmt() ) {
      if ( !check_function_statement(stmt->next_stmt()) ) {
	return false;
      }
    }
    return check_function_statement(stmt->body());

  case AstStmt::SeqBlock:
  case AstStmt::NamedSeqBlock:
    for ( auto stmt1: stmt->stmt_list() ) {
      if ( !check_function_statement(stmt1) ) {
	return false;
      }
    }
    return true;

  default:
    break;
  }
  std::ostringstream buf;
  buf << stmt->stmt_name()
      << " cannot be used in function declaration.";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  stmt->file_region(),
		  MsgType::Error,
		  "PARS",
		  buf.str());
  return false;
}

// default ラベルが2つ以上含まれていないかどうかのチェック
bool
Parser::check_default_label(
  const PtCaseItemList* ci_list
)
{
  SizeType n = 0;
  for ( auto ci: *ci_list ) {
    if ( ci->label_num() == 0 ) {
      ++ n;
      if ( n > 1 ) {
	MsgMgr::put_msg(__FILE__, __LINE__,
			ci->file_region(),
			MsgType::Error,
			"PARS",
			" more than one 'default' labels.");
	return false;
      }
    }
  }
  return true;
}

// @brief 階層名の生成
PtHierName*
Parser::new_HierName(
  const char* head_name,
  const char* name
)
{
  auto nb = mFactory.new_NameBranch(head_name);
  return new_HierName(nb, name);
}

// @brief 階層名の生成
PtHierName*
Parser::new_HierName(
  const char* head_name,
  int index,
  const char* name
)
{
  auto nb = mFactory.new_NameBranch(head_name, index);
  return new_HierName(nb, name);
}

// @brief 階層名の生成
PtHierName*
Parser::new_HierName(
  const AstNameBranch* nb,
  const char* name
)
{
  return mFactory.new_HierName(nb, name);
}

// @brief 階層名の追加
void
Parser::add_HierName(
  PtHierName* hname,
  const char* name
)
{
  auto nb = mFactory.new_NameBranch(hname->tail_name());
  hname->add(nb, name);
}

// @brief 階層名の追加
void
Parser::add_HierName(
  PtHierName* hname,
  int index,
  const char* name
)
{
  auto nb = mFactory.new_NameBranch(hname->tail_name(), index);
  hname->add(nb, name);
}

// @brief parameter port 宣言ヘッダを追加する．
void
Parser::add_paramport_head(
  PtDeclHead* head,
  PtAttrInstList* attr_list
)
{
  if ( head ) {
    reg_attrinst(head, attr_list);
    mParamPortHeadList.push_back(head);
  }
}

// @brief parameter port 宣言の終わり
void
Parser::flush_paramport()
{
  if ( !mDeclItemList.empty() ) {
    ASSERT_COND( !mParamPortHeadList.empty() );
    auto last = mParamPortHeadList.back();
    last->set_elem(PtDeclItemArray(mAlloc, mDeclItemList));
    mDeclItemList.clear();
  }
}

// @brief IOポート宣言リストにIO宣言ヘッダを追加する．
void
Parser::add_ioport_head(
  PtIOHead* head,
  PtAttrInstList* attr_list
)
{
  if ( head ) {
    reg_attrinst(head, attr_list);
    mCurIOHeadList->push_back(head);
  }
}

// @brief IO宣言の終わり
void
Parser::flush_io()
{
  if ( !mIOItemList.empty() ) {
    ASSERT_COND( !mCurIOHeadList->empty() );
    auto last = mCurIOHeadList->back();
    last->set_elem(PtIOItemArray(mAlloc, mIOItemList));
    mIOItemList.clear();
  }
}

// @brief IO宣言リストにIO宣言ヘッダを追加する．
void
Parser::add_io_head(
  PtIOHead* head,
  PtAttrInstList* attr_list
)
{
  add_ioport_head(head, attr_list);
  flush_io();
}

// @brief 宣言リストに宣言ヘッダを追加する．
void
Parser::add_decl_head(
  PtDeclHead* head,
  PtAttrInstList* attr_list
)
{
  if ( head ) {
    reg_attrinst(head, attr_list);
    cur_declhead_list().push_back(head);
    if ( !mDeclItemList.empty() ) {
      head->set_elem(PtDeclItemArray(mAlloc, mDeclItemList));
    }
  }
  mDeclItemList.clear();
}

// @brief item リストに要素を追加する．
void
Parser::add_item(
  PtItem* item,
  PtAttrInstList* attr_list
)
{
  if ( item ) {
    reg_attrinst(item, attr_list);
    cur_item_list().push_back(item);
  }
}

// @brief block-statment の開始
void
Parser::init_block()
{
  push_declhead_list();
}

// @brief block-statement の終了
void
Parser::end_block()
{
  mCurDeclArray = pop_declhead_list();
}

// @brief AttrInst のリストを作る．
PtAttrInstList*
Parser::new_attrinst_list()
{
  return new PtAttrInstList;
}

// @brief AttrSpec のリストを作る．
PtAttrSpecList*
Parser::new_attrspec_list()
{
  return new PtAttrSpecList;
}

// @brief CaseItem のリストを作る．
PtCaseItemList*
Parser::new_caseitem_list()
{
  return new PtCaseItemList;
}

// @brief Connection のリストを作る．
PtConnectionList*
Parser::new_connection_list()
{
  return new PtConnectionList;
}

// @brief Expr のリストを作る．
PtExprList*
Parser::new_expr_list()
{
  return new PtExprList;
}

// @brief GenCaseItem のリストを作る．
PtGenCaseItemList*
Parser::new_gencaseitem_list()
{
  return new PtGenCaseItemList;
}

// @brief Range のリストを作る．
PtRangeList*
Parser::new_range_list()
{
  return new PtRangeList;
}

// @brief Stmt のリストを作る．
PtStmtList*
Parser::new_stmt_list()
{
  return new PtStmtList;
}

END_NAMESPACE_YM_VERILOG
