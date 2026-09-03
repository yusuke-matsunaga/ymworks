#ifndef YM_VL_ASTGENCASEITEM_H
#define YM_VL_ASTGENCASEITEM_H

/// @file ym/vl/AstGenCaseItem.h
/// @brief AstGenCaseItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtGenCaseItem;

//////////////////////////////////////////////////////////////////////
/// @class AstGenCaseItem AstGenCaseItem.h "ym/vl/AstGenCaseItem.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief gencaseitemを表すクラス
//////////////////////////////////////////////////////////////////////
class AstGenCaseItem :
  public AstBaseWithPtr<const PtGenCaseItem>
{
public:

  /// @brief コンストラクタ
  explicit
  AstGenCaseItem(
    const PtGenCaseItem* ptr = nullptr ///< [in] 実体のポインタ
  ) : AstBaseWithPtr<const PtGenCaseItem>(ptr)
  {
  }

  /// @brief デストラクタ
  ~AstGenCaseItem() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ラベルリストの取得
  AstExprList
  label_list() const;

  /// @brief 宣言リストの取得
  AstDeclHeadList
  declhead_list() const;

  /// @brief item リストの取得
  AstItemList
  item_list() const;


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
  AstGenCaseItem
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

#endif // YM_VL_ASTGENCASEITEM_H
