#ifndef EIPRIMHEADUD_H
#define EIPRIMHEADUD_H

/// @file EiPrimHead.h
/// @brief EiPrimitive のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiPrimHeadU.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiPrimHeadUD EiPrimHeadUD.h "EiPrimHeadUD.h"
/// @brief 遅延付きの EiPrimHeadU
//////////////////////////////////////////////////////////////////////
class EiPrimHeadUD :
  public EiPrimHeadU
{
public:

  /// @brief コンストラクタ
  EiPrimHeadUD(
    const VlScope* parent,     ///< [in] 親のスコープ
    const AstItem& ast_header, ///< [in] パース木の定義
    const VlUdpDefn* udp       ///< [in] 定義
  ) : EiPrimHeadU(parent, ast_header, udp)
  {
  }

  /// @brief デストラクタ
  ~EiPrimHeadUD() = default;


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

#endif // EIPRIMHEADUD_H
