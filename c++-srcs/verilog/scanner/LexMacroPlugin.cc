
/// @file LexMacroPlugin.cc
/// @brief LexMacroPlugin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C)  2025 Yusuke Matsunaga
/// All rights reserved.

#include "LexMacroPlugin.h"
#include "scanner/RawLex.h"
#include "MacroSource.h"
#include "TokenInfo.h"
#include "common.h"
#include "print_token.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// @class LpDefine
// @ingroup VlParser
// @brief `define 用プラグイン
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LpDefine::LpDefine(
  RawLex& lex,
  const char* name
) : LexPlugin(lex, name)
{
}

// @brief デストラクタ
LpDefine::~LpDefine()
{
}

// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
bool
LpDefine::parse()
{
  // 次の非空白文字が IDENTIFIER でなければエラー
  if ( !expect(IDENTIFIER) ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "expecting an identifier after `define.");
    return false;
  }

  if ( !lex().check_pluginname(cur_string()) ) {
    // 定義済みのコンパイラディレクティブと同名のマクロ
    std::ostringstream buf;
    buf << "Could not overwrite predefined compiler directive: "
	<< cur_string() << ".";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    buf.str());
    return false;
  }

  auto defsymbol = cur_string();
  auto macro_loc = cur_token_loc();

  // パラメータ名をキーにして位置番号を格納する連想配列
  std::unordered_map<std::string, int> param_dic;

  // ここは空白が重要なので get_raw_token() を呼ぶ．
  int id = get_raw_token();
  if ( id == SPACE || id == NL || id == COMMENT1 || id == COMMENT2 || id == EOF ) {
    // パラメータなし
  }
  else if ( id == '(' ) {
    // パラメータあり
    // パラメータを param_dic に記録
    int pos = 0;
    if ( !expect(IDENTIFIER) ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      cur_token_loc(),
		      MsgType::Error,
		      "LEX",
		      "Syntax error: "
		      "expecting an identifier after '('.");
      return false;
    }
    param_dic[cur_string()] = pos;
    ++ pos;
    for ( bool go = true; go; ) {
      int id = get_nospace_token();
      switch ( id ) {
      case ')':
	go = false;
	break;

      case ',':
	if ( !expect(IDENTIFIER) ) {
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  cur_token_loc(),
			  MsgType::Error,
			  "LEX",
			  "Syntax error: "
			  "expecting an identifier after ','.");
	  return false;
	}
	param_dic[cur_string()] = pos;
	++ pos;
	break;

      default:
	MsgMgr::put_msg(__FILE__, __LINE__,
			cur_token_loc(),
			MsgType::Error,
			"LEX",
			"Syntax error: "
			"expecting ')' or ',' after an identifier.");
	return false;
      }
    }
  }
  else {
    // それ以外はエラー
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "expecting '(' or SPACE after a macro-name.");
    return false;
  }

  // マクロをプラグインとして生成
  const char* macroname = defsymbol.c_str();
  int n = param_dic.size();
  auto macro = new LpMacro(lex(), macroname, n);

  // マクロ本体を macro に記録
  if ( id != NL && id != EOF ) {
    for (int id = get_nospace_token();
	 id != NL && id != EOF;
	 id = get_nospace_token()) {
      if ( id == IDENTIFIER && param_dic.count(cur_string()) > 0 ) {
	int pos = param_dic.at(cur_string());
	// 置き換え対象のパラメータ
	macro->mTokenList.add(pos);
      }
      else {
	// それ以外のトークンはそのまま記録
	macro->mTokenList.add(cur_token_loc(), id, cur_string());
      }
    }
  }

  if ( debug() ) {
    std::ostringstream buf;
    buf << "registering new macro \"" << macroname << "\"" << std::endl
	<< "# of parameters : " << n << std::endl;
    const char* sep = "";
    for (const TokenInfo* token = macro->mTokenList.top();
	 token; token = token->next()) {
      buf << sep;
      sep = ", ";
      int pos = token->pos();
      if ( pos >= 0 ) {
	buf << "PARAM#" << pos;
      }
      else {
	buf << *token;
      }
    }
    MsgMgr::put_msg(__FILE__, __LINE__,
		    macro_loc,
		    MsgType::Debug,
		    "LEX",
		    buf.str());
  }

  return true;
}


//////////////////////////////////////////////////////////////////////
// @class LpUndef
// @ingroup VlParser
// @brief `undef 用プラグイン
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LpUndef::LpUndef(
  RawLex& lex,
  const char* name
) : LexPlugin(lex, name)
{
}

// @brief デストラクタ
LpUndef::~LpUndef()
{
}

// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
bool
LpUndef::parse()
{
  if ( !expect(IDENTIFIER) ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "expecting an identifier after `undef.");
    return false;
  }

  auto macroname = cur_string();
  auto cur_loc = cur_token_loc();

  if ( !expect_nl() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "expecting new-line after macro-name.");
    return false;
  }

  if ( lex().erase_plugin(macroname.c_str()) ) {
    if ( debug() ) {
      std::ostringstream buf;
      buf << "forgetting a macro \"" << macroname << "\".";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      cur_loc,
		      MsgType::Debug,
		      "LEX",
		      buf.str());
    }
  }
  else {
    // warning にするんだそうだ
    std::ostringstream buf;
    buf << "macro `" << macroname << " is not defined.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_loc,
		    MsgType::Warning,
		    "LEX",
		    buf.str());
  }

  return true;
}


//////////////////////////////////////////////////////////////////////
// @class LpMacro
// @ingroup VlParser
// @brief マクロ置換用プラグイン
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LpMacro::LpMacro(
  RawLex& lex,
  const char* name,
  int num_param
) : LexPlugin(lex, name),
    mNumParam(num_param)
{
}

// @brief デストラクタ
LpMacro::~LpMacro()
{
}

// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
bool
LpMacro::parse()
{
  // self-loop のチェックを行う．
  if ( lex().check_macro(mName) ) {
    std::ostringstream buf;
    buf << "macro `" << mName << " depends on itself.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    buf.str());
    return false;
  }

  if ( debug() ) {
    std::ostringstream buf;
    buf << "macro `" << mName << " found.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Debug,
		    "LEX",
		    buf.str());
  }

  // マクロ置換用のデータを作る．
  TokenList* param_array = nullptr;
  if ( mNumParam > 0 ) {
    param_array = new TokenList[mNumParam];
    if ( !expect('(') ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      cur_token_loc(),
		      MsgType::Error,
		      "LEX",
		      "Syntax error: "
		      "'(' is expected.");
      return false;
    }
    int pos = 0;
    for (bool go = true; go && pos < mNumParam; ) {
      int id = get_nospace_token();
      switch ( id ) {
      case ')':
	go = false;
	++ pos;
	break;
      case ',':
	++ pos;
	break;
      default:
	param_array[pos].add(cur_token_loc(), id, cur_string());
	break;
      }
    }
    if ( pos != mNumParam ) {
      // パラメータの数が合わない
      std::ostringstream buf;
      buf << "# of actual parameters ("
	  << pos << ") differs from"
	  << " # of formal paramters ("
	  << mNumParam << ").";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      cur_token_loc(),
		      MsgType::Error,
		      "LEX",
		      buf.str());
      return false;
    }
  }
  lex().push_macro(mName, mTokenList.top(), param_array);

  return true;
}

// @brief マクロの時 true を返す仮想関数
bool
LpMacro::is_macro()
{
  return true;
}

END_NAMESPACE_YM_VERILOG
