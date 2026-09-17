#ifndef EIUDPDEFN_H
#define EIUDPDEFN_H

/// @file EiUdpDefn.h
/// @brief EiUdpDefn のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

// UDP定義を表すクラス
// IEEE Std 1364-2001 26.6.14 UDP

#include "elaborator/ElbUdpDefn.h"
#include "ym/vl/VlIODecl.h"
#include "ym/vl/VlScalarVal.h"
#include "ym/vl/AstUdpEntry.h"


BEGIN_NAMESPACE_YM_VERILOG

class EiUdpIO;
class EiTableEntry;

//////////////////////////////////////////////////////////////////////
/// @class EiUdpDefn ElbUdpDefn.h "ElbUdpDefn.h"
/// @brief ElbUdpDefn の実装クラス
//////////////////////////////////////////////////////////////////////
class EiUdpDefn :
  public ElbUdpDefn
{
public:

  /// @brief コンストラクタ
  EiUdpDefn(
    const AstUdp& ast_udp, ///< [in] パース木の UDP 定義
    bool is_protected,     ///< [in] プロテクト属性
    SizeType io_num,       ///< [in] ポート数
    SizeType table_num     ///< [in] テーブルの行数
  );

  /// @brief デストラクタ
  ~EiUdpDefn();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlUdpDefn の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定義された名前を返す．
  std::string
  def_name() const override;

  /// @brief primitive type を返す．
  VpiPrimType
  prim_type() const override;

  /// @brief ポート数を返す．
  SizeType
  port_num() const override;

  /// @brief 入力の宣言要素を返す．
  const VlIODecl*
  input(
    SizeType pos ///< [in] 入力番号 ( 0 <= pos < port_num() - 1 )
  ) const override;

  /// @brief 出力の宣言要素を返す．
  const VlIODecl*
  output() const override;

  /// @brief protected かどうかを返す．
  bool
  is_protected() const override;

  /// @brief 初期値を返す．
  /// @return 0/1/X を返す．
  VlScalarVal
  init_val() const override;

  /// @brief 初期値を表す文字列を返す．
  std::string
  init_val_string() const override;

  /// @brief table entry の行数を返す．
  SizeType
  table_size() const override;

  /// @brief table entry を返す．
  const VlTableEntry*
  table_entry(
    SizeType pos ///< [in] 行番号
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力オブジェクトを追加する．
  void
  add_io(
    const AstIOHead& ast_header, ///< [in] パース木の宣言ヘッダ
    const AstIOItem& ast_item    ///< [in] パース木の宣言要素
  ) override;

  /// @brief 初期値を設定する．
  void
  set_initial(
    const AstExpr& init_expr,   ///< [in] 初期値を表す式
    const VlScalarVal& init_val ///< [in] 初期値
  ) override;

  /// @brief table entry の内容を追加する．
  void
  add_tableentry(
    const AstUdpEntry& ast_udp_entry, ///< [in] パース木の一行分の定義
    const std::vector<VlUdpVal>& vals ///< [in] シンボル値の配列
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の UDP 定義
  AstUdp mAstUdp;

  // ポート数
  SizeType mPortNum;

  // protect されているとき true にするフラグ
  bool mProtected;

  // IO decl のリスト(配列)
  std::vector<EiUdpIO> mIODeclList;

  // 初期値の式
  AstExpr mInitExpr;

  // 初期値
  VlScalarVal mInitVal;

  // table entry の配列
  std::vector<EiTableEntry> mTableEntryList;

};

END_NAMESPACE_YM_VERILOG

#endif // EIUDPDEFN_H
