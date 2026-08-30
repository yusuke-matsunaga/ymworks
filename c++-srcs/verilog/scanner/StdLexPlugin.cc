
/// @file StdLexPlugin.cc
/// @brief StdLexPlugin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "StdLexPlugin.h"
#include "InputMgr.h"

#include "scanner/RawLex.h"
#include "common.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// @class LpInclude
// @ingroup VlParser
// @brief `include 用プラグイン
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LpInclude::LpInclude(
  RawLex& lex,
  const std::string& name,
  InputMgr* input_mgr
) : LexPlugin(lex, name),
    mInputMgr(input_mgr)
{
}

// @brief デストラクタ
LpInclude::~LpInclude()
{
}

// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
bool
LpInclude::parse()
{
  // 次の非空白文字が STRING でなければならない．
  if ( !expect(STRING) ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "`include should be followed by a quoted string.");
    return false;
  }

  auto fileloc = cur_token_loc();
  auto filename = cur_string();

  // filename が存在するか調べる．
  auto realpath = mInputMgr->search_file(filename);
  if ( realpath.empty() ) {
    std::ostringstream buf;
    buf << filename << " : No such file.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    fileloc,
		    MsgType::Error,
		    "LEX",
		    buf.str());
    return false;
  }

  // さらにその次は NL でなければならない．
  if ( !expect_nl() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "expectiong new-line after filename.");
    return false;
  }

  auto realpath_string = std::string(realpath);
  const char* realpathp = realpath_string.c_str();
  if ( mInputMgr->check_file(realpathp) ) {
    std::ostringstream buf;
    buf << filename << " includes itself.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    buf.str());
    return false;
  }

  if ( debug() ) {
    std::ostringstream buf;
    buf << "including " << filename << ".";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    fileloc,
		    MsgType::Debug,
		    "LEX",
		    buf.str());
  }

  if ( !mInputMgr->open_file(realpathp, fileloc.start_loc()) ) {
    std::ostringstream buf;
    buf << filename << " : Could not open.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    fileloc,
		    MsgType::Failure,
		    "LEX",
		    buf.str());
    return false;
  }

  return true;
}


//////////////////////////////////////////////////////////////////////
// @class LpLine
// @ingroup VlParser
// @brief `line 用プラグイン
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LpLine::LpLine(
  RawLex& lex,
  const std::string& name,
  InputMgr* input_mgr
) : LexPlugin(lex, name),
    mInputMgr(input_mgr)
{
}

// @brief デストラクタ
LpLine::~LpLine()
{
}

// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
bool
LpLine::parse()
{
  if ( !expect(UNUM_INT) ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "expecting a number after `line.");
    return false;
  }

  auto line_loc = cur_token_loc();

  // 次の行をこの行数に設定する．
  int new_line = std::stoi(cur_string());

  if ( !expect(STRING) ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "expecting a file-name after a line-number.");
    return false;
  }

  auto new_file = cur_string();

  int level = 0;
  for ( ; ; ) {
    if ( expect(UNUM_INT) ) {
      level = std::stoi(cur_string());
      if ( level >= 0 && level <= 2 ) {
	break;
      }
    }
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "expecting a level number(0, 1, or 2).");
    return false;
  }

  if ( !expect_nl() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "expecting new-line.");
    return false;
  }

  // ファイルの情報を書き換える．
  mInputMgr->set_file_loc(new_file.c_str(), new_line, level);

  if ( debug() ) {
    std::ostringstream buf;
    buf << "location change: file - " << new_file
	<< " : line - " << new_line
	<< " : level - " << level;
    MsgMgr::put_msg(__FILE__, __LINE__,
		    line_loc,
		    MsgType::Debug,
		    "LEX",
		    buf.str());
  }

  return true;
}


//////////////////////////////////////////////////////////////////////
// @class LpResetAll
// @ingroup VlParser
// @brief `resetall 用プラグイン
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LpResetAll::LpResetAll(
  RawLex& lex,
  const std::string& name
) : LexPlugin(lex, name)
{
}

// @brief デストラクタ
LpResetAll::~LpResetAll()
{
}

// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
bool
LpResetAll::parse()
{
  auto loc = cur_token_loc();

  if ( !expect_nl() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "expecting new-line after `resetall.");
    return false;
  }

  if ( debug() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    loc,
		    MsgType::Debug,
		    "LEX",
		    "resetall");
  }

  resetall(loc);

  return true;
}

END_NAMESPACE_YM_VERILOG
