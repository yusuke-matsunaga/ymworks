#ifndef YM_VL_ASTUDPENTRY_H
#define YM_VL_ASTUDPENTRY_H

/// @file ym/vl/AstUdpEntry.h
/// @brief AstUdpEntry のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtUdpEntry;

//////////////////////////////////////////////////////////////////////
/// @class AstUdpEntry AstUdpEntry.h "ym/vl/AstUdpEntry.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief UDP の中身のテーブルを表すクラス
//////////////////////////////////////////////////////////////////////
class AstUdpEntry :
  public AstBaseWithPtr<const PtUdpEntry>
{
public:

  /// @brief コンストラクタ
  explicit
  AstUdpEntry(
    const PtUdpEntry* ptr = nullptr ///< [in] 実体のポインタ
  ) : AstBaseWithPtr<const PtUdpEntry>(ptr)
  {
  }

  /// @brief デストラクタ
  ~AstUdpEntry() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力値のリストを取り出す．
  AstUdpValueList
  input_list() const;

  /// @brief 現状態の値を取り出す．
  AstUdpValue
  current() const;

  /// @brief 出力の値を取り出す．
  AstUdpValue
  output() const;


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
  // AstList<> の要素のための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を返す．
  AstUdpEntry
  next() const;


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

#endif // YM_VL_ASTUDPENTRY_H
