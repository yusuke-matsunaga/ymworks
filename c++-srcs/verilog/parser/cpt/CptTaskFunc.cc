
/// @file CptTaskFunc.cc
/// @brief CptTaskFunc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptTaskFunc.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"
#include "parser/PtDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptTf
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptTf::file_region() const
{
  return mFileRegion;
}

// 名前を返す．
const char*
CptTf::name() const
{
  return mName;
}

// automatic なら true
bool
CptTf::automatic() const
{
  return mAutomatic;
}

// @brief IO宣言ヘッダリストの取得
AstIOHeadList
CptTf::iohead_list() const
{
  return AstIOHeadList(mIOHeadTop);
}

// @brief IO宣言の要素数の取得
SizeType
CptTf::ioitem_num() const
{
  return mIOItemNum;
}

// @brief 宣言ヘッダリストの取得
AstDeclHeadList
CptTf::declhead_list() const
{
  return AstDeclHeadList(mDeclHeadTop);
}

// 本体を取り出す．
const AstStmt*
CptTf::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// クラス CptTask
//////////////////////////////////////////////////////////////////////

// 型を返す．
AstItem::Type
CptTask::type() const
{
  return Task;
}


//////////////////////////////////////////////////////////////////////
// クラス CptFunction
//////////////////////////////////////////////////////////////////////

// 型を返す．
AstItem::Type
CptFunction::type() const
{
  return Func;
}

// 符号の有無を返す．
bool
CptFunction::is_signed() const
{
  return mSigned;
}

// @brief 範囲の取得
const AstRange*
CptFunction::range() const
{
  return nullptr;
}

// @brief 戻値のデータ型の取得
VpiVarType
CptFunction::data_type() const
{
  return VpiVarType::None;
}

// constant function の展開中の印をつける．
void
CptFunction::set_in_use() const
{
  mInUse = true;
}

// constant function の展開中の印を消す．
void
CptFunction::clear_in_use() const
{
  mInUse = false;
}

// 使用中(constant function として展開中)の時 true を返す．
bool
CptFunction::is_in_use() const
{
  return mInUse;
}


//////////////////////////////////////////////////////////////////////
// クラス CptSizedFunc
//////////////////////////////////////////////////////////////////////

// 範囲を得る．
const AstRange*
CptSizedFunc::range() const
{
  return mRange;
}


//////////////////////////////////////////////////////////////////////
/// クラス CptTypedFunc
//////////////////////////////////////////////////////////////////////

// 戻値の型を返す．
VpiVarType
CptTypedFunc::data_type() const
{
  return mDataType;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// task 文を生成する．
PtItem*
PtFactory::new_Task(
  const FileRegion& file_region,
  const char* name,
  bool automatic,
  PtIOHead* iohead_top,
  PtDeclHead* declhead_top,
  const AstStmt* stmt
)
{
  void* p = mAlloc.get_memory(sizeof(CptTask));
  return new (p) CptTask(file_region, name, automatic,
			 iohead_top, declhead_top,
			 stmt);
}

// function 文を生成する．
PtItem*
PtFactory::new_Function(
  const FileRegion& file_region,
  const char* name,
  bool automatic,
  PtIOHead* iohead_top,
  PtDeclHead* declhead_top,
  const AstStmt* stmt
)
{
  void* p = mAlloc.get_memory(sizeof(CptFunction));
  return new (p) CptFunction(file_region, name, automatic, false,
			     iohead_top, declhead_top,
			     stmt);
}

// ビットベクタ型 function の生成
PtItem*
PtFactory::new_SizedFunc(
  const FileRegion& file_region,
  const char* name,
  bool automatic,
  bool sign,
  const AstRange* range,
  PtIOHead* iohead_top,
  PtDeclHead* declhead_top,
  const AstStmt* stmt
)
{
  void* p = mAlloc.get_memory(sizeof(CptSizedFunc));
  return new (p) CptSizedFunc(file_region,
			      name, automatic,
			      sign, range,
			      iohead_top, declhead_top,
			      stmt);
}

// 組み込み型 function の生成
PtItem*
PtFactory::new_TypedFunc(
  const FileRegion& file_region,
  const char* name,
  bool automatic,
  bool sign,
  VpiVarType func_type,
  PtIOHead* iohead_top,
  PtDeclHead* declhead_top,
  const AstStmt* stmt
)
{
  void* p = mAlloc.get_memory(sizeof(CptTypedFunc));
  return new (p) CptTypedFunc(file_region, name,
			      automatic, sign, func_type,
			      iohead_top, declhead_top,
			      stmt);
}

END_NAMESPACE_YM_VERILOG
