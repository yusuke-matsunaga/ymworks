#ifndef YM_VL_ASTINST_H
#define YM_VL_ASTINST_H

/// @file ym/vl/AstInst.h
/// @brief AstInst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtInst;

//////////////////////////////////////////////////////////////////////
/// @class AstInst AstInst.h "ym/vl/AstInst.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief module instance/UDP/gate instance を表すクラス
//////////////////////////////////////////////////////////////////////
class AstInst :
  public AstNamedBaseWithPtr<const PtInst>
{
public:

  /// @brief コンストラクタ
  explicit
  AstInst(
    const PtInst* ptr = nullptr ///< [in] 実体のポインタ
  ) : AstNamedBaseWithPtr<const PtInst>(ptr)
  {
  }

  /// @brief デストラクタ
  ~AstInst() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲の取得
  /// @return 範囲
  AstRange
  range() const;

  /// @brief ポートリストの取得
  AstConnectionList
  port_list() const;


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
  AstInst
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

#endif // YM_VL_ASTINST_H
