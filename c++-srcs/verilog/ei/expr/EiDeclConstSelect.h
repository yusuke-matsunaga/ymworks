#ifndef EIDECLCONSTSELECT_H
#define EIDECLCONSTSELECT_H

/// @file EiDeclConstSelect.h
/// @brief EiDeclConstSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiDeclSelect.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiDeclConstSelect EiDeclConstSelect.h "EiDeclConstSelect.h"
/// @brief 固定インデックスのビット/部分指定を表すクラス
//////////////////////////////////////////////////////////////////////
class EiDeclConstSelect :
  public EiDeclSelect
{
public:

  /// @brief コンストラクタ
  EiDeclConstSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDecl* obj,       ///< [in] 本体のオブジェクト
    SizeType bit_offset,     ///< [in] ビット位置のオフセット
    SizeType width           ///< [in] 選択するビット幅
  ) : EiDeclSelect(ast_expr, obj, width),
      mBitOffset{bit_offset}
  {
  }

  /// @brief デストラクタ
  ~EiDeclConstSelect() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言要素に対する固定ビット/部分選択を持つ時 true を返す．
  bool
  has_const_bit_select() const override;

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

#endif // EIDECLCONSTSELECT_H
