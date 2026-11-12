#ifndef EIDECLSELECT_H
#define EIDECLSELECT_H

/// @file EiDeclSelect.h
/// @brief EiDeclSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiDeclPrimary.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiDeclSelect EiDeclSelect.h "EiDeclSelect.h"
/// @brief Decl の部分選択を表す基底クラス
//////////////////////////////////////////////////////////////////////
class EiDeclSelect :
  public EiDeclPrimary
{
public:

  /// @brief コンストラクタ
  EiDeclSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDecl* obj,       ///< [in] 本体のオブジェクト
    SizeType width           ///< [in] 選択するビット幅
  ) : EiDeclPrimary(ast_expr, obj),
      mWidth{width}
  {
  }

  /// @brief デストラクタ
  ~EiDeclSelect() = default;


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

#endif // EIDECLSELECT_H
