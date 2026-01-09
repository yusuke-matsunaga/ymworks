#ifndef LEXMODULEPLUGIN_H
#define LEXMODULEPLUGIN_H

/// @file LexModulePlugin.h
/// @brief 標準のプラグインのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "LexPlugin.h"

/// 仕様書覚書
///
/// 19.1 `celldefine and `endcelldefine
///   - 一つのファイル中に複数の `celldefine/`endcelldefine が現れてもよい
///   - 任意の場所におけるがモジュールの外側に置くのが望ましい．
///   - `resetall は `endcelldefine の効果を持つ．
///
/// 19.2 `default_nettype
///   - モジュール定義の外側に置かなければならない．
///   - 複数の `default_nettype を持つことができるが，最後の記述が有効となる．
///   - `default_nettype がない場合や `resetall の後では wire タイプが指定
///     されたものと扱う．
///   - none タイプが指定された場合，すべてのネット定義は明示的になされなけ
///     ればならない．
///
/// 19.8 `timescale
///   - `timescale 記述の後に現れるモジュールはこのタイムスケールを用いる．
///   - `timescale 記述がない場合や `resetall の後ではシミュレータで設定さ
///      れた値が用いられる．
///   - 一部のモジュールのみ `timescale 記述を持ち，他のモジュールが持たない
///     場合にはエラーとなる．
///   - 単位(unit)は精度(precision)よりも等しいか粗くなくてはならない．
///
/// 19.9 `unconnected_drive and `nounconnected_drive
///   - `unconnected_drive は pull1 か pull0 を引数にとる．
///   - `unconnected_drive と `nounconnected_drive は対でモジュール定義の
///     外側で用いられなければならない．
///   - `resetall は `nounconnected_drive の効果を持つ．


BEGIN_NAMESPACE_YM_VERILOG

class LexModuleState;

//////////////////////////////////////////////////////////////////////
/// @class LexModulePlugin
/// @ingroup VlParser
/// @brief LexModuleState を持つプラグイン基底クラス
//////////////////////////////////////////////////////////////////////
class LexModulePlugin :
  public LexPlugin
{
public:

  /// @brief コンストラクタ
  LexModulePlugin(
    RawLex& lex,             ///< [in] 親の Lex オブジェクト
    const std::string& name, ///< [in] ディレクティブ名
    LexModuleState* state    ///< [in] 状態を保持するオブジェクト
  );

  /// @brief デストラクタ
  ~LexModulePlugin();


public:

  /// @brief LexModuleState を返す．
  LexModuleState*
  module_state();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 状態
  LexModuleState* mState;

};


//////////////////////////////////////////////////////////////////////
/// @class LpCellDefine
/// @ingroup VlParser
/// @brief `celldefine 用プラグイン
//////////////////////////////////////////////////////////////////////
class LpCellDefine :
  public LexModulePlugin
{
public:

  /// @brief コンストラクタ
  LpCellDefine(
    RawLex& lex,             ///< [in] 親の Lex オブジェクト
    const std::string& name, ///< [in] ディレクティブ名
    LexModuleState* state    ///< [in] 状態を保持するオブジェクト
  );

  /// @brief デストラクタ
  ~LpCellDefine();


public:

  /// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
  /// @return エラーが起きたら false を返す．
  bool
  parse() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

};


//////////////////////////////////////////////////////////////////////
/// @class LpEndCellDefine
/// @ingroup VlParser
/// @brief `endcelldefine 用プラグイン
//////////////////////////////////////////////////////////////////////
class LpEndCellDefine :
  public LexModulePlugin
{
public:

  /// @brief コンストラクタ
  LpEndCellDefine(
    RawLex& lex,             ///< [in] 親の Lex オブジェクト
    const std::string& name, ///< [in] ディレクティブ名
    LexModuleState* state    ///< [in] 状態を保持するオブジェクト
  );

  /// @brief デストラクタ
  ~LpEndCellDefine();


public:

  /// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
  /// @return エラーが起きたら false を返す．
  bool
  parse() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

};


//////////////////////////////////////////////////////////////////////
/// @class LpNetType
/// @ingroup VlParser
/// @brief `default_nettype 用プラグイン
//////////////////////////////////////////////////////////////////////
class LpNetType :
  public LexModulePlugin
{
public:

  /// @brief コンストラクタ
  LpNetType(
    RawLex& lex,             ///< [in] 親の Lex オブジェクト
    const std::string& name, ///< [in] ディレクティブ名
    LexModuleState* state    ///< [in] 状態を保持するオブジェクト
  );

  /// @brief デストラクタ
  ~LpNetType();


public:

  /// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
  /// @return エラーが起きたら false を返す．
  bool
  parse() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

};


