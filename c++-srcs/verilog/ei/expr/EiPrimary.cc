
/// @file EiPrimary.cc
/// @brief EiPrimary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiPrimary.h"
#include "elaborator/ElbDecl.h"
#include "elaborator/ElbParameter.h"
#include "elaborator/ElbPrimitive.h"
#include "ym/vl/VlValue.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief プライマリ式を生成する．
ElbExpr*
EiFactory::new_Primary(
  const AstExpr& ast_expr,
  const VlDecl* obj
)
{
  return new EiPrimary(ast_expr, obj);
}

// @brief プライマリ式を生成する．
ElbExpr*
EiFactory::new_Primary(
  const AstIOItem& ast_item,
  const VlDecl* obj
)
{
  return new EiIOPrimary(ast_item, obj);
}

// @brief プライマリ式を生成する．
ElbExpr*
EiFactory::new_Primary(
  const AstDeclItem& ast_item,
  const VlDecl* obj
)
{
  return new EiDeclPrimary(ast_item, obj);
}

// @brief プライマリ式を生成する．
ElbExpr*
EiFactory::new_Primary(
  const AstExpr& ast_expr,
  ElbParameter* obj
)
{
  return new EiParamPrimary(ast_expr, obj);
}

// @brief プライマリ式を生成する(配列要素版)．
ElbExpr*
EiFactory::new_Primary(
  const AstExpr& ast_expr,
  const VlDeclArray* obj,
  const std::vector<ElbExpr*>& index_list
)
{
  return new EiArrayElemPrimary(ast_expr, obj, index_list);
}

// @brief プライマリ式を生成する(固定インデックスの配列要素版)．
ElbExpr*
EiFactory::new_Primary(
  const AstExpr& ast_expr,
  const VlDeclArray* obj,
  SizeType offset
)
{
  return new EiConstArrayElemPrimary(ast_expr, obj, offset);
}

// @brief システム関数/システムタスクの引数を生成する．
ElbExpr*
EiFactory::new_ArgHandle(
  const AstExpr& ast_expr,
  const VlScope* arg
)
{
  return new EiScopePrimary(ast_expr, arg);
}

// @brief システム関数/システムタスクの引数を生成する．
ElbExpr*
EiFactory::new_ArgHandle(
  const AstExpr& ast_expr,
  const VlPrimitive* arg
)
{
  return new EiPrimitivePrimary(ast_expr, arg);
}

