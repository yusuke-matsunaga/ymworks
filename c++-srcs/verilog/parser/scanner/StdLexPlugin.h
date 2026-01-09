#ifndef STDEXPRLUGIN_H
#define STDEXPRLUGIN_H

/// @file StdLexPlugin.h
/// @brief 標準のプラグインのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "LexPlugin.h"

/// 仕様書覚書
///
/// 19.5 `include
///   - ネスト可能
///   - ただし無限(間接的/直接的に自分自身を読む)ループは不可
///
/// 19.6 `resetall
///   - コンパイラディレクティブの影響をリセットする．
///
/// 19.7 `line
///   - ファイル名と行番号の情報を上書きする．
///   - `resetall の影響を受けない．


BEGIN_NAMESPACE_YM_VERILOG

class InputMgr;

//////////////////////////////////////////////////////////////////////
/// @class LpInclude
/// @ingroup VlParser
/// @brief `include 用プラグイン
//////////////////////////////////////////////////////////////////////
class LpInclude :
  public LexPlugin
{
public:

  /// @brief コンストラクタ
  LpInclude(
    RawLex& lex,             ///< [in] 親の Lex オブジェクト
    const std::string& name, ///< [in] ディレクティブ名
    InputMgr* input_mgr      ///< [in] InputMgr
  );

  /// @brief デストラクタ
  ~LpInclude();


public:

  /// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
  /// @return エラーが起きたら false を返す．
  bool
  parse() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ファイルを管理するオブジェクト
  InputMgr* mInputMgr;

};


//////////////////////////////////////////////////////////////////////
/// @class LpLine
/// @ingroup VlParser
/// @brief `line 用プラグイン
//////////////////////////////////////////////////////////////////////
class LpLine :
  public LexPlugin
{
public:

  /// @brief コンストラクタ
  LpLine(
    RawLex& lex,             ///< [in] 親の Lex オブジェクト
    const std::string& name, ///< [in] ディレクティブ名
    InputMgr* input_mgr      ///< [in] InputMgr
  );

  /// @brief デストラクタ
  ~LpLine();


public:

  /// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
  /// @return エラーが起きたら false を返す．
  bool
  parse() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ファイルを管理するオブジェクト
  InputMgr* mInputMgr;

};


//////////////////////////////////////////////////////////////////////
/// @class LpResetAll
/// @ingroup VlParser
/// @brief `resetall 用プラグイン
//////////////////////////////////////////////////////////////////////
class LpResetAll :
  public LexPlugin
{
public:

  /// @brief コンストラクタ
  LpResetAll(
    RawLex& lex,            ///< [in] 親の Lex オブジェクト
    const std::string& name ///< [in] ディレクティブ名
  );

  /// @brief デストラクタ
  ~LpResetAll();


public:

  /// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
  /// @return エラーが起きたら false を返す．
  bool
  parse() override;

};

END_NAMESPACE_YM_VERILOG

#endif // STDEXPRLUGIN_H
