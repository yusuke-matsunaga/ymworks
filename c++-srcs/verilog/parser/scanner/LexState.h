#ifndef LEXSTATE_H
#define LEXSTATE_H

/// @file LexState.h
/// @brief LexState のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/FileRegion.h"
#include "scanner/RawLex.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class LexState Lex.h "Lex.h"
/// @ingroup VlParser
/// @brief Lex に付随した状態を保持する基底クラス
//////////////////////////////////////////////////////////////////////
class LexState
{
public:

  /// @brief コンストラクタ
  LexState(
    RawLex& lex ///< [in] 親の lexer オブジェクト
  );

  /// @brief デストラクタ
  virtual
  ~LexState();


public:

  /// @brief 状態を初期化する．
  ///
  /// デフォルトの実装はなにもしない．
  virtual
  void
  resetall(
    const FileRegion& file_region
  );


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスで用いる便利関数
  //////////////////////////////////////////////////////////////////////

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
  lex() const
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

#endif // LEXSTATE_H
