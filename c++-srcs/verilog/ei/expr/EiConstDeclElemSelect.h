#ifndef EICONSTDECLELEMSELECT_H
#define EICONSTDECLELEMSELECT_H

/// @file EiConstDeclElemSelect.h
/// @brief EiConstDeclElemSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiConstDeclElemPrimary.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiConstDeclElemSelect EiConstDeclElemSelect.h "EiConstDeclElemSelect.h"
/// @brief DeclArray の固定インデックス要素の部分選択を表す基底クラス
//////////////////////////////////////////////////////////////////////
class EiConstDeclElemSelect :
  public EiConstDeclElemPrimary
{
public:

  /// @brief コンストラクタ
  EiConstDeclElemSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDeclArray* obj,  ///< [in] 本体のオブジェクト
    SizeType array_offset,   ///< [in] 配列位置のオフセット
    SizeType width           ///< [in] 選択するビット幅
  ) : EiConstDeclElemPrimary(ast_expr, obj, array_offset),
      mWidth{width}
  {
  }


  /// @brief デストラクタ
  ~EiConstDeclElemSelect() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  VlValueType
  value_type() const override;

  /// @brief 式のビット幅を返す．
  ///
  /// value_type().size() を同じ
  SizeType
  bit_size() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 選択するビット幅
  SizeType mWidth;

};

END_NAMESPACE_YM_VERILOG

#endif // EICONSTDECLELEMSELECT_H
