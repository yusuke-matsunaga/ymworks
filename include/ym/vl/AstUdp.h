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
  ///
  /// - pos >= port_num() の時 std::out_of_range 例外を送出する．
  virtual
  const AstPort*
  port(
    SizeType pos ///< [in] 位置 ( 0 <= pos < port_num() )
  ) const = 0;

  /// @brief ポートのリストを取り出す．
  std::vector<const AstPort*>
  port_list() const
  {
    auto n = port_num();
    std::vector<const AstPort*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(port(i));
    }
    return vec;
  }

  /// @brief 入出力宣言ヘッダ配列の要素数の取得
  virtual
  SizeType
  iohead_num() const = 0;

  /// @brief 入出力宣言ヘッダの取得
  ///
  /// - pos >= iohead_num() の時 std::out_of_range 例外を送出する．
  virtual
  const AstIOHead*
  iohead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < iohead_num() )
  ) const = 0;

  /// @brief 入出力のリストの取得
  std::vector<const AstIOHead*>
  iohead_list() const
  {
    auto n = iohead_num();
    std::vector<const AstIOHead*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(iohead(i));
    }
    return vec;
  }

  /// @brief 初期値を取出す．
  virtual
  const AstExpr*
  init_value() const = 0;

  /// @brief テーブルの要素数を取り出す．
  virtual
  SizeType
  table_num() const = 0;

  /// @brief テーブルの要素を取り出す．
  ///
  /// - pos >= table_num() の時 std::out_of_range 例外を送出する．
  virtual
  const AstUdpEntry*
  table(
    SizeType pos ///< [in] 位置 ( 0 <= pos < table_num() )
  ) const = 0;

  /// @brief テーブルのリストを返す．
  std::vector<const AstUdpEntry*>
  table_list() const
  {
    auto n = table_num();
    std::vector<const AstUdpEntry*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(table(i));
    }
    return vec;
  }

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
  ///
  /// - pos >= input_num() の時 std::out_of_range 例外を送出する．
  virtual
  const AstUdpValue*
  input(
    SizeType pos ///< [in] 位置 ( 0 <= pos < input_num() )
  ) const = 0;

  /// @brief 入力値のリストを取り出す．
  std::vector<const AstUdpValue*>
  input_list() const
  {
    auto n = input_num();
    std::vector<const AstUdpValue*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(input(i));
    }
    return vec;
  }

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
