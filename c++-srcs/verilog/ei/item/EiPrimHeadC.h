#ifndef EIPRIMHEADC_H
#define EIPRIMHEADC_H

/// @file EiPrimHeadC.h
/// @brief EiPrimHeadC のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiPrimHead.h"
#include "ym/ClibCell.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiPrimHeadC EiPrimHeadC.h "EiPrimHeadC.h"
/// @brief セル型の EiPrimHead
//////////////////////////////////////////////////////////////////////
class EiPrimHeadC :
  public EiPrimHead
{
public:

  /// @brief コンストラクタ
  EiPrimHeadC(
    const VlScope* parent,     ///< [in] 親のスコープ
    const AstItem& ast_header, ///< [in] パース木の定義
    const ClibCell& cell       ///< [in] セル
  ) : EiPrimHead(parent, ast_header),
      mCell{cell}
  {
  }

  /// @brief デストラクタ
  ~EiPrimHeadC() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // EiPrimHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief primitive type を返す．
  VpiPrimType
  prim_type() const override;

  /// @brief プリミティブの定義名を返す．
  std::string
  def_name() const override;

  /// @brief セルを返す．
  ClibCell
  cell() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セル
  ClibCell mCell;

};

END_NAMESPACE_YM_VERILOG

#endif // EIPRIMHEADC_H
