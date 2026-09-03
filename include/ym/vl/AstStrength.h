#ifndef YM_VL_ASTSTRENGTH_H
#define YM_VL_ASTSTRENGTH_H

/// @file ym/vl/AstStrength.h
/// @brief AstStrength のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtStrength;

//////////////////////////////////////////////////////////////////////
/// @class AstStrength AstStrength.h "ym/vl/AstStrength.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief strength を表すクラス
///
/// drive strength を表す場合と charge strength を表す場合の２通りがある．
/// drive strength の場合の charge() は NoStrength を返す．
/// charge strength の場合の drive0() と driver1() は NoStrength を返す．
//////////////////////////////////////////////////////////////////////
class AstStrength :
  public AstBaseWithPtr<const PtStrength>
{
public:

  /// @brief コンストラクタ
  explicit
  AstStrength(
    const PtStrength* ptr = nullptr ///< [in] 実体のポインタ
  ) : AstBaseWithPtr<const PtStrength>(ptr)
  {
  }

  /// @brief デストラクタ
  ~AstStrength() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief drive strength0 の取得
  /// @return 0 の強度
  VpiStrength
  drive0() const;

  /// @brief drive strength1 の取得
  /// @return 1 の強度
  VpiStrength
  drive1() const;

  /// @brief charge strength の取得
  /// @return 電荷の強度
  VpiStrength
  charge() const;


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

#endif // YM_VL_ASTSTRENGTH_H
