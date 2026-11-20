
/// @file EiExpr.cc
/// @brief EiExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiExpr.h"
#include "EiOperation.h"
#include "ym/vl/BitVector.h"
#include "ym/vl/VlTime.h"
#include "ym/vl/VlValue.h"
#include "ym/vl/AstExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス EiExpr
//////////////////////////////////////////////////////////////////////

// @brief 定数の時 true を返す．
bool
EiExpr::is_const() const
{
  return false;
}

// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
bool
EiExpr::is_primary() const
{
  return false;
}

// @brief 演算子の時に true を返す．
bool
EiExpr::is_operation() const
{
  return false;
}

// @brief 関数呼び出しの時に true を返す．
bool
EiExpr::is_funccall() const
{
  return false;
}

// @brief システム関数よびあどい時に true を返す．
bool
EiExpr::is_sysfunccall() const
{
  return false;
}

// @brief 左辺式の時に true を返す．
bool
EiExpr::is_lhs() const
{
  return false;
}

// @brief 定数の型を返す．
VpiConstType
EiExpr::constant_type() const
{
  throw std::logic_error{"EiExpr::constant_type()"};
}

// @brief 定数値を返す．
VlValue
EiExpr::constant_value() const
{
  throw std::logic_error{"EiExpr::constant_value()"};
}

// @brief プライマリの種類を返す．
VlPrimaryType
EiExpr::primary_type() const
{
  throw std::logic_error{"EiExpr::primary_type()"};
}

// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
const VlDeclBase*
EiExpr::decl_base() const
{
  throw std::logic_error{"EiExpr::decl_base()"};
}

// @brief 対象のオブジェクトを返す．
const VlDecl*
EiExpr::decl_obj() const
{
  throw std::logic_error{"EiExpr::decl_obj()"};
}

// @brief 宣言要素の配列への参照の場合，対象のnオブジェクトを返す．
const VlDeclArray*
EiExpr::declarray_obj() const
{
  throw std::logic_error{"EiExpr::declarray_obj()"};
}

// @brief 配列型宣言要素への参照の場合，配列のインデックスリストを返す．
std::vector<const VlExpr*>
EiExpr::declarray_index_list() const
{
  throw std::logic_error{"EiExpr::declarray_index_list()"};
}

// @brief 配列型宣言要素への参照のオフセットを返す．
SizeType
EiExpr::declarray_offset() const
{
  throw std::logic_error{"EiExpr::declarray_offset()"};
}

// @brief 宣言要素の番号を返す．
SizeType
EiExpr::decl_id() const
{
  throw std::logic_error{"EiExpr::decl_id()"};
}

// @brief 宣言要素に対するビット/部分選択の種類を返す．
VlSelectType
EiExpr::select_type() const
{
  throw std::logic_error{"EiExpr::select_type()"};
}

// @brief 宣言要素に対する固定ビット/部分選択のオフセットを返す．
SizeType
EiExpr::bit_offset() const
{
  throw std::logic_error{"EiExpr::bit_offset()"};
}

// @brief 宣言要素に対する可変ビット/部分選択のインデックス式を返す．
const VlExpr*
EiExpr::bit_index() const
{
  throw std::logic_error{"EiExpr::bit_index()"};
}

// @brief 対象のオブジェクトを返す．
const VlScope*
EiExpr::scope_obj() const
{
  throw std::logic_error{"EiExpr::scope_obj()"};
}

// @brief 対象のオブジェクトを返す．
const VlPrimitive*
EiExpr::primitive_obj() const
{
  throw std::logic_error{"EiExpr::primitive_obj()"};
}

// @brief 演算子のタイプを返す．
VpiOpType
EiExpr::op_type() const
{
  throw std::logic_error{"EiExpr::op_type()"};
}

// @brief オペランド数を返す．
SizeType
EiExpr::operand_num() const
{
  throw std::logic_error{"EiExpr::operand_num()"};
}

// @brief オペランドを返す．
const VlExpr*
EiExpr::operand(
  SizeType pos
) const
{
  throw std::logic_error{"EiExpr::operand(pos)"};
}

// @brief オペランドのリストを返す．
std::vector<const VlExpr*>
EiExpr::operand_list() const
{
  throw std::logic_error{"EiExpr::operand_list()"};
}

// @brief 繰り返し数を返す．
SizeType
EiExpr::rep_num() const
{
  throw std::logic_error{"EiExpr::rep_num()"};
}

// @brief 対象の関数を返す．
const VlTaskFunc*
EiExpr::function() const
{
  throw std::logic_error{"EiExpr::function()"};
}

// @brief 対象のシステム関数を返す．
const VlUserSystf*
EiExpr::user_systf() const
{
  throw std::logic_error{"EiExpr::user_systf()"};
}

// @brief 引数の数を返す．
SizeType
EiExpr::argument_num() const
{
  throw std::logic_error{"EiExpr::argument_num()"};
}

// @brief 引数を返す．
const VlExpr*
EiExpr::argument(
  SizeType pos
) const
{
  throw std::logic_error{"EiExpr::argument(pos)"};
}

// @brief 引数のリストを返す．
std::vector<const VlExpr*>
EiExpr::argument_list() const
{
  throw std::logic_error{"EiExpr::argument_list()"};
}

// @brief 要素数を返す．
SizeType
EiExpr::lhs_elem_num() const
{
  throw std::logic_error{"EiExpr::lhs_elem_num()"};
}

// @brief 要素を返す．
const VlExpr*
EiExpr::lhs_elem(
  SizeType index
) const
{
  throw std::logic_error{"EiExpr::lhs_elem()"};
}

// @brief 要素のリストを返す．
std::vector<const VlExpr*>
EiExpr::lhs_elem_list() const
{
  throw std::logic_error{"EiExpr::lhs_elem_list()"};
}


//////////////////////////////////////////////////////////////////////
// クラス EiExprBase
//////////////////////////////////////////////////////////////////////

// @brief ファイル位置を返す．
FileRegion
EiExprBase::file_region() const
{
  return ast_expr().file_region();
}

// @brief Verilog-HDL の文字列を得る．
std::string
EiExprBase::decompile() const
{
  return ast_expr().decompile();
}


//////////////////////////////////////////////////////////////////////
// クラス EiOperation
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiOperation::type() const
{
  return VpiObjType::Operation;
}

// @brief 式のタイプを返す．
VlExprType
EiOperation::expr_type() const
{
  return VlExprType::OPERATION;
}

// @brief 演算子の時に true を返す．
bool
EiOperation::is_operation() const
{
  return true;
}

// @brief 演算子のタイプを返す．
VpiOpType
EiOperation::op_type() const
{
  return ast_expr().op_type();
}

END_NAMESPACE_YM_VERILOG
