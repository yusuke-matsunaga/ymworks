#ifndef TOKENLIST_H
#define TOKENLIST_H

/// @file TokenList.h
/// @brief TokenList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

class TokenInfo;

//////////////////////////////////////////////////////////////////////
/// @class TokenList TokenList.h "TokenList.h"
/// @ingroup VlParser
/// @brief トークンのリストを表すクラス
//////////////////////////////////////////////////////////////////////
class TokenList
{
public:

  /// @brief コンストラクタ
  TokenList();

  /// @brief コピーコンストラクタ
  ///
  /// 実際にはコピーしない．
  TokenList(
    const TokenList& src
  );

  /// @brief デストラクタ
  ~TokenList();


public:

  /// @brief 要素を追加する (位置パラメータ)
  void
  add(
    int pos ///< [in] 位置パラメータ
  );

  /// @brief 要素を追加する (トークン)
  void
  add(
    const FileRegion& loc, ///< [in] ファイル上の位置
    int id,                ///< [in] トークン番号
    const std::string& str ///< [in] 文字列
  );

  /// @brief 先頭のトークンを取り出す．
  const TokenInfo*
  top() const;


private:

  /// @brief add() の下請
  void
  add(
    TokenInfo* token
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  TokenInfo* mTop;

  // 末尾の要素
  TokenInfo* mEnd;

};

END_NAMESPACE_YM_VERILOG

#endif // TOKENLIST_H
