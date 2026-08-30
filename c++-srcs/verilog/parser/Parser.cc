
/// @file Parser.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "parser/Parser.h"
#include "scanner/Lex.h"
#include "parser/PtFactory.h"
#include "parser/AstMgr.h"
#include "parser/PtModule.h"
#include "parser/PtUdp.h"


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
  const AstCaseItemList& caseitem_list
)
{
  SizeType n = 0;
  for ( auto ci: caseitem_list ) {
    if ( ci->label_list().size() == 0 ) {
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

END_NAMESPACE_YM_VERILOG
