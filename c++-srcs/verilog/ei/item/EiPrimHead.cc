
/// @file EiPrimHead.cc
/// @brief EiPrimHead の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiPrimHead.h"

#include "ym/vl/VlUdpDefn.h"
#include "ym/vl/VlDelay.h"
#include "ym/ClibCell.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief ゲートプリミティブのヘッダを生成する．
ElbPrimHead*
EiFactory::new_PrimHead(
  const VlScope* parent,
  const AstItem& ast_header,
  bool has_delay
)
{
  if ( has_delay ) {
    return new EiPrimHeadD(parent, ast_header);
  }
  else {
    return new EiPrimHead(parent, ast_header);
  }
}

// @brief UDPプリミティブのヘッダを生成する．
ElbPrimHead*
EiFactory::new_UdpHead(
  const VlScope* parent,
  const AstItem& ast_header,
  const VlUdpDefn* udp,
  bool has_delay
)
{
  if ( has_delay ) {
    return new EiPrimHeadUD(parent, ast_header, udp);
  }
  else {
    return new EiPrimHeadU(parent, ast_header, udp);
  }
}

// @brief セルプリミティブのヘッダを生成する．
ElbPrimHead*
EiFactory::new_CellHead(
  const VlScope* parent,
  const AstItem& ast_header,
  const ClibCell& cell
)
{
  return new EiPrimHeadC(parent, ast_header, cell);
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiPrimHead::EiPrimHead(
  const VlScope* parent,
  const AstItem& ast_header
) : mParent{parent},
    mAstHead{ast_header}
{
}

// @brief デストラクタ
EiPrimHead::~EiPrimHead()
{
}

// @brief このオブジェクトの属しているスコープを返す．
const VlScope*
EiPrimHead::parent_scope() const
{
  return mParent;
}

// @brief primitive type を返す．
VpiPrimType
EiPrimHead::prim_type() const
{
  return mAstHead.prim_type();
}

// @brief プリミティブの定義名を返す．
std::string
EiPrimHead::def_name() const
{
  std::string nm;
  switch ( prim_type() ) {
  case VpiPrimType::And:      nm = "and"; break;
  case VpiPrimType::Nand:     nm = "nand"; break;
  case VpiPrimType::Nor:      nm = "nor"; break;
  case VpiPrimType::Or:       nm = "or"; break;
  case VpiPrimType::Xor:      nm = "xor"; break;
  case VpiPrimType::Xnor:     nm = "xnor"; break;
  case VpiPrimType::Buf:      nm = "buf"; break;
  case VpiPrimType::Not:      nm = "not"; break;
  case VpiPrimType::Bufif0:   nm = "bufif0"; break;
  case VpiPrimType::Bufif1:   nm = "bufif1"; break;
  case VpiPrimType::Notif0:   nm = "notif0"; break;
  case VpiPrimType::Notif1:   nm = "notif1"; break;
  case VpiPrimType::Nmos:     nm = "nmos"; break;
  case VpiPrimType::Pmos:     nm = "pmos"; break;
  case VpiPrimType::Cmos:     nm = "cmos"; break;
  case VpiPrimType::Rnmos:    nm = "rnmos"; break;
  case VpiPrimType::Rpmos:    nm = "rpmos"; break;
  case VpiPrimType::Rcmos:    nm = "rcmos"; break;
  case VpiPrimType::Rtran:    nm = "rtran"; break;
  case VpiPrimType::Rtranif0: nm = "rtranif0"; break;
  case VpiPrimType::Rtranif1: nm = "rtranif1"; break;
  case VpiPrimType::Tran:     nm = "tran"; break;
  case VpiPrimType::Tranif0:  nm = "tranif0"; break;
  case VpiPrimType::Tranif1:  nm = "tranif1"; break;
  case VpiPrimType::Pullup:   nm = "pullup"; break;
  case VpiPrimType::Pulldown: nm = "pulldown"; break;
  case VpiPrimType::Cell:     nm = "cell"; break;
  case VpiPrimType::Seq:
  case VpiPrimType::Comb:
    ASSERT_NOT_REACHED;
    break;
  }
  return nm;
}

// @brief UDP 定義を返す．
const VlUdpDefn*
EiPrimHead::udp_defn() const
{
  return nullptr;
}

// @brief セルを返す．
ClibCell
EiPrimHead::cell() const
{
  // 不正値
  return {};
}

// @brief 0 の強さを得る．
VpiStrength
EiPrimHead::drive0() const
{
  if ( mAstHead.strength().is_invalid() ) {
    return VpiStrength::NoStrength;
  }
  return mAstHead.strength().drive0();
}

// @brief 1 の強さを得る．
VpiStrength
EiPrimHead::drive1() const
{
  if ( mAstHead.strength().is_invalid() ) {
    return VpiStrength::NoStrength;
  }
  return mAstHead.strength().drive1();
}

// @brief 遅延式を得る．
const VlDelay*
EiPrimHead::delay() const
{
  return nullptr;
}

// @brief 遅延式を設定する．
void
EiPrimHead::set_delay(
  const VlDelay* expr
)
{
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimHeadD
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiPrimHeadD::EiPrimHeadD(
  const VlScope* parent,
  const AstItem& ast_header
) : EiPrimHead(parent, ast_header)
{
}

// @brief デストラクタ
EiPrimHeadD::~EiPrimHeadD()
{
}

// @brief 遅延式を得る．
const VlDelay*
EiPrimHeadD::delay() const
{
  return mDelay;
}

// @brief 遅延式を設定する．
void
EiPrimHeadD::set_delay(
  const VlDelay* expr
)
{
  mDelay = expr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimHeadU
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiPrimHeadU::EiPrimHeadU(
  const VlScope* parent,
  const AstItem& ast_header,
  const VlUdpDefn* udp
) : EiPrimHead(parent, ast_header),
    mUdp{udp}
{
}

// @brief デストラクタ
EiPrimHeadU::~EiPrimHeadU()
{
}

// @brief primitive type を返す．
VpiPrimType
EiPrimHeadU::prim_type() const
{
  return mUdp->prim_type();
}

// @brief プリミティブの定義名を返す．
std::string
EiPrimHeadU::def_name() const
{
  return mUdp->def_name();
}

// @brief UDP 定義を返す．
const VlUdpDefn*
EiPrimHeadU::udp_defn() const
{
  return mUdp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimHeadUD
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiPrimHeadUD::EiPrimHeadUD(
  const VlScope* parent,
  const AstItem& ast_header,
  const VlUdpDefn* udp
) : EiPrimHeadU(parent, ast_header, udp)
{
}

// @brief デストラクタ
EiPrimHeadUD::~EiPrimHeadUD()
{
}

// @brief 遅延式を得る．
const VlDelay*
EiPrimHeadUD::delay() const
{
  return mDelay;
}

// @brief 遅延式を設定する．
void
EiPrimHeadUD::set_delay(
  const VlDelay* expr
)
{
  mDelay = expr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimHeadC
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiPrimHeadC::EiPrimHeadC(
  const VlScope* parent,
  const AstItem& ast_header,
  const ClibCell& cell
) : EiPrimHead(parent, ast_header),
    mCell{cell}
{
}

// @brief デストラクタ
EiPrimHeadC::~EiPrimHeadC()
{
}

// @brief primitive type を返す．
VpiPrimType
EiPrimHeadC::prim_type() const
{
  return VpiPrimType::Cell;
}

// @brief プリミティブの定義名を返す．
std::string
EiPrimHeadC::def_name() const
{
  return mCell.name();
}

// @brief セル番号を返す．
ClibCell
EiPrimHeadC::cell() const
{
  return mCell;
}

END_NAMESPACE_YM_VERILOG
