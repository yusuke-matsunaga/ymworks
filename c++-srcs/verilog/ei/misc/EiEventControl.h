#ifndef EIEVENTCONTROL_H
#define EIEVENTCONTROL_H

/// @file EiEventControl.h
/// @brief EiEventControl のヘッダファイル
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
/// @class EiEventControl EiEventControl.h "EiEventControl.h"
/// @brief event control statement を表すクラス
/// IEEE1364-2001 では procedural timing control
/// とこれを混同している．
/// 正確には event control はステートメントではない．
//////////////////////////////////////////////////////////////////////
class EiEventControl :
  public EiControl
{
public:

  /// @brief コンストラクタ
  EiEventControl(
    const AstControl& ast_control,          ///< [in] パース木の定義要素
    const std::vector<ElbExpr*>& event_list ///< [in] イベントリスト
  ) : EiControl(ast_control),
      mEventList{event_list}
  {
  }

  /// @brief デストラクタ
  ~EiEventControl() = default;


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

  /// @brief イベント条件式の数を返す．
  SizeType
  event_num() const override;

  /// @brief イベント条件式を返す．
  const VlExpr*
  event(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < event_num() )
  ) const override;

  /// @brief イベント条件式のリストを返す．
  std::vector<const VlExpr*>
  event_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // イベントリスト
  std::vector<ElbExpr*> mEventList;

};

END_NAMESPACE_YM_VERILOG

#endif // EIEVENTCONTROL_H
