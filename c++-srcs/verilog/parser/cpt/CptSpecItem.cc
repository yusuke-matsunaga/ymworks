
/// @file CptSpecItem.cc
/// @brief CptSpecItem の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptSpecItem.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// specify_block の item を表すクラス
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptSpecItem::file_region() const
{
  return mFileRegion;
}

// 型を返す．
AstItem::Type
CptSpecItem::type() const
{
  return SpecItem;
}

// トークン番号を返す．??? たぶん変更される．
VpiSpecItemType
CptSpecItem::specitem_type() const
{
  return mId;
}

// @brief ターミナルの要素数の取得
SizeType
CptSpecItem::terminal_num() const
{
  return mTerminalList->size();
}

// @brief ターミナルの取得
const AstExpr*
CptSpecItem::terminal(
  SizeType index
) const
{
  return mTerminalList->expr(index);
}

// @brief ターミナルリストの取得
AstExprVec
CptSpecItem::terminal_list() const
{
  return mTerminalList->to_vector();
}


//////////////////////////////////////////////////////////////////////
// specify_block の path_declaration を表すクラス
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptSpecPath::file_region() const
{
  return mFileRegion;
}

// 型を返す．
AstItem::Type
CptSpecPath::type() const
{
  return SpecPath;
}

// トークン番号を返す．??? たぶん変更される．
VpiSpecPathType
CptSpecPath::specpath_type() const
{
  return mId;
}

// モジュールパスの式を返す．
const AstExpr*
CptSpecPath::cond_expr() const
{
  return mExpr;
}

// パス記述を返す．
const AstPathDecl*
CptSpecPath::path_decl() const
{
  return mPathDecl;
}


//////////////////////////////////////////////////////////////////////
// path_decl を表すクラス
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptPathDecl::file_region() const
{
  return mFileRegion;
}

// edge_descriptor を取り出す．
int
CptPathDecl::edge() const
{
  return mEdge;
}

// @brief 入力のリストの要素数の取得
SizeType
CptPathDecl::input_num() const
{
  return mInputList->size();
}

// @brief 入力の取得
const AstExpr*
CptPathDecl::input(
  SizeType index
) const
{
  return mInputList->expr(index);
}

// @brief 入力のリストの取得
AstExprVec
CptPathDecl::input_list() const
{
  return mInputList->to_vector();
}

// 入力の極性を取り出す．
int
CptPathDecl::input_pol() const
{
  return mInputPol;
}

// パス記述子(?)を得る．vpiParallel か vpiFull
VpiPathType
CptPathDecl::op() const
{
  return mOp;
}

// @brief 出力のリストの要素数の取得
SizeType
CptPathDecl::output_num() const
{
  return mOutputList->size();
}

// @brief 出力の取得
const AstExpr*
CptPathDecl::output(
  SizeType index
) const
{
  return mOutputList->expr(index);
}

// @brief 出力リストの取得
AstExprVec
CptPathDecl::output_list() const
{
  return mOutputList->to_vector();
}

// 出力の極性を取り出す．
int
CptPathDecl::output_pol() const
{
  return mOutputPol;
}

// 式を取り出す．
const AstExpr*
CptPathDecl::expr() const
{
  return mExpr;
}

// path_delay_value を取り出す．
const AstPathDelay*
CptPathDecl::path_delay() const
{
  return mPathDelay;
}


//////////////////////////////////////////////////////////////////////
// path_delay_value を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPathDelay::CptPathDelay(
  const FileRegion& file_region,
  const AstExpr* value
) : mFileRegion{file_region},
    mValues{value,   nullptr, nullptr,
	    nullptr, nullptr, nullptr,
	    nullptr, nullptr, nullptr,
	    nullptr, nullptr, nullptr}
{
}

CptPathDelay::CptPathDelay(
  const FileRegion& file_region,
  const AstExpr* value1,
  const AstExpr* value2
) : mFileRegion{file_region},
    mValues{value1,  value2,  nullptr,
	    nullptr, nullptr, nullptr,
	    nullptr, nullptr, nullptr,
	    nullptr, nullptr, nullptr}

{
}
CptPathDelay::CptPathDelay(
  const FileRegion& file_region,
  const AstExpr* value1,
  const AstExpr* value2,
  const AstExpr* value3
) : mFileRegion{file_region},
    mValues{value1,  value2,  value3,
	    nullptr, nullptr, nullptr,
	    nullptr, nullptr, nullptr,
	    nullptr, nullptr, nullptr}
{
}

CptPathDelay::CptPathDelay(
  const FileRegion& file_region,
  const AstExpr* value1,
  const AstExpr* value2,
  const AstExpr* value3,
  const AstExpr* value4,
  const AstExpr* value5,
  const AstExpr* value6
) : mFileRegion{file_region},
    mValues{value1, value2, value3,
	    value4, value5, value6,
	    nullptr, nullptr, nullptr,
	    nullptr, nullptr, nullptr}
{
}

