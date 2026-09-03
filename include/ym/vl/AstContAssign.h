#ifndef YM_VL_ASTCONTASSIGN_H
#define YM_VL_ASTCONTASSIGN_H

/// @file ym/vl/AstContAssign.h
/// @brief AstContAssign のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtContAssign;

//////////////////////////////////////////////////////////////////////
/// @class AstContAssign AstContAssign.h "ym/vl/AstContAssign.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief continuous assign 文
//////////////////////////////////////////////////////////////////////
class AstContAssign :
  public AstBaseWithPtr<const PtContAssign>
{
public:

  /// @brief コンストラクタ
  explicit
  AstContAssign(
    const PtContAssign* ptr = nullptr ///< [in] 実体のポインタ
  ) : AstBaseWithPtr<const PtContAssign>(ptr)
  {
  }

  /// @brief デストラクタ
  ~AstContAssign() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 左辺式の取得
  /// @return 左辺式
  AstExpr
  lhs() const;

  /// @brief 右辺式の取得
  /// @return 右辺式
  AstExpr
  rhs() const;


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
  AstContAssign
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

#endif // YM_VL_ASTCONTASSIGN_H
