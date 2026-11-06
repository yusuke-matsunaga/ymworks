#ifndef EIFUNCCALL_H
#define EIFUNCCALL_H

/// @file EiFuncCall.h
/// @brief EiFuncCall のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiFcBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiFuncCall EiFuncCall.h "EiFuncCall.h"
/// @brief 関数呼び出しを表すクラス
//////////////////////////////////////////////////////////////////////
class EiFuncCall :
  public EiFcBase
{
public:

  /// @brief コンストラクタ
  EiFuncCall(
    const AstExpr& ast_expr,              ///< [in] パース木の定義要素
    const VlTaskFunc* func,               ///< [in] 関数
    const std::vector<ElbExpr*>& arg_list ///< [in] 引数のリスト
  ) : EiFcBase(ast_expr, arg_list),
      mFunc{func}
  {
  }

  /// @brief デストラクタ
  ~EiFuncCall() = default;


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

  /// @brief 関数呼び出しの時に true を返す．
  bool
  is_funccall() const override;

  /// @brief 対象の関数を返す．
  const VlTaskFunc*
  function() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数
  const VlTaskFunc* mFunc;

};

END_NAMESPACE_YM_VERILOG

#endif // EIFUNCCALL_H
