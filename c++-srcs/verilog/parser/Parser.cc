
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
#include "parser/PtCaseItem.h"
#include "parser/PtModule.h"
#include "parser/PtUdp.h"


const int debug = 0;
#define dout cout


BEGIN_NAMESPACE_YM_VERILOG

#include "verilog_grammer.hh"

//////////////////////////////////////////////////////////////////////
// Verilog-HDL のパーサークラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Parser::Parser(
  AstMgr& ast_mgr,
  LogMgr& log_mgr
) : mAlloc{ast_mgr.alloc()},
    mAstMgr{ast_mgr},
    mLogMgr{log_mgr},
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
    mLogMgr.put_failure(__FILE__, __LINE__,
			FileRegion(),
			"PARS_OPEN",
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
  const PtStmt* stmt
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
    for ( auto item: PtList<const PtCaseItem>::new_obj(stmt->caseitem_top()) ) {
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
    for ( auto stmt1: PtList<const PtStmt>::new_obj(stmt->stmt_top()) ) {
      if ( !check_function_statement(stmt1) ) {
	return false;
      }
    }
    return true;

  default:
    break;
  }

  std::ostringstream buf;
  buf << AstStmt(stmt).stmt_name()
      << " cannot be used in function declaration.";
  mLogMgr.put_error(__FILE__, __LINE__,
		    stmt->file_region(),
		    "PARS_ILLEGAL_STMT_IN_CF",
		    buf.str());
  return false;
}

// default ラベルが2つ以上含まれていないかどうかのチェック
bool
Parser::check_default_label(
  const PtCaseItem* caseitem_top
)
{
  SizeType n = 0;
  for ( auto ci: PtList<const PtCaseItem>::new_obj(caseitem_top) ) {
    if ( ci->label_top() == nullptr ) {
      ++ n;
      if ( n > 1 ) {
	mLogMgr.put_error(__FILE__, __LINE__,
			  ci->file_region(),
			  "PARS_DUP_DEFAULT_LABEL",
			  "More than one 'default' label.");
	return false;
      }
    }
  }
  return true;
}

// @brief GenFor 文のチェックを行う．
bool
Parser::check_GenFor(
  const FileRegion& fr,
  const char* loop_var,
  const char* next_var
)
{
  if ( strcmp(loop_var, next_var) == 0 ) {
    return true;
  }

  std::ostringstream buf;
  buf << "Lhs of the increment statement ("
      << next_var
      << ") does not match with Lhs of the initial statement ("
      << loop_var
      << ")";
  mLogMgr.put_error(__FILE__, __LINE__,
		    fr,
		    "PARS_GENFOR_VAR_MISMATCH",
		    buf.str());
  return false;
}

// @brief パーサーのエラー
void
Parser::put_error(
  const char* file,
  int line,
  const FileRegion& loc,
  const char* message
)
{
  std::string message2;
  // 好みの問題だけど "parse error" よりは "syntax error" の方が好き．
  if ( !strncmp(message, "parse error", 11) ) {
    message2 ="syntax error";
    message2 += (message + 11);
  }
  else {
    message2 = message;
  }

  mLogMgr.put_error(file, line, loc,
		    "PARS_SYMTAX_ERROR",
		    message2);
}

END_NAMESPACE_YM_VERILOG
