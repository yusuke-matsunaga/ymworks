#ifndef YM_VL_ASTPATHDECL_H
#define YM_VL_ASTPATHDECL_H

/// @file ym/vl/AstPathDecl.h
/// @brief AstPathDecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtPathDecl;

//////////////////////////////////////////////////////////////////////
/// @class AstPathDecl AstPathDecl.h "ym/vl/AstPathDecl.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief path_delay_declaration を表すクラス
//////////////////////////////////////////////////////////////////////
class AstPathDecl :
  public AstBaseWithPtr<const PtPathDecl>
{
public:

  /// @brief コンストラクタ
  explicit
  AstPathDecl(
    const PtPathDecl* ptr = nullptr ///< [in] 実体のポインタ
  ) : AstBaseWithPtr<const PtPathDecl>(ptr)
  {
  }

  /// @brief デストラクタ
  ~AstPathDecl() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief edge_descriptor の取得
  /// @return edge_descriptor
  ///
  /// 0の場合もある．
  int
  edge() const;

  /// @brief 入力のリストの取得
  AstExprList
  input_list() const;

  /// @brief 入力の極性の取得
  /// @return 入力の極性\n
  /// 0の場合もありうる．
  int
  input_pol() const;

  /// @brief パス記述子(?)の取得
  /// @return vpiParallel か vpiFull
  VpiPathType
  op() const;

  /// @brief 出力リストの取得
  AstExprList
  output_list() const;

  /// @brief 出力の極性の取得
  /// @return 出力の極性\n
  /// 0の場合もありうる．
  int
  output_pol() const;

  /// @brief 式の取得
  /// @return 式
  AstExpr
  expr() const;

  /// @brief path_delay_value の取得
  /// @return path_delay_value
  AstPathDelay
  path_delay() const;


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

#endif // YM_VL_ASTPATHDECL_H
