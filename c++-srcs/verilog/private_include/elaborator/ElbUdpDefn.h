#ifndef ELBUDPDEFN_H
#define ELBUDPDEFN_H

/// @file ElbUdpDefn.h
/// @brief ElbUdpDefn のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/VlUdpDefn.h"
#include "ym/vl/AstIOHead.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbUdpDefn ElbUdpDefn.h "ElbUdpDefn.h"
/// @brief UDP定義を表すクラス
/// IEEE Std 1364-2001 26.6.14 UDP
//////////////////////////////////////////////////////////////////////
class ElbUdpDefn :
  public VlUdpDefn
{
public:

  /// @brief コンストラクタ
  ElbUdpDefn() = default;

  /// @brief デストラクタ
  ~ElbUdpDefn() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbUdpDefn の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力オブジェクトを追加する．
  virtual
  void
  add_io(
    const AstIOHead& ast_header, ///< [in] パース木のヘッダ
    const AstIOItem& ast_item    ///< [in] パース木の要素
  ) = 0;

  /// @brief 初期値を設定する．
  virtual
  void
  set_initial(
    const AstExpr& init_expr,   ///< [in] 初期値を表す式
    const VlScalarVal& init_val ///< [in] 初期値
  ) = 0;

  /// @brief table entry の内容を追加する．
  virtual
  void
  add_tableentry(
    const AstUdpEntry& ast_udp_entry, ///< [in] パース木の一行分の定義
    const std::vector<VlUdpVal>& vals ///< [in] シンボル値の配列
  ) = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBUDPDEFN_H
