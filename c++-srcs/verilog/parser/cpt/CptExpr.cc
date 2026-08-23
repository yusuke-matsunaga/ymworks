
/// @file CptExpr.cc
/// @brief CptExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptExpr.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"
#include "ym/vl/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// expression を表す基底クラス
//////////////////////////////////////////////////////////////////////

// @brief 演算子の種類の取得
VpiOpType
CptExpr::op_type() const
{
  throw std::logic_error{"Not an Operation Type"};
}

// @brief 階層ブランチの要素数の取得
SizeType
CptExpr::namebranch_num() const
{
  throw std::logic_error{"namebranch_num(): type mismatch"};
}

// @brief 先頭の階層ブランチを返す．
const AstNameBranch*
CptExpr::namebranch_top() const
{
  throw std::logic_error{"namebranch(): type mismatch"};
}

// @brief 階層ブランチのリストを返す．
AstNameBranchVec
CptExpr::namebranch_list() const
{
  throw std::logic_error{"namebranch_list(): type mismatch"};
}

// @brief 末尾の名前の取得
const char*
CptExpr::name() const
{
  return nullptr;
}

// @brief 0番目のオペランドの取得
const AstExpr*
CptExpr::operand0() const
{
  throw std::logic_error{"Does not have operand0"};
}

// @brief 1番目のオペランドの取得
const AstExpr*
CptExpr::operand1() const
{
  throw std::logic_error{"Does not have operand1"};
}

// @brief 2番目のオペランドの取得
const AstExpr*
CptExpr::operand2() const
{
  throw std::logic_error{"Does not have operand2"};
}

// @brief オペランドの数の取得
SizeType
CptExpr::operand_num() const
{
  throw std::logic_error{"operand_num(): Does not have operand_list()"};
}

// @brief オペランドのリストの取得
const AstExpr*
CptExpr::operand(
  SizeType index
) const
{
  throw std::logic_error{"operand(): Does not have operand_list()"};
}

// @brief オペランドのリストの取得
AstExprVec
CptExpr::operand_list() const
{
  throw std::out_of_range{"operand_list(): Does not have operand_list"};
}

// @brief multi-concat の繰り返し数
const AstExpr*
CptExpr::rep() const
{
  throw std::logic_error{"rep(): Not a multi-concat oprator"};
}

// @brief 定数インデックスのチェック
bool
CptExpr::is_const_index() const
{
  throw std::logic_error{"is_const_index(): Not a Primary type"};
}

// @brief インデックスリストのサイズの取得
SizeType
CptExpr::index_num() const
{
  throw std::logic_error{"index_num(): Not a Primary type"};
}

// @brief インデックスの取得
const AstExpr*
CptExpr::index(
  SizeType i
) const
{
  throw std::logic_error{"index(): Not a Primary type"};
}

// @brief インデックスリストの取得
AstExprVec
CptExpr::index_list() const
{
  throw std::logic_error{"index_list(); Not a Primary type"};
}

// @brief 範囲指定の取得
const AstPart*
CptExpr::part() const
{
  throw std::logic_error{"part(): Not a Primary type"};
}

// @brief 定数の種類の取得
VpiConstType
CptExpr::const_type() const
{
  throw std::logic_error{"const_type(): Not a CONST type"};
}

