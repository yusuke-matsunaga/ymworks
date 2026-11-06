#ifndef EIVARDECLELEMVARSELECT_H
#define EIVARDECLELEMVARSELECT_H

/// @file EiVarDeclElemVarSelect.h
/// @brief EiVarDeclElemVarSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiVarDeclElemSelect.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiVarDeclElemVarSelect EiVarDeclElemVarSelect.h "EiVarDeclElemVarSelect.h"
/// @brief 可変インデックスを持つ部分選択を表すクラス
//////////////////////////////////////////////////////////////////////
class EiVarDeclElemVarSelect :
  public EiVarDeclElemSelect
{
public:

  /// @brief コンストラクタ
  EiVarDeclElemVarSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDeclArray* obj,  ///< [in] 本体のオブジェクト
    const std::vector<const VlExpr*>& index_list, ///< [in] インデックスのリスト
    const VlExpr* bit_index, ///< [in] ビット位置のインデックス式
    SizeType width           ///< [in] 選択するビット幅
  ) : EiVarDeclElemSelect(ast_expr, obj, index_list, width),
      mBitIndex{bit_index}
  {
  }

  /// @brief デストラクタ
  ~EiVarDeclElemVarSelect() = default;


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

#endif // EIVARDECLELEMVARSELECT_H
