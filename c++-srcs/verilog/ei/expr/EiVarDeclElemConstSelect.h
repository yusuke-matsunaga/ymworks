#ifndef EIVARDECLELEMCONSTSELECT_H
#define EIVARDECLELEMCONSTSELECT_H

/// @file EiVarDeclElemConstSelect.h
/// @brief EiVarDeclElemConstSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiVarDeclElemSelect.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiVarDeclElemConstSelect EiVarDeclElemConstSelect.h "EiVarDeclElemConstSelect.h"
/// @brief 固定インデックスのビット/部分指定を表すクラス
//////////////////////////////////////////////////////////////////////
class EiVarDeclElemConstSelect :
  public EiVarDeclElemSelect
{
public:

  /// @brief コンストラクタ
  EiVarDeclElemConstSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDeclArray* obj,  ///< [in] 本体のオブジェクト
    const std::vector<const VlExpr*>& index_list, ///< [in] インデックスのリスト
    SizeType bit_offset,     ///< [in] ビット位置のオフセット
    SizeType width           ///< [in] 選択するビット幅
  ) : EiVarDeclElemSelect(ast_expr, obj, index_list, width),
      mBitOffset{bit_offset}
  {
  }

  /// @brief デストラクタ
  ~EiVarDeclElemConstSelect() = default;


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

#endif // EIVARDECLELEMCONSTSELECT_H