// @brief 整数型の定数のサイズの取得
SizeType
CptExpr::const_size() const
{
  throw std::logic_error{"const_size(): Not a CONST type"};
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
// min/typ/max delay のベース実装クラス
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptMinTypMax::file_region() const
{
  return FileRegion(mValue[0]->file_region(), mValue[2]->file_region());
}

// クラスの型を返す．
AstExpr::Type
CptMinTypMax::type() const
{
  return Opr;
}

// 演算子の種類の取得
VpiOpType
CptMinTypMax::op_type() const
{
  return VpiOpType::MinTypMax;
}

// @brief 0番目のオペランドの取得
const AstExpr*
CptMinTypMax::operand0() const
{
  return mValue[0];
}

// @brief 1番目のオペランドの取得
const AstExpr*
CptMinTypMax::operand1() const
{
  return mValue[1];
}

// @brief 2番目のオペランドの取得
const AstExpr*
CptMinTypMax::operand2() const
{
  return mValue[2];
}


//////////////////////////////////////////////////////////////////////
// function call / system function call に共通の基底クラス
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

// @brief オペランドの数の取得
SizeType
CptFuncCallBase::operand_num() const
{
  return mArgList->size();
}

// @brief オペランドのリストの取得
const AstExpr*
CptFuncCallBase::operand(
  SizeType index
) const
{
  return mArgList->expr(index);
}

// @brief オペランドのリストの取得
AstExprVec
CptFuncCallBase::operand_list() const
{
  return mArgList->to_vector();
}


//////////////////////////////////////////////////////////////////////
// 階層なし名前を持つ function callを表すクラス
//////////////////////////////////////////////////////////////////////

// クラスの型を返す．
AstExpr::Type
CptFuncCall::type() const
{
  return FuncCall;
}


//////////////////////////////////////////////////////////////////////
// 階層つき名前を持つ function call を表すクラス
//////////////////////////////////////////////////////////////////////

// @brief 階層ブランチの要素数の取得
SizeType
CptFuncCallH::namebranch_num() const
{
  return mNbTop->count_num();
}

// @brief 先頭の階層ブランチを返す．
const AstNameBranch*
CptFuncCallH::namebranch_top() const
{
  return mNbTop;
}

// @brief 階層ブランチのリストを返す．
AstNameBranchVec
CptFuncCallH::namebranch_list() const
{
  return mNbTop->to_vector();
}


//////////////////////////////////////////////////////////////////////
// system function callを表すクラス
//////////////////////////////////////////////////////////////////////

// クラスの型を返す．
AstExpr::Type
CptSysFuncCall::type() const
{
  return SysFuncCall;
}


//////////////////////////////////////////////////////////////////////
// クラス CptExprList
//////////////////////////////////////////////////////////////////////

// @brief 要素数を返す．
SizeType
CptExprList::size() const
{
  return mExprList.size();
}

// @brief 要素を取り出す．
const AstExpr*
CptExprList::expr(
  SizeType index
) const
{
  return mExprList[index];
}

// @brief ベクタに変換する．
AstExprVec
CptExprList::to_vector() const
{
  return mExprList.to_vector();
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// min-typ-max を生成する．
PtExpr*
PtFactory::new_MinTypMax(
  const FileRegion& file_region,
  const AstExpr* val0,
  const AstExpr* val1,
  const AstExpr* val2
)
{
  // 実は file_region は不要
  auto p = mAlloc.get_memory(sizeof(CptMinTypMax));
  return new (p) CptMinTypMax(val0, val1, val2);
}

// function call を生成する．
PtExpr*
PtFactory::new_FuncCall(
  const FileRegion& file_region,
  const char* name,
  const AstExprList* arg_list
)
{
  auto p = mAlloc.get_memory(sizeof(CptFuncCall));
  return new (p) CptFuncCall(file_region, name, arg_list);
}

// function call を生成する．
PtExpr*
PtFactory::new_FuncCall(
  const FileRegion& file_region,
  const PtHierName& hname,
  const AstExprList* arg_list
)
{
  auto nb_top = hname.nb_top->reverse();
  auto tail_name = hname.tail_name;
  auto p = mAlloc.get_memory(sizeof(CptFuncCallH));
  return new (p) CptFuncCallH(file_region,
			      nb_top, tail_name,
			      arg_list);
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
  const AstExprList* arg_list
)
{
  auto p = mAlloc.get_memory(sizeof(CptSysFuncCall));
  return new CptSysFuncCall(file_region, name, arg_list);
}

// @brief ExprList の生成
PtExprList*
PtFactory::new_ExprList(
  const AstExprVec& expr_list
)
{
  auto p = mAlloc.get_memory(sizeof(CptExprList));
  return new (p) CptExprList(PtExprArray(mAlloc, expr_list));
}

END_NAMESPACE_YM_VERILOG
