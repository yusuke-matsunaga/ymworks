#ifndef YM_VL_ASTMODULE_H
#define YM_VL_ASTMODULE_H

/// @file AstModule.h
/// @brief AstModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtModule;

//////////////////////////////////////////////////////////////////////
/// @class AstModule AstModule.h "ym/vl/AstModule.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief module を表すノード
//////////////////////////////////////////////////////////////////////
class AstModule :
  public AstNamedBaseWithPtr<const PtModule>
{
public:

  /// @brief コンストラクタ
  explicit
  AstModule(
    const PtModule* ptr = nullptr ///< [in] 実体のポインタ
  ) : AstNamedBaseWithPtr<const PtModule>(ptr)
  {
  }

  /// @brief デストラクタ
  ~AstModule() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief macromodule 情報の取得
  /// @retval true macromodule の場合
  /// @retval false module の場合
  bool
  is_macromodule() const;

  /// @brief cell 情報の取得
  /// @retval true `celldefine --- `endcelldefine に挟まれたモジュールの場合
  /// @retval false 上記以外
  bool
  is_cell() const;

  /// @brief protect 情報の取得
  /// @return プロテクトされていたら true を返す．
  bool
  is_protected() const;

  /// @brief time unit の取得
  /// @return 時間の単位を表す 2 〜 -15 の整数\n
  /// もしくは未定義を表す -16
  int
  time_unit() const;

  /// @brief time precision の取得
  /// @return 時間の精度を表す 2 〜 -15 の整数\n
  /// もしくは未定義を表す -16
  int
  time_precision() const;

  /// @brief default net type の取得
  /// @return default net type
  VpiNetType
  nettype() const;

  /// @brief unconnected drive の取得
  /// @return unconnected drive
  VpiUnconnDrive
  unconn_drive() const;

  /// @brief default delay mode の取得
  /// @return default delay mode
  VpiDefDelayMode
  delay_mode() const;

  /// @brief default decay time の取得
  /// @return default decay time
  int
  decay_time() const;

  /// @brief portfaults 情報の取得
  /// @return true で enable_portfaults を表す．
  bool
  portfaults() const;

  /// @brief suppress_faults 情報の取得
  /// @return true で suppress_faults が効いていることを表す．
  bool
  suppress_faults() const;

  /// @brief config 情報の取得
  /// @return config 情報
  const std::string&
  config() const;

  /// @brief library 情報の取得
  /// @return library 情報
  const std::string&
  library() const;

  /// @brief cell 情報の取得
  /// @return cell 情報
  const std::string&
  cell() const;

  /// @brief パラメータポート宣言のリストの取得
  AstDeclHeadList
  paramport_list() const;

  /// @brief ポートのリストを返す．
  AstPortList
  port_list() const;

  /// @brief 入出力宣言のヘッダのリストを返す．
  AstIOHeadList
  iohead_list() const;

  /// @brief 入出力宣言の要素数の取得
  ///
  /// 個々のヘッダが持つ要素数の総和を計算する．
  SizeType
  iodecl_num() const;

  /// @brief 宣言ヘッダのリストを返す．
  AstDeclHeadList
  declhead_list() const;

  /// @brief item のリストを返す．
  AstItemList
  item_list() const;

  /// @brief top_module フラグを下ろす
  void
  clear_topmodule() const;

  /// @brief top module のチェック
  /// @return top module の場合に true を返す．
  bool
  is_topmodule() const;

  /// @brief in_use フラグの設定
  void
  set_in_use() const;

  /// @brief in_use フラグの解除
  void
  reset_in_use() const;

  /// @brief in_use フラグの取得
  bool
  is_in_use() const;

  /// @brief ハッシュ値を返す．
  SizeType
  hash() const;


public:
  //////////////////////////////////////////////////////////////////////
  // AstBase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 適切な値を持っている時 true を返す．
  bool
  is_valid() const override;

  /// @brief ファイル位置の取得
  /// @return ファイル位置
  FileRegion
  file_region() const override;

  /// @brief 比較用のユニークなキーを返す．
  PtrIntType
  key() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // AstNamedBase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  /// @return 名前
  const char*
  name() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief json_obj() の下請け関数
  void
  json_sub(
    JsonValue& jobj ///< [in] 対象の JSON オブジェクト
  ) const override;

};

END_NAMESPACE_YM_VERILOG

BEGIN_NAMESPACE_STD

// const AstModule をキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::nsVerilog::AstModule>
{
  SizeType
  operator()(const YM_NAMESPACE::nsVerilog::AstModule& module) const
  {
    return module.hash();
  }
};

END_NAMESPACE_STD

#endif // YM_VL_ASTMODULE_H
