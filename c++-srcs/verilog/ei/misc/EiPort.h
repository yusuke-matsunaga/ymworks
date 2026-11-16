#ifndef EIPORT_H
#define EIPORT_H

/// @file EiPort.h
/// @brief EiPort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/VlPort.h"
#include "elaborator/ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiPort EiPort.h "EiPort.h"
/// @brief VlPort の実装クラスの基底クラス
//////////////////////////////////////////////////////////////////////
class EiPort :
  public VlPort
{
public:

  /// @brief コンストラクタ
  EiPort(
    const FileRegion& loc,  ///< [in] ファイル位置
    const VlModule* parent, ///< [in] 親のモジュール
    SizeType index,         ///< [in] ポート番号
    const VlExpr* low_conn, ///< [in] 下位の接続
    VpiDir dir              ///< [in] 向き
  );

  /// @brief デストラクタ
  ~EiPort() {}


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const override;


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

  /// @brief 明示的に名前がついているとき true を返す．
  bool
  has_explicit_name() const override;

  /// @brief 名前を返す．
  std::string
  name() const override;

  /// @brief 親のモジュールを取出す
  const VlModule*
  module() const override;

  /// @brief ポートリストの何番目のポートかを表すインデックスを返す．
  SizeType
  port_index() const override;

  /// @brief 上位の接続先を返す(入力ポート用)．
  const VlExpr*
  high_conn() const override;

  /// @brief 下位の接続先を返す．
  const VlExpr*
  low_conn() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief high_conn を接続する．
  void
  set_high_conn(
    const VlExpr* high_conn,
    bool conn_by_name
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 親のモジュール
  const VlModule* mModule;

  // ポート番号+向き+conn_by_name の情報をパックした変数
  //    0: conn_by_name フラグ
  //  1-2: 向き(NoDirectionは使わない)
  // 3-63: インデックス
  std::uint64_t mChunk{0};

  // 上位の接続先
  const VlExpr* mHighConn;

  // 下位の接続先
  const VlExpr* mLowConn;

};

END_NAMESPACE_YM_VERILOG

#endif // EIPORT_H
