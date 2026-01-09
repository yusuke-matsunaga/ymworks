#ifndef FUNCIMPL_EXPR_H
#define FUNCIMPL_EXPR_H

/// @file FuncImpl_Expr.h
/// @brief FuncImpl_Expr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "FuncImpl.h"
#include "ym/Expr.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
/// @class FuncImpl_Expr FuncImpl_Expr.h "FuncImpl_Expr.h"
/// @brief 論理式型の関数情報を表すクラス
//////////////////////////////////////////////////////////////////////
class FuncImpl_Expr :
  public FuncImpl
{
public:

  /// @brief コンストラクタ
  FuncImpl_Expr(
    const Expr& expr ///< [in] 論理式
  );

  /// @brief デストラクタ
  ~FuncImpl_Expr() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 関数の種類を返す．
  BnFunc::Type
  type() const override;

  /// @brief 論理式型の時 true を返す．
  bool
  is_expr() const override;

  /// @brief 入力数を返す．
  SizeType
  input_num() const override;

  /// @brief 論理式を返す．
  ///
  /// - is_expr() が true の時のみ意味を持つ．
  /// - それ以外の時は std::invalid_argument 例外を送出する．
  Expr
  expr() const override;

  /// @brief コピーを作る．
  std::unique_ptr<FuncImpl>
  copy(
    BddMgr& bdd_mgr ///< [in] BddMgr
  ) const override;

  /// @brief ハッシュ用のユニークな文字列を返す．
  std::string
  signature() const override;

  /// @brief 内容を出力する．
  void
  print(
    std::ostream& s ///< [in] 出力先のストリーム
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 論理式
  Expr mExpr;

};

END_NAMESPACE_YM_BN

#endif // FUNCIMPL_EXPR_H
