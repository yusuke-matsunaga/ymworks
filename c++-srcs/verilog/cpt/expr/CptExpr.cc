
/// @file CptExpr.cc
/// @brief CptExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptExpr.h"
#include "CptFuncCall.h"
#include "alloc/Alloc.h"
#include "ym/vl/BitVector.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptExpr
//////////////////////////////////////////////////////////////////////

// @brief 階層ブランチのリストの先頭を返す．
const PtNameBranch*
CptExpr::namebranch_top() const
{
  throw std::logic_error{"namebranch_top(): type mismatch"};
}

// @brief 末尾の名前の取得
const char*
CptExpr::name() const
{
  throw std::logic_error{"name(): type mismatch"};
}

// @brief 演算子の種類の取得
VpiOpType
CptExpr::op_type() const
{
  throw std::logic_error{"op_type(): type mismatch"};
}

// @brief 固定オペランド数の取得
SizeType
CptExpr::operand_num() const
{
  throw std::logic_error{"operand_num(): type mismatch"};
}

// @brief 0番目のオペランドの取得
const PtExpr*
CptExpr::operand0() const
{
  throw std::logic_error{"operand0(): type mismtach"};
}

// @brief 1番目のオペランドの取得
const PtExpr*
CptExpr::operand1() const
{
  throw std::logic_error{"operand0(): type mismtach"};
}

// @brief 2番目のオペランドの取得
const PtExpr*
CptExpr::operand2() const
{
  throw std::logic_error{"operand0(): type mismtach"};
}

// @brief オペランドのリストの取得
const PtExpr*
CptExpr::operand_top() const
{
  throw std::logic_error{"operand_top(): type mismatch"};
}

// @brief multi-concat の繰り返し数
const PtExpr*
CptExpr::rep() const
{
  throw std::logic_error{"rep(): Not a multi-concat oprator"};
}

// @brief 引数リストの取得
const PtExpr*
CptExpr::arg_top() const
{
  throw std::logic_error{"arg_top(): type mismatch"};
}

// @brief 定数インデックスのチェック
bool
CptExpr::is_const_index() const
{
  throw std::logic_error{"is_const_index(): type mismatch"};
}

// @brief インデックスリストの取得
const PtExpr*
CptExpr::index_top() const
{
  throw std::logic_error{"index_top(); type mismatch"};
}

// @brief 範囲指定の取得
const PtPart*
CptExpr::part() const
{
  throw std::logic_error{"part(): type mismatch"};
}

// @brief 定数の種類の取得
VpiConstType
CptExpr::const_type() const
{
  throw std::logic_error{"const_type(): type mismatch"};
}

// @brief 整数型の定数のサイズの取得
SizeType
CptExpr::const_size() const
{
  throw std::logic_error{"const_size(): type mismatch"};
}

// @brief 整数型の値の取得
BitVector
CptExpr::const_bitvect() const
{
  throw std::logic_error{"const_bitvect(): Not a INT CONST type"};
}

// @brief 整数型および文字列型の定数の文字列表現の取得
const char*
CptExpr::const_str() const
{
  throw std::logic_error{"const_str(): Not a INT/STRING CONST type"};
}

// @brief 実数型の値の取得
double
CptExpr::const_real() const
{
  throw std::logic_error{"const_real(): Not a REAL CONST type"};
}

// @brief インデックスとして使える式のチェック
bool
CptExpr::is_index_expr() const
{
  return false;
}

// @brief インデックスの値の取得
int
CptExpr::index_value() const
{
  throw std::logic_error{"is_index_expr() = false"};
}

// @brief simple primary のチェック
bool
CptExpr::is_simple() const
{
  throw std::logic_error{"is_simple(): Not a PRIMARY type"};
}


//////////////////////////////////////////////////////////////////////
// クラス CptFuncCallBase
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptFuncCallBase::file_region() const
{
  return mFileRegion;
}

// 末尾の名前を返す．
const char*
CptFuncCallBase::name() const
{
  return mName;
}

// @brief 階層ブランチのリストの先頭を返す．
const PtNameBranch*
CptFuncCallBase::namebranch_top() const
{
  return nullptr;
}

// @brief 引数リストの先頭の取得
const PtExpr*
CptFuncCallBase::arg_top() const
{
  return mArgTop;
}


//////////////////////////////////////////////////////////////////////
// クラス CptFuncCall
//////////////////////////////////////////////////////////////////////

// クラスの型を返す．
AstExpr::Type
CptFuncCall::type() const
{
  return AstExpr::FuncCall;
}


//////////////////////////////////////////////////////////////////////
// クラス CptFuncCallH
//////////////////////////////////////////////////////////////////////

// @brief 階層ブランチのリストの先頭を返す．
const PtNameBranch*
CptFuncCallH::namebranch_top() const
{
  return mNbTop;
}


//////////////////////////////////////////////////////////////////////
// クラス CptSysFuncCall
//////////////////////////////////////////////////////////////////////

// クラスの型を返す．
AstExpr::Type
CptSysFuncCall::type() const
{
  return AstExpr::SysFuncCall;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// function call を生成する．
PtExpr*
PtFactory::new_FuncCall(
  const FileRegion& file_region,
  const char* name,
  const PtExpr* arg_top
)
{
  auto p = mAlloc.get_memory(sizeof(CptFuncCall));
  return new (p) CptFuncCall(file_region, name, arg_top);
}

// function call を生成する．
PtExpr*
PtFactory::new_FuncCall(
  const FileRegion& file_region,
  const PtHierName& hname,
  const PtExpr* arg_top
)
{
  auto p = mAlloc.get_memory(sizeof(CptFuncCallH));
  return new (p) CptFuncCallH(file_region, hname, arg_top);
}

// system function call を生成する．
PtExpr*
PtFactory::new_SysFuncCall(
  const FileRegion& file_region,
  const char* name
)
{
  auto p = mAlloc.get_memory(sizeof(CptSysFuncCall));
  return new CptSysFuncCall(file_region, name, nullptr);
}

// system function call を生成する．
PtExpr*
PtFactory::new_SysFuncCall(
  const FileRegion& file_region,
  const char* name,
  const PtExpr* arg_top
)
{
  auto p = mAlloc.get_memory(sizeof(CptSysFuncCall));
  return new CptSysFuncCall(file_region, name, arg_top);
}

END_NAMESPACE_YM_VERILOG
