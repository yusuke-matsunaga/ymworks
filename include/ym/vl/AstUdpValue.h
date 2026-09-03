#ifndef YM_VL_ASTUDPVALUE_H
#define YM_VL_ASTUDPVALUE_H

/// @file ym/vl/AstUdpValue.h
/// @brief AstUdp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtUdpValue;

//////////////////////////////////////////////////////////////////////
/// @class AstUdpValue AstUdpValue.h "ym/vl/AstUdpValue.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief UDP のテーブルの値を表すクラス
//////////////////////////////////////////////////////////////////////
class AstUdpValue :
  public AstBaseWithPtr<const PtUdpValue>
{
public:

  /// @brief コンストラクタ
  explicit
  AstUdpValue(
    const PtUdpValue* ptr = nullptr ///< [in] 実体のポインタ
  ) : AstBaseWithPtr<const PtUdpValue>(ptr)
  {
  }

  /// @brief デストラクタ
  ~AstUdpValue() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を取り出す．
  VlUdpVal
  symbol() const;


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
  AstUdpValue
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

#endif // YM_VL_ASTUDPVALUE_H
