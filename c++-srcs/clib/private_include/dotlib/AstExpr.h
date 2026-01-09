#ifndef ASTEXPR_H
#define ASTEXPR_H

/// @file AstExpr.h
/// @brief AstExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/dotlib_nsdef.h"
#include "ym/Expr.h"
#include "ym/ShString.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class AstExpr AstExpr.h "AstExpr.h"
/// @brief 式を表すノードのクラス
//////////////////////////////////////////////////////////////////////
class AstExpr
{
public:

  /// @brief 型を表す列挙型
  enum Type {
    Bool,  ///< ブール値
    Float, ///< 数値
    Str,   ///< 文字列
    VDD,   ///< VDD
    VSS,   ///< VSS
    VCC,   ///< VCC
    Plus,  ///< + 演算子
    Minus, ///< - 演算子
    Mult,  ///< * 演算子
    Div,   ///< / 演算子
    Not,   ///< NOT 演算子
    And,   ///< AND 演算子
    Or,    ///< OR 演算子
    Xor,   ///< XOR 演算子
    Null   ///< 無効値
  };


public:

  /// @brief ブール値を作る．
  static
  AstExprPtr
  new_bool(
    bool value,           ///< [in] 値
    const FileRegion& loc ///< [in] 位置
  );

  /// @brief 数値を作る．
  static
  AstExprPtr
  new_float(
    double value,         ///< [in] 値
    const FileRegion& loc ///< [in] 位置
  );

  /// @brief 文字列を作る．
  static
  AstExprPtr
  new_string(
    const ShString& value, ///< [in] 値
    const FileRegion& loc  ///< [in] 位置
  );

  /// @brief VDDシンボルを作る．
  static
  AstExprPtr
  new_vdd(
    const FileRegion& loc ///< [in] 位置
  );

  /// @brief VSSシンボルを作る．
  static
  AstExprPtr
  new_vss(
    const FileRegion& loc ///< [in] 位置
  );

  /// @brief VCCシンボルを作る．
  static
  AstExprPtr
  new_vcc(
    const FileRegion& loc ///< [in] 位置
  );

  /// @brief not 演算子を作る．
  static
  AstExprPtr
  new_not(
    AstExprPtr&& opr1,    ///< [in] 第１オペランド
    const FileRegion& loc ///< [in] 演算子の位置
  );

  /// @brief plus 演算子を作る．
  static
  AstExprPtr
  new_plus(
    AstExprPtr&& opr1,    ///< [in] 第1オペランド
    AstExprPtr&& opr2, 	  ///< [in] 第2オペランド
    const FileRegion& loc ///< [in] 演算子の位置
  );

  /// @brief minus 演算子を作る．
  static
  AstExprPtr
  new_minus(
    AstExprPtr&& opr1,    ///< [in] 第1オペランド
    AstExprPtr&& opr2, 	  ///< [in] 第2オペランド
    const FileRegion& loc ///< [in] 演算子の位置
  );

  /// @brief mult 演算子を作る．
  static
  AstExprPtr
  new_mult(
    AstExprPtr&& opr1,    ///< [in] 第1オペランド
    AstExprPtr&& opr2, 	  ///< [in] 第2オペランド
    const FileRegion& loc ///< [in] 演算子の位置
  );

  /// @brief div 演算子を作る．
  static
  AstExprPtr
  new_div(
    AstExprPtr&& opr1,    ///< [in] 第1オペランド
    AstExprPtr&& opr2, 	  ///< [in] 第2オペランド
    const FileRegion& loc ///< [in] 演算子の位置
  );

  /// @brief and 演算子を作る．
  static
  AstExprPtr
  new_and(
    AstExprPtr&& opr1,    ///< [in] 第1オペランド
    AstExprPtr&& opr2, 	  ///< [in] 第2オペランド
    const FileRegion& loc ///< [in] 演算子の位置
  );

  /// @brief or 演算子を作る．
  static
  AstExprPtr
  new_or(
    AstExprPtr&& opr1,    ///< [in] 第1オペランド
    AstExprPtr&& opr2, 	  ///< [in] 第2オペランド
    const FileRegion& loc ///< [in] 演算子の位置
  );

  /// @brief xor 演算子を作る．
  static
  AstExprPtr
  new_xor(
    AstExprPtr&& opr1,    ///< [in] 第1オペランド
    AstExprPtr&& opr2, 	  ///< [in] 第2オペランド
    const FileRegion& loc ///< [in] 演算子の位置
  );

  /// @brief 無効なリファレンスを返す．
  static
  const AstExpr&
  null_ref();

  /// @brief デストラクタ
  virtual
  ~AstExpr() = default;


protected:

  /// @brief コンストラクタ
  AstExpr(
    const FileRegion& loc ///< [in] 位置情報
  );

  /// @brief コピーコンストラクタは削除
  AstExpr(
    const AstExpr& src
  ) = delete;

  /// @brief 代入演算子も削除
  AstExpr&
  operator=(
    const AstExpr& src
  ) = delete;


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  Type
  type() const = 0;

  /// @brief 二項演算子型(Plus, Minus, Mult, Div, And, Or, Xor)の時に true を返す．
  virtual
  bool
  is_opr() const;

  /// @brief ブール値を返す．
  ///
  /// Bool の時のみ意味を持つ．
  virtual
  bool
  bool_value() const;

  /// @brief 浮動小数点値を返す．
  ///
  /// Float の時のみ意味を持つ．
  virtual
  double
  float_value() const;

  /// @brief 文字列シンボルを返す．
  ///
  /// Str の時のみ意味を持つ．
  virtual
  ShString
  string_value() const;

  /// @brief 第一オペランドを返す．
  ///
  /// Not, Plus, Minus, Mult, Div, And, Or, Xor の時のみ意味を持つ．
  virtual
  const AstExpr&
  opr1() const;

  /// @brief 第二オペランドを返す．
  ///
  /// Plus, Minus, Mult, Div, And, Or, Xor の時のみ意味を持つ．
  virtual
  const AstExpr&
  opr2() const;

  /// @brief Expr を作る．
  /// @return 対応する式(Expr)を返す．
  virtual
  Expr
  to_expr(
    const std::unordered_map<ShString, SizeType>& pin_map ///< [in] ピン名をキーにしてピン番号を保持する辞書
  ) const = 0;

  /// @brief 位置を返す．
  FileRegion
  loc() const { return mLoc; }

  /// @brief 内容を表す文字列を返す．
  virtual
  std::string
  decompile() const = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 位置
  FileRegion mLoc;

};

END_NAMESPACE_YM_DOTLIB

#endif // ASTEXPR_H
