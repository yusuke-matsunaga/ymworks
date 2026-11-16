#ifndef EIPRIMARYBASE_H
#define EIPRIMARYBASE_H

/// @file EiPrimaryBase.h
/// @brief EiPrimaryBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiPrimaryBase EiPrimary.h "EiPrimary.h"
/// @brief プライマリ式の基底クラス
///
/// プライマリ式は AstExpr 以外のオブジェクトから作られる場合もある
/// ので EiExprBase を継承しない
//////////////////////////////////////////////////////////////////////
class EiPrimaryBase :
  public EiExpr
{
public:

  /// @brief コンストラクタ
  EiPrimaryBase() = default;

  /// @brief デストラクタ
  ~EiPrimaryBase() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  VlExprType
  expr_type() const override;

  /// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
  bool
  is_primary() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  void
  _set_reqsize(
    const VlValueType& type ///< [in] 要求される式の型
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

};

END_NAMESPACE_YM_VERILOG

#endif // EIPRIMARYBASE_H
