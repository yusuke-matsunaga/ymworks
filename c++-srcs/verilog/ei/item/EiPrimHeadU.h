#ifndef EIPRIMHEADU_H
#define EIPRIMHEADU_H

/// @file EiPrimHeadU.h
/// @brief EiPrimHeadU のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiPrimHead.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiPrimHeadU EiPrimHeadU.h "EiPrimHeadU.h"
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
  ) : EiPrimHead(parent, ast_header),
      mUdp{udp}
  {
  }

  /// @brief デストラクタ
  ~EiPrimHeadU() = default;


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

END_NAMESPACE_YM_VERILOG

#endif // EIPRIMHEADU_H
