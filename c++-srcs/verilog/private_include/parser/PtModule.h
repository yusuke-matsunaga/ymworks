#ifndef PTMODULE_H
#define PTMODULE_H

/// @file PtModule.h
/// @brief PtModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

//#include "ym/vl/AstModule.h"
#include "parser/PtBase.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtDeclHead;
class PtPort;
class PtIOHead;
class PtItem;

//////////////////////////////////////////////////////////////////////
/// @class PtModule PtModule.h "parser/PtModule.h"
/// @brief AstModule の実装クラス
//////////////////////////////////////////////////////////////////////
class PtModule :
  public PtBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtModule の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  /// @return 名前
  virtual
  const char*
  name() const = 0;

  /// @brief macromodule 情報の取得
  /// @retval true macromodule の場合
  /// @retval false module の場合
  virtual
  bool
  is_macromodule() const = 0;

  /// @brief cell 情報の取得
  /// @retval true `celldefine --- `endcelldefine に挟まれたモジュールの場合
  /// @retval false 上記以外
  virtual
  bool
  is_cell() const = 0;

  /// @brief protect 情報の取得
  /// @return プロテクトされていたら true を返す．
  virtual
  bool
  is_protected() const = 0;

  /// @brief time unit の取得
  /// @return 時間の単位を表す 2 〜 -15 の整数\n
  /// もしくは未定義を表す -16
  virtual
  int
  time_unit() const = 0;

  /// @brief time precision の取得
  /// @return 時間の精度を表す 2 〜 -15 の整数\n
  /// もしくは未定義を表す -16
  virtual
  int
  time_precision() const = 0;

  /// @brief default net type の取得
  /// @return default net type
  virtual
  VpiNetType
  nettype() const = 0;

  /// @brief unconnected drive の取得
  /// @return unconnected drive
  virtual
  VpiUnconnDrive
  unconn_drive() const = 0;

  /// @brief default delay mode の取得
  /// @return default delay mode
  virtual
  VpiDefDelayMode
  delay_mode() const = 0;

  /// @brief default decay time の取得
  /// @return default decay time
  virtual
  int
  decay_time() const = 0;

  /// @brief portfaults 情報の取得
  /// @return true で enable_portfaults を表す．
  virtual
  bool
  portfaults() const = 0;

  /// @brief suppress_faults 情報の取得
  /// @return true で suppress_faults が効いていることを表す．
  virtual
  bool
  suppress_faults() const = 0;

  /// @brief config 情報の取得
  /// @return config 情報
  virtual
  const std::string&
  config() const = 0;

  /// @brief library 情報の取得
  /// @return library 情報
  virtual
  const std::string&
  library() const = 0;

  /// @brief cell 情報の取得
  /// @return cell 情報
  virtual
  const std::string&
  cell() const = 0;

  /// @brief パラメータポート宣言のリストの取得
  virtual
  const PtDeclHead*
  paramport_top() const = 0;

  /// @brief ポートのリストを返す．
  virtual
  const PtPort*
  port_top() const = 0;

  /// @brief 入出力宣言のヘッダのリストを返す．
  virtual
  const PtIOHead*
  iohead_top() const = 0;

  /// @brief 入出力宣言の要素数の取得
  ///
  /// 個々のヘッダが持つ要素数の総和を計算する．
  virtual
  SizeType
  iodecl_num() const = 0;

  /// @brief 宣言ヘッダのリストを返す．
  virtual
  const PtDeclHead*
  declhead_top() const = 0;

  /// @brief item のリストを返す．
  virtual
  const PtItem*
  item_top() const = 0;

  /// @brief top_module フラグを下ろす
  virtual
  void
  clear_topmodule() const = 0;

  /// @brief top module のチェック
  /// @return top module の場合に true を返す．
  virtual
  bool
  is_topmodule() const = 0;

  /// @brief in_use フラグの設定
  virtual
  void
  set_in_use() const = 0;

  /// @brief in_use フラグの解除
  virtual
  void
  reset_in_use() const = 0;

  /// @brief in_use フラグの取得
  virtual
  bool
  is_in_use() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // PTMODULE_H
