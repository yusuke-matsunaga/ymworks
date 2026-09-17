#ifndef EIUDPIO_H
#define EIUDPIO_H

/// @file EiUdpDefn.h
/// @brief EiUdpDefn のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

// UDP定義を表すクラス
// IEEE Std 1364-2001 26.6.14 UDP

#include "ym/vl/VlIODecl.h"
#include "ym/vl/AstIOHead.h"
#include "ym/vl/AstIOItem.h"
#include "elaborator/ElbUdpDefn.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiUdpIO EiUdpIO.h "ei/EiUdpIO.h"
/// @brief UDP 用 IO decl の基底クラス
//////////////////////////////////////////////////////////////////////
class EiUdpIO :
  public VlIODecl
{
public:

  /// @brief 空のコンストラクタ
  EiUdpIO() = default;

  /// @brief コンストラクタ
  EiUdpIO(
    ElbUdpDefn* udp,             ///< [in] 親のUDP
    const AstIOHead& ast_header, ///< [in] パース木のIO宣言ヘッダ
    const AstIOItem& ast_item    ///< [in] パース木のIO宣言定義
  ) : mUdp{udp},
      mAstHeader{ast_header},
      mAstItem{ast_item}
  {
  }

  /// @brief デストラクタ
  ~EiUdpIO() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlIODecl に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  std::string
  name() const override;

  /// @brief 方向を返す．
  VpiDir
  direction() const override;

  /// @brief 符号の属性の取得
  /// @return 符号付きのとき true を返す．
  bool
  is_signed() const override;

  /// @brief 範囲指定を持つとき true を返す．
  bool
  has_range() const override;

  /// @brief MSB の値を返す．
  int
  left_range_val() const override;

  /// @brief LSB の値を返す．
  int
  right_range_val() const override;

  /// @brief 範囲のMSBを表す文字列の取得
  std::string
  left_range_string() const override;

  /// @brief 範囲のLSBを表す文字列の取得
  std::string
  right_range_string() const override;

  /// @brief サイズを返す．
  /// このクラスは 1 を返す．
  SizeType
  bit_size() const override;

  /// @brief 対応する宣言要素を返す．
  const VlDecl*
  decl() const override;

  /// @brief 親のモジュールの取得
  const VlModule*
  module() const override;

  /// @brief 親のUDP定義を返す．
  const VlUdpDefn*
  udp_defn() const override;

  /// @brief 親のタスクの取得
  const VlTaskFunc*
  task() const override;

  /// @brief 親の関数の取得
  const VlTaskFunc*
  function() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の UDP
  ElbUdpDefn* mUdp;

  // パース木のIO宣言ヘッダ
  AstIOHead mAstHeader;

  // パース木のIO宣言定義
  AstIOItem mAstItem;

};

END_NAMESPACE_YM_VERILOG

#endif // EIUDPIO_H
