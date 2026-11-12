#ifndef EICONSTDECLELEMCONSTSELECT_H
#define EICONSTDECLELEMCONSTSELECT_H

/// @file EiConstDeclElemConstSelect.h
/// @brief EiConstDeclElemConstSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiConstDeclElemSelect.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiConstDeclElemConstSelect EiConstDeclElemConstSelect.h "EiConstDeclElemConstSelect.h"
/// @brief EiConstDeclElemPrimary の固定インデックスのビット/部分指定を表すクラス
//////////////////////////////////////////////////////////////////////
class EiConstDeclElemConstSelect :
  public EiConstDeclElemSelect
{
public:

  /// @brief コンストラクタ
  EiConstDeclElemConstSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDeclArray* obj,  ///< [in] 本体のオブジェクト
    SizeType array_offset,   ///< [in] 配列位置のオフセット
    SizeType bit_offset,     ///< [in] ビット位置のオフセット
    SizeType width           ///< [in] 選択するビット幅
  ) : EiConstDeclElemSelect(ast_expr, obj, array_offset, width),
      mBitOffset{bit_offset}
  {
  }

  /// @brief デストラクタ
  ~EiConstDeclElemConstSelect() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言要素に対するビット/部分選択の種類を返す．
  ///
  /// - expr_type() が PRIMARY かつ primary_type() が DECL,
  ///   CONST_DECL_ARRAY, VAR_DECL_ARRAY の場合に意味を持つ．
  VlSelectType
  select_type() const override;

  /// @brief 宣言要素に対する固定ビット/部分選択のオフセットを返す．
  ///
  /// - has_const_bit_select() == true の場合のみ意味を持つ．
  SizeType
  bit_offset() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ビット位置のオフセット
  SizeType mBitOffset;

};

END_NAMESPACE_YM_VERILOG

#endif // EICONSTDECLELEMCONSTSELECT_H
