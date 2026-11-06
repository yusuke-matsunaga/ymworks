#ifndef EIPRIMHEAD_H
#define EIPRIMHEAD_H

/// @file EiPrimHead.h
/// @brief EiPrimitive のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbPrimHead.h"
#include "ym/vl/AstItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiPrimHead EiPrimHead.h "ei/EiPrimHead.h"
/// @brief primitive/primitive array のヘッダ情報
//////////////////////////////////////////////////////////////////////
class EiPrimHead :
  public ElbPrimHead
{
public:

  /// @brief コンストラクタ
  EiPrimHead(
    const VlScope* parent,    ///< [in] 親のスコープ
    const AstItem& ast_header ///< [in] パース木の定義
  ) : mParent{parent},
      mAstHead{ast_header}
  {
  }

  /// @brief デストラクタ
  ~EiPrimHead() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbPrimHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  const VlScope*
  parent_scope() const override;

  /// @brief primitive type を返す．
  VpiPrimType
  prim_type() const override;

  /// @brief プリミティブの定義名を返す．
  std::string
  def_name() const override;

  /// @brief UDP 定義を返す．
  const VlUdpDefn*
  udp_defn() const override;

  /// @brief セルを返す．
  ClibCell
  cell() const override;

  /// @brief 0 の強さを得る．
  VpiStrength
  drive0() const override;

  /// @brief 1 の強さを得る．
  VpiStrength
  drive1() const override;

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

  // 親のスコープ
  const VlScope* mParent;

  // パース木の定義
  AstItem mAstHead;

};

END_NAMESPACE_YM_VERILOG

#endif // EIPRIMHEAD_H
