#ifndef YM_VL_ASTDECL_H
#define YM_VL_ASTDECL_H

/// @file ym/vl/AstDecl.h
/// @brief 宣言要素関係の部品クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class AstIOHead AstDecl.h "ym/vl/AstDecl.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief IO宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class AstIOHead :
  public AstBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // AstIOH の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 方向の取得
  /// @return 方向(Input, Output, Inout)
  virtual
  VpiDir
  direction() const = 0;

  /// @brief 補助的な型の取得
  /// @return 補助的な型
  virtual
  VpiAuxType
  aux_type() const = 0;

  /// @brief 補助的なネット型の取得
  /// @return ネット型
  virtual
  VpiNetType
  net_type() const = 0;

  /// @brief 補助的な変数型の取得
  /// @return 変数型
  virtual
  VpiVarType
  var_type() const = 0;

  /// @brief 符号の取得
  /// @retval true 符号付き
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const = 0;

  /// @brief 範囲の取得
  /// @retval 範囲
  /// @retval nullptr 範囲を持たないとき
  virtual
  const AstRange*
  range() const = 0;

  /// @brief 要素数の取得
  virtual
  SizeType
  item_num() const = 0;

  /// @brief 要素を返す．
  virtual
  const AstIOItem*
  item(
    SizeType index ///< [in] インデックス ( 0 <= index < item_num() )
  ) const = 0;

  /// @brief 要素のリストを返す．
  virtual
  AstIOItemVec
  item_list() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class AstIOItem AstDecl.h "ym/vl/AstDecl.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief IO宣言要素の基底クラス
//////////////////////////////////////////////////////////////////////
class AstIOItem :
  public AstNamedBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // AstIOItem の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期値の取得
  /// @retval 初期値
  /// @retval nullptr 初期値を持たないとき
  virtual
  const AstExpr*
  init_value() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class AstDeclHead AstDecl.h "ym/vl/AstDecl.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief 宣言要素のヘッダの基底クラス
//////////////////////////////////////////////////////////////////////
class AstDeclHead :
  public AstBase
{
public:

  /// @brief AstDeclH の派生クラスを区別するための列挙型
  enum Type : std::uint8_t {
    Param,      ///< @brief parameter
    LocalParam, ///< @brief localparam
    Reg,        ///< @brief reg
    Var,        ///< @brief variables
    Genvar,     ///< @brief genvar
    Net,        ///< @brief net
    Event,      ///< @brief named event
    SpecParam   ///< @brief specparam
  };


public:
  //////////////////////////////////////////////////////////////////////
  // 共通な関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言要素の型の取得
  /// @return 宣言要素の型
  virtual
  Type
  type() const = 0;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const = 0;

  /// @brief 範囲Bの取得
  /// @retval 範囲 範囲を持つとき
  /// @retval nullptr 範囲を持たないとき
  virtual
  const AstRange*
  range() const = 0;

  /// @brief データ型の取得
  /// @retval データ型 type() = Param, LocalParam, Var の場合
  /// @retval VpiVarType::None 上記以外
  virtual
  VpiVarType
  data_type() const = 0;

  /// @brief ネット型の取得
  /// @retval ネット型 type() = Net の場合
  /// @retval VpiNetType::None ネット型でない場合
  virtual
  VpiNetType
  net_type() const = 0;

  /// @brief vectored|scalared 属性の取得
  /// @retval None vectored|scalared 指定なし
  /// @retval Vectored vectored 指定あり
  /// @retval Scalared scalared 指定あり
  virtual
  VpiVsType
  vs_type() const = 0;

  /// @brief strength の取得
  /// @retval strength
  /// @retval nullptr strength の指定なし
  virtual
  const AstStrength*
  strength() const = 0;

  /// @brief delay の取得
  /// @retval delay
  /// @retval nullptr delay の指定なし
  virtual
  const AstDelay*
  delay() const = 0;

  /// @brief 要素数の取得
  virtual
  SizeType
  item_num() const = 0;

  /// @brief 要素を返す．
  virtual
  const AstDeclItem*
  item(
    SizeType index ///< [in] インデックス ( 0 <= index < item_num() )
  ) const = 0;

  /// @brief 要素のリストを返す．
  virtual
  AstDeclItemVec
  item_list() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class AstDeclItem AstDecl.h "ym/vl/AstDecl.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief 宣言要素のクラス
//////////////////////////////////////////////////////////////////////
class AstDeclItem :
  public AstNamedBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // AstDeclItem の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲のリストの要素数を返す．
  virtual
  SizeType
  range_num() const = 0;

  /// @brief 範囲を返す．
  virtual
  const AstRange*
  range(
    SizeType index ///< [in] インデックス ( 0 <= index < range_num() )
  ) const = 0;

  /// @brief 範囲のリストを返す．
  virtual
  AstRangeVec
  range_list() const = 0;

  /// @brief 初期値の取得
  /// @retval 初期値
  /// @retval nullptr 設定がない場合
  virtual
  const AstExpr*
  init_value() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class AstRange AstDecl.h "ym/vl/AstDecl.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief 範囲を表すクラス
//////////////////////////////////////////////////////////////////////
class AstRange :
  public AstBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // AstRange の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲の MSB を取り出す．
  virtual
  const AstExpr*
  left() const = 0;

  /// @brief 範囲の LSB を取り出す．
  virtual
  const AstExpr*
  right() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_ASTDECL_H
