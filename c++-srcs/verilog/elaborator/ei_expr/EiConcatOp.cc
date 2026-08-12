
/// @file EiConcatOp.cc
/// @brief EiConcatOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiConcatOp.h"
#include "ym/vl/AstExpr.h"
#include "ym/vl/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 連結演算子を生成する．
ElbExpr*
EiFactory::new_ConcatOp(
  const AstExpr* ast_expr,
  const std::vector<ElbExpr*>& opr_list
)
{
  return new EiConcatOp(ast_expr, opr_list);
}

// @brief 反復連結演算子を生成する．
ElbExpr*
EiFactory::new_MultiConcatOp(
  const AstExpr* ast_expr,
  SizeType rep_num,
  ElbExpr* rep_expr,
  const std::vector<ElbExpr*>& opr_list
)
{
  return new EiMultiConcatOp(ast_expr, rep_num, rep_expr, opr_list);
}


//////////////////////////////////////////////////////////////////////
// クラス EiConcatOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiConcatOp::EiConcatOp(
  const AstExpr* ast_expr,
  const std::vector<ElbExpr*>& opr_list
) : EiOperation(ast_expr),
    mOprList{opr_list}
{
  mSize = 0;
  for ( auto opr: mOprList ) {
    auto type1 = opr->value_type();
    if ( type1.is_real_type() ) {
      throw std::logic_error{"type1.is_real_type()"};
    }

    SizeType size1 = type1.size();
    mSize += size1;
    // オペランドのサイズは self determined
    opr->set_selfsize();
  }
  // 自分自身のサイズも self determined
  set_selfsize();
}

// @brief デストラクタ
EiConcatOp::~EiConcatOp()
{
}

// @brief 式のタイプを返す．
VlValueType
EiConcatOp::value_type() const
{
  return VlValueType(false, true, mSize);
}

// @brief 定数の時 true を返す．
bool
EiConcatOp::is_const() const
{
  for ( const auto& opr: mOprList ) {
    if ( !opr->is_const() ) {
      return false;
    }
  }
  return true;
}

// @brief 要求される式の型を計算してセットする．
void
EiConcatOp::_set_reqsize(
  const VlValueType& type
)
{
  // なにもしない．
}

// @brief オペランド数を返す．
SizeType
EiConcatOp::operand_num() const
{
  return mOprList.size();
}

// @brief オペランドを返す．
const VlExpr*
EiConcatOp::operand(
  SizeType pos
) const
{
  if ( pos >= operand_num() ) {
    throw std::out_of_range{"pos is out of range"};
  }
  return mOprList[pos];
}

// @brief オペランドのリストを返す．
std::vector<const VlExpr*>
EiConcatOp::operand_list() const
{
  return std::vector<const VlExpr*>{mOprList.begin(), mOprList.end()};
}

// @brief ビット幅を返す．
SizeType
EiConcatOp::bit_size() const
{
  return mSize;
}


//////////////////////////////////////////////////////////////////////
// クラス EiMultiConcatOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiMultiConcatOp::EiMultiConcatOp(
  const AstExpr* ast_expr,
  SizeType rep_num,
  ElbExpr* rep_expr,
  const std::vector<ElbExpr*>& opr_list
) : EiConcatOp(ast_expr, opr_list),
    mRepNum{rep_num},
    mRepExpr{rep_expr}
{
}

// @brief デストラクタ
EiMultiConcatOp::~EiMultiConcatOp()
{
}

// @brief 式のタイプを返す．
VlValueType
EiMultiConcatOp::value_type() const
{
  return VlValueType(false, true, bit_size() * mRepNum);
}

// @brief オペランド数を返す．
SizeType
EiMultiConcatOp::operand_num() const
{
  return EiConcatOp::operand_num() + 1;
}
// @brief オペランドを返す．
const VlExpr*
EiMultiConcatOp::operand(
  SizeType pos
) const
{
  if ( pos == 0 ) {
    return mRepExpr;
  }
  return EiConcatOp::operand(pos - 1);
}

// @brief オペランドのリストを返す．
std::vector<const VlExpr*>
EiMultiConcatOp::operand_list() const
{
  std::vector<const VlExpr*> ans_list;
  auto n = EiConcatOp::operand_num();
  ans_list.reserve(n);
  ans_list.push_back(mRepExpr);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto expr = EiConcatOp::operand(i);
    ans_list.push_back(expr);
  }
  return ans_list;
}

// @brief 繰り返し数を返す．
SizeType
EiMultiConcatOp::rep_num() const
{
  return mRepNum;
}

END_NAMESPACE_YM_VERILOG
