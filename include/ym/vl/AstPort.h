#ifndef YM_VL_ASTPORT_H
#define YM_VL_ASTPORT_H

/// @file ym/vl/AstPort.h
/// @brief AstPort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

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
  public AstBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // AstPort の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 外向の名前の取得
  /// @return 外向の名前(本当のポート名)
  ///
  /// 無い場合は nullptr を返す
  virtual
  const char*
  ext_name() const = 0;

  /// @brief 内部の結線を表す式の取得
  ///
  /// - nullptr の場合もある．
  /// - 複数の結線からなる場合には Concat 演算となる．
  virtual
  const AstExpr*
  expr() const = 0;

  /// @brief 内部のポート結線リストのサイズの取得
  virtual
  SizeType
  portref_size() const = 0;

  /// @brief 内部のポート結線の取得
  virtual
  const AstExpr*
  portref(
    SizeType index ///< [in] インデックス ( 0 <= index < portref_size() )
  ) const = 0;

  /// @brief 内部のポート結線のリストの取得
  ///
  /// portef_size() <= 1 の時は nullptr を返す．
  virtual
  AstExprVec
  portref_list() const = 0;

  /// @brief 内部のポート結線の向きの取得
  virtual
  VpiDir
  portref_dir(
    SizeType index ///< [in] インデックス ( 0 <= index < portref_size() )
  ) const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_ASTPORT_H
