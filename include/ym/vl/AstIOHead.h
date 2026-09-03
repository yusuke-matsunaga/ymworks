#ifndef YM_VL_ASTIOHEAD_H
#define YM_VL_ASTIOHEAD_H

/// @file ym/vl/AstIOHead.h
/// @brief AstIOHead のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtIOHead;

//////////////////////////////////////////////////////////////////////
/// @class AstIOHead AstIOHead.h "ym/vl/AstIOHead.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief IO宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class AstIOHead :
  public AstBaseWithPtr<const PtIOHead>
{
public:

  /// @brief コンストラクタ
  explicit
  AstIOHead(
    const PtIOHead* ptr = nullptr ///< [in] 実体のポインタ
  ) : AstBaseWithPtr<const PtIOHead>(ptr)
  {
  }

  /// @brief デストラクタ
  ~AstIOHead() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 方向の取得
  /// @return 方向(Input, Output, Inout)
  VpiDir
  direction() const;

  /// @brief 補助的な型の取得
  /// @return 補助的な型
  VpiAuxType
  aux_type() const;

  /// @brief 補助的なネット型の取得
  /// @return ネット型
  VpiNetType
  net_type() const;

  /// @brief 補助的な変数型の取得
  /// @return 変数型
  VpiVarType
  var_type() const;

  /// @brief 符号の取得
  /// @retval true 符号付き
  /// @retval false 符号なし
  bool
  is_signed() const;

  /// @brief 範囲の取得
  /// @retval 範囲
  /// @retval nullptr 範囲を持たないとき
  AstRange
  range() const;

  /// @brief 要素のリストを返す．
  AstIOItemList
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
  AstIOHead
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

#endif // YM_VL_ASTIOHEAD_H
