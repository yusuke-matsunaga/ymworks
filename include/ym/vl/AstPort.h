#ifndef YM_VL_ASTPORT_H
#define YM_VL_ASTPORT_H

/// @file ym/vl/AstPort.h
/// @brief AstPort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"
#include "ym/vl/AstList.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtPort;

//////////////////////////////////////////////////////////////////////
/// @class AstPort AstPort.h "ym/vl/AstPort.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief port を表すクラス
///
/// 通常はただの入出力なので名前と AstExpr だけでよいが，
/// Verilog-HDL の場合には複数の式を結合したものをポートに対応付け
/// できるのでその場合を考慮する必要がある．
/// 結合演算を用いればそれも式として扱えるが，
/// 複数の式のリストの方が扱いやすい．
///
/// ポートの「向き」は対応する式から推測することが可能だが
/// 面倒なので向きの情報も持たせている．
/// ただし複数の式が結合している場合は個々の式ごとに向きが
/// 異なる可能性がある．
//////////////////////////////////////////////////////////////////////
class AstPort :
  public AstBaseWithPtr<const PtPort>
{
public:

  /// @brief コンストラクタ
  explicit
  AstPort(
    const PtPort* ptr = nullptr ///< [in] 実体のポインタ
  ) : AstBaseWithPtr<const PtPort>(ptr)
  {
  }

  /// @brief デストラクタ
  ~AstPort() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 外向の名前の取得
  /// @return 外向の名前(本当のポート名)
  ///
  /// 無い場合は nullptr を返す
  const char*
  ext_name() const;

  /// @brief 内部の結線を表す式の取得
  ///
  /// - 無効な値の場合もある．
  /// - 複数の結線からなる場合には Concat 演算となる．
  AstExpr
  expr() const;

  /// @brief 内部のポート結線のリストの取得
  ///
  /// portef_size() <= 1 の時は空リストを返す．
  AstExprList
  portref_list() const;

#if 0
  /// @brief 内部のポート結線の向きの取得
  VpiDir
  portref_dir(
    SizeType index ///< [in] インデックス ( 0 <= index < portref_size() )
  ) const;
#endif


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
  AstPort
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

#endif // YM_VL_ASTPORT_H
