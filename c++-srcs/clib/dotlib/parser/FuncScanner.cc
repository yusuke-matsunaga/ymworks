
/// @file FuncScanner.cc
/// @brief FuncScanner の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "FuncScanner.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス FuncScanner
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FuncScanner::FuncScanner(
  const std::string& str,
  const FileRegion& loc
) : mSrcString{str},
    mSrcLoc{loc},
    mCurPos{0}
{
}

// @brief トークンを読み込む．
Token
FuncScanner::peek_token()
{
  if ( mCurToken.type() != TokenType::ERROR ) {
    return mCurToken;
  }

  mCurString.clear();

  int c;

 ST_INIT: // 初期状態
  c = get();
  set_first_loc();
  if ( isalpha(c) || c == '_' ) {
    put_char(c);
    goto ST_ID;
  }

  switch ( c ) {
  case '\0':
    mCurToken = Token(TokenType::END, cur_loc());
    goto END;

  case ' ':
  case '\t':
    goto ST_INIT; // 最初の空白は読み飛ばす．

  case '!':
    mCurToken = Token(TokenType::NOT, cur_loc());
    goto END;

  case '|':
  case '+':
    mCurToken = Token(TokenType::OR, cur_loc());
    goto END;

  case '&':
  case '*':
    mCurToken = Token(TokenType::AND, cur_loc());
    goto END;

  case '^':
    mCurToken = Token(TokenType::XOR, cur_loc());
    goto END;

  case '(':
    mCurToken = Token(TokenType::LP, cur_loc());
    goto END;

  case ')':
    mCurToken = Token(TokenType::RP, cur_loc());
    goto END;

  case '\'':
    mCurToken = Token(TokenType::PRIME, cur_loc());
    goto END;

  case '0':
    mCurToken = Token(TokenType::BOOL_0, cur_loc());
    goto END;

  case '1':
    mCurToken = Token(TokenType::BOOL_1, cur_loc());
    goto END;

  default:
    // それ以外はエラー
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_loc(),
		    MsgType::Error,
		    "DOTLIB_PARSER",
		    "Syntax error");
    throw std::invalid_argument{"syntax error"};
  }

 ST_ID: // 文字列モード
  c = peek();
  if ( isalnum(c) || c == '_' ) {
    accept();
    put_char(c);
    goto ST_ID;
  }
  mCurToken = Token(TokenType::SYMBOL, cur_loc());
  goto END;

 END:
  return mCurToken;
}

// @brief 読み込んだトークンを確定する．
void
FuncScanner::accept_token()
{
  mCurToken = Token(TokenType::ERROR, FileRegion{});
}

// @brief トークンを読み込み確定する．
Token
FuncScanner::read_token()
{
  auto ans = peek_token();
  accept_token();
  return ans;
}

// @brief 直前の read_token() で読んだトークン文字列を返す．
const char*
FuncScanner::cur_string()
{
  return mCurString.c_str();
}

// @brief 直前の read_token() で読んだトークンの整数値を返す．
int
FuncScanner::cur_int()
{
  return strtol(mCurString.c_str(), nullptr, 10);
}

// @brief 一文字読み出す．
int
FuncScanner::get()
{

  int c = peek();
  accept();
  return c;
}

// @brief 一文字先読みする．
int
FuncScanner::peek()
{
  return mSrcString[mCurPos];
}

// @brief 先読みを確定する．
void
FuncScanner::accept()
{
  ++ mCurPos;
}

// @brief トークンの先頭のファイル位置を設定する．
void
FuncScanner::set_first_loc()
{
  mFirstColumn = mCurPos;
}

// @brief 現在のファイル位置を返す．
FileRegion
FuncScanner::cur_loc()
{
  return FileRegion(mSrcLoc.start_file_info(),
		    mSrcLoc.start_line(), mSrcLoc.start_column() + mFirstColumn,
		    mSrcLoc.start_line(), mSrcLoc.start_column() + mCurPos);
}

END_NAMESPACE_YM_DOTLIB
