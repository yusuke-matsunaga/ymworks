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

//////////////////////////////////////////////////////////////////////
/// @class AstModule AstModule.h "ym/vl/AstModule.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief module を表すノード
//////////////////////////////////////////////////////////////////////
class AstModule :
  public AstNamedBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // AstModule の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

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

  /// @brief パラメータポート宣言配列の要素数の取得
  virtual
  SizeType
  paramport_num() const = 0;

  /// @brief パラメータポート宣言の取得
  ///
  /// - pos >= paramport_num() の時 std::out_of_range 例外を送出する．
  virtual
  const AstDeclHead*
  paramport(
    SizeType pos ///< [in] 位置 ( 0 <= pos < paramport_num() )
  ) const = 0;

  /// @brief パラメータポート宣言のリストを返す．
  std::vector<const AstDeclHead*>
  paramport_list() const
  {
    auto n = paramport_num();
    std::vector<const AstDeclHead*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(paramport(i));
    }
    return vec;
  }

  /// @brief ポート数を取り出す．
  virtual
  SizeType
  port_num() const = 0;

  /// @brief ポートを取り出す．
  ///
  /// - pos >= port_num() の時 std::out_of_range 例外を送出する．
  virtual
  const AstPort*
  port(
    SizeType pos ///< [in] 位置 ( 0 <= pos < port_num() )
  ) const = 0;

  /// @brief ポートのリストを返す．
  std::vector<const AstPort*>
  port_list() const
  {
    auto n = port_num();
    std::vector<const AstPort*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(port(i));
    }
    return vec;
  }

  /// @brief 入出力宣言ヘッダ配列の要素数の取得
  virtual
  SizeType
  iohead_num() const = 0;

  /// @brief 入出力宣言の取得
  ///
  /// - pos >= iohead_num() の時 std::out_of_range 例外を送出する．
  virtual
  const AstIOHead*
  iohead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < iohead_num() )
  ) const = 0;

  /// @brief 入出力宣言のヘッダのリストを返す．
  std::vector<const AstIOHead*>
  iohead_list() const
  {
    auto n = iohead_num();
    std::vector<const AstIOHead*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(iohead(i));
    }
    return vec;
  }

  /// @brief 入出力宣言の要素数の取得
  /// @note 個々のヘッダが持つ要素数の総和を計算する．
  virtual
  SizeType
  iodecl_num() const = 0;

  /// @brief 宣言ヘッダ配列の要素数の取得
  virtual
  SizeType
  declhead_num() const = 0;

  /// @brief 宣言ヘッダの取得
  ///
  /// - pos >= declhead_num() の時 std::out_of_range 例外を送出する．
  virtual
  const AstDeclHead*
  declhead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < declhead_num() )
  ) const = 0;

  /// @brief 宣言ヘッダのリストを返す．
  std::vector<const AstDeclHead*>
  declhead_list() const
  {
    auto n = declhead_num();
    std::vector<const AstDeclHead*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(declhead(i));
    }
    return vec;
  }

  /// @brief item 配列の要素数の取得
  virtual
  SizeType
  item_num() const = 0;

  /// @brief item の取得
  ///
  /// - pos >= item_num() の時 std::out_of_range 例外を送出する．
  virtual
  const AstItem*
  item(
    SizeType pos ///< [in] 位置 ( 0 <= pos < item_num() )
  ) const = 0;

  /// @brief item のリストを返す．
  std::vector<const AstItem*>
  item_list() const
  {
    auto n = item_num();
    std::vector<const AstItem*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(item(i));
    }
    return vec;
  }

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

#endif // YM_VL_ASTMODULE_H
