#ifndef YM_VL_ASTRANGE_H
#define YM_VL_ASTRANGE_H

/// @file ym/vl/AstRange.h
/// @brief AstRange のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtRange;

//////////////////////////////////////////////////////////////////////
/// @class AstRange AstRange.h "ym/vl/AstRange.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief 範囲を表すクラス
//////////////////////////////////////////////////////////////////////
class AstRange :
  public AstBaseWithPtr<const PtRange>
{
public:

  /// @brief コンストラクタ
  explicit
  AstRange(
    const PtRange* ptr = nullptr ///< [in] 実体のポインタ
  ) : AstBaseWithPtr<const PtRange>(ptr)
  {
  }

  /// @brief デストラクタ
  ~AstRange() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲の MSB を取り出す．
  AstExpr
  left() const;

  /// @brief 範囲の LSB を取り出す．
  AstExpr
  right() const;


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
  AstRange
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

#endif // YM_VL_ASTRANGE_H
