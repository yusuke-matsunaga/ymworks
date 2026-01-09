#ifndef MACROSOURCE_H
#define MACROSOURCE_H

/// @file MacroSource.h
/// @brief MacroSource のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

class TokenList;
class TokenInfo;

//////////////////////////////////////////////////////////////////////
/// @class MacroSource MacroSource.h "MacroSource.h"
/// @ingroup VlParser
/// @brief define macro の入力切り替えのためのクラス
//////////////////////////////////////////////////////////////////////
class MacroSource
{
public:

  /// @brief コンストラクタ
  MacroSource();

  /// @brief デストラクタ
  ~MacroSource();


public:

  /// @brief 内容をセットする．
  void
  set(
    const std::string& name, ///< [in] マクロ名
    const TokenInfo* top,    ///< [in] トークンリストの先頭
    TokenList* param_array   ///< [in] パラメータリスト
  );

  /// @brief マクロ名のチェック
  /// @return マクロ名が name に等しいとき true を返す．
  bool
  check_name(
    const std::string& name ///< [in] チェックするマクロ名
  ) const
  {
    return mName == name;
  }

  /// @brief 内容を取り出す．
  const TokenInfo*
  get();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マクロ名
  std::string mName;

  // 次に読み出すトークンその1
  const TokenInfo* mNextToken1;

  // 次に読み出すトークンその2
  const TokenInfo* mNextToken2;

  // 置き換え用のトークンリストの配列
  TokenList* mParamArray;

};

END_NAMESPACE_YM_VERILOG

#endif // MACROSOURCE_H
