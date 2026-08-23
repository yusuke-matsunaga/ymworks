
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
// クラス CptPathDeclBase
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptPathDeclBase::file_region() const
{
  return mFileRegion;
}

// edge_descriptor を取り出す．
int
CptPathDeclBase::edge() const
{
  return mEdge;
}

// @brief 入力のリストの要素数の取得
SizeType
CptPathDeclBase::input_num() const
{
  return mInputList->size();
}

// @brief 入力の取得
const AstExpr*
CptPathDeclBase::input(
  SizeType index
) const
{
  return mInputList->expr(index);
}

// @brief 入力のリストの取得
AstExprVec
CptPathDeclBase::input_list() const
{
  return mInputList->to_vector();
}

// 入力の極性を取り出す．
int
CptPathDeclBase::input_pol() const
{
  return mInputPol;
}

// パス記述子(?)を得る．vpiParallel か vpiFull
VpiPathType
CptPathDeclBase::op() const
{
  return mOp;
}

// 出力の極性を取り出す．
int
CptPathDeclBase::output_pol() const
{
  return mOutputPol;
}

// 式を取り出す．
const AstExpr*
CptPathDeclBase::expr() const
{
  return mExpr;
}

// path_delay_value を取り出す．
const AstPathDelay*
CptPathDeclBase::path_delay() const
{
  return mPathDelay;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPathDecl1
//////////////////////////////////////////////////////////////////////

// @brief 出力のリストの要素数の取得
SizeType
CptPathDecl1::output_num() const
{
  return 1;
}

// @brief 出力の取得
const AstExpr*
CptPathDecl1::output(
  SizeType index
) const
{
  if ( index > 0 ) {
    throw std::out_of_range{"output(index): index is out of range"};
  }
  return mOutput;
}

// @brief 出力リストの取得
AstExprVec
CptPathDecl1::output_list() const
{
  return {mOutput};
}


//////////////////////////////////////////////////////////////////////
// クラス CptPatHDecl2
//////////////////////////////////////////////////////////////////////

// @brief 出力のリストの要素数の取得
SizeType
CptPathDecl2::output_num() const
{
  return mOutputList->size();
}

// @brief 出力の取得
const AstExpr*
CptPathDecl2::output(
  SizeType index
) const
{
  return mOutputList->expr(index);
}

// @brief 出力リストの取得
AstExprVec
CptPathDecl2::output_list() const
{
  return mOutputList->to_vector();
}


//////////////////////////////////////////////////////////////////////
// クラス CptPathDelayBase
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptPathDelayBase::file_region() const
{
  return mFileRegion;
}

// 値を取り出す．
const AstExpr*
CptPathDelayBase::value(
  SizeType index
) const
{
  if ( index >= 12 ) {
    throw std::out_of_range{"value(index): index is out of range"};
  }
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPathDelay1
//////////////////////////////////////////////////////////////////////

// 値を取り出す．
const AstExpr*
CptPathDelay1::value(
  SizeType index
) const
{
  if ( index == 0 ) {
    return mValue;
  }
  return CptPathDelayBase::value(index);
}


//////////////////////////////////////////////////////////////////////
// クラス CptPathDelay2
//////////////////////////////////////////////////////////////////////

// 値を取り出す．
const AstExpr*
CptPathDelay2::value(
  SizeType index
) const
{
  if ( index < 2 ) {
    return mValues[index];
  }
  return CptPathDelayBase::value(index);
}


//////////////////////////////////////////////////////////////////////
// クラス CptPathDelay3
//////////////////////////////////////////////////////////////////////

// 値を取り出す．
const AstExpr*
CptPathDelay3::value(
  SizeType index
) const
{
  if ( index < 3 ) {
    return mValues[index];
  }
  return CptPathDelayBase::value(index);
}


//////////////////////////////////////////////////////////////////////
// クラス CptPathDelay6
//////////////////////////////////////////////////////////////////////

// 値を取り出す．
const AstExpr*
CptPathDelay6::value(
  SizeType index
) const
{
  if ( index < 6 ) {
    return mValues[index];
  }
  return CptPathDelayBase::value(index);
}


//////////////////////////////////////////////////////////////////////
// クラス CptPathDelay12
//////////////////////////////////////////////////////////////////////

// 値を取り出す．
const AstExpr*
CptPathDelay12::value(
  SizeType index
) const
{
  if ( index < 12 ) {
    return mValues[index];
  }
  return CptPathDelayBase::value(index);
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
  const AstExpr* output,
  int output_pol,
  const AstExpr* expr,
  const AstPathDelay* path_delay
)
{
  void* p = mAlloc.get_memory(sizeof(CptPathDecl1));
  return new (p) CptPathDecl1(file_region, edge,
			      input_list, input_pol,
			      op,
			      output, output_pol,
			      expr, path_delay);
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
  void* p = mAlloc.get_memory(sizeof(CptPathDecl2));
  return new (p) CptPathDecl2(file_region, edge,
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
  void* p = mAlloc.get_memory(sizeof(CptPathDelay1));
  return new (p) CptPathDelay1(file_region, value);
}

// path delay value を生成する．
PtPathDelay*
PtFactory::new_PathDelay(
  const FileRegion& file_region,
  const AstExpr* value1,
  const AstExpr* value2
)
{
  void* p = mAlloc.get_memory(sizeof(CptPathDelay2));
  return new (p) CptPathDelay2(file_region, value1, value2);
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
  void* p = mAlloc.get_memory(sizeof(CptPathDelay3));
  return new (p) CptPathDelay3(file_region, value1, value2, value3);
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
  void* p = mAlloc.get_memory(sizeof(CptPathDelay6));
  return new (p) CptPathDelay6(file_region,
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
  void* p = mAlloc.get_memory(sizeof(CptPathDelay12));
  return new (p) CptPathDelay12(file_region,
				value1, value2, value3,
				value4, value5, value6,
				value7, value8, value9,
				value10, value11, value12);
}

END_NAMESPACE_YM_VERILOG
