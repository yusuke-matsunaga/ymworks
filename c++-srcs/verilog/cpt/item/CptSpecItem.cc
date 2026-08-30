
/// @file CptSpecItem.cc
/// @brief CptSpecItem の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptSpecItem.h"
#include "alloc/Alloc.h"
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

// @brief ターミナルリストの取得
AstExprList
CptSpecItem::terminal_list() const
{
  return AstExprList(mTerminalTop);
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
// クラス CptPathDecl
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

// @brief 入力のリストの取得
AstExprList
CptPathDecl::input_list() const
{
  return AstExprList(mInputTop);
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

// @brief 出力リストの取得
AstExprList
CptPathDecl::output_list() const
{
  return AstExprList(mOutputTop);
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
  PtExpr* terminal_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptSpecItem));
  return new (p) CptSpecItem(file_region, id, terminal_top);
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
  PtExpr* input_top,
  int input_pol,
  VpiPathType op,
  PtExpr* output_top,
  int output_pol,
  const AstExpr* expr,
  const AstPathDelay* path_delay
)
{
  void* p = mAlloc.get_memory(sizeof(CptPathDecl));
  return new (p) CptPathDecl(file_region, edge,
			     input_top, input_pol,
			     op,
			     output_top, output_pol,
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
