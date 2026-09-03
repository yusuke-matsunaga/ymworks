#ifndef YM_VL_ASTPATHDELAY_H
#define YM_VL_ASTPATHDELAY_H

/// @file ym/vl/AstPathDelay.h
/// @brief AstItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtPathDelay;

//////////////////////////////////////////////////////////////////////
/// @class AstPathDelay AstPathDelay.h "ym/vl/AstPathDelay.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief path_delay_value を表すクラス
//////////////////////////////////////////////////////////////////////
class AstPathDelay :
  public AstBaseWithPtr<const PtPathDelay>
{
public:

  /// @brief コンストラクタ
  explicit
  AstPathDelay(
    const PtPathDelay* ptr = nullptr ///< [in] 実体のポインタ
  ) : AstBaseWithPtr<const PtPathDelay>(ptr)
  {
  }

  /// @brief デストラクタ
  ~AstPathDelay() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を取り出す．
  /// 0の場合もある．
  AstExpr
  value(
    SizeType pos ///< [in] 位置 ( 0 <= pos < 3 )
  ) const;


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

#endif // YM_VL_ASTPATHDELAY_H
