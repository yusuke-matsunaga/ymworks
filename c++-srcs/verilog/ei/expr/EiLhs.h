#ifndef EILHS_H
#define EILHS_H

/// @file EiLhs.h
/// @brief EiLhs のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "EiExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiLhs EiLhs.h "EiLhs.h"
/// @brief 複数要素の左辺式を表す VlExpr
//////////////////////////////////////////////////////////////////////
class EiLhs :
  public EiExpr
{
public:

  /// @brief コンストラクタ
  EiLhs(
    const std::vector<const VlExpr*>& expr_list ///< [in] 要素のリスト
  ) : mElemList{expr_list}
  {
  }

  /// @brief デストラクタ
  ~EiLhs() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式の値のタイプを返す．
  ///
  /// このタイプで評価する．
  VlValueType
  value_type() const override;

  /// @brief Verilog-HDL の文字列を得る．
  std::string
  decompile() const override;

  /// @brief 式のビット幅を返す．
  ///
  /// value_type().size() を同じ
  SizeType
  bit_size() const override;

  /// @brief 式のタイプを返す．
  VlExprType
  expr_type() const override;

  /// @brief 左辺式の時に true を返す．
  bool
  is_lhs() const override;

  /// @brief 要素数を返す．
  SizeType
  lhs_elem_num() const override;

  /// @brief 要素を返す．
  const VlExpr*
  lhs_elem(
    SizeType index ///< [in] インデックス ( 0 <= index < elem_num() )
  ) const override;


protected:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief set_reqsize() の下請け関数
  ///
  /// 必要であればオペランドに対して再帰的に処理を行なう．
  void
  _set_reqsize(
    const VlValueType& type ///< [in] 要求される式の型
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素のリスト
  std::vector<const VlExpr*> mElemList;

};

END_NAMESPACE_YM_VERILOG

#endif // EILHS_H
