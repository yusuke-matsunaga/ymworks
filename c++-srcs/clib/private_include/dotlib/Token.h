#ifndef TOKEN_H
#define TOKEN_H

/// @file Token.h
/// @brief Token のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2021 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/dotlib_nsdef.h"
#include "ym/FileRegion.h"
#include "ym/ShString.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class Token Token.h "Token.h"
/// @brief トークンを表すクラス
//////////////////////////////////////////////////////////////////////
class Token
{
public:

  /// @brief 空のコンストラクタ
  Token() = default;

  /// @brief 内容を指定したコンストラクタ
  Token(
    TokenType type,             ///< [in] トークンの種類
    const FileRegion& loc,      ///< [in] トークンの位置
    ShString value = ShString{} ///< [in] トークンの値
  ) : mType{type},
      mStr{value},
      mLoc{loc}
  {
  }

  /// @brief デストラクタ
  ~Token() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief トークンの種類を返す．
  TokenType
  type() const { return mType; }

  /// @brief トークンの値を返す．
  ///
  /// type() == TokenType::Symbol の時のみ意味を持つ．
  ShString
  value() const { return mStr; }

  /// @brief トークンの位置を返す．
  FileRegion
  loc() const { return mLoc; }

  /// @brief トークンを表す文字列を返す．
  std::string
  str_value() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 種類
  TokenType mType{TokenType::ERROR};

  // 文字列
  ShString mStr;

  // 位置
  FileRegion mLoc{};

};

inline
std::ostream&
operator<<(
  std::ostream& s,
  const Token& token
)
{
  s << token.loc() << ": " << token.type()
    << "[" << token.str_value() << "]";
  return s;
}

END_NAMESPACE_YM_DOTLIB

#endif // TOKEN_H
