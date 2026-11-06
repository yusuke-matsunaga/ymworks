#ifndef EISYSFUNCCALL_H
#define EISYSFUNCCALL_H

/// @file EiSysFuncCall.h
/// @brief EiSysFuncCall のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiFcBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiSysFuncCall EiSysFuncCall.h "EiSysFuncCall.h"
/// @brief システム関数呼び出しを表すクラス
//////////////////////////////////////////////////////////////////////
class EiSysFuncCall :
  public EiFcBase
{
public:

  /// @brief コンストラクタ
  EiSysFuncCall(
    const AstExpr& ast_expr,              ///< [in] パース木の定義要素
    const VlUserSystf* user_systf,        ///< [in] システム関数
    const std::vector<ElbExpr*>& arg_list ///< [in] 引数のリスト
  ) : EiFcBase(ast_expr, arg_list),
      mUserSystf{user_systf}
  {
  }

  /// @brief デストラクタ
  ~EiSysFuncCall() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  VlValueType
  value_type() const override;

  /// @brief 式のタイプを返す．
  VlExprType
  expr_type() const override;

  /// @brief システム関数の時に true を返す．
  bool
  is_sysfunccall() const override;

  /// @brief 対象のシステム関数を返す．
  const VlUserSystf*
  user_systf() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // システム関数
  const VlUserSystf* mUserSystf;

};

END_NAMESPACE_YM_VERILOG

#endif // EISYSFUNCCALL_H
