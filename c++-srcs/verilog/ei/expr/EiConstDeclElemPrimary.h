#ifndef EICONSTDECLELEMPRIMARY_H
#define EICONSTDECLELEMPRIMARY_H

/// @file EiConstDeclElemPrimary.h
/// @brief EiConstDeclElemPrimary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiDeclElemPrimary.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiConstDeclElemPrimary EiConstDeclElemPrimary.h "EiConstDeclElemPrimary.h"
/// @brief 固定インデックスの配列要素のプライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiConstDeclElemPrimary :
  public EiDeclElemPrimary
{
public:

  /// @brief コンストラクタ
  EiConstDeclElemPrimary(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDeclArray* obj,  ///< [in] 本体のオブジェクト
    SizeType array_offset    ///< [in] 配列上の位置のオフセット
  ) : EiDeclElemPrimary(ast_expr, obj),
      mOffset{array_offset}
  {
  }

  /// @brief デストラクタ
  ~EiConstDeclElemPrimary() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 配列型宣言要素への参照のオフセットを返す．
  SizeType
  declarray_offset() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オフセット
  SizeType mOffset;

};

END_NAMESPACE_YM_VERILOG

#endif // EICONSTDECLELEMPRIMARY_H
