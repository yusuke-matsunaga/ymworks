#ifndef EIVARDECLELEMPLUSSELECT_H
#define EIVARDECLELEMPLUSSELECT_H

/// @file EiVarDeclElemPlusSelect.h
/// @brief EiVarDeclElemPlusSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiVarDeclElemVarSelect.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiVarDeclElemPlusSelect EiVarDeclElemPlusSelect.h "EiVarDeclElemPlusSelect.h"
/// @brief 可変インデックスを持つ部分選択を表すクラス
//////////////////////////////////////////////////////////////////////
class EiVarDeclElemPlusSelect :
  public EiVarDeclElemVarSelect
{
public:

  /// @brief コンストラクタ
  EiVarDeclElemPlusSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDeclArray* obj,  ///< [in] 本体のオブジェクト
    const std::vector<const VlExpr*>& index_list, ///< [in] インデックスのリスト
    const VlExpr* bit_index, ///< [in] ビット位置のインデックス式
    SizeType width           ///< [in] 選択するビット幅
  ) : EiVarDeclElemVarSelect(ast_expr, obj, index_list,
			     bit_index, width)
  {
  }

  /// @brief デストラクタ
  ~EiVarDeclElemPlusSelect() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言要素に対するプラス可変ビット/部分選択を持つ時 true を返す．
  ///
  /// - has_bit_select() == true の場合のみ意味を持つ．
  /// - 可変ビット選択の場合も true を返す．
  bool
  has_plus_bit_select() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // EIVARDECLELEMPLUSSELECT_H
