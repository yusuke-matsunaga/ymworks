#ifndef MISLIBEXPR_H
#define MISLIBEXPR_H

/// @file MislibExpr.h
/// @brief MislibExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "MislibNode.h"
#include "ym/ShString.h"
#include "ym/Expr.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
/// @class MislibExpr MislibExpr.h "MislibExpr.h"
/// @brief 式を表す AST
//////////////////////////////////////////////////////////////////////
class MislibExpr :
  public MislibNode
{
public:

  /// @brief ノードの種類
  enum Type {
    VarName, ///< 変数名
    Const0,  ///< 定数0
    Const1,  ///< 定数1
    Not,     ///< NOT演算子
    And,     ///< AND演算子
    Or,      ///< OR演算子
    Xor,     ///< XOR演算子
  };


public:

  /// @brief コンストラクタ
  MislibExpr(
    const FileRegion& loc ///< [in] 位置情報
  ) : MislibNode{loc}
  {
  }

  /// @brief デストラクタ
  virtual
  ~MislibExpr() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 共通な関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を取り出す．
  virtual
  Type
  type() const = 0;

  /// @brief 対応する論理式を生成する．
  virtual
  Expr
  to_expr(
    const MislibNameMap& name_map ///< [in] 端子名をキーにして端子番号を取り出す連想配列
  ) const = 0;

  /// @brief 内容を出力する(デバッグ用)．
  virtual
  void
  dump(
    std::ostream& s ///< [in] 出力ストリーム
  ) const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 変数名型(VarName)のときに意味のある関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数名を取り出す
  ///
  /// デフォルトの実装ではエラー(アボート)となる．
  virtual
  ShString
  varname() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 論理演算型(Not, And, Or, Xor)のときに意味のある関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 1番目のオペランドを取り出す．
  ///
  /// デフォルトの実装ではエラー(アボート)となる．
  virtual
  const MislibExpr*
  opr1() const;

  /// @brief 2番目のオペランドを取り出す．
  ///
  /// デフォルトの実装ではエラー(アボート)となる．
  virtual
  const MislibExpr*
  opr2() const;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibNot MislibNot.h "MislibNot.h"
/// @brief NOT論理式を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibNot :
  public MislibExpr
{
public:

  /// @brief コンストラクタ
  MislibNot(
    const FileRegion& loc, ///< [in] 位置情報
    MislibExprPtr&& opr1   ///< [in] オペランド
  ) : MislibExpr{loc},
      mOpr1{std::move(opr1)}
  {
  }

  /// @brief デストラクタ
  ~MislibNot() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // MislibExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を取り出す．
  Type
  type() const override;

  /// @brief 1番目のオペランドを取り出す．
  const MislibExpr*
  opr1() const override;

  /// @brief 対応する論理式を生成する．
  Expr
  to_expr(
    const MislibNameMap& name_map ///< [in] 端子名をキーにして端子番号を取り出す連想配列
  ) const override;

  /// @brief 内容を出力する．
  void
  dump(
    std::ostream& s ///< [in] 出力ストリーム
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オペランド
  MislibExprPtr mOpr1;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibBop MislibBop.h "MislibBop.h"
/// @brief 2項演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibBop :
  public MislibExpr
{
public:

  /// @brief コンストラクタ
  MislibBop(
    const FileRegion& loc, ///< [in] 位置情報
    MislibExprPtr&& opr1,  ///< [in] 左のオペランド
    MislibExprPtr&& opr2   ///< [in] 右のオペランド
  ) : MislibExpr{loc},
      mOpr1{std::move(opr1)},
      mOpr2{std::move(opr2)}
  {
  }

  /// @brief デストラクタ
  ~MislibBop() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 1番目のオペランドを取り出す．
  const MislibExpr*
  opr1() const override;

  /// @brief 2番目のオペランドを取り出す．
  const MislibExpr*
  opr2() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 1番目のオペランド
  MislibExprPtr mOpr1;

  // 2番目のオペランド
  MislibExprPtr mOpr2;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibAnd MislibBop.h "MislibBop.h"
/// @brief AND論理式を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibAnd :
  public MislibBop
{
public:

  /// @brief コンストラクタ
  MislibAnd(
    const FileRegion& loc, ///< [in] 位置情報
    MislibExprPtr&& opr1,  ///< [in] 左のオペランド
    MislibExprPtr&& opr2   ///< [in] 右のオペランド
  ) : MislibBop{loc, std::move(opr1), std::move(opr2)}
  {
  }

  /// @brief デストラクタ
  ~MislibAnd() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // MislibExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を取り出す．
  Type
  type() const override;

  /// @brief 対応する論理式を生成する．
  Expr
  to_expr(
    const MislibNameMap& name_map ///< [in] 端子名をキーにして端子番号を取り出す連想配列
  ) const override;

  /// @brief 内容を出力する．
  void
  dump(
    std::ostream& s ///< [in] 出力ストリーム
  ) const override;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibOr MislibBop.h "MislibBop.h"
/// @brief OR論理式を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibOr :
  public MislibBop
{
public:

  /// @brief コンストラクタ
  MislibOr(
    const FileRegion& loc, ///< [in] 位置情報
    MislibExprPtr&& opr1,  ///< [in] 左のオペランド
    MislibExprPtr&& opr2  ///< [in] 右のオペランド
  ) : MislibBop{loc, std::move(opr1), std::move(opr2)}
  {
  }

  /// @brief デストラクタ
  ~MislibOr() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // MislibExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を取り出す．
  Type
  type() const override;

  /// @brief 対応する論理式を生成する．
  Expr
  to_expr(
    const MislibNameMap& name_map ///< [in] 端子名をキーにして端子番号を取り出す連想配列
  ) const override;

  /// @brief 内容を出力する．
  void
  dump(
    std::ostream& s ///< [in] 出力ストリーム
  ) const override;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibXor MislibBop.h "MislibBop.h"
/// @brief XOR論理式を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibXor :
  public MislibBop
{
public:

  /// @brief コンストラクタ
  MislibXor(
    const FileRegion& loc, ///< [in] 位置情報
    MislibExprPtr&& opr1,  ///< [in] 左のオペランド
    MislibExprPtr&& opr2   ///< [in] 右のオペランド
  ) : MislibBop{loc, std::move(opr1), std::move(opr2)}
  {
  }

  /// @brief デストラクタ
  ~MislibXor() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // MislibExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を取り出す．
  Type
  type() const override;

  /// @brief 対応する論理式を生成する．
  Expr
  to_expr(
    const MislibNameMap& name_map ///< [in] 端子名をキーにして端子番号を取り出す連想配列
  ) const override;

  /// @brief 内容を出力する．
  void
  dump(
    std::ostream& s ///< [in] 出力ストリーム
  ) const override;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibVarName MislibVarName.h "MislibVarName.h"
/// @brief 変数名を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibVarName :
  public MislibExpr
{
public:

  /// @brief コンストラクタ
  MislibVarName(
    const FileRegion& loc, ///< [in] 位置情報
    const char* str        ///< [in] 文字列
  ) : MislibExpr{loc},
      mVarName{str}
  {
  }

  /// @brief デストラクタ
  ~MislibVarName() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // MislibExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を取り出す．
  Type
  type() const override;

  /// @brief 変数名を取り出す
  ShString
  varname() const override;

  /// @brief 対応する論理式を生成する．
  Expr
  to_expr(
    const MislibNameMap& name_map ///< [in] 端子名をキーにして端子番号を取り出す連想配列
  ) const override;

  /// @brief 内容を出力する．
  void
  dump(
    std::ostream& s ///< [in] 出力ストリーム
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数名
  ShString mVarName;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibConst0 MislibConst.h "MislibConst.h"
/// @brief 定数0を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibConst0 :
  public MislibExpr
{
public:

  /// @brief コンストラクタ
  explicit
  MislibConst0(
    const FileRegion& loc ///< [in] 位置情報
  ) : MislibExpr{loc}
  {
  }

  /// @brief デストラクタ
  ~MislibConst0() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // MislibExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を取り出す．
  Type
  type() const override;

  /// @brief 対応する論理式を生成する．
  Expr
  to_expr(
    const MislibNameMap& name_map ///< [in] 端子名をキーにして端子番号を取り出す連想配列
  ) const override;

  /// @brief 内容を出力する．
  void
  dump(
    std::ostream& s ///< [in] 出力ストリーム
  ) const override;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibConst1 MislibConst.h "MislibConst.h"
/// @brief 定数1を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibConst1 :
  public MislibExpr
{
public:

  /// @brief コンストラクタ
  explicit
  MislibConst1(
    const FileRegion& loc ///< [in] 位置情報
  ) : MislibExpr{loc}
  {
  }

  /// @brief デストラクタ
  ~MislibConst1() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // MislibExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を取り出す．
  Type
  type() const override;

  /// @brief 対応する論理式を生成する．
  Expr
  to_expr(
    const MislibNameMap& name_map ///< [in] 端子名をキーにして端子番号を取り出す連想配列
  ) const override;

  /// @brief 内容を出力する．
  void
  dump(
    std::ostream& s ///< [in] 出力ストリーム
  ) const override;

};

END_NAMESPACE_YM_MISLIB

#endif // MISLIBEXPR_H
