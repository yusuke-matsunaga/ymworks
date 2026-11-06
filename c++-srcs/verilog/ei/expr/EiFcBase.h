#ifndef EIFCBASE_H
#define EIFCBASE_H

/// @file EiFcBase.h
/// @brief EiFcBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiFcBase EiFcBase.h "EiFcBase.h"
/// @brief function call/system-function call に共通な基底クラス
//////////////////////////////////////////////////////////////////////
class EiFcBase :
  public EiExprBase
{
public:

  /// @brief コンストラクタ
  EiFcBase(
    const AstExpr& ast_expr,              ///< [in] パース木の定義要素
    const std::vector<ElbExpr*>& arg_list ///< [in] 引数のリスト
  ) : EiExprBase(ast_expr),
      mArgList{arg_list}
  {
  }

  /// @brief デストラクタ
  ~EiFcBase() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 引数の数を返す．
  SizeType
  argument_num() const override;

  /// @brief 引数の取得
  const VlExpr*
  argument(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < argument_num() )
  ) const override;

  /// @brief 引数のリストの取得
  std::vector<const VlExpr*>
  argument_list() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  void
  _set_reqsize(
    const VlValueType& type ///< [in] 要求される式の型
  ) override;


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 引数の配列
  std::vector<ElbExpr*> mArgList;

};

END_NAMESPACE_YM_VERILOG

#endif // EIFCBASE_H
