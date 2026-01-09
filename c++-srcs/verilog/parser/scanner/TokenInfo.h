#ifndef TOKENINFO_H
#define TOKENINFO_H

/// @file TokenInfo.h
/// @brief TokenInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class TokenInfo TokenInfo.h "TokenInfo.h"
/// @ingroup VlParser
/// @brief トークンの情報を表す構造体
//////////////////////////////////////////////////////////////////////
class TokenInfo
{
  friend class TokenList;

public:

  /// @brief 空のコンストラクタ
  TokenInfo() :
    mLink(nullptr)
  {
  }

  /// @brief マクロの置き換え用の位置パラメータを表すときのコンストラクタ
  TokenInfo(
    int pos ///< [in] 位置パラメータ
  ) : mId(- (pos + 1)),
      mLink(nullptr)
  {
  }

  /// @brief 値をセットするためのコンストラクタ
  TokenInfo(
    const FileRegion& loc, ///< [in] ファイル上の位置
    int id,                ///< [in] トークン番号
    const std::string& str ///< [in] 文字列
  ) : mLoc(loc),
      mId(id),
      mString(str),
      mLink(nullptr)
  {
  }

  /// @brief デストラクタ
  ~TokenInfo() = default;


public:

  /// @brief 位置パラメータの取得
  /// @return 位置パラメータのときその位置を返す．\n
  /// そうでなければ -1 を返す．
  int
  pos() const
  {
    if ( mId < 0 ) {
      return - (mId + 1);
    }
    else {
      return -1;
    }
  }

  /// @brief トークン番号を取り出す．
  int
  id() const
  {
    return mId;
  }

  /// @brief 文字列を取り出す．
  const std::string&
  str() const
  {
    return mString;
  }

  /// @brief ファイル位置を取り出す．
  const FileRegion&
  loc() const
  {
    return mLoc;
  }

  /// @brief 次の要素を取り出す．
  const TokenInfo*
  next() const
  {
    return mLink;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を表す構造体
  FileRegion mLoc;

  // トークン番号
  int mId;

  // 実際の文字列
  std::string mString;

  // 次の要素
  TokenInfo* mLink;

};

/// @relates TokenInfo
/// @brief TokenInfo の内容を出力するための関数
/// @return s を返す．
std::ostream&
operator<<(
  std::ostream& s,       ///< [in] 出力ストリーム
  const TokenInfo& token ///< [in] トークン情報
);

END_NAMESPACE_YM_VERILOG

#endif // TOKENINFO_H
