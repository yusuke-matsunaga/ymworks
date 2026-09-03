#ifndef YM_VL_ASTCONNECTION_H
#define YM_VL_ASTCONNECTION_H

/// @file ym/vl/AstConnection.h
/// @brief AstConnection のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtConnection;

//////////////////////////////////////////////////////////////////////
/// @class AstConnection AstConnection.h "ym/vl/AstConnection.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief orered_connection/named_connection を表すクラス
//////////////////////////////////////////////////////////////////////
class AstConnection :
  public AstBaseWithPtr<const PtConnection>
{
public:

  /// @brief コンストラクタ
  explicit
  AstConnection(
    const PtConnection* ptr = nullptr ///< [in] 実体のポインタ
  ) : AstBaseWithPtr<const PtConnection>(ptr)
  {
  }

  /// @brief デストラクタ
  ~AstConnection() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  /// @retval 名前 named connection の場合
  /// @retval "" ordered connection の場合
  const char*
  name() const;

  /// @brief 式の取得
  /// @return 式
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
  // AstList<> の要素のための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を返す．
  AstConnection
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

#endif // YM_VL_ASTCONNECTION_H
