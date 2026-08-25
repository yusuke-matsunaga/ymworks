#ifndef YM_VL_ASTUDP_H
#define YM_VL_ASTUDP_H

/// @file ym/vl/AstUdp.h
/// @brief AstUdp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"
#include "ym/vl/AstPort.h"
#include "ym/vl/AstMisc.h"


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
  friend class AstList<const AstUdp>;
  friend class AstListIter<const AstUdp>;

public:
  //////////////////////////////////////////////////////////////////////
  // AstUdp の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief primitive type を返す．
  virtual
  VpiPrimType
  prim_type() const = 0;

  /// @brief ポートのリストを取り出す．
  virtual
  AstPortList
  port_list() const = 0;

  /// @brief 入出力宣言ヘッダのリストの取得
  virtual
  AstIOHeadList
  iohead_list() const = 0;

  /// @brief 初期値を取出す．
  virtual
  const AstExpr*
  init_value() const = 0;

  /// @brief テーブルのリストを返す．
  virtual
  AstUdpEntryList
  table_list() const = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素の取得
  virtual
  const AstUdp*
  link() const = 0;

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
  friend class AstList<const AstUdpEntry>;
  friend class AstListIter<const AstUdpEntry>;

public:
  //////////////////////////////////////////////////////////////////////
  // AstUdpEntry の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力値のリストを取り出す．
  virtual
  AstUdpValueList
  input_list() const = 0;

  /// @brief 現状態の値を取り出す．
  virtual
  const AstUdpValue*
  current() const = 0;

  /// @brief 出力の値を取り出す．
  virtual
  const AstUdpValue*
  output() const = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素の取得
  virtual
  const AstUdpEntry*
  link() const = 0;

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
  friend class AstList<const AstUdpValue>;
  friend class AstListIter<const AstUdpValue>;

public:
  //////////////////////////////////////////////////////////////////////
  // AstUdpValue の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を取り出す．
  virtual
  VlUdpVal
  symbol() const = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素の取得
  virtual
  const AstUdpValue*
  link() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_ASTUDP_H
