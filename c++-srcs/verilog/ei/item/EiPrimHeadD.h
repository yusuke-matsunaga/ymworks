#ifndef EIPRIMHEADD_H
#define EIPRIMHEADD_H

/// @file EiPrimHeadD.h
/// @brief EiPrimHeadD のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiPrimHead.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiPrimHeadD EiPrimHead.h "EiPrimHead.h"
/// @brief 遅延付きの EiPrimHead
//////////////////////////////////////////////////////////////////////
class EiPrimHeadD :
  public EiPrimHead
{
public:

  /// @brief コンストラクタ
  EiPrimHeadD(
    const VlScope* parent,    ///< [in] 親のスコープ
    const AstItem& ast_header ///< [in] パース木の定義
  ) : EiPrimHead(parent, ast_header)
  {
  }

  /// @brief デストラクタ
  ~EiPrimHeadD() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // EiPrimHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 遅延式を得る．
  const VlDelay*
  delay() const override;

  /// @brief 遅延式を設定する．
  void
  set_delay(
    const VlDelay* expr
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 遅延値
  const VlDelay* mDelay;

};

END_NAMESPACE_YM_VERILOG

#endif // EIPRIMHEADD_H
