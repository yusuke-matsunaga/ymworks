#ifndef FUNCSCANNER_H
#define FUNCSCANNER_H

/// @file FuncScanner.h
/// @brief FuncScanner のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/dotlib_nsdef.h"
#include "dotlib/Token.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class FuncScanner FuncScanner.h "FuncScanner.h"
/// @brief FuncHandler 用の字句解析器
//////////////////////////////////////////////////////////////////////
class FuncScanner
{
public:

  /// @brief コンストラクタ
  FuncScanner(
    const std::string& str, ///< [in] 文字列
    const FileRegion& loc   ///< [in] str のファイル上の位置
  );

  /// @brief デストラクタ
  ~FuncScanner() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief トークンを読み込む．
  Token
  peek_token();

  /// @brief 読み込んだトークンを確定する．
  void
  accept_token();

  /// @brief トークンを読み込み確定する．
  Token
  read_token();

  /// @brief 直前の read_token() で読んだトークンの文字列を返す．
  const char*
  cur_string();

  /// @brief 直前の read_token() で読んだトークンの整数値を返す．
  int
  cur_int();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 一文字読み出す．
  int
  get();

  /// @brief 一文字先読みする．
  int
  peek();

  /// @brief 先読みを確定する．
  void
  accept();

  /// @brief mCurString に文字を追加する．
  void
  put_char(
    int c
  )
  {
    mCurString += static_cast<char>(c);
  }

  /// @brief トークンの先頭のファイル位置を設定する．
  void
  set_first_loc();

  /// @brief 現在のファイル位置を返す．
  FileRegion
  cur_loc();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パーズ中の文字列
  std::string mSrcString;

  // mString のファイル位置
  FileRegion mSrcLoc;

  // 現在の読み出し位置
  int mCurPos;

  // 現在のトークン文字列
  std::string mCurString;

  // 最初のコラム位置
  int mFirstColumn;

  // 現在のトークン
  Token mCurToken;

};

END_NAMESPACE_YM_DOTLIB

#endif // FUNCSCANNER_H
