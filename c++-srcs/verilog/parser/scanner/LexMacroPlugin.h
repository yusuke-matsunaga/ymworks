#ifndef LEXMACROPLUGIN_H
#define LEXMACROPLUGIN_H

/// @file LexMacroPlugin.h
/// @brief 標準のプラグインのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.


#include "LexPlugin.h"
#include "TokenList.h"

/// 仕様書覚書
///
/// 19.3 `define and `undef
///   - マクロ置換は `resetall の影響を受けない
/// 19.3.1 `define
///   - `define はモジュール定義の外側でも内側でも使える．
///   - 定義されたマクロ名の先頭に '`' をつけたものが置換の対象となる．
///   - コンパイラディレクティブは「定義済みマクロ」とみなすことができる．
///   - コンパイラディレクティブと同名のマクロを定義することはできない
///   - 一般のマクロを多重定義することはできる．最後の定義が使用される．
///   - あるマクロが他のマクロを参照することは可能．その場合，参照される
///     マクロは参照時に置換される．定義時ではない．
///   - 自分自身への参照を含むマクロを定義することはできない．
/// 19.3.2 `undef
///   - 定義されていないマクロを `undef すると警告となる．


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class LpDefine
/// @ingroup VlParser
/// @brief `define 用プラグイン
//////////////////////////////////////////////////////////////////////
class LpDefine :
  public LexPlugin
{
public:

  /// @brief コンストラクタ
  LpDefine(
    RawLex& lex,     ///< [in] 親の Lex オブジェクト
    const char* name ///< [in] マクロ名
  );

  /// @brief デストラクタ
  ~LpDefine();


public:

  /// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
  /// @return エラーが起きたら false を返す．
  bool
  parse() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マクロ名
  std::string mName;

};


//////////////////////////////////////////////////////////////////////
/// @class LpUndef
/// @ingroup VlParser
/// @brief `undef 用プラグイン
//////////////////////////////////////////////////////////////////////
class LpUndef :
  public LexPlugin
{
public:

  /// @brief コンストラクタ
  LpUndef(
    RawLex& lex,     ///< [in] 親の Lex オブジェクト
    const char* name ///< [in] compiler directive 名
  );

  /// @brief デストラクタ
  ~LpUndef();


public:

  /// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
  /// @return エラーが起きたら false を返す．
  bool
  parse() override;

};


//////////////////////////////////////////////////////////////////////
/// @class LpMacro
/// @ingroup VlParser
/// @brief マクロ置換用プラグイン
//////////////////////////////////////////////////////////////////////
class LpMacro :
  public LexPlugin
{
  friend class LpDefine;

public:

  /// @brief コンストラクタ
  LpMacro(
    RawLex& lex,      ///< [in] 親の Lex オブジェクト
    const char* name, ///< [in] compiler directive 名
    int num_param     ///< [in] パラメータ数
  );

  /// @brief デストラクタ
  ~LpMacro();


public:

  /// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
  /// @return エラーが起きたら false を返す．
  bool
  parse() override;

  /// @brief マクロの時 true を返す仮想関数
  bool
  is_macro() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マクロ名
  std::string mName;

  // パラメータ数
  int mNumParam;

  // 置き換えテンプレート
  TokenList mTokenList;

};

END_NAMESPACE_YM_VERILOG

#endif // LEXMACROPLUGIN_H
