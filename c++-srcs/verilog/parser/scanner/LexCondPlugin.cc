
/// @file LexCondPlugin.cc
/// @brief LexCondPlugin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.


#include "LexCondPlugin.h"
#include "LexCondState.h"
#include "common.h"

#include "scanner/RawLex.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// @class LexCondState
// @ingroup VlParser
// @brief 条件コンパイル関係の compiler directive 用状態を表すクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LexCondState::LexCondState(
  RawLex& lex
) : LexState(lex),
    mCurCond(true),
    mFalseNestLevel(0),
    mTrueNestLevel(0)
{
}

// @brief デストラクタ
LexCondState::~LexCondState()
{
}

// @brief 条件が成り立っているかを示す．
bool
LexCondState::cond() const
{
  return mCurCond;
}

// @brief 状態を初期化する．
void
LexCondState::resetall(
  const FileRegion& file_region
)
{
  mCurCond = true;
  mFalseNestLevel = 0;
  mTrueNestLevel = 0;
  mElseFlag.clear();
  mElseFlag.push_back(false);
}


//////////////////////////////////////////////////////////////////////
// @class LexCondPlugin
// @ingroup VlParser
// @brief 条件コンパイル関係のプラグインの基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LexCondPlugin::LexCondPlugin(
  RawLex& lex,
  const std::string& name,
  LexCondState* cond_state
) : LexPlugin(lex, name),
    mCondState{cond_state}
{
}

// @brief デストラクタ
LexCondPlugin::~LexCondPlugin()
{
}

// @brief 条件コンパイル用のプラグインの時 true を返す仮想関数
bool
LexCondPlugin::is_cond_plugin()
{
  return true;
}

// @brief `ifdef/`ifndef 文の現在の条件の取得
bool
LexCondPlugin::cond()
{
  return mCondState->mCurCond;
}

// @brief condition の書き換え
void
LexCondPlugin::set_cond(
  bool flag
)
{
  mCondState->mCurCond = flag;
}

// @brief condition の反転
void
LexCondPlugin::flip_cond()
{
  mCondState->mCurCond = !mCondState->mCurCond;
}

// @brief true-nest-level の取得
int
LexCondPlugin::true_nest_level()
{
  return mCondState->mTrueNestLevel;
}

// @brief true-nest-level を増やす
void
LexCondPlugin::inc_true_nest_level()
{
  ++ mCondState->mTrueNestLevel;
}

// @brief true-nest-level を減らす
void
LexCondPlugin::dec_true_nest_level()
{
  -- mCondState->mTrueNestLevel;
}

// @brief false-nest-level の取得
int
LexCondPlugin::false_nest_level()
{
  return mCondState->mFalseNestLevel;
}

// @brief false-nest-level を増やす
void
LexCondPlugin::inc_false_nest_level()
{
  ++ mCondState->mFalseNestLevel;
}

// @brief false-nest-level を減らす
void
LexCondPlugin::dec_false_nest_level()
{
  -- mCondState->mFalseNestLevel;
}

// @brief else-flag の取得
bool
LexCondPlugin::else_flag()
{
  return mCondState->mElseFlag.back();
}

// @brief else-flag の設定
void
LexCondPlugin::set_else_flag(
  bool flag
)
{
  mCondState->mElseFlag.back() = flag;
}

// @brief else-flag の退避
void
LexCondPlugin::push_else_flag(
  bool flag
)
{
  mCondState->mElseFlag.push_back(flag);
}

// @brief else-flag の復帰
void
LexCondPlugin::pop_else_flag()
{
  mCondState->mElseFlag.pop_back();
}

// @brief マクロ定義の検査
bool
LexCondPlugin::is_macro_defined(
  const std::string& name
)
{
  return lex().is_macro_defined(name);
}


//////////////////////////////////////////////////////////////////////
// @class LpIfdef
// @ingroup VlParser
// @brief `ifdef/`ifndef 用プラグイン
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LpIfdef::LpIfdef(
  RawLex& lex,
  const std::string& name,
  LexCondState* cond_state
) : LexCondPlugin(lex, name, cond_state)
{
  mInvert = ( name == "ifndef" );
}

