#ifndef EIINPUTPORT_H
#define EIINPUTPORT_H

/// @file EiInputPort.h
/// @brief EiInputPort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiPortBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiInputPort EiInputPort.h "EiInputPort.h"
/// @brief 入力タイプの EiPortBase
///
/// high_conn が ElbExpr*
//////////////////////////////////////////////////////////////////////
class EiInputPort :
  public EiPortBase
{
public:

  /// @brief コンストラクタ
  EiInputPort(
    const FileRegion& loc,  ///< [in] ファイル位置
    const VlModule* parent, ///< [in] 親のモジュール
    SizeType index,         ///< [in] ポート番号
    const VlTerm* low_conn, ///< [in] 下位の接続
    VpiDir dir              ///< [in] 向き
  ) : EiPortBase(loc, parent, index, low_conn, dir),
      mHighConn{nullptr}
  {
  }

  /// @brief デストラクタ
  ~EiInputPort() {}


public:
  //////////////////////////////////////////////////////////////////////
  // VlPort の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 上位の接続先を返す(入力ポート用)．
  const VlExpr*
  high_conn_expr() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // EiPortBaseの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief high_conn を接続する．
  void
  set_high_conn(
    ElbExpr* high_conn,
    bool conn_by_name
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 上位の接続先
  ElbExpr* mHighConn;

};

END_NAMESPACE_YM_VERILOG

#endif // EIINPUTPORT_H
