#ifndef YM_VL_ASTUDP_H
#define YM_VL_ASTUDP_H

/// @file ym/vl/AstUdp.h
/// @brief AstUdp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtUdp;

//////////////////////////////////////////////////////////////////////
/// @class AstUdp AstUdp.h "ym/vl/AstUdp.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief UDP を表すノード
//////////////////////////////////////////////////////////////////////
class AstUdp :
  public AstNamedBaseWithPtr<const PtUdp>
{
public:

  /// @brief コンストラクタ
  explicit
  AstUdp(
    const PtUdp* ptr = nullptr ///< [in] 実体のポインタ
  ) : AstNamedBaseWithPtr<const PtUdp>(ptr)
  {
  }

  /// @brief デストラクタ
  ~AstUdp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief primitive type を返す．
  VpiPrimType
  prim_type() const;

  /// @brief ポートのリストを取り出す．
  AstPortList
  port_list() const;

  /// @brief 入出力宣言ヘッダのリストの取得
  AstIOHeadList
  iohead_list() const;

  /// @brief 初期値を取出す．
  AstExpr
  init_value() const;

  /// @brief テーブルのリストを返す．
  AstUdpEntryList
  table_list() const;


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

#endif // YM_VL_ASTUDP_H