// @brief デストラクタ
LpIfdef::~LpIfdef()
{
}

// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
bool
LpIfdef::parse()
{
  if ( !expect(IDENTIFIER) ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "expecting an identifier after `ifdef/`ifndef");
    return false;
  }

  auto macroname = cur_string();

  if ( !expect_nl() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "expecting new-line after an identifier.");
    return false;
  }

  push_else_flag(false);

  if ( cond() ) {
    // マクロが定義されていたら def = true となる．
    bool def = is_macro_defined(macroname);
    if ( mInvert ) {
      def = !def;
    }
    if ( !def ) {
      set_cond(false);
    }
    inc_true_nest_level();
  }
  else {
    // もともと条件が成り立っていなければレベル合わせだけすればよい．
    inc_false_nest_level();
  }

  return true;
}


//////////////////////////////////////////////////////////////////////
// @class LpElse
// @ingroup VlParser
// @brief `else 用プラグイン
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LpElse::LpElse(
  RawLex& lex,
  const std::string& name,
  LexCondState* cond_state
) : LexCondPlugin(lex, name, cond_state)
{
}

// @brief デストラクタ
LpElse::~LpElse()
{
}

// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
// @return エラーが起きたら false を返す．
bool
LpElse::parse()
{
  auto loc = cur_token_loc();

  if ( else_flag() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "duplicated `else.");
    return false;
  }
  set_else_flag(true);

  if ( !expect_nl() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "expecting new-line after `else.");
    return false;
  }

  if ( false_nest_level() == 0 ) {
    if ( true_nest_level() == 0 ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      loc,
		      MsgType::Error,
		      "LEX",
		      "Unbalanced `else.");
      return false;
    }
    flip_cond();
  }

  return true;
}


//////////////////////////////////////////////////////////////////////
// @class LpElsif
// @ingroup VlParser
// @brief `elseif 用プラグイン
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LpElsif::LpElsif(
  RawLex& lex,
  const std::string& name,
  LexCondState* cond_state
) : LexCondPlugin(lex, name, cond_state)
{
}

// @brief デストラクタ
LpElsif::~LpElsif()
{
}

// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
bool
LpElsif::parse()
{
  if ( else_flag() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "duplicated `elsif.");
    return false;
  }

  if ( !expect(IDENTIFIER) ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "expecting an identifier after `elsif.");
    return false;
  }

  std::string macroname = cur_string();
  auto loc = cur_token_loc();

  if ( !expect_nl() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "expecting new-line after an identifier.");
    return false;
  }

  set_else_flag(true);
  push_else_flag(false);

  if ( false_nest_level() == 0 ) {
    if ( true_nest_level() == 0 ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      loc,
		      MsgType::Error,
		      "LEX",
		      "Unbalanced `elsif.");
      return false;
    }
    if ( !cond() ) {
      set_cond( is_macro_defined(macroname.c_str()) );
    }
  }

  return true;
}


//////////////////////////////////////////////////////////////////////
// @class LpEndif
// @ingroup VlParser
// @brief `endif 用プラグイン
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LpEndif::LpEndif(
  RawLex& lex,
  const std::string& name,
  LexCondState* cond_state
) : LexCondPlugin(lex, name, cond_state)
{
}

// @brief デストラクタ
LpEndif::~LpEndif()
{
}

// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
bool
LpEndif::parse()
{
  auto loc = cur_token_loc();

  if ( !expect_nl() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "expecting new-line after `endif.");
    return false;
  }

  pop_else_flag();
  if ( false_nest_level() == 0 ) {
    if ( true_nest_level() == 0 ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      loc,
		      MsgType::Error,
		      "LEX",
		      "Unbalanced `endif.");
      return false;
    }
    set_cond(true);
    dec_true_nest_level();
  }
  else {
    dec_false_nest_level();
  }

  return true;
}

END_NAMESPACE_YM_VERILOG
