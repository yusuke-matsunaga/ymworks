#ifndef YM_VL_ASTUDP_H
#define YM_VL_ASTUDP_H

/// @file ym/vl/AstUdp.h
/// @brief AstUdp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class AstUdp AstUdp.h "ym/vl/AstUdp.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief UDP を表すノード
//////////////////////////////////////////////////////////////////////
class AstUdp :
  public AstNamedBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // AstUdp の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief primitive type を返す．
  virtual
  VpiPrimType
  prim_type() const = 0;

  /// @brief ポート数を取り出す．
  virtual
  SizeType
  port_num() const = 0;

  /// @brief ポートを取り出す．
  virtual
  const AstPort*
  port(
    SizeType index ///< [in] インデックス ( 0 <= index < port_num() )
  ) const = 0;

  /// @brief ポートのリストを取り出す．
  virtual
  AstPortVec
  port_list() const = 0;

  /// @brief 入出力宣言ヘッダ配列の要素数の取得
  virtual
  SizeType
  iohead_num() const = 0;

  /// @brief 入出力宣言ヘッダの取得
  virtual
  const AstIOHead*
  iohead(
    SizeType index ///< [in] インデックス ( 0 <= index < iohead_num() )
  ) const = 0;

  /// @brief 入出力宣言ヘッダのリストの取得
  virtual
  AstIOHeadVec
  iohead_list() const = 0;

  /// @brief 初期値を取出す．
  virtual
  const AstExpr*
  init_value() const = 0;

  /// @brief テーブルの要素数を取り出す．
  virtual
  SizeType
  table_num() const = 0;

  /// @brief テーブルを返す．
  virtual
  const AstUdpEntry*
  table(
    SizeType index ///< [in] インデックス ( 0 <= index < table_num() )
  ) const = 0;

  /// @brief テーブルのリストを返す．
  virtual
  AstUdpEntryVec
  table_list() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class AstUdpEntry AstUdp.h "ym/vl/AstUdp.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief UDP の中身のテーブルを表すクラス
//////////////////////////////////////////////////////////////////////
class AstUdpEntry :
  public AstBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // AstUdpEntry の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力値の配列の要素数を取り出す．
  virtual
  SizeType
  input_num() const = 0;

  /// @brief 入力値を取り出す．
  virtual
  const AstUdpValue*
  input(
    SizeType index ///< [in] インデックス ( 0 <= index < input_num() )
  ) const = 0;

  /// @brief 入力値のリストを取り出す．
  virtual
  AstUdpValueVec
  input_list() const = 0;

  /// @brief 現状態の値を取り出す．
  virtual
  const AstUdpValue*
  current() const = 0;

  /// @brief 出力の値を取り出す．
  virtual
  const AstUdpValue*
  output() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class AstUdpValue AstUdp.h "ym/vl/AstUdp.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief UDP のテーブルの値を表すクラス
//////////////////////////////////////////////////////////////////////
class AstUdpValue :
  public AstBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // AstUdpValue の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を取り出す．
  virtual
  VlUdpVal
  symbol() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_ASTUDP_H
