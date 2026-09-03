
/// @file CptOpr.cc
/// @brief CptOpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr.h"
#include "CptOpr_Condition.h"
#include "CptOpr_MinTypMax.h"
#include "CptOpr_Concat.h"
#include "CptOpr_MultiConcat.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptOpr
//////////////////////////////////////////////////////////////////////

// クラスの型を返す．
AstExpr::Type
CptOpr::type() const
{
  return AstExpr::Opr;
}

// @brief 固定オペランド数の取得
SizeType
CptOpr::operand_num() const
{
  return 0;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr3
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptOpr3::file_region() const
{
  return FileRegion(mOpr[0]->file_region(), mOpr[2]->file_region());
}

// @brief 固定オペランド数の取得
SizeType
CptOpr3::operand_num() const
{
  return 3;
}

// @brief 0番目のオペランドの取得
const PtExpr*
CptOpr3::operand0() const
{
  return mOpr[0];
}

// @brief 1番目のオペランドの取得
const PtExpr*
CptOpr3::operand1() const
{
  return mOpr[1];
}

// @brief 2番目のオペランドの取得
const PtExpr*
CptOpr3::operand2() const
{
  return mOpr[2];
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_MinTypMax
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_MinTypMax::op_type() const
{
  return VpiOpType::MinTypMax;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_Condition
//////////////////////////////////////////////////////////////////////

// 演算子のトークン番号を得る．
VpiOpType
CptOpr_Condition::op_type() const
{
  return VpiOpType::Condition;
}

//////////////////////////////////////////////////////////////////////
// クラス CptOpr_Concat
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptOpr_Concat::file_region() const
{
  return mFileRegion;
}

///演算子の種類の取得
VpiOpType
CptOpr_Concat::op_type() const
{
  return VpiOpType::Concat;
}

// @brief オペランドのリストの先頭の取得
const PtExpr*
CptOpr_Concat::operand_top() const
{
  return mExprTop;
}


//////////////////////////////////////////////////////////////////////
// クラス CptOpr_MultiConcat
//////////////////////////////////////////////////////////////////////

// 演算子の種類の取得
VpiOpType
CptOpr_MultiConcat::op_type() const
{
  return VpiOpType::MultiConcat;
}

// @brief multi-concat の繰り返し数
const PtExpr*
CptOpr_MultiConcat::rep() const
{
  return mRep;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

template <typename T>
PtExpr*
new_opr(
  Alloc& alloc,
  const PtExpr* opr1,
  const PtExpr* opr2,
  const PtExpr* opr3
)
{
  auto p = alloc.get_memory(sizeof(T));
  return new (p) T(opr1, opr2, opr3);
}

PtExpr*
PtFactory::new_Condition(
  const PtExpr* opr1,
  const PtExpr* opr2,
  const PtExpr* opr3
)
{
  return new_opr<CptOpr_Condition>(mAlloc, opr1, opr2, opr3);
}

// min-typ-max を生成する．
PtExpr*
PtFactory::new_MinTypMax(
  const PtExpr* val0,
  const PtExpr* val1,
  const PtExpr* val2
)
{
  return new_opr<CptOpr_MinTypMax>(mAlloc, val0, val1, val2);
}

// concatination を生成する．
PtExpr*
PtFactory::new_Concat(
  const FileRegion& file_region,
  const PtExpr* expr_top
)
{
  auto p = mAlloc.get_memory(sizeof(CptOpr_Concat));
  return new (p) CptOpr_Concat(file_region, expr_top);
}

// multiple concatenation を生成する．
PtExpr*
PtFactory::new_MultiConcat(
  const FileRegion& file_region,
  const PtExpr* rep,
  const PtExpr* expr_top
)
{
  auto p = mAlloc.get_memory(sizeof(CptOpr_MultiConcat));
  return new (p) CptOpr_MultiConcat(file_region, rep, expr_top);
}

END_NAMESPACE_YM_VERILOG
