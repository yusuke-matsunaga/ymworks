#ifndef ASTEXPR_INT_H
#define ASTEXPR_INT_H

/// @file AstExpr_int.h
/// @brief AstExpr 派生クラスの定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/AstExpr.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class AstBoolExpr AstExpr_int.h "AstExpr_int.h"
/// @brief ブール値を表す AstExpr
//////////////////////////////////////////////////////////////////////
class AstBoolExpr :
  public AstExpr
{
public:

  /// @brief コンストラクタ
  AstBoolExpr(
    const FileRegion& loc, ///< [in] 位置情報
    bool val               ///< [in] 値
  );

  /// @brief デストラクタ
  ~AstBoolExpr() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  Type
  type() const override;

  /// @brief ブール値を返す．
  ///
  /// Bool の時のみ意味を持つ．
  bool
  bool_value() const override;

  /// @brief Expr を作る．
  /// @return 対応する式(Expr)を返す．
  Expr
  to_expr(
    const std::unordered_map<ShString, SizeType>& pin_map ///< [in] ピン名をキーにしてピン番号を保持する辞書
  ) const override;

  /// @brief 内容を表す文字列を返す．
  std::string
  decompile() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  bool mValue;

};


//////////////////////////////////////////////////////////////////////
/// @class AstFloatExpr AstExpr_int.h "AstExpr_int.h"
/// @brief 浮動小数点値を表す AstExpr
//////////////////////////////////////////////////////////////////////
class AstFloatExpr :
  public AstExpr
{
public:

  /// @brief コンストラクタ
  AstFloatExpr(
    const FileRegion& loc, ///< [in] 位置情報
    double val             ///< [in] 値
  );

  /// @brief デストラクタ
  ~AstFloatExpr() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  Type
  type() const override;

  /// @brief 浮動小数点値を返す．
  ///
  /// Float の時のみ意味を持つ．
  double
  float_value() const override;

  /// @brief Expr を作る．
  /// @return 対応する式(Expr)を返す．
  Expr
  to_expr(
    const std::unordered_map<ShString, SizeType>& pin_map ///< [in] ピン名をキーにしてピン番号を保持する辞書
  ) const override;

  /// @brief 内容を表す文字列を返す．
  std::string
  decompile() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  double mValue;

};


//////////////////////////////////////////////////////////////////////
/// @class AstStrExpr AstExpr_int.h "AstExpr_int.h"
/// @brief 文字列値を表す AstExpr
//////////////////////////////////////////////////////////////////////
class AstStrExpr :
  public AstExpr
{
public:

  /// @brief コンストラクタ
  AstStrExpr(
    const FileRegion& loc, ///< [in] 位置情報
    const ShString& val    ///< [in] 値
  );

  /// @brief デストラクタ
  ~AstStrExpr() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  Type
  type() const override;

  /// @brief 文字列値を返す．
  ///
  /// Str の時のみ意味を持つ．
  ShString
  string_value() const override;

  /// @brief Expr を作る．
  /// @return 対応する式(Expr)を返す．
  Expr
  to_expr(
    const std::unordered_map<ShString, SizeType>& pin_map ///< [in] ピン名をキーにしてピン番号を保持する辞書
  ) const override;

  /// @brief 内容を表す文字列を返す．
  std::string
  decompile() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  ShString mValue;

};


//////////////////////////////////////////////////////////////////////
/// @class AstSymbolExpr AstExpr_int.h "AstExpr_int.h"
//////////////////////////////////////////////////////////////////////
class AstSymbolExpr :
  public AstExpr
{
public:

  /// @brief コンストラクタ
  AstSymbolExpr(
    const FileRegion& loc, ///< [in] ファイル上の位置
    Type type              ///< [in] シンボルの種類(VDD, VSS, VCC)
  );

  /// @brief デストラクタ
  ~AstSymbolExpr() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  Type
  type() const override;

  /// @brief Expr を作る．
  /// @return 対応する式(Expr)を返す．
  Expr
  to_expr(
    const std::unordered_map<ShString, SizeType>& pin_map ///< [in] ピン名をキーにしてピン番号を保持する辞書
  ) const override;

  /// @brief 内容を表す文字列を返す．
  std::string
  decompile() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 型
  Type mType;

};


//////////////////////////////////////////////////////////////////////
/// @class AstNot AstExpr_int.h "AstExpr_int.h"
/// @brief NOT 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class AstNot :
  public AstExpr
{
public:

  /// @brief コンストラクタ
  AstNot(
    const FileRegion& loc,  ///< [in] ファイル上の位置
    AstExprPtr&& opr        ///< [in] オペランド
  );

  /// @brief デストラクタ
   ~AstNot() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  Type
  type() const override;

  /// @brief 第一オペランドを返す．
  ///
  /// type() が演算子の型の時のみ意味を持つ．
  const AstExpr&
  opr1() const override;

  /// @brief Expr を作る．
  /// @return 対応する式(Expr)を返す．
  Expr
  to_expr(
    const std::unordered_map<ShString, SizeType>& pin_map ///< [in] ピン名をキーにしてピン番号を保持する辞書
  ) const override;

  /// @brief 内容を表す文字列を返す．
  std::string
  decompile() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 第一オペランド
  AstExprPtr mOpr1;

};


//////////////////////////////////////////////////////////////////////
/// @class AstOpr AstExpr_int.h "AstExpr_int.h"
/// @brief 二項演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class AstOpr :
  public AstExpr
{
public:

  /// @brief コンストラクタ
  AstOpr(
    Type type,         ///< [in] 演算子の型
    AstExprPtr&& opr1, ///< [in] 第1オペランド
    AstExprPtr&& opr2  ///< [in] 第2オペランド
  );

  /// @brief デストラクタ
  ~AstOpr() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  Type
  type() const override;

  /// @brief 演算子型(Plus, Minsu, Mult, Div, And, Or, Xor)の時に true を返す．
  bool
  is_opr() const override;

  /// @brief 第一オペランドを返す．
  ///
  ///  type() が演算子の型の時のみ意味を持つ．
  const AstExpr&
  opr1() const override;

  /// @brief 第二オペランドを返す．
  ///
  /// type() が演算子の型の時のみ意味を持つ．
  const AstExpr&
  opr2() const override;

  /// @brief Expr を作る．
  /// @return 対応する式(Expr)を返す．
  Expr
  to_expr(
    const std::unordered_map<ShString, SizeType>& pin_map ///< [in] ピン名をキーにしてピン番号を保持する辞書
  ) const override;

  /// @brief 内容を表す文字列を返す．
  std::string
  decompile() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 型
  Type mType;

  // 第一オペランド
  AstExprPtr mOpr1;

  // 第二オペランド
  AstExprPtr mOpr2;

};


//////////////////////////////////////////////////////////////////////
/// @class AstNullExpr AstNullExpr.h "AstNullExpr.h"
/// @brief 無効な AstExpr を表すクラス
//////////////////////////////////////////////////////////////////////
class AstNullExpr :
  public AstExpr
{
public:

  /// @brief コンストラクタ
  AstNullExpr();

  /// @brief デストラクタ
  ~AstNullExpr() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  Type
  type() const override;

  /// @brief Expr を作る．
  /// @return 対応する式(Expr)を返す．
  Expr
  to_expr(
    const std::unordered_map<ShString, SizeType>& pin_map ///< [in] ピン名をキーにしてピン番号を保持する辞書
  ) const override;

  /// @brief 内容を表す文字列を返す．
  std::string
  decompile() const override;

};

END_NAMESPACE_YM_DOTLIB

#endif // ASTEXPR_INT_H
