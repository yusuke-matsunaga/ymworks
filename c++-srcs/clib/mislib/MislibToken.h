#ifndef MISLIBTOKEN_H
#define MISLIBTOKEN_H

/// @file MislibToken.h
/// @brief MislibToken のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "mislib_nsdef.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
/// @class MislibToken MislibToken.h "MislibToken.h"
/// @brief mislib 形式のトークンを表すクラス
//////////////////////////////////////////////////////////////////////
class MislibToken
{
public:

  /// @brief トークンの種類を表す列挙型
  enum Type {
    STR,     ///< 文字列(識別子)
    NUM,     ///< 数値
    LP,      ///< '('
    RP,      ///< ')'
    SEMI,    ///< ';'
    EQ,      ///< '='
    GATE,    ///< 'gate'
    PIN,     ///< 'pin'
    NONINV,  ///< 'noinv'
    INV,     ///< 'inv'
    UNKNOWN, ///< 'unknown'
    CONST0,  ///< 'const0'
    CONST1,  ///< 'const1'
    PLUS,    ///< '+'
    HAT,     ///< '^'
    STAR,    ///< '*'
    NOT,     ///< '!'
    END,     ///< 末尾
    ERROR    ///< エラー
  };

public:

  /// @brief コンストラクタ
  MislibToken() = default;

  /// @brief 値を指定したコンストラクタ
  MislibToken(
    Type type,            ///< [in] トークンの種類
    const FileRegion& loc ///< [in] ファイル上の位置
  ) : mType{type},
      mLoc{loc}
  {
  }

  /// @brief デストラクタ
  ~MislibToken() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief トークンの種類を返す．
  Type
  type() const { return mType; }

  /// @brief ファイル上の位置を返す．
  FileRegion
  loc() const { return mLoc; }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 種類
  Type mType{Type::ERROR};

  // 位置
  FileRegion mLoc;

};

/// @brief MislibToken::Type のストリーム出力
inline
std::ostream&
operator<<(
  std::ostream& s,
  MislibToken::Type type)
{
  switch ( type ) {
  case MislibToken::STR:     s << "STR"; break;
  case MislibToken::NUM:     s << "NUM"; break;
  case MislibToken::LP:      s << "LP"; break;
  case MislibToken::RP:      s << "RP"; break;
  case MislibToken::SEMI:    s << "SEMI"; break;
  case MislibToken::EQ:      s << "EQ"; break;
  case MislibToken::GATE:    s << "GATE"; break;
  case MislibToken::PIN:     s << "PIN"; break;
  case MislibToken::NONINV:  s << "NONINV"; break;
  case MislibToken::INV:     s << "INV"; break;
  case MislibToken::UNKNOWN: s << "UNKNOWN"; break;
  case MislibToken::CONST0:  s << "CONST0"; break;
  case MislibToken::CONST1:  s << "CONST1"; break;
  case MislibToken::PLUS:    s << "PLUS"; break;
  case MislibToken::HAT:     s << "HAT"; break;
  case MislibToken::STAR:    s << "STAR"; break;
  case MislibToken::NOT:     s << "NOT"; break;
  case MislibToken::END:     s << "END"; break;
  case MislibToken::ERROR:   s << "ERROR"; break;
  default:
    s << "----";
    ASSERT_NOT_REACHED;
  }
  return s;
}

END_NAMESPACE_YM_MISLIB

#endif // MISLIBTOKEN_H
