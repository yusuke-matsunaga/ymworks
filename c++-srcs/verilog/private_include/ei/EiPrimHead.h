#ifndef EIPRIMHEAD_H
#define EIPRIMHEAD_H

/// @file EiPrimHead.h
/// @brief EiPrimitive のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbPrimHead.h"
#include "ym/ClibCell.h"
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
  );

  /// @brief デストラクタ
  ~EiPrimHead();


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
  );

  /// @brief デストラクタ
  ~EiPrimHeadD();


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


//////////////////////////////////////////////////////////////////////
/// @class EiPrimHeadU EiPrimHead.h "EiPrimHead.h"
/// @brief UDP 型の EiPrimHead
//////////////////////////////////////////////////////////////////////
class EiPrimHeadU :
  public EiPrimHead
{
public:

  /// @brief コンストラクタ
  EiPrimHeadU(
    const VlScope* parent,     ///< [in] 親のスコープ
    const AstItem& ast_header, ///< [in] パース木の定義
    const VlUdpDefn* udp       ///< [in] 定義
  );

  /// @brief デストラクタ
  ~EiPrimHeadU();


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

  /// @brief UDP 定義を返す．
  const VlUdpDefn*
  udp_defn() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // UDP 定義
  const VlUdpDefn* mUdp;

};


//////////////////////////////////////////////////////////////////////
/// @class EiPrimHeadUD EiPrimHead.h "EiPrimHead.h"
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
  );

  /// @brief デストラクタ
  ~EiPrimHeadUD();


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


//////////////////////////////////////////////////////////////////////
/// @class EiPrimHeadC EiPrimHead.h "EiPrimHead.h"
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
  );

  /// @brief デストラクタ
  ~EiPrimHeadC();


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

#endif // EIPRIMHEAD_H
