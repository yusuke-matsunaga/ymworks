#ifndef YM_VL_ASTIOITEM_H
#define YM_VL_ASTIOITEM_H

/// @file ym/vl/AstIOItem.h
/// @brief AstIOItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtIOItem;

//////////////////////////////////////////////////////////////////////
/// @class AstIOItem AstIOItem.h "ym/vl/AstIOItem.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief IO宣言要素の基底クラス
//////////////////////////////////////////////////////////////////////
class AstIOItem :
  public AstNamedBaseWithPtr<const PtIOItem>
{
public:

  /// @brief コンストラクタ
  explicit
  AstIOItem(
    const PtIOItem* ptr = nullptr ///< [in] 実体のポインタ
  ) : AstNamedBaseWithPtr<const PtIOItem>(ptr)
  {
  }

  /// @brief デストラクタ
  ~AstIOItem() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期値の取得
  ///
  /// 初期値を持たないときは無効な値が返される
  AstExpr
  init_value() const;


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


public:
  //////////////////////////////////////////////////////////////////////
  // AstList<> の要素のための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を返す．
  AstIOItem
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

#endif // YM_VL_ASTIOITEM_H
