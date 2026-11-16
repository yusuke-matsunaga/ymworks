#ifndef EIEXPR_H
#define EIEXPR_H

/// @file EiExpr.h
/// @brief EiExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbExpr.h"
#include "ym/vl/Ast.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiExpr EiExpr.h "EiExpr.h"
/// @brief ExprExpr の実装クラス
//////////////////////////////////////////////////////////////////////
class EiExpr :
  public ElbExpr
{
protected:

  /// @brief コンストラクタ
  EiExpr() = default;

  /// @brief デストラクタ
  ~EiExpr() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数の時 true を返す．
  bool
  is_const() const override;

  /// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
  bool
  is_primary() const override;

  /// @brief 演算子の時に true を返す．
  bool
  is_operation() const override;

  /// @brief 関数呼び出しの時に true を返す．
  bool
  is_funccall() const override;

  /// @brief システム関数よびあどい時に true を返す．
  bool
  is_sysfunccall() const override;

  /// @brief 左辺式の時に true を返す．
  bool
  is_lhs() const override;

  /// @brief 定数の型を返す．
  VpiConstType
  constant_type() const override;

  /// @brief 定数値を返す．
  VlValue
  constant_value() const override;

  /// @brief プライマリの種類を返す．
  ///
  /// is_primary() == true の場合のみ意味を持つ．
  VlPrimaryType
  primary_type() const override;

  /// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
  const VlDeclBase*
  decl_base() const override;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  const VlDecl*
  decl_obj() const override;

  /// @brief 宣言要素の配列への参照の場合，対象のオブジェクトを返す．
  const VlDeclArray*
  declarray_obj() const override;

  /// @brief 配列型宣言要素への参照の場合，配列のインデックスリストを返す．
  ///
  /// - primary_type() が CONST_DECL_ELEM, VAR_DECL_ELEM の場合のみ意味を持つ．
  std::vector<const VlExpr*>
  declarray_index_list() const override;

  /// @brief 配列型宣言要素への参照のオフセットを返す．
  SizeType
  declarray_offset() const override;

  /// @brief 宣言要素の番号を返す．
  SizeType
  decl_id() const override;

  /// @brief 宣言要素に対するビット/部分選択の種類を返す．
  ///
  /// - expr_type() が PRIMARY かつ primary_type() が DECL,
  ///   CONST_DECL_ARRAY, VAR_DECL_ARRAY の場合に意味を持つ．
  VlSelectType
  select_type() const override;

  /// @brief 宣言要素に対する固定ビット/部分選択のオフセットを返す．
  ///
  /// - has_const_bit_select() == true の場合のみ意味を持つ．
  SizeType
  bit_offset() const override;

  /// @brief 宣言要素に対する可変ビット/部分選択のインデックス式を返す．
  ///
  /// - has_plus_bit_select() か has_minus_bit_select() が true
  ///   の場合のみ意味を持つ．
  const VlExpr*
  bit_index() const override;

  /// @brief スコープへの参照の場合，対象のオブジェクトを返す．
  const VlScope*
  scope_obj() const override;

  /// @brief primitive への参照の場合，対象のオブジェクトを返す．
  const VlPrimitive*
  primitive_obj() const override;

  /// @brief 演算子のタイプを返す．
  VpiOpType
  op_type() const override;

  /// @brief オペランド数を返す．
  SizeType
  operand_num() const override;

  /// @brief オペランドを返す．
  const VlExpr*
  operand(
    SizeType pos ///< [in] 位置番号
  ) const override;

  /// @brief オペランドのリストを返す．
  std::vector<const VlExpr*>
  operand_list() const override;

  /// @brief 繰り返し数を返す．
  SizeType
  rep_num() const override;

  /// @brief 対象の関数を返す．
  const VlTaskFunc*
  function() const override;

  /// @brief 対象のシステム関数を返す．
  const VlUserSystf*
  user_systf() const override;

  /// @brief 引数の数を返す．
  SizeType
  argument_num() const override;

  /// @brief 引数を返す．
  const VlExpr*
  argument(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < argument_num() )
  ) const override;

  /// @brief 引数のリストを返す．
  std::vector<const VlExpr*>
  argument_list() const override;

  /// @brief 要素数を返す．
  SizeType
  lhs_elem_num() const override;

  /// @brief 要素を返す．
  const VlExpr*
  lhs_elem(
    SizeType index ///< [in] インデックス ( 0 <= index < lhs_elem_num() )
  ) const override;

};


//////////////////////////////////////////////////////////////////////
/// @class EiExprBase EiExpr.h "EiExpr.h"
/// @brief AstExpr を持つ EiExpr の派生クラス
//////////////////////////////////////////////////////////////////////
class EiExprBase :
  public EiExpr
{
protected:

  /// @brief コンストラクタ
  EiExprBase(
    const AstExpr& ast_expr ///< [in] パース木の定義要素
  ) : mAstExpr{ast_expr}
  {
  }

  /// @brief デストラクタ
  ~EiExprBase() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Verilog-HDL の文字列を得る．
  std::string
  decompile() const override;


protected:
  //////////////////////////////////////////////////////////////////////
  // 派生クラスから使われる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief AstExpr を得る．
  const AstExpr&
  ast_expr() const { return mAstExpr; }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の定義要素
  AstExpr mAstExpr;

};

END_NAMESPACE_YM_VERILOG

#endif // EIEXPR_H
