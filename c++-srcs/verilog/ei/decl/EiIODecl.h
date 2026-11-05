#ifndef EIIODECL_H
#define EIIODECL_H

/// @file EiIODecl.h
/// @brief EiIODecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/VlIODecl.h"
#include "ym/vl/AstIOItem.h"
#include "elaborator/ElbIOHead.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiIODecl EiIODecl.h "EiIODecl.h"
/// @brief IO 要素を表すクラス
//////////////////////////////////////////////////////////////////////
class EiIODecl :
  public VlIODecl
{
public:

  /// @brief コンストラクタ
  EiIODecl(
    ElbIOHead* head,           ///< [in] ヘッダ
    const AstIOItem& ast_item, ///< [in] パース木のIO宣言要素
    const VlDecl* decl         ///< [in] 対応する宣言要素
  ) : mHead{head},
      mAstItem{ast_item},
      mDecl{decl}
  {
  }

  /// @brief デストラクタ
  ~EiIODecl() = default;


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
  // VlIODecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  std::string
  name() const override;

  /// @brief 方向を返す．
  VpiDir
  direction() const override;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  bool
  is_signed() const override;

  /// @brief 範囲指定を持つとき true を返す．
  bool
  has_range() const override;

  /// @brief 範囲の MSB の値を返す．
  int
  left_range_val() const override;

  /// @brief 範囲の LSB の値を返す．
  int
  right_range_val() const override;

  /// @brief 範囲のMSBを表す文字列の取得
  std::string
  left_range_string() const override;

  /// @brief 範囲のLSBを表す文字列の取得
  std::string
  right_range_string() const override;

  /// @brief ビット幅を返す．
  SizeType
  bit_size() const override;

  /// @brief 対応する宣言要素を返す．
  const VlDecl*
  decl() const override;

  /// @brief 親のモジュールの取得
  const VlModule*
  module() const override;

  /// @brief 親の UDP の取得
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

  // ヘッダ
  ElbIOHead* mHead;

  // パース木の IO 宣言
  AstIOItem mAstItem;

  // 対応する宣言要素
  const VlDecl* mDecl;

};

END_NAMESPACE_YM_VERILOG

#endif // EIIODECL_H
