#ifndef YM_VL_ASTDECLITEM_H
#define YM_VL_ASTDECLITEM_H

/// @file ym/vl/AstDeclItem.h
/// @brief AstDeclItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtDeclItem;

//////////////////////////////////////////////////////////////////////
/// @class AstDeclItem AstDeclItem.h "ym/vl/AstDeclItem.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief 宣言要素のクラス
//////////////////////////////////////////////////////////////////////
class AstDeclItem :
  public AstNamedBaseWithPtr<const PtDeclItem>
{
public:

  /// @brief コンストラクタ
  explicit
  AstDeclItem(
    const PtDeclItem* ptr = nullptr ///< [in] 実体のポインタ
  ) : AstNamedBaseWithPtr<const PtDeclItem>(ptr)
  {
  }

  /// @brief デストラクタ
  ~AstDeclItem() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲のリストを返す．
  AstRangeList
  range_list() const;

  /// @brief 初期値の取得
  ///
  /// 設定がない場合は無効な値が返される．
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
  AstDeclItem
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

#endif // YM_VL_ASTDECLITEM_H
