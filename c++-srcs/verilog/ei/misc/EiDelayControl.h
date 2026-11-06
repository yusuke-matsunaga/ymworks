#ifndef EIDELAYCONTROL_H
#define EIDELAYCONTROL_H

/// @file EiDelayControl.h
/// @brief EiDelayControl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

// IEEE Std 1364-2001 26.6.29 Delay control
// IEEE Std 1364-2001 26.6.30 Event control
// IEEE Std 1364-2001 26.6.31 Repeat control

#include "EiControl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiDelayControl EiDelayControl.h "EiDelayControl.h"
/// @brief delay control statement を表すクラス
/// IEEE1364-2001 では procedural timing control
/// とこれを混同している．
/// 正確には delay control はステートメントではない．
//////////////////////////////////////////////////////////////////////
class EiDelayControl :
  public EiControl
{
public:

  /// @brief コンストラクタ
  EiDelayControl(
    const AstControl& ast_control, ///< [in] パース木の定義要素
    ElbExpr* delay                 ///< [in] 遅延式
  ) : EiControl(ast_control),
      mDelay{delay}
  {
  }

  /// デストラクタ
  ~EiDelayControl() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlControl に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// 遅延式を返す．
  const VlExpr*
  delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 遅延式
  ElbExpr* mDelay;

};

END_NAMESPACE_YM_VERILOG

#endif // EIDELAYCONTROL_H
