#ifndef EIMODULEHEAD_H
#define EIMODULEHEAD_H

/// @file EiModuleHead.h
/// @brief EiModuleHead のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbModule.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @calss EiModuleHead EiModuleHead.h "EiModuleHead.h"
/// @brief EiModuleArray と EiModule に共通なヘッダ部分
//////////////////////////////////////////////////////////////////////
class EiModuleHead
{
public:

  /// @brief コンストラクタ
  EiModuleHead(
    const VlScope* parent,       ///< [in] 親のスコープ
    const AstModule& ast_module, ///< [in] モジュールテンプレート
    const AstItem& ast_head,     ///< [in] パース木のヘッダ定義
    const AstInst& ast_inst      ///< [in] インスタンス定義
  ) : mParent{parent},
      mAstModule{ast_module},
      mAstHead{ast_head},
      mAstInst{ast_inst}
  {
  }

  /// @brief デストラクタ
  ~EiModuleHead() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // EiModuleHead の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  const VlScope*
  parent() const;

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const;

  /// @brief インスタンス名を返す．
  std::string
  name() const;

  /// @brief definition location を返す．
  /// @return 定義側のファイル位置の情報を返す．
  FileRegion
  def_file_region() const;

  /// @brief definition name を返す．
  /// @return 定義名を返す．
  std::string
  def_name() const;

  /// @brief ポート数を返す．
  SizeType
  port_num() const;

  /// @brief 入出力宣言数を返す．
  SizeType
  io_num() const;

  /// @brief cell instance のチェック
  /// @return cell instance の場合に true を返す．
  bool
  is_cell_instance() const;

  /// @brief protect のチェック
  /// @return protect されていたら true を返す．
  bool
  is_protected() const;

  /// @brief top module の時 true を返す．
  bool
  is_top_module() const;

  /// @brief time unit を返す．
  /// @return 結果は 2 〜 -15 の整数
  /// @return もしくは未定義を表す -16
  int
  time_unit() const;

  /// @brief time precision を返す．
  /// @return 結果は 2 〜 -15 の整数
  /// @return もしくは未定義を表す -16
  int
  time_precision() const;

  /// @brief default net type を返す．
  VpiNetType
  def_net_type() const;

  /// @brief unconnected drive を返す．
  VpiUnconnDrive
  unconn_drive() const;

  /// @brief default delay mode を返す．
  VpiDefDelayMode
  def_delay_mode() const;

  /// @brief default decay time を返す．
  int
  def_decay_time() const;

  /// @brief config 情報を返す．
  std::string
  config() const;

  /// @brief library 情報を返す．
  std::string
  library() const;

  /// @brief cell 情報を返す．
  std::string
  cell() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlScope* mParent;

  // パース木のモジュール定義
  AstModule mAstModule;

  // パース木のヘッダ定義
  AstItem mAstHead;

  // パース木のモジュールインスタンス定義
  AstInst mAstInst;

};

END_NAMESPACE_YM_VERILOG

#endif // EIMODULEHEAD_H
