#ifndef YM_VL_ASTDEFPARAM_H
#define YM_VL_ASTDEFPARAM_H

/// @file ym/vl/AstDefParam.h
/// @brief AstItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtDefParam;

//////////////////////////////////////////////////////////////////////
/// @class AstDefParam AstDefParam.h "ym/vl/AstDefParam.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief defparam 文を表すクラス
//////////////////////////////////////////////////////////////////////
class AstDefParam :
  public AstHierNamedBaseWithPtr<const PtDefParam>
{
public:

  /// @brief コンストラクタ
  explicit
  AstDefParam(
    const PtDefParam* ptr = nullptr ///< [in] 実体のポインタ
  ) : AstHierNamedBaseWithPtr<const PtDefParam>(ptr)
  {
  }

  /// @brief デストラクタ
  ~AstDefParam() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 値の取得
  /// @return 値を表す式
  AstExpr
  expr() const;


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
  // AstHierNamedBase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 階層ブランチのリストを返す．
  AstNameBranchList
  namebranch_list() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // AstList<> の要素のための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を返す．
  AstDefParam
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

#endif // YM_VL_ASTDEFPARAM_H
