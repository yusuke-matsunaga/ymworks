#ifndef EXPRLUGIN_H
#define EXPRLUGIN_H

/// @file LexPlugin.h
/// @brief LexPlugin のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/FileRegion.h"
#include "scanner/RawLex.h"


BEGIN_NAMESPACE_YM_VERILOG

class MacroData;
class TokenInfo;

//////////////////////////////////////////////////////////////////////
/// @class LexPlugin Lex.h "Lex.h"
/// @ingroup VlParser
/// @brief Lex に付随するプラグインの基底クラス
//////////////////////////////////////////////////////////////////////
class LexPlugin
{
public:

  /// @brief コンストラクタ
  ///
  /// name はこのクラス自体には保存しない．
  /// lex に登録する時のキーとして用いる．
  LexPlugin(
    RawLex& lex,            ///< [in] 親の Lex オブジェクト
    const std::string& name ///< [in] ディレクティブ名
  );

  /// @brief デストラクタ
  virtual
  ~LexPlugin();


public:
  //////////////////////////////////////////////////////////////////////
  // LexPlugin の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  parse() = 0;

  /// @brief 条件コンパイル用のプラグインの時 true を返す仮想関数
  ///
  /// デフォルトの実装では false を返す．
  virtual
  bool
  is_cond_plugin();

  /// @brief マクロの時 true を返す仮想関数
  ///
  /// デフォルトの実装では false を返す．
  virtual
  bool
  is_macro();


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスで用いる便利関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 状態を初期化する
  void
  resetall(
    const FileRegion& file_region ///< [in] 初期化をしているソースファイル上のファイル位置
  )
  {
    mLex.resetall(file_region);
  }

  /// @brief トークンの位置の取得
  /// @return 最後に読んだトークンのファイル上の位置
  FileRegion
  cur_token_loc() const
  {
    return mLex.cur_token_loc();
  }

  /// @brief トークン文字列の取得
  /// @return 最後に読んだトークン文字列
  std::string
  cur_string() const
  {
    return mLex.cur_string();
  }

  /// @brief 指定されたトークンを期待して待つ．
  /// @retval true tok というトークンを読み出した．
  /// @retval false tok 以外の非空白トークンを読み出した．
  ///
  /// tok というトークンが読み出されるまで空白文字を読み飛ばす
  bool
  expect(
    int tok ///< [in] 期待するトークン
  );

  /// @brief 改行を期待して待つ．
  /// @retval true 改行もしくは EOF を読み出した．
  /// @retval false 改行もしくは EOF 以外の非空白トークンを読み出した．
  /// @note 空白トークンを読み飛ばす．
  bool
  expect_nl();

  /// @brief 空白以外の次のトークンの取得
  /// @return 空白以外の次のトークンの id
  int
  get_nospace_token();

  /// @brief トークンを取出す本当の関数
  /// @return 本当に次のトークンの id
  int
  get_raw_token()
  {
    return mLex.get_raw_token();
  }

  /// @brief モジュール定義の中にいるとき true を返す
  /// @return モジュール定義の中にいるとき true を返す．
  bool
  is_in_module()
  {
    return mLex.mModuleMode;
  }

  /// @brief デバッグフラグのチェック
  /// @return デバッグフラグが立っていたら true を返す．
  bool
  debug() const
  {
    return mLex.debug();
  }

  /// @brief 親の Lex の取得
  /// @return 親の Lex オブジェクト
  RawLex&
  lex()
  {
    return mLex;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の Lex
  RawLex& mLex;

};

END_NAMESPACE_YM_VERILOG

#endif // EXPRLUGIN_H