//////////////////////////////////////////////////////////////////////
/// @class LpTimeScale
/// @ingroup VlParser
/// @brief `timescale のパース
//////////////////////////////////////////////////////////////////////
class LpTimeScale :
  public LexModulePlugin
{
public:

  /// @brief コンストラクタ
  LpTimeScale(
    RawLex& lex,             ///< [in] 親の Lex オブジェクト
    const std::string& name, ///< [in] ディレクティブ名
    LexModuleState* state    ///< [in] 状態を保持するオブジェクト
  );

  /// @brief デストラクタ
  ~LpTimeScale();


public:

  /// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
  /// @return エラーが起きたら false を返す．
  bool
  parse() override;


private:

  /// @brief `timescale 内の時間単位を読む関数
  /// @retval true 正しいフォーマットで値が読み込まれた．
  /// @retval false 読み込み中にエラーが起きた．
  ///
  /// 値は 100s 〜 1fs で，2 〜 -15 に対応する．
  bool
  parse_unit(
    int& unit ///< [in] 読み込まれた値を格納する変数
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

};


//////////////////////////////////////////////////////////////////////
/// @class LpUnconnDrive
/// @ingroup VlParser
/// @brief unconnected_drive のパース用プラグイン
//////////////////////////////////////////////////////////////////////
class LpUnconnDrive :
  public LexModulePlugin
{
public:

  /// @brief コンストラクタ
  LpUnconnDrive(
    RawLex& lex,             ///< [in] 親の Lex オブジェクト
    const std::string& name, ///< [in] ディレクティブ名
    LexModuleState* state    ///< [in] 状態を保持するオブジェクト
  );

  /// @brief デストラクタ
  ~LpUnconnDrive();


public:

  /// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
  /// @return エラーが起きたら false を返す．
  bool
  parse() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

};


//////////////////////////////////////////////////////////////////////
/// @class LpNounconnDrive
/// @ingroup VlParser
/// @brief unconnected_drive のパース用プラグイン
//////////////////////////////////////////////////////////////////////
class LpNounconnDrive :
  public LexModulePlugin
{
public:

  /// @brief コンストラクタ
  LpNounconnDrive(
    RawLex& lex,             ///< [in] 親の Lex オブジェクト
    const std::string& name, ///< [in] ディレクティブ名
    LexModuleState* state    ///< [in] 状態を保持するオブジェクト
  );

  /// @brief デストラクタ
  ~LpNounconnDrive();


public:

  /// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
  /// @return エラーが起きたら false を返す．
  bool
  parse() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

};


//////////////////////////////////////////////////////////////////////
/// @class LpDecayTime
/// @ingroup VlParser
/// @brief `decay_time のパース用プラグイン
//////////////////////////////////////////////////////////////////////
class LpDecayTime :
  public LexModulePlugin
{
public:

  /// @brief コンストラクタ
  LpDecayTime(
    RawLex& lex,             ///< [in] 親の Lex オブジェクト
    const std::string& name, ///< [in] ディレクティブ名
    LexModuleState* state    ///< [in] 状態を保持するオブジェクト
  );

  /// @brief デストラクタ
  ~LpDecayTime();


public:

  /// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
  /// @return エラーが起きたら false を返す．
  bool
  parse() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

};


//////////////////////////////////////////////////////////////////////
/// @class LpTriregStrength
/// @ingroup VlParser
/// @brief `trireg_strength のパース用プラグイン
//////////////////////////////////////////////////////////////////////
class LpTriregStrength :
  public LexModulePlugin
{
public:

  /// @brief コンストラクタ
  LpTriregStrength(
    RawLex& lex,             ///< [in] 親の Lex オブジェクト
    const std::string& name, ///< [in] ディレクティブ名
    LexModuleState* state    ///< [in] 状態を保持するオブジェクト
  );

  /// @brief デストラクタ
  ~LpTriregStrength();


public:

  /// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
  /// @return エラーが起きたら false を返す．
  bool
  parse() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

};


//////////////////////////////////////////////////////////////////////
/// @class LpDelayMode
/// @ingroup VlParser
/// @brief `delay_mode のパース用プラグイン
//////////////////////////////////////////////////////////////////////
class LpDelayMode :
  public LexModulePlugin
{
public:

  /// @brief コンストラクタ
  LpDelayMode(
    RawLex& lex,             ///< [in] 親の Lex オブジェクト
    const std::string& name, ///< [in] ディレクティブ名
    LexModuleState* state,   ///< [in] 状態を保持するオブジェクト
    VpiDefDelayMode mode     ///< [in] delay mode
  );

  /// @brief デストラクタ
  ~LpDelayMode();


public:

  /// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
  /// @return エラーが起きたら false を返す．
  bool
  parse() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // delay_mode
  VpiDefDelayMode mMode;

};

END_NAMESPACE_YM_VERILOG

#endif // LEXMODULEPLUGIN_H
