#ifndef EIMODULEBASE_H
#define EIMODULEBASE_H

/// @file EiModuleBase.h
/// @brief EiModuleBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbModule.h"


BEGIN_NAMESPACE_YM_VERILOG

class EiModuleHead;
class EiPortBase;

//////////////////////////////////////////////////////////////////////
/// @class EiModuleBase EiModuleBase.h "EiModuleBase.h"
/// @brief ElbModule の実装クラスの基底クラス
//////////////////////////////////////////////////////////////////////
class EiModuleBase :
  public ElbModule
{
protected:

  /// @brief コンストラクタ
  EiModuleBase() = default;

  /// @brief デストラクタ
  ~EiModuleBase() = default;


public:

  /// @brief ポート配列とIO配列を初期化する．
  void
  init(
    SizeType port_num, ///< [in] ポート数
    SizeType io_num    ///< [in] IO 数
  );


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
  // VlNamedObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  const VlScope*
  parent_scope() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlModule の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief definition location を返す．
  /// @return 定義側のファイル位置の情報を返す．
  FileRegion
  def_file_region() const override;

  /// @brief definition name を返す．
  /// @return 定義名を返す．
  std::string
  def_name() const override;

  /// @brief cell instance のチェック
  /// @return cell instance の場合に true を返す．
  bool
  is_cell_instance() const override;

  /// @brief protect のチェック
  /// @return protect されていたら true を返す．
  bool
  is_protected() const override;

  /// @brief top module の時 true を返す．
  bool
  is_top_module() const override;

  /// @brief time unit を返す．
  /// @return 結果は 2 〜 -15 の整数
  /// @return もしくは未定義を表す -16
  int
  time_unit() const override;

  /// @brief time precision を返す．
  /// @return 結果は 2 〜 -15 の整数
  /// @return もしくは未定義を表す -16
  int
  time_precision() const override;

  /// @brief default net type を返す．
  VpiNetType
  def_net_type() const override;

  /// @brief unconnected drive を返す．
  VpiUnconnDrive
  unconn_drive() const override;

  /// @brief default delay mode を返す．
  VpiDefDelayMode
  def_delay_mode() const override;

  /// @brief default decay time を返す．
  int
  def_decay_time() const override;

  /// @brief config 情報を返す．
  std::string
  config() const override;

  /// @brief library 情報を返す．
  std::string
  library() const override;

  /// @brief cell 情報を返す．
  std::string
  cell() const override;

  /// @brief ポート数を返す．
  SizeType
  port_num() const override;

  /// @brief ポートの取得
  /// @return pos 番目のポートを返す．
  const VlPort*
  port(
    SizeType pos ///< [in] 取得するポートの位置 (0 <= pos < port_num())
  ) const override;

  /// @brief ポートのリストの取得
  std::vector<const VlPort*>
  port_list() const override;

  /// @brief 入出力数を得る．
  SizeType
  io_num() const override;

  /// @brief 入出力の取得
  const VlIODecl*
  io(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < io_num() )
  ) const override;

  /// @brief 入出力のリストの取得
  std::vector<const VlIODecl*>
  io_list() const override;

  /// @brief 宣言要素に対応する入出力を得る．
  ///
  /// 対応する入出力がない場合は nullptr を返す．
  const VlIODecl*
  find_io(
    const VlDecl* decl ///< [in] 対象の宣言要素
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbModule の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力を追加する．
  void
  add_iodecl(
    const VlIODecl* io_decl ///< [in] 入出力宣言要素
  ) override;

  /// @brief 名前付きポートを追加する．
  void
  add_port(
    const FileRegion& loc,  ///< [in] ファイル位置
    const char* name,       ///< [in] 名前
    const VlTerm* low_conn, ///< [in] 下位の接続
    VpiDir dir              ///< [in] 向き
  ) override;

  /// @brief ポートの high_conn を接続する．
  void
  set_port_high_conn(
    SizeType index,     ///< [in] ポート番号
    ElbExpr* high_conn, ///< [in] 上位の接続の式
    bool conn_by_name   ///< [in] 名前による割り当て時に true とするフラグ
  ) override;

  /// @brief ポートの high_conn を接続する．
  void
  set_port_high_conn(
    SizeType index,          ///< [in] ポート番号
    const VlTerm* high_conn, ///< [in] 上位の接続の式
    bool conn_by_name        ///< [in] 名前による割り当て時に true とするフラグ
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // EiModuleBase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ヘッダ情報を返す．
  virtual
  const EiModuleHead&
  head() const = 0;

  /// @brief ヘッダ情報を返す．
  virtual
  EiModuleHead&
  head() = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief add_port() の共通処理
  void
  add_port_sub(
    std::function<EiPortBase*(SizeType)> new_func
  )
  {
    auto port_index = mPortList.size();
    auto port = new_func(port_index);
    mPortList.push_back(std::unique_ptr<EiPortBase>(port));
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ポートの配列
  std::vector<std::unique_ptr<EiPortBase>> mPortList;

  // 入出力の配列
  std::vector<std::unique_ptr<const VlIODecl>> mIODeclList;

  // 宣言要素と入出力の対応を持つ辞書
  std::unordered_map<const VlDecl*, const VlIODecl*> mIODict;

};

END_NAMESPACE_YM_VERILOG

#endif // EIMODULEBASE_H
