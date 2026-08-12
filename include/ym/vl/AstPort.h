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
/// また，各々の式の方向の情報も合わせて持っている．
//////////////////////////////////////////////////////////////////////
class AstPort :
  public AstBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // AstPort の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 外向の名前の取得
  /// @return 外向の名前(本当のポート名)\n
  /// 無い場合は nullptr を返す
  virtual
  const char*
  ext_name() const = 0;

  /// @brief 内部の結線を表す式の取得
  virtual
  const AstExpr*
  expr() const = 0;

  /// @brief 内部のポート結線リストのサイズの取得
  virtual
  SizeType
  portref_size() const = 0;

  /// @brief 内部のポート結線リストの取得
  ///
  /// - pos >= portref_size() の時 std::out_of_range 例外を送出する．
  virtual
  const AstExpr*
  portref_elem(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < portref_num() )
  ) const = 0;

  /// @brief 内部ポート結線の方向の取得
  ///
  /// - pos >= portref_size() の時 std::out_of_range 例外を送出する．
  virtual
  VpiDir
  portref_dir(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < portref_num() )
  ) const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_ASTPORT_H
