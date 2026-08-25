#ifndef CPTMODULE_H
#define CPTMODULE_H

/// @file CptModule.h
/// @brief CptModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtModule.h"
#include "parser/PtPort.h"
#include "parser/PtDecl.h"
#include "parser/PtItem.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief module を表すノード
//////////////////////////////////////////////////////////////////////
class CptModule :
  public PtModule
{
public:

  /// @brief コンストラクタ
  CptModule(
    const FileRegion& file_region,
    const char* name,
    bool macro,
    bool is_cell,
    bool is_protected,
    int time_unit,
    int time_precision,
    VpiNetType net_type,
    VpiUnconnDrive unconn,
    VpiDefDelayMode delay,
    int decay,
    bool explicit_name,
    bool portfaults,
    bool suppress_faults,
    const std::string& config,
    const std::string& library,
    const std::string& cell,
    const AstDeclHead* paramport_top,
    const AstPort* port_top,
    const AstIOHead* iohead_top,
    const AstDeclHead* declhead_top,
    const AstItem* item_top
  );

  /// @brief デストラクタ
  ~CptModule();


public:
  //////////////////////////////////////////////////////////////////////
  // PtModule の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const override;

  /// @brief 名前の取得
  const char*
  name() const override;

  /// @brief macromodule 情報の取得
  bool
  is_macromodule() const override;

  /// @brief cell 情報の取得
  bool
  is_cell() const override;

  /// @brief protect 情報の取得
  bool
  is_protected() const override;

  /// @brief time unit の取得
  int
  time_unit() const override;

  /// @brief time precision の取得
  int
  time_precision() const override;

  /// @brief default net type の取得
  VpiNetType
  nettype() const override;

  /// @brief unconnected drive の取得
  VpiUnconnDrive
  unconn_drive() const override;

  /// @brief default delay mode の取得
  VpiDefDelayMode
  delay_mode() const override;

  /// @brief default decay time の取得
  int
  decay_time() const override;

  /// @brief portfaults 情報の取得
  bool
  portfaults() const override;

  /// @brief suppress_faults 情報の取得
  bool
  suppress_faults() const override;

  /// @brief config 情報の取得
  const std::string&
  config() const override;

  /// @brief library 情報の取得
  const std::string&
  library() const override;

  /// @brief cell 情報の取得
  const std::string&
  cell() const override;

  /// @brief パラメータポート宣言のリストの取得
  AstDeclHeadList
  paramport_list() const override;

  /// @brief ポートのリストを返す．
  AstPortList
  port_list() const override;

  /// @brief 入出力宣言のヘッダのリストを返す．
  AstIOHeadList
  iohead_list() const override;

  /// @brief 入出力宣言の要素数の取得
  ///
  /// 個々のヘッダが持つ要素数の総和を計算する．
  SizeType
  iodecl_num() const override;

  /// @brief 宣言ヘッダのリストを返す．
  AstDeclHeadList
  declhead_list() const override;

  /// @brief item のリストを返す．
  AstItemList
  item_list() const override;

  /// @brief top_module フラグを下ろす．
  void
  clear_topmodule() const override;

  /// @brief top module のチェック
  bool
  is_topmodule() const override;

  /// @brief in_use フラグの設定
  void
  set_in_use() const override;

  /// @brief in_use フラグの解除
  void
  reset_in_use() const override;

  /// @brief in_use フラグの取得
  bool
  is_in_use() const override;


private:

  // すべてのポートが名前を持っていることを記録する．
  void
  set_named_port();

  // すべてのポートが外部名を持っているときに true を返す．
  // { a, b } のような名無しポートがあると false となる．
  // true の時しか名前による結合は行えない．
  bool
  explicit_name() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 名前
  const char* mName;

  // 様々な情報をパックしたもの
  mutable
  std::uint32_t mFlags;

  // decay time
  std::int32_t mDefDecayTime;

  // config 情報
  std::string mConfig;

  // library 情報
  std::string mLibrary;

  // cell 情報
  std::string mCell;

  // パラメータポート宣言の先頭
  const AstDeclHead* mParamPortTop;

  // ポートの先頭
  const AstPort* mPortTop;

  // 入出力宣言の先頭
  const AstIOHead* mIOHeadTop;

  // 入出力宣言の要素数
  SizeType mIODeclNum;

  // 宣言の先頭
  const AstDeclHead* mDeclHeadTop;

  // 要素の先頭
  const AstItem* mItemTop;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTMODULE_H
