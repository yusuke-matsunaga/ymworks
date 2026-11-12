#ifndef EIVARDECLELEMSELECT_H
#define EIVARDECLELEMSELECT_H

/// @file EiVarDeclElemSelect.h
/// @brief EiVarDeclElemSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiVarDeclElemPrimary.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiVarDeclElemSelect EiVarDeclElemSelect.h "EiVarDeclElemSelect.h"
/// @brief Decl の部分選択を表す基底クラス
//////////////////////////////////////////////////////////////////////
class EiVarDeclElemSelect :
  public EiVarDeclElemPrimary
{
public:

  /// @brief コンストラクタ
  EiVarDeclElemSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDeclArray* obj,  ///< [in] 本体のオブジェクト
    const std::vector<const VlExpr*>& index_list, ///< [in] インデックスのリスト
    SizeType width           ///< [in] 選択するビット幅
  ) : EiVarDeclElemPrimary(ast_expr, obj, index_list),
      mWidth{width}
  {
  }

  /// @brief デストラクタ
  ~EiVarDeclElemSelect() = default;


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

#endif // EIVARDECLELEMSELECT_H