CptPathDelay::CptPathDelay(
  const FileRegion& file_region,
  const AstExpr* value1,
  const AstExpr* value2,
  const AstExpr* value3,
  const AstExpr* value4,
  const AstExpr* value5,
  const AstExpr* value6,
  const AstExpr* value7,
  const AstExpr* value8,
  const AstExpr* value9,
  const AstExpr* value10,
  const AstExpr* value11,
  const AstExpr* value12
) : mFileRegion{file_region},
    mValues{value1,  value2,  value3,
	    value4,  value5,  value6,
	    value7,  value8,  value9,
	    value10, value11, value12}
{
}

// デストラクタ
CptPathDelay::~CptPathDelay()
{
}

// ファイル位置を返す．
FileRegion
CptPathDelay::file_region() const
{
  return mFileRegion;
}

// 値を取り出す．
const AstExpr*
CptPathDelay::value(
  SizeType pos
) const
{
  if ( pos >= 12 ) {
    throw std::out_of_range{"pos is out of range"};
  }
  return mValues[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// specify block item を生成する．
PtItem*
PtFactory::new_SpecItem(
  const FileRegion& file_region,
  VpiSpecItemType id,
  const AstExprList* terminal_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptSpecItem));
  return new (p) CptSpecItem(file_region, id, terminal_list);
}

// path 仕様を生成する．
PtItem*
PtFactory::new_SpecPath(
  const FileRegion& file_region,
  VpiSpecPathType id,
  const AstExpr* expr,
  const AstPathDecl* path_decl
)
{
  void* p = mAlloc.get_memory(sizeof(CptSpecPath));
  return new (p) CptSpecPath(file_region, id, expr, path_decl);
}

// path 記述を生成する．
PtPathDecl*
PtFactory::new_PathDecl(
  const FileRegion& file_region,
  int edge,
  const AstExprList* input_list,
  int input_pol,
  VpiPathType op,
  const AstExprList* output_list,
  int output_pol,
  const AstExpr* expr,
  const AstPathDelay* path_delay
)
{
  void* p = mAlloc.get_memory(sizeof(CptPathDecl));
  return new (p) CptPathDecl(file_region, edge,
			     input_list, input_pol,
			     op,
			     output_list, output_pol,
			     expr, path_delay);
}

// path delay value を生成する．
PtPathDelay*
PtFactory::new_PathDelay(
  const FileRegion& file_region,
  const AstExpr* value
)
{
  void* p = mAlloc.get_memory(sizeof(CptPathDelay));
  return new (p) CptPathDelay(file_region, value);
}

// path delay value を生成する．
PtPathDelay*
PtFactory::new_PathDelay(
  const FileRegion& file_region,
  const AstExpr* value1,
  const AstExpr* value2
)
{
  void* p = mAlloc.get_memory(sizeof(CptPathDelay));
  return new (p) CptPathDelay(file_region, value1, value2);
}

// path delay value を生成する．
PtPathDelay*
PtFactory::new_PathDelay(
  const FileRegion& file_region,
  const AstExpr* value1,
  const AstExpr* value2,
  const AstExpr* value3
)
{
  void* p = mAlloc.get_memory(sizeof(CptPathDelay));
  return new (p) CptPathDelay(file_region, value1, value2, value3);
}

// path delay value を生成する．
PtPathDelay*
PtFactory::new_PathDelay(
  const FileRegion& file_region,
  const AstExpr* value1,
  const AstExpr* value2,
  const AstExpr* value3,
  const AstExpr* value4,
  const AstExpr* value5,
  const AstExpr* value6
)
{
  void* p = mAlloc.get_memory(sizeof(CptPathDelay));
  return new (p) CptPathDelay(file_region,
			      value1, value2, value3,
			      value4, value5, value6);
}

// path delay value を生成する．
PtPathDelay*
PtFactory::new_PathDelay(
  const FileRegion& file_region,
  const AstExpr* value1,
  const AstExpr* value2,
  const AstExpr* value3,
  const AstExpr* value4,
  const AstExpr* value5,
  const AstExpr* value6,
  const AstExpr* value7,
  const AstExpr* value8,
  const AstExpr* value9,
  const AstExpr* value10,
  const AstExpr* value11,
  const AstExpr* value12
)
{
  void* p = mAlloc.get_memory(sizeof(CptPathDelay));
  return new (p) CptPathDelay(file_region,
			      value1, value2, value3,
			      value4, value5, value6,
			      value7, value8, value9,
			      value10, value11, value12);
}

END_NAMESPACE_YM_VERILOG
