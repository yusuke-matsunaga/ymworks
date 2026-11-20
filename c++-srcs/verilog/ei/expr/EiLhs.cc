
/// @file EiLhs.cc
/// @brief EiLhs の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbMgr.h"
#include "EiLhs.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbMgr
//////////////////////////////////////////////////////////////////////

// @brief 左辺式を生成する．
const VlExpr*
ElbMgr::new_Lhs(
  const std::vector<const VlExpr*>& elem_list
)
{
  auto n = elem_list.size();
  if ( n == 0 ) {
    throw std::logic_error{"ElbMgr::new_Lhs(elem_list): elem_list is empty()"};
  }

  if ( n == 1 ) {
    auto expr = elem_list.front();
    return expr;
  }
  auto expr = new EiLhs(elem_list);
  reg_obj(expr);
  return expr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiLhs
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiLhs::type() const
{
  return VpiObjType::Operation;
}

// @brief ファイル位置を返す．
FileRegion
EiLhs::file_region() const
{
  auto expr1 = mElemList.front();
  auto expr2 = mElemList.back();
  auto loc = FileRegion(expr1->file_region(), expr2->file_region());
  return loc;
}

// @brief 式の値のタイプを返す．
VlValueType
EiLhs::value_type() const
{
  return VlValueType(false, true, bit_size());
}

// @brief Verilog-HDL の文字列を得る．
std::string
EiLhs::decompile() const
{
  std::ostringstream buf;
  buf << "{";
  const char* comma = "";
  for ( auto expr: mElemList ) {
    buf << comma << expr->decompile();
    comma = ", ";
  }
  buf << "}";
  return buf.str();
}

// @brief 式のビット幅を返す．
SizeType
EiLhs::bit_size() const
{
  SizeType size = 0;
  for ( auto expr: mElemList ) {
    size += expr->bit_size();
  }
  return size;
}

// @brief 式のタイプを返す．
VlExprType
EiLhs::expr_type() const
{
  return VlExprType::LHS;
}

// @brief 左辺式の時に true を返す．
bool
EiLhs::is_lhs() const
{
  return true;
}

// @brief 左辺式の要素数を返す．
SizeType
EiLhs::lhs_elem_num() const
{
  return mElemList.size();
}

// @brief 要素を返す．
const VlExpr*
EiLhs::lhs_elem(
  SizeType index
) const
{
  if ( index >= lhs_elem_num() ) {
    throw std::out_of_range{"EiLhs::lhs_elem(index): index is out of range"};
  }
  return mElemList[index];
}

// @brief 要素のリストを返す．
std::vector<const VlExpr*>
EiLhs::lhs_elem_list() const
{
  return mElemList;
}

// @brief set_reqsize() の下請け関数
void
EiLhs::_set_reqsize(
  const VlValueType& type
)
{
  // なにもしない．
}

END_NAMESPACE_YM_VERILOG
