#ifndef EICONSTDECLELEMVARSELECT_H
#define EICONSTDECLELEMVARSELECT_H

/// @file EiConstDeclElemVarSelect.h
/// @brief EiConstDeclElemVarSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiConstDeclElemSelect.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiConstDeclElemVarSelect EiConstDeclElemVarSelect.h "EiConstDeclElemVarSelect.h"
/// @brief 可変インデックスを持つ部分選択を表すクラス
//////////////////////////////////////////////////////////////////////
class EiConstDeclElemVarSelect :
  public EiConstDeclElemSelect
{
public:

  /// @brief コンストラクタ
  EiConstDeclElemVarSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDeclArray* obj,  ///< [in] 本体のオブジェクト
    SizeType array_offset,   ///< [in] 配列位置のオフセット
    const VlExpr* bit_index, ///< [in] ビット位置のインデックス式
    SizeType width           ///< [in] 選択するビット幅
  ) : EiConstDeclElemSelect(ast_expr, obj, array_offset, width),
      mBitIndex{bit_index}
  {
  }

  /// @brief デストラクタ
  ~EiConstDeclElemVarSelect() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言要素に対する可変ビット/部分選択のインデックス式を返す．
  ///
  /// - has_plus_bit_select() か has_minus_bit_select() が true
  ///   の場合のみ意味を持つ．
  const VlExpr*
  bit_index() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ビットインデックスの式
  const VlExpr* mBitIndex;

};

END_NAMESPACE_YM_VERILOG

#endif // EICONSTDECLELEMVARSELECT_H
