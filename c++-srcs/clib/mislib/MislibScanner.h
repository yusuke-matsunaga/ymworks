#ifndef MISLIBSCANNER_H
#define MISLIBSCANNER_H

/// @file MislexScanner.h
/// @brief MislexScanner のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "mislib_nsdef.h"
#include "MislibToken.h"

#include "ym/Scanner.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
/// @class MislibScanner MislibScanner.h "MislibScanner.h"
/// @brief Mislib 用の LEX クラス
//////////////////////////////////////////////////////////////////////
class MislibScanner :
  public Scanner
{
public:

  /// @brief コンストラクタ
  MislibScanner(
    std::istream& s,          ///< [in] 入力ストリーム
    const FileInfo& file_info ///< [in] ファイル情報
  );

  /// @brief デストラクタ
  ~MislibScanner() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief トークンを一つとってくる．
  MislibToken
  read_token();

  /// @brief 直前の read_token() に対応する文字列を返す．
  const char*
  cur_string() const { return mCurString.c_str(); }

  /// @brief cur_string() を double に変換したものを返す．
  double
  cur_float() const { return strtod(cur_string(), static_cast<char**>(nullptr)); }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief read_token の下請け関数
  /// @return トークンの種類を返す．
  MislibToken::Type
  scan();

  /// @brief mCurString に文字を追加する．
  void
  put_char(
    int c
  )
  {
    mCurString += static_cast<char>(c);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 現在読込中の文字列を貯めておくバッファ
  std::string mCurString;

};

END_NAMESPACE_YM_MISLIB

#endif // MISLIBSCANNER_H
