#ifndef YM_VL_VLUDPDEFN_H
#define YM_VL_VLUDPDEFN_H

/// @file ym/vl/VlUdpDefn.h
/// @brief VlUdpDefn のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/VlObj.h"
#include "ym/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlUdpDefn VlUdpDefn.h "ym/vl/VlUdpDefn.h"
/// @brief UDP定義を表すクラス
/// IEEE Std 1364-2001 26.6.14 UDP
//////////////////////////////////////////////////////////////////////
class VlUdpDefn :
  public VlObj
{
public:
  //////////////////////////////////////////////////////////////////////
  // VlUdpDefn に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定義された名前を返す．
  virtual
  std::string
  def_name() const = 0;

  /// @brief primitive type を返す．
  virtual
  VpiPrimType
  prim_type() const = 0;

  /// @brief ポート数を返す．
  virtual
  SizeType
  port_num() const = 0;

  /// @brief 入力の宣言要素を返す．
  virtual
  const VlIODecl*
  input(
    SizeType pos ///< [in] 入力番号 ( 0 <= pos < port_num() - 1 )
  ) const = 0;

  /// @brief 出力の宣言要素を返す．
  virtual
  const VlIODecl*
  output() const = 0;

  /// @brief protected かどうかを返す．
  virtual
  bool
  is_protected() const = 0;

  /// @brief 初期値を返す．
  /// @return 0/1/X を返す．
  virtual
  VlScalarVal
  init_val() const = 0;

  /// @brief 初期値を表す文字列を返す．
  virtual
  std::string
  init_val_string() const = 0;

  /// @brief table entry の行数を返す．
  virtual
  SizeType
  table_size() const = 0;

  /// @brief table entry を返す．
  virtual
  const VlTableEntry*
  table_entry(
    SizeType pos ///< [in] 行番号
  ) const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_VLUDPDEFN_H
