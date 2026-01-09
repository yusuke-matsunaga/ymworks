#ifndef FUNCPARSER_H
#define FUNCPARSER_H

/// @file FuncParser.h
/// @brief FuncParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/dotlib_nsdef.h"
#include "FuncScanner.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class FuncParser FuncParser.h "FuncParser.h"
/// @brief function 属性の値をパーズするクラス
///
/// やっていることは ExprHandler と似ているが，こっちはもとのデータが
/// 文字列なので一旦 FuncScanner に設定し直している．
//////////////////////////////////////////////////////////////////////
class FuncParser
{
public:

  /// @brief コンストラクタ
  FuncParser(
    const std::string& str, ///< [in] 対象の文字列
    const FileRegion& loc   ///< [in] ファイル上の位置
  );

  /// @brief デストラクタ
  ~FuncParser() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理式を読み込む．
  /// @return 読み込んだ式を表す値を返す．
  AstValuePtr
  operator()();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief primary を読み込む．
  AstExprPtr
  read_primary();

  /// @brief プライム付きの primary を読み込む．
  AstExprPtr
  read_primary2();

  /// @brief product を読み込む．
  AstExprPtr
  read_product();

  /// @brief expression を読み込む．
  AstExprPtr
  read_expr(
    TokenType end_marker
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 字句解析器
  FuncScanner mScanner;

};

END_NAMESPACE_YM_DOTLIB

#endif // FUNCPARSER_H
