
/// @file LexPlugin.cc
/// @brief LexPlugin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "LexPlugin.h"
#include "scanner/RawLex.h"

#include "common.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// Lex に付随するプラグインの基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LexPlugin::LexPlugin(
  RawLex& lex,
  const std::string& name
) : mLex{lex}
{
  mLex.add_plugin(name, this);
}

// @brief デストラクタ
LexPlugin::~LexPlugin()
{
}

// @brief 条件コンパイル用のプラグインの時 true を返す仮想関数
bool
LexPlugin::is_cond_plugin()
{
  return false;
}

// @brief マクロの時 true を返す仮想関数
bool
LexPlugin::is_macro()
{
  return false;
}

// @brief 指定されたトークンを期待して待つ．
bool
LexPlugin::expect(
  int tok
)
{
  int id = 0;
  do {
    id = mLex.get_raw_token();
  }
  while ( id == SPACE || id == COMMENT1 || id == COMMENT2 );
  return id == tok;
}

// @brief 空白文字以外の次のトークンが改行なら true を返す．
bool
LexPlugin::expect_nl()
{
  int id = 0;
  do {
    id = mLex.get_raw_token();
  }
  while ( id == SPACE || id == COMMENT1 || id == COMMENT2 );
  return id == NL;
}

// @brief 空白以外の次のトークンの取得
int
LexPlugin::get_nospace_token()
{
  int id = 0;
  do {
    id = mLex.get_raw_token();
  }
  while ( id == SPACE || id == COMMENT1 || id == COMMENT2 ) ;
  return id;
}

END_NAMESPACE_YM_VERILOG
