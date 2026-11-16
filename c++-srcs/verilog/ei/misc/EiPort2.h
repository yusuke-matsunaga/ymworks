#ifndef EIPORT2_H
#define EIPORT2_H

/// @file EiPort2.h
/// @brief EiPort2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiPortBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiPort2 EiPort2.h "EiPort2.h"
/// @brief low_conn が複数要素の連結の EiPort
//////////////////////////////////////////////////////////////////////
class EiPort2 :
  public EiPortBase
{
public:

  /// @brief コンストラクタ
  EiPort2(
    const FileRegion& loc,                      ///< [in] ファイル位置
    const VlModule* parent,                     ///< [in] 親のモジュール
    SizeType index,                             ///< [in] ポート番号
    const std::vector<const VlExpr*>& low_conn, ///< [in] 下位の接続
    VpiDir dir                                  ///< [in] 向き
  ) : EiPortBase(loc, parent, index, dir),
      mLowConn{low_conn}
  {
  }

  /// @brief デストラクタ
  ~EiPort2() {}


public:
  //////////////////////////////////////////////////////////////////////
  // VlPort の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ビット幅を返す．
  SizeType
  bit_size() const override;

  /// @brief 下位の接続の要素数を返す．
  SizeType
  low_conn_size() const override;

  /// @brief 下位の接続先を返す．
  const VlExpr*
  low_conn(
    SizeType index ///< [in] インデックス ( 0 <= index < low_conn_size() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 下位の接続先
  std::vector<const VlExpr*> mLowConn;

};

END_NAMESPACE_YM_VERILOG

#endif // EIPORT_H