// @brief システム関数/システムタスクの引数を生成する．
ElbExpr*
EiFactory::new_ArgHandle(
  const AstExpr& ast_expr,
  const VlDeclArray* arg
)
{
  return new EiDeclArrayPrimary(ast_expr, arg);
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimaryBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiPrimaryBase::EiPrimaryBase(
  const AstExpr& ast_expr
) : EiExprBase(ast_expr)
{
}

// @brief デストラクタ
EiPrimaryBase::~EiPrimaryBase()
{
}

// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
bool
EiPrimaryBase::is_primary() const
{
  return true;
}

// @brief 要求される式の型を計算してセットする．
void
EiPrimaryBase::_set_reqsize(
  const VlValueType& type
)
{
  // なにもしない．
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiPrimary::EiPrimary(
  const AstExpr& ast_expr,
  const VlDecl* obj
) : EiPrimaryBase(ast_expr),
    mObj{obj}
{
}

// @brief デストラクタ
EiPrimary::~EiPrimary()
{
}

// @brief 型の取得
VpiObjType
EiPrimary::type() const
{
  return mObj->type();
}

// @brief 式のタイプを返す．
VlValueType
EiPrimary::value_type() const
{
  return mObj->value_type();
}

// @brief 定数の時 true を返す．
bool
EiPrimary::is_const() const
{
  return false;
}

// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
const VlDeclBase*
EiPrimary::decl_base() const
{
  return mObj;
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
const VlDecl*
EiPrimary::decl_obj() const
{
  return mObj;
}

// @brief 左辺式の要素数の取得
SizeType
EiPrimary::lhs_elem_num() const
{
  return 1;
}

// @brief 左辺式の要素の取得
const VlExpr*
EiPrimary::lhs_elem(
  SizeType pos
) const
{
  if ( pos != 0 ) {
    throw std::logic_error{"pos != 0"};
  }
  return this;
}

// @brief 左辺式の要素のリストの取得
std::vector<const VlExpr*>
EiPrimary::lhs_elem_list() const
{
  return {this};
}


//////////////////////////////////////////////////////////////////////
// クラス EiIOPrimary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiIOPrimary::EiIOPrimary(
  const AstIOItem& ast_item,
  const VlDecl* obj
) : mAstObj{ast_item},
    mObj{obj}
{
}

// @brief デストラクタ
EiIOPrimary::~EiIOPrimary()
{
}

// @brief 型の取得
VpiObjType
EiIOPrimary::type() const
{
  return mObj->type();
}

// @brief 式のタイプを返す．
VlValueType
EiIOPrimary::value_type() const
{
  return mObj->value_type();
}

// @brief 定数の時 true を返す．
bool
EiIOPrimary::is_const() const
{
  return false;
}

// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
bool
EiIOPrimary::is_primary() const
{
  return true;
}

// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
const VlDeclBase*
EiIOPrimary::decl_base() const
{
  return mObj;;
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
const VlDecl*
EiIOPrimary::decl_obj() const
{
  return mObj;
}

// @brief Verilog-HDL の文字列を得る．
std::string
EiIOPrimary::decompile() const
{
  return mAstObj.name();
}

// @brief 左辺式の要素数の取得
SizeType
EiIOPrimary::lhs_elem_num() const
{
  return 1;
}

// @brief 左辺式の要素の取得
const VlExpr*
EiIOPrimary::lhs_elem(
  SizeType pos
) const
{
  if ( pos != 0 ) {
    throw std::logic_error{"pos != 0"};
  }
  return this;
}

// @brief 左辺式の要素のリストの取得
std::vector<const VlExpr*>
EiIOPrimary::lhs_elem_list() const
{
  return {this};
}

// @brief 要求される式の型を計算してセットする．
void
EiIOPrimary::_set_reqsize(
  const VlValueType& type
)
{
  // なにもしない．
}

// @brief パース木の定義要素を返す．
const AstBase&
EiIOPrimary::ast_obj() const
{
  return mAstObj;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclPrimary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiDeclPrimary::EiDeclPrimary(
  const AstDeclItem& ast_item,
  const VlDecl* obj
) : mAstObj{ast_item},
    mObj{obj}
{
}

// @brief デストラクタ
EiDeclPrimary::~EiDeclPrimary()
{
}

// @brief 型の取得
VpiObjType
EiDeclPrimary::type() const
{
  return mObj->type();
}

// @brief 式のタイプを返す．
VlValueType
EiDeclPrimary::value_type() const
{
  return mObj->value_type();
}

// @brief 定数の時 true を返す．
bool
EiDeclPrimary::is_const() const
{
  return false;
}

// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
bool
EiDeclPrimary::is_primary() const
{
  return true;
}

// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
const VlDeclBase*
EiDeclPrimary::decl_base() const
{
  return mObj;;
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
const VlDecl*
EiDeclPrimary::decl_obj() const
{
  return mObj;
}

// @brief Verilog-HDL の文字列を得る．
std::string
EiDeclPrimary::decompile() const
{
  return mAstObj.name();
}

// @brief 左辺式の要素数の取得
SizeType
EiDeclPrimary::lhs_elem_num() const
{
  return 1;
}

// @brief 左辺式の要素の取得
const VlExpr*
EiDeclPrimary::lhs_elem(
  SizeType pos
) const
{
  if ( pos != 0 ) {
    throw std::logic_error{"pos != 0"};
  }
  return this;
}

// @brief 左辺式の要素のリストの取得
std::vector<const VlExpr*>
EiDeclPrimary::lhs_elem_list() const
{
  return {this};
}

// @brief 要求される式の型を計算してセットする．
void
EiDeclPrimary::_set_reqsize(
  const VlValueType& type
)
{
  // なにもしない．
}

// @brief パース木の定義要素を返す．
const AstBase&
EiDeclPrimary::ast_obj() const
{
  return mAstObj;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclArrayPrimary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiDeclArrayPrimary::EiDeclArrayPrimary(
  const AstExpr& ast_expr,
  const VlDeclArray* obj
) : mAstObj{ast_expr},
    mObj{obj}
{
}

// @brief デストラクタ
EiDeclArrayPrimary::~EiDeclArrayPrimary()
{
}

// @brief 型の取得
VpiObjType
EiDeclArrayPrimary::type() const
{
  return mObj->type();
}

// @brief 式のタイプを返す．
VlValueType
EiDeclArrayPrimary::value_type() const
{
  return mObj->value_type();
}

// @brief 定数の時 true を返す．
// @note 参照している要素の型によって決まる．
bool
EiDeclArrayPrimary::is_const() const
{
  return false;
}

// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
bool
EiDeclArrayPrimary::is_primary() const
{
  return true;
}

// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
const VlDeclBase*
EiDeclArrayPrimary::decl_base() const
{
  return mObj;
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
const VlDecl*
EiDeclArrayPrimary::decl_obj() const
{
  return nullptr;
}

// @brief Verilog-HDL の文字列を得る．
std::string
EiDeclArrayPrimary::decompile() const
{
  return mAstObj.name();
}

// @brief 左辺式の要素数の取得
SizeType
EiDeclArrayPrimary::lhs_elem_num() const
{
  return 1;
}

// @brief 左辺式の要素の取得
const VlExpr*
EiDeclArrayPrimary::lhs_elem(
  SizeType pos
) const
{
  if ( pos != 0 ) {
    throw std::logic_error{"pos != 0"};
  }
  return this;
}

// @brief 左辺式の要素のリストの取得
std::vector<const VlExpr*>
EiDeclArrayPrimary::lhs_elem_list() const
{
  return {this};
}

// @brief 要求される式の型を計算してセットする．
void
EiDeclArrayPrimary::_set_reqsize(
  const VlValueType& type
)
{
  // なにもしない．
}

// @brief パース木の定義要素を返す．
const AstBase&
EiDeclArrayPrimary::ast_obj() const
{
  return mAstObj;
}


//////////////////////////////////////////////////////////////////////
// クラス EiParamPrimary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiParamPrimary::EiParamPrimary(
  const AstExpr& ast_expr,
  ElbParameter* obj
) : EiPrimaryBase(ast_expr),
    mObj{obj}
{
}

// @brief デストラクタ
EiParamPrimary::~EiParamPrimary()
{
}

// @brief 型の取得
VpiObjType
EiParamPrimary::type() const
{
  return mObj->type();
}

// @brief 式のタイプを返す．
VlValueType
EiParamPrimary::value_type() const
{
  return mObj->value_type();
}

// @brief 定数の時 true を返す．
bool
EiParamPrimary::is_const() const
{
  return true;
}

// @brief 定数値を返す．
VlValue
EiParamPrimary::constant_value() const
{
  return mObj->value();
}

// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
const VlDeclBase*
EiParamPrimary::decl_base() const
{
  return mObj;
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
const VlDecl*
EiParamPrimary::decl_obj() const
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス EiArrayElemPrimary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiArrayElemPrimary::EiArrayElemPrimary(
  const AstExpr& ast_expr,
  const VlDeclArray* obj,
  const std::vector<ElbExpr*>& index_list
) : EiPrimaryBase(ast_expr),
    mObj{obj},
    mIndexList{index_list}
{
}

// @brief デストラクタ
EiArrayElemPrimary::~EiArrayElemPrimary()
{
}

// @brief 型の取得
VpiObjType
EiArrayElemPrimary::type() const
{
  return mObj->type();
}

// @brief 式のタイプを返す．
VlValueType
EiArrayElemPrimary::value_type() const
{
  return mObj->value_type();
}

// @brief 定数の時 true を返す．
bool
EiArrayElemPrimary::is_const() const
{
  // 配列要素は定数ではない．
  return false;
}

// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
const VlDeclBase*
EiArrayElemPrimary::decl_base() const
{
  return mObj;
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
const VlDeclArray*
EiArrayElemPrimary::declarray_obj() const
{
  return mObj;
}

// @brief 配列型宣言要素への参照の場合，配列の次元を返す．
SizeType
EiArrayElemPrimary::declarray_dimension() const
{
  return mIndexList.size();
}

// @brief 配列型宣言要素への参照の場合，配列のインデックスを返す．
const VlExpr*
EiArrayElemPrimary::declarray_index(
  SizeType pos
) const
{
  return mIndexList[pos];
}

// @brief 左辺式の要素数の取得
SizeType
EiArrayElemPrimary::lhs_elem_num() const
{
  return 1;
}

// @brief 左辺式の要素の取得
const VlExpr*
EiArrayElemPrimary::lhs_elem(
  SizeType pos
) const
{
  if ( pos != 0 ) {
    throw std::logic_error{"pos != 0"};
  }
  return this;
}

// @brief 左辺式の要素のリストの取得
std::vector<const VlExpr*>
EiArrayElemPrimary::lhs_elem_list() const
{
  return {this};
}


//////////////////////////////////////////////////////////////////////
// クラス EiConstArrayElemPrimary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiConstArrayElemPrimary::EiConstArrayElemPrimary(
  const AstExpr& ast_expr,
  const VlDeclArray* obj,
  SizeType offset
) : EiPrimaryBase(ast_expr),
    mObj{obj},
    mOffset{offset}
{
}

// @brief デストラクタ
EiConstArrayElemPrimary::~EiConstArrayElemPrimary()
{
}

// @brief 型の取得
VpiObjType
EiConstArrayElemPrimary::type() const
{
  return mObj->type();
}

// @brief 式のタイプを返す．
VlValueType
EiConstArrayElemPrimary::value_type() const
{
  return mObj->value_type();
}

// @brief 定数の時 true を返す．
bool
EiConstArrayElemPrimary::is_const() const
{
  // 配列要素は定数ではない．
  return false;
}

// @brief 部分/ビット指定が定数の時 true を返す．
bool
EiConstArrayElemPrimary::is_constant_select() const
{
  return true;
}

// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
const VlDeclBase*
EiConstArrayElemPrimary::decl_base() const
{
  return mObj;
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
const VlDeclArray*
EiConstArrayElemPrimary::declarray_obj() const
{
  return mObj;
}

// @brief 配列型宣言要素への参照の場合，配列の次元を返す．
SizeType
EiConstArrayElemPrimary::declarray_dimension() const
{
  return 0;
}

// @brief 配列型宣言要素への参照の場合，配列のインデックスを返す．
const VlExpr*
EiConstArrayElemPrimary::declarray_index(
  SizeType pos
) const
{
  return nullptr;
}

// @brief 配列型宣言要素への参照のオフセットを返す．
SizeType
EiConstArrayElemPrimary::declarray_offset() const
{
  return mOffset;
}

// @brief 左辺式の要素数の取得
SizeType
EiConstArrayElemPrimary::lhs_elem_num() const
{
  return 1;
}

// @brief 左辺式の要素の取得
const VlExpr*
EiConstArrayElemPrimary::lhs_elem(
  SizeType pos
) const
{
  if ( pos != 0 ) {
    throw std::logic_error{"pos != 0"};
  }
  return this;
}

// @brief 左辺式の要素のリストの取得
std::vector<const VlExpr*>
EiConstArrayElemPrimary::lhs_elem_list() const
{
  return {this};
}


//////////////////////////////////////////////////////////////////////
// クラス EiScopePrimary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiScopePrimary::EiScopePrimary(
  const AstExpr& ast_expr,
  const VlScope* obj
) : EiPrimaryBase{ast_expr},
    mObj{obj}
{
}

// @brief デストラクタ
EiScopePrimary::~EiScopePrimary()
{
}

// @brief 型の取得
VpiObjType
EiScopePrimary::type() const
{
  return VpiObjType::Scope;
}

// @brief 式のタイプを返す．
VlValueType
EiScopePrimary::value_type() const
{
  return VlValueType();
}

// @brief 定数の時 true を返す．
bool
EiScopePrimary::is_const() const
{
  return false;
}

// @brief 対象のオブジェクトを返す．
const VlScope*
EiScopePrimary::scope_obj() const
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimitivePrimary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiPrimitivePrimary::EiPrimitivePrimary(
  const AstExpr& ast_expr,
  const VlPrimitive* obj
) : EiPrimaryBase(ast_expr),
    mObj{obj}
{
}

// @brief デストラクタ
EiPrimitivePrimary::~EiPrimitivePrimary()
{
}

// @brief 型の取得
VpiObjType
EiPrimitivePrimary::type() const
{
  return mObj->type();
}

// @brief 式のタイプを返す．
VlValueType
EiPrimitivePrimary::value_type() const
{
  return VlValueType();
}

// @brief 定数の時 true を返す．
bool
EiPrimitivePrimary::is_const() const
{
  return false;
}

// @brief 対象のオブジェクトを返す．
const VlPrimitive*
EiPrimitivePrimary::primitive_obj() const
{
  return mObj;
}

END_NAMESPACE_YM_VERILOG
