#ifndef EIREPEATCONTROL_H
#define EIREPEATCONTROL_H

/// @file EiControl.h
/// @brief VlControl の実装クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

// IEEE Std 1364-2001 26.6.29 Delay control
// IEEE Std 1364-2001 26.6.30 Event control
// IEEE Std 1364-2001 26.6.31 Repeat control

#include "EiEventControl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiRepeatControl EiRepeatControl.h "EiRepeatControl.h"
/// @brief repeat control を表すクラス
/// IEEE Std 1364-2001 26.6.31 Repeat control
//////////////////////////////////////////////////////////////////////
class EiRepeatControl :
  public EiEventControl
{
public:

  /// @brief コンストラクタ
  EiRepeatControl(
    const AstControl& ast_control,          ///< [in] パース木の定義要素
    ElbExpr* rep,                           ///< [in] 繰り返し式
    const std::vector<ElbExpr*>& event_list ///< [in] イベントリスト
  ) : EiEventControl(ast_control, event_list),
      mExpr{rep}
  {
  }

  /// デストラクタ
  ~EiRepeatControl() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlControl に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// 繰り返し式を返す．
  const VlExpr*
  expr() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 繰り返し式
  ElbExpr* mExpr;

};

END_NAMESPACE_YM_VERILOG

#endif // EIREPEATCONTROL_H
