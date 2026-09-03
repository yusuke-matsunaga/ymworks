#ifndef YM_VL_ASTPART_H
#define YM_VL_ASTPART_H

/// @file ym/vl/AstPart.h
/// @brief AstPart のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtPart;

//////////////////////////////////////////////////////////////////////
/// @class AstPart AstPart.h "ym/vl/AstPart.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief 範囲指定を表すクラス
///
/// - Const: [a: b]
/// - Plust: [a+:b]
/// - Minus: [a-:b]
//////////////////////////////////////////////////////////////////////
class AstPart :
  public AstBaseWithPtr<const PtPart>
{
public:

  /// @brief コンストラクタ
  explicit
  AstPart(
    const PtPart* ptr = nullptr ///< [in] 実体のポインタ
  ) : AstBaseWithPtr<const PtPart>(ptr)
  {
  }

  /// @brief デストラクタ
  ~AstPart() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲指定のモードを返す．
  VpiRangeMode
  mode() const;

  /// @brief 1番目の式を取り出す．
  AstExpr
  left() const;

  /// @brief 2番めの式を取り出す．
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

#endif // YM_VL_ASTPART_H
