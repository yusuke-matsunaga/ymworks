
/// @file MislibScanner.cc
/// @brief MislibScanner の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2019, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "MislibScanner.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_MISLIB

// コンストラクタ
MislibScanner::MislibScanner(
  std::istream& s,
  const FileInfo& file_info
) : Scanner(s, file_info)
{
}

// トークンを一つとってくる．
MislibToken
MislibScanner::read_token()
{
  MislibToken::Type type = scan();
  return {type, cur_region()};
}

// @brief read_token の下請け関数
// @return トークンを返す．
MislibToken::Type
MislibScanner::scan()
{
  int c = 0;

  mCurString.clear();

 state1:
  c = get();
  set_first_loc();
  if ( isalpha(c) || (c == '_') ) {
    put_char(c);
    goto state2;
  }
  if ( isdigit(c) || (c == '-') ) {
    put_char(c);
    goto state3;
  }
  switch (c) {
  case EOF: return MislibToken::END;
  case ' ':
  case '\t': goto state1;
  case '\n': goto state1;
  case '.': put_char(c); goto state4;
  case ';': return MislibToken::SEMI;
  case '=': return MislibToken::EQ;
  case '(': return MislibToken::LP;
  case ')': return MislibToken::RP;
  case '!': return MislibToken::NOT;
  case '*': return MislibToken::STAR;
  case '&': return MislibToken::STAR;
  case '+': return MislibToken::PLUS;
  case '|': return MislibToken::PLUS;
  case '^': return MislibToken::HAT;
  case '#': goto state6;
  case '\"': goto state7;
  }
  goto error;

  // 一文字目が[a-zA-Z_]の時
 state2:
  c = peek();
  if ( isalpha(c) || isdigit(c) || (c == '_') ) {
    accept();
    put_char(c);
    goto state2;
  }

  // 予約語（？）の検査
  // 数が少ないのでナイーブな方法をとっている．
  if ( mCurString == "GATE" ) {
    return MislibToken::GATE;
  }
  if ( mCurString == "PIN" ) {
    return MislibToken::PIN;
  }
  if ( mCurString == "NONINV" ) {
    return MislibToken::NONINV;
  }
  if ( mCurString == "INV" ) {
    return MislibToken::INV;
  }
  if ( mCurString == "UNKNOWN" ) {
    return MislibToken::UNKNOWN;
  }
  if ( mCurString == "CONST0" ) {
    return MislibToken::CONST0;
  }
  if ( mCurString == "CONST1" ) {
    return MislibToken::CONST1;
  }
  return MislibToken::STR;

  // 一文字目が[0-9]の時
 state3:
  c = peek();
  if ( isdigit(c) ) {
    accept();
    put_char(c);
    goto state3;
  }
  if ( c == '.' ) {
    accept();
    put_char(c);
    goto state5;
  }
  if ( c == 'E' || c == 'e' ) {
    accept();
    put_char(c);
    goto state8;
  }
  return MislibToken::NUM;

  // 一文字目が"."の時
 state4:
  c = get();
  if ( isdigit(c) ) {
    put_char(c);
    goto state5;
  }
  goto error;

  // [0-9]*"."を読み終えたところ
 state5:
  c = peek();
  if ( isdigit(c) ) {
    accept();
    put_char(c);
    goto state5;
  }
  if ( c == 'E' || c == 'e' ) {
    accept();
    put_char(c);
    goto state8;
  }
  return MislibToken::NUM;

  // [0-9]*"."[Ee]を読み終えたところ
 state8:
  c = peek();
  if ( c == '-' ) {
    accept();
    put_char(c);
    goto state9;
  }
  if ( isdigit(c) ) {
    accept();
    put_char(c);
    goto state9;
  }
  return MislibToken::NUM;

  // [0-9]*"."[Ee]-?を読み終えたところ
 state9:
  c = peek();
  if ( isdigit(c) ) {
    accept();
    put_char(c);
    goto state9;
  }
  return MislibToken::NUM;

  // '#'があったら改行までループする．
 state6:
  c = get();
  if ( c == '\n' ) {
    goto state1;
  }
  goto state6;

  // "があったら次の"までを強制的に文字列だと思う．
 state7:
  c = get();
  if ( c == '\"' ) {
    return MislibToken::STR;
  }
  put_char(c);
  goto state7;

 error:
  {
    std::ostringstream buf;
    buf << "Syntax error: illegal input (" << mCurString << ").";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_region(),
		    MsgType::Error,
		    "MISLIB_PARSE",
		    buf.str().c_str());
  }
  return MislibToken::ERROR;
}

END_NAMESPACE_YM_MISLIB
