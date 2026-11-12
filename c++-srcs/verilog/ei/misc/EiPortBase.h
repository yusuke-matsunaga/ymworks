#ifndef EIPORTBASE_H
#define EIPORTBASE_H

/// @file EiPortBase.h
/// @brief EiPortBase のヘッダファイル
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
/// @class EiPortBase EiPortBase.h "EiPortBase.h"
/// @brief VlPort の実装クラスの基底クラス
//////////////////////////////////////////////////////////////////////
class EiPortBase :
  public VlPort
{
public:

  /// @brief コンストラクタ
  EiPortBase(
    const FileRegion& loc,  ///< [in] ファイル位置
    const VlModule* parent, ///< [in] 親のモジュール
    SizeType index,         ///< [in] ポート番号
    const VlTerm* low_conn, ///< [in] 下位の接続
    VpiDir dir              ///< [in] 向き
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
  high_conn_expr() const override;

  /// @brief 上位の接続先を返す(出力ポート用)．
  const VlTerm*
  high_conn_term() const override;

  /// @brief 下位の接続先を返す．
  const VlTerm*
  low_conn() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief high_conn を接続する．
  virtual
  void
  set_high_conn(
    ElbExpr* high_conn,
    bool conn_by_name
  );

  /// @brief high_conn を接続する．
  virtual
  void
  set_high_conn(
    const VlTerm* high_conn,
    bool conn_by_name
  );


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief conn_by_name 属性を設定する．
  void
  set_conn_by_name(
    bool conn_by_name
  )
  {
    if ( conn_by_name ) {
      mChunk |= 1ULL;
    }
  }


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

  // 下位の接続先
  const VlTerm* mLowConn;

};

END_NAMESPACE_YM_VERILOG

#endif // EIPORTBASE_H
