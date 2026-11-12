#ifndef YM_VL_VEXPR_H
#define YM_VL_VEXPR_H

/// @file ym/vl/VlExpr.h
/// @brief VlExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/VlObj.h"
#include "ym/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlExpr VlExpr.h "ym/vl/VlExpr.h"
/// @brief エラボレーション中の expression を表す基底クラス
///
/// 一応 VlObj の継承クラスとして VpiObjType の値を持つ．
/// 具体的には以下のいずれか
/// - 定数系
/// - primary 系
///   - IntegerVar
///   - Net
///   - Parameter
///   - RealVar
///   - Reg
///   - SpecParam
///   - TimeVar
/// - bit_select 系
///   - BitSelect
///   - NetBit
///   - RegBit
/// - part_select 系
///   - PartSelect
///   - VarSelect
/// - FuncCall
/// - SysFuncCall
/// - Operattion
///
/// ただし，これだと使い勝手が悪いので独自に VlExprType と VlPrimaryType
/// という列挙型を用意する．
/// 注意が必要なのはネットのビット選択等で，これはプライマリ型とする．
/// これはビット指定のインデックス変換にそのネットの範囲情報が必要と
/// なるため．
/// それに対して式に対するビット選択は常に0から始まる固定オフセットを用いる．
/// 同様に式に対する部分選択も常に固定オフセットを用いる．
/// これらは配列型インスタンスの結線時にのみ用いられる．
//////////////////////////////////////////////////////////////////////
class VlExpr :
  public VlObj
{
public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式の値のタイプを返す．
  ///
  /// このタイプで評価する．
  virtual
  VlValueType
  value_type() const = 0;

  /// @brief 要求された値のタイプを返す．
  ///
  /// こちらは上位の式あるいは左辺の式から決まるタイプ
  /// 評価後にこのタイプに変換する．
  virtual
  VlValueType
  req_type() const = 0;

  /// @brief Verilog-HDL の文字列を得る．
  virtual
  std::string
  decompile() const = 0;

  /// @brief 式のビット幅を返す．
  ///
  /// value_type().size() を同じ
  virtual
  SizeType
  bit_size() const = 0;

  /// @brief 式のタイプを返す．
  virtual
  VlExprType
  expr_type() const = 0;

  /// @brief 定数式の時 true を返す．
  virtual
  bool
  is_const() const = 0;

  /// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
  virtual
  bool
  is_primary() const = 0;

  /// @brief ビット/部分選択の時に true を返す．
  virtual
  bool
  is_select() const = 0;

  /// @brief 演算子の時に true を返す．
  virtual
  bool
  is_operation() const = 0;

  /// @brief 関数呼び出しの時に true を返す．
  virtual
  bool
  is_funccall() const = 0;

  /// @brief システム関数呼び出しの時に true を返す．
  virtual
  bool
  is_sysfunccall() const = 0;

  /// @brief 左辺値の場合に要素のリストに分解する．
  ///
  /// 結果はプライマリタイプの式のみのリストとなる．
  std::vector<const VlExpr*>
  lhs_decompose() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 定数系の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数型を返す．
  ///
  /// - is_const() = true の時，意味を持つ．
  /// - それ以外ではエラーとなる．
  virtual
  VpiConstType
  constant_type() const = 0;

  /// @brief 定数値を返す．
  ///
  /// - is_const() = true の時，意味を持つ．
  /// - それ以外ではエラーとなる．
  virtual
  VlValue
  constant_value() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // プライマリ系の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief プライマリの種類を返す．
  ///
  /// is_primary() == true の場合のみ意味を持つ．
  virtual
  VlPrimaryType
  primary_type() const = 0;

  /// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
  ///
  /// - primary_type() が DECL, DECLARRAY, CONST_DECL_ELEM, VAR_DECL_ELEM
  ///   の場合のみ意味を持つ．
  virtual
  const VlDeclBase*
  decl_base() const = 0;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  ///
  /// - primary_type() が DECL の場合のみ意味を持つ．
  virtual
  const VlDecl*
  decl_obj() const = 0;

  /// @brief 宣言要素の配列への参照の場合，対象のオブジェクトを返す．
  ///
  /// - primary_type() が DECL_ARRAY, CONST_DECL_ELEM, VAR_DECL_ELEM
  ///   の場合のみ意味を持つ．
  virtual
  const VlDeclArray*
  declarray_obj() const = 0;

  /// @brief 配列型宣言要素への参照の場合，配列のインデックスリストを返す．
  ///
  /// - primary_type() が CONST_DECL_ELEM, VAR_DECL_ELEM の場合のみ意味を持つ．
  virtual
  std::vector<const VlExpr*>
  declarray_index_list() const = 0;

  /// @brief 配列型宣言要素への参照のオフセットを返す．
  ///
  /// - primary_type() が CONST_DECL_ARRAY の場合のみ意味を持つ．
  virtual
  SizeType
  declarray_offset() const = 0;

  /// @brief 宣言要素に対するビット/部分選択の種類を返す．
  ///
  /// - expr_type() が PRIMARY かつ primary_type() が DECL,
  ///   CONST_DECL_ARRAY, VAR_DECL_ARRAY の場合に意味を持つ．
  virtual
  VlSelectType
  select_type() const = 0;

  /// @brief 宣言要素に対する固定ビット/部分選択のオフセットを返す．
  ///
  /// - select_type() が CONST の場合に意味を持つ．
  /// - expr_type() が SELECT の場合に意味を持つ．
  virtual
  SizeType
  bit_offset() const = 0;

  /// @brief 宣言要素に対する可変ビット/部分選択のインデックス式を返す．
  ///
  /// - select_type() が PLUS/MINUS の場合のみ意味を持つ．
  virtual
  const VlExpr*
  bit_index() const = 0;

  /// @brief スコープへの参照の場合，対象のオブジェクトを返す．
  ///
  /// - primary_type() が SCOPE の時のみ意味を持つ．
  /// - スコープへの参照が式で用いられるのはシステム関数/タスクの引数だけ
  virtual
  const VlScope*
  scope_obj() const = 0;

  /// @brief primitive への参照の場合，対象のオブジェクトを返す．
  ///
  /// - primary_type() が PRIMITIVE  の時のみ意味を持つ．
  /// - primitiveへの参照が式で用いられるのはシステム関数/タスクの引数だけ
  virtual
  const VlPrimitive*
  primitive_obj() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // ビット/部分選択系の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親の式を返す．
  ///
  /// 式に対するビット選択/範囲選択の時，意味を持つ．
  virtual
  const VlExpr*
  parent_expr() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 演算子系の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子の型を返す．
  ///
  /// - is_operation() = true の時，意味を持つ．
  /// - それ以外ではエラーとなる．
  virtual
  VpiOpType
  op_type() const = 0;

  /// @brief オペランド数を返す．
  ///
  /// - is_operation() = true の時，意味を持つ．
  /// - それ以外ではエラーとなる．
  virtual
  SizeType
  operand_num() const = 0;

  /// @brief オペランドを返す．
  ///
  /// - is_operation() = true の時，意味を持つ．
  /// - それ以外ではエラーとなる．
  /// - pos が範囲外の時，std::out_of_range 例外を送出する．
  virtual
  const VlExpr*
  operand(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < operand_num() )
  ) const = 0;

  /// @brief オペランドのリストを返す．
  ///
  /// - is_operation() = true の時，意味を持つ．
  /// - それ以外ではエラーとなる．
  virtual
  std::vector<const VlExpr*>
  operand_list() const = 0;

  /// @brief 繰り返し数を返す．
  ///
  /// - multiple concatenation の時のみ意味を持つ．
  /// - multiple concatenation の時は operand(0) と等しい．
  virtual
  SizeType
  rep_num() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 関数呼び出し系の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 対象の関数を返す．
  ///
  /// - is_funccall() = true の時，意味を持つ．
  /// - それ以外ではエラーとなる．
  virtual
  const VlTaskFunc*
  function() const = 0;

  /// @brief 対象のシステム関数を返す．
  ///
  /// - is_sysfunccall() = true の時，意味を持つ．
  /// - それ以外ではエラーとなる．
  virtual
  const VlUserSystf*
  user_systf() const = 0;

  /// @brief 引数の数を返す．
  ///
  /// - is_funccall() = true/is_sysfunccall() = true の時，意味を持つ．
  /// - それ以外ではエラーとなる．
  virtual
  SizeType
  argument_num() const = 0;

  /// @brief 引数を返す．
  ///
  /// - is_funccall() = true/is_sysfunccall() = true の時，意味を持つ．
  /// - それ以外ではエラーとなる．
  virtual
  const VlExpr*
  argument(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < argument_num() )
  ) const = 0;

  /// @brief 引数のリストを返す．
  ///
  /// - is_funccall() = true/is_sysfunccall() = true の時，意味を持つ．
  /// - それ以外ではエラーとなる．
  virtual
  std::vector<const VlExpr*>
  argument_list() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_VEXPR_H
