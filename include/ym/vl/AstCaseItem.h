#ifndef YM_VL_ASTCASEITEM_H
#define YM_VL_ASTCASEITEM_H

/// @file ym/vl/AstCaseItem.h
/// @brief AstStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtCaseItem;

//////////////////////////////////////////////////////////////////////
/// @class AstCaseItem AstCaseItem.h "ym/vl/AstCaseItem.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief caseitemを表すクラス
//////////////////////////////////////////////////////////////////////
class AstCaseItem :
  public AstBaseWithPtr<const PtCaseItem>
{
public:

  /// @brief コンストラクタ
  explicit
  AstCaseItem(
    const PtCaseItem* ptr = nullptr ///< [in] 実体のポインタ
  ) : AstBaseWithPtr<const PtCaseItem>(ptr)
  {
  }

  /// @brief デストラクタ
  ~AstCaseItem() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ラベルリストの取得
  AstExprList
  label_list() const;

  /// @brief 本体のステートメントの取得
  /// @return 本体のステートメント
  AstStmt
  body() const;


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
  AstCaseItem
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

#endif // YM_VL_ASTCASEITEM_H
