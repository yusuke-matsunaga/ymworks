#ifndef DOTLIB_NSDEF_H
#define DOTLIB_NSDEF_H

/// @file dotlib_nsdef.h
/// @brief dotlib サブモジュールの定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"
#include "ym/FileRegion.h"


//////////////////////////////////////////////////////////////////////
// 名前空間を定義するためのマクロ
//////////////////////////////////////////////////////////////////////

/// @brief dotlib の名前空間の開始
#define BEGIN_NAMESPACE_YM_DOTLIB \
BEGIN_NAMESPACE_YM_CLIB \
BEGIN_NAMESPACE(nsDotlib)

/// @brief dotlib の名前空間の終了
#define END_NAMESPACE_YM_DOTLIB \
END_NAMESPACE(nsDotlib) \
END_NAMESPACE_YM_CLIB


BEGIN_NAMESPACE_YM_DOTLIB

// クラス名の前方宣言
class Parser;
class DotlibScanner;

class AstAttr;
class AstExpr;
class AstValue;

class Token;

/// @brief トークンの値
/// 基本的には dotlib のシンタックスにしたがっているが，
/// 一部，function 属性の文字列の中身をパーズする時のみ現れるシンボルがある．
/// AND, OR, XOR, PRIME がそれ．
enum class TokenType : std::uint8_t {
  COLON,      ///< コロン(:)
  SEMI,       ///< セミコロン(;)
  COMMA,      ///< コンマ(,)
  PLUS,       ///< プラス(+)
  MINUS,      ///< マイナス(-)
  MULT,       ///< 掛ける(*)
  DIV,        ///< 割る(/)
  NOT,        ///< NOT
  AND,        ///< AND
  OR,         ///< OR
  XOR,        ///< XOR
  LP,         ///< 左括弧( ( )
  RP,         ///< 右括弧( ) )
  LCB,        ///< 左中括弧( { )
  RCB,        ///< 右中括弧( } )
  PRIME,      ///< プライム( ' )
  SYMBOL,     ///< シンボル
  BOOL_0,     ///< ブール値0
  BOOL_1,     ///< ブール値1
  EXPRESSION, ///< 式
  NL,         ///< 改行

  ERROR,      ///< エラー
  END,        ///< ファイルの末尾
};


/// @brief TokenType 内容をストリームに出力する．
std::ostream&
operator<<(
  std::ostream& s, ///< [in] 出力先のストリーム
  TokenType type   ///< [in] トークンタイプ
);

// AstXXX の unique_ptr
using AstExprPtr = std::unique_ptr<const AstExpr>;
using AstValuePtr = std::unique_ptr<const AstValue>;
using AstAttrPtr = std::unique_ptr<const AstAttr>;

// simple attribute を読み込む関数の型定義
using SimpleHandler = std::function<AstValuePtr(DotlibScanner&)>;

// group statement の要素を読み込む関数の型定義
using AttrHandler = std::function<AstAttrPtr(Parser&,
					     const std::string&,
					     const FileRegion&)>;


//////////////////////////////////////////////////////////////////////
// エラー出力用の便利関数
//////////////////////////////////////////////////////////////////////

// @brief 未対応の属性名に対するエラーメッセージを出力する．
extern
void
syntax_error(
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性の定義されている位置
);

/// @brief 同じ属性が重複して定義されている時のエラーを出力する．
extern
void
duplicate_error(
  const std::string& kwd,    ///< [in] 属性名
  const FileRegion& kwd_loc, ///< [in] 属性の定義されている位置
  const AstAttr* prev_attr   ///< [in] 以前に定義された属性
);

END_NAMESPACE_YM_DOTLIB

#endif // DOTLIB_NSDEF_H
