
/// @file CptItem_TaskFunc.cc
/// @brief CptItem_TaskFunc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptItem_TaskFunc.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"
//#include "parser/PtDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptItem_TaskFunc
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptItem_TaskFunc::file_region() const
{
  return mFileRegion;
}

// 名前を返す．
const char*
CptItem_TaskFunc::name() const
{
  return mName;
}

// automatic なら true
bool
CptItem_TaskFunc::automatic() const
{
  return mAutomatic;
}

// @brief IO宣言ヘッダリストの取得
const PtIOHead*
CptItem_TaskFunc::iohead_top() const
{
  return mIOHeadTop;
}

// @brief IO宣言の要素数の取得
SizeType
CptItem_TaskFunc::ioitem_num() const
{
  return mIOItemNum;
}

// @brief 宣言ヘッダリストの取得
const PtDeclHead*
CptItem_TaskFunc::declhead_top() const
{
  return mDeclHeadTop;
}

// 本体を取り出す．
const PtStmt*
CptItem_TaskFunc::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// クラス CptItem_Task
//////////////////////////////////////////////////////////////////////

// 型を返す．
AstItem::Type
CptItem_Task::type() const
{
  return AstItem::Task;
}


//////////////////////////////////////////////////////////////////////
// クラス CptItem_Function
//////////////////////////////////////////////////////////////////////

// 型を返す．
AstItem::Type
CptItem_Function::type() const
{
  return AstItem::Func;
}

// 符号の有無を返す．
bool
CptItem_Function::is_signed() const
{
  return mSigned;
}

// @brief 範囲の取得
const PtRange*
CptItem_Function::range() const
{
  return nullptr;
}

// @brief 戻値のデータ型の取得
VpiVarType
CptItem_Function::data_type() const
{
  return VpiVarType::None;
}

// constant function の展開中の印をつける．
void
CptItem_Function::set_in_use() const
{
  mInUse = true;
}

// constant function の展開中の印を消す．
void
CptItem_Function::clear_in_use() const
{
  mInUse = false;
}

// 使用中(constant function として展開中)の時 true を返す．
bool
CptItem_Function::is_in_use() const
{
  return mInUse;
}


//////////////////////////////////////////////////////////////////////
// クラス CptItem_SizedFunc
//////////////////////////////////////////////////////////////////////

// 範囲を得る．
const PtRange*
CptItem_SizedFunc::range() const
{
  return mRange;
}


//////////////////////////////////////////////////////////////////////
/// クラス CptItem_TypedFunc
//////////////////////////////////////////////////////////////////////

// 戻値の型を返す．
VpiVarType
CptItem_TypedFunc::data_type() const
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
  const PtIOHead* iohead_top,
  const PtDeclHead* declhead_top,
  const PtStmt* stmt
)
{
  void* p = mAlloc.get_memory(sizeof(CptItem_Task));
  return new (p) CptItem_Task(file_region, name, automatic,
			      iohead_top, declhead_top,
			      stmt);
}

// function 文を生成する．
PtItem*
PtFactory::new_Function(
  const FileRegion& file_region,
  const char* name,
  bool automatic,
  const PtIOHead* iohead_top,
  const PtDeclHead* declhead_top,
  const PtStmt* stmt
)
{
  void* p = mAlloc.get_memory(sizeof(CptItem_Function));
  return new (p) CptItem_Function(file_region, name, automatic, false,
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
  const PtRange* range,
  const PtIOHead* iohead_top,
  const PtDeclHead* declhead_top,
  const PtStmt* stmt
)
{
  void* p = mAlloc.get_memory(sizeof(CptItem_SizedFunc));
  return new (p) CptItem_SizedFunc(file_region,
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
  const PtIOHead* iohead_top,
  const PtDeclHead* declhead_top,
  const PtStmt* stmt
)
{
  void* p = mAlloc.get_memory(sizeof(CptItem_TypedFunc));
  return new (p) CptItem_TypedFunc(file_region, name,
				   automatic, sign, func_type,
				   iohead_top, declhead_top,
				   stmt);
}

END_NAMESPACE_YM_VERILOG
