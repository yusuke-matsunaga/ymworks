#ifndef YM_VL_ASTDELAY_H
#define YM_VL_ASTDELAY_H

/// @file ym/vl/AstDelay.h
/// @brief AstDelay のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtDelay;

//////////////////////////////////////////////////////////////////////
/// @class AstDelay AstDelay.h "ym/vl/AstDelay.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief delay を表すクラス
///
/// 最大で3つの値（式）を持つ．
/// 該当する値がない時は nullptr を返す．
//////////////////////////////////////////////////////////////////////
class AstDelay :
  public AstBaseWithPtr<const PtDelay>
{
public:

  /// @brief コンストラクタ
  explicit
  AstDelay(
    const PtDelay* ptr = nullptr ///< [in] 実体のポインタ
  ) : AstBaseWithPtr<const PtDelay>(ptr)
  {
  }

  /// @brief デストラクタ
  ~AstDelay() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 値の取得
  ///
  /// 該当する要素がなければ nullptr を返す．
  AstExpr
  value0() const;

  /// @brief 値の取得
  ///
  /// 該当する要素がなければ nullptr を返す．
  AstExpr
  value1() const;

  /// @brief 値の取得
  ///
  /// 該当する要素がなければ nullptr を返す．
  AstExpr
  value2() const;


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

#endif // YM_VL_ASTDELAY_H
