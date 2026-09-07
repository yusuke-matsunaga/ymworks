#ifndef EIPORT_H
#define EIPORT_H

/// @file EiPort.h
/// @brief EiPort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/VlPort.h"
#include "ym/vl/AstPort.h"
#include "ym/vl/AstIOItem.h"
#include "elaborator/ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiPort EiPort.h "EiPort.h"
/// @brief VlPort の実装クラスの基底クラス
//////////////////////////////////////////////////////////////////////
class EiPortBase :
  public VlPort
{
public:

  /// @brief コンストラクタ
  EiPortBase(
    const VlModule* parent,  ///< [in] 親のモジュール
    SizeType index,          ///< [in] ポート番号
    ElbExpr* low_conn,       ///< [in] 下位の接続
    VpiDir dir               ///< [in] 向き
  );

  /// @brief デストラクタ
  ~EiPortBase() {}


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlPort の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力の区別を得る．
  VpiDir
  direction() const override;

  /// @brief ビット幅を返す．
  SizeType
  bit_size() const override;

  /// @brief 名前による接続を持つとき true を返す．
  bool
  is_conn_by_name() const override;

  /// @brief 親のモジュールを取出す
  const VlModule*
  module() const override;

  /// @brief ポートリストの何番目のポートかを表すインデックスを返す．
  SizeType
  port_index() const override;

  /// @brief 上位の接続先を返す．
  const VlExpr*
  high_conn() const override;

  /// @brief 下位の接続先を返す．
  const VlExpr*
  low_conn() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // EiPortBaseの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief high_conn を接続する．
  void
  set_high_conn(
    ElbExpr* high_conn,
    bool conn_by_name
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のモジュール
  const VlModule* mModule;

  // ポート番号
  SizeType mIndex;

  // 上位の接続先
  ElbExpr* mHighConn;

  // 下位の接続先
  ElbExpr* mLowConn;

  // 向き
  VpiDir mDirection;

  // 名前による結合の時 true となるフラグ
  bool mConnByName;

};


//////////////////////////////////////////////////////////////////////
/// @class EiPort1 EiPort.h "EiPort.h"
/// @brief AstPort から作られたポートを表すクラス
//////////////////////////////////////////////////////////////////////
class EiPort1 :
  public EiPortBase
{
public:

  /// @brief コンストラクタ
  EiPort1(
    const VlModule* parent,  ///< [in] 親のモジュール
    const AstPort& ast_port, ///< [in] パース木のポート定義
    SizeType index,          ///< [in] ポート番号
    ElbExpr* low_conn,       ///< [in] 下位の接続
    VpiDir dir               ///< [in] 向き
  );

  /// @brief デストラクタ
  ~EiPort1() {}


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlPort の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 明示的に名前がついているとき true を返す．
  bool
  has_explicit_name() const override;

  /// @brief 名前を返す．
  std::string
  name() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木のポート定義
  AstPort mAstPort;

};


//////////////////////////////////////////////////////////////////////
/// @class EiPort2 EiPort.h "EiPort.h"
/// @brief AstIOItem から作られたポートを表すクラス
//////////////////////////////////////////////////////////////////////
class EiPort2 :
  public EiPortBase
{
public:

  /// @brief コンストラクタ
  EiPort2(
    const VlModule* parent,  ///< [in] 親のモジュール
    const AstIOItem& ast_ioitem, ///< [in] パース木のポート定義
    SizeType index,          ///< [in] ポート番号
    ElbExpr* low_conn,       ///< [in] 下位の接続
    VpiDir dir               ///< [in] 向き
  );

  /// @brief デストラクタ
  ~EiPort2() {}


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlPort の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 明示的に名前がついているとき true を返す．
  bool
  has_explicit_name() const override;

  /// @brief 名前を返す．
  std::string
  name() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木のIO要素定義
  AstIOItem mAstIOItem;

};

END_NAMESPACE_YM_VERILOG

#endif // EIPORT_H
