#ifndef ELBMODULE_H
#define ELBMODULE_H

/// @file ElbModule.h
/// @brief ElbModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/VlModule.h"
#include "ym/vl/Ast.h"
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbModule ElbModule.h "ElbModule.h"
/// @brief elaboration 中の module を表すクラス
//////////////////////////////////////////////////////////////////////
class ElbModule :
  public VlModule
{
public:
  //////////////////////////////////////////////////////////////////////
  // ElbModule の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力を追加する．
  virtual
  void
  add_iodecl(
    const VlIODecl* io_decl ///< [in] 入出力宣言要素
  ) = 0;

  /// @brief ポートを追加する．
  ///
  /// low_conn が複数の const VlExpr* の連結の場合
  virtual
  void
  add_port(
    const FileRegion& loc,   ///< [in] ファイル位置
    const char* name,        ///< [in] 名前
    const VlExpr* low_conn,  ///< [in] 下位の接続
    VpiDir dir               ///< [in] 向き
  ) = 0;

  /// @brief ポートの high_conn を接続する．
  virtual
  void
  set_port_high_conn(
    SizeType index,          ///< [in] ポート番号
    const VlExpr* high_conn, ///< [in] 上位の接続の式
    bool conn_by_name        ///< [in] 名前による割り当て時に true とするフラグ
  ) = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBMODULE_H
