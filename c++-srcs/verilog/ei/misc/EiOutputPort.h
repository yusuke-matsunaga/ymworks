#ifndef EIOUTPUTPORT_H
#define EIOUTPUTPORT_H

/// @file EiOutputPort.h
/// @brief EiOutputPort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiPortBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiOutPortBase EiOutputPort.h "EiOutputPort.h"
/// @brief 出力タイプの EiPortBase
//////////////////////////////////////////////////////////////////////
class EiOutputPort :
  public EiPortBase
{
public:

  /// @brief コンストラクタ
  EiOutputPort(
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
  ~EiOutputPort() {}


public:
  //////////////////////////////////////////////////////////////////////
  // VlPort の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 上位の接続先を返す．
  const VlTerm*
  high_conn_term() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // EiOutputPortの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief high_conn を接続する．
  void
  set_high_conn(
    const VlTerm* high_conn,
    bool conn_by_name
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 上位の接続先
  const VlTerm* mHighConn;

};

END_NAMESPACE_YM_VERILOG

#endif // EIOUTPUTPORT_H
