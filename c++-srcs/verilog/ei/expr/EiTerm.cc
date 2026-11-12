
/// @file EiTerm.cc
/// @brief EiTerm の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbMgr.h"
#include "EiTerm1.h"
#include "EiTerm2.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbMgr
//////////////////////////////////////////////////////////////////////

// @brief 端子を生成する．
const VlTerm*
ElbMgr::new_Term(
  ElbExpr* expr
)
{
  auto term = new EiTerm1(expr);
  reg_obj(term);
  return term;
}

// @brief 端子を生成する．
const VlTerm*
ElbMgr::new_Term(
  const std::vector<ElbPrimary>& primary_list
)
{
  auto n = primary_list.size();
  if ( n == 0 ) {
    throw std::logic_error{"ElbMgr::new_Term(primary_list): primary_list is empty()"};
  }

  const VlTerm* term = nullptr;
  if ( n == 1 ) {
    auto& primary = primary_list.front();
    auto expr = new_Primary(primary);
    term = new EiTerm1(expr);
  }
  else {
    std::vector<const VlExpr*> expr_list;
    expr_list.reserve(n);
    for ( auto& primary: primary_list ) {
      auto expr = new_Primary(primary);
      expr_list.push_back(expr);
    }
    term = new EiTerm2(expr_list);
  }
  reg_obj(term);
  return term;
}

// @brief 端子を生成する．
const VlTerm*
ElbMgr::new_Term(
  const std::vector<ElbPrimary>& primary_list,
  SizeType base,
  SizeType width
)
{
  SizeType n = primary_list.size();
  SizeType cur = 0;
  SizeType index1 = 0;
  SizeType index2 = 0;
  SizeType offset1 = 0;
  SizeType offset2 = 0;
  for ( SizeType i = 0; i < n; ++ i ) {
    auto& primary = primary_list[i];
    auto next = cur + primary.bit_width();
    if ( base < next ) {
      index1 = i;
      offset1 = base - cur;
    }
    if ( base + width <= next ) {
      index2 = i;
      offset2 = base + width - cur;
    }
    cur = next;
  }

  if ( index1 == index2 ) {
    auto& primary = primary_list[index1];
    auto primary1 = primary.slice(offset1, width);
    auto expr = new_Primary(primary1);
    return new_Term(expr);
  }
  else {
    std::vector<ElbPrimary> tmp_list;
    tmp_list.reserve(index2 - index1 + 1);
    auto& primary1 = primary_list[index1];
    auto new_primary1 = primary1.slice(offset1, primary1.bit_width() - offset1);
    tmp_list.push_back(new_primary1);
    for ( SizeType i = index1 + 1; i < index2; ++ i ) {
      auto& primary1 = primary_list[i];
      tmp_list.push_back(primary1);
    }
    auto& primary2 = primary_list[index2];
    auto new_primary2 = primary2.slice(0, offset2);
    tmp_list.push_back(new_primary2);
    return new_Term(tmp_list);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス EiTerm1
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiTerm1::type() const
{
  return mPrimary->type();
}

// @brief ファイル位置の取得
FileRegion
EiTerm1::file_region() const
{
  return mPrimary->file_region();
}

// @brief Verilog-HDL の文字列を得る．
std::string
EiTerm1::decompile() const
{
  return mPrimary->decompile();
}

// @brief 式の値のタイプを返す．
VlValueType
EiTerm1::value_type() const
{
  return mPrimary->value_type();
}

// @brief 式のビット幅を返す．
SizeType
EiTerm1::bit_size() const
{
  return mPrimary->bit_size();
}

// @brief 要素数を返す．
SizeType
EiTerm1::elem_num() const
{
  return 1;
}

// @brief 要素を返す．
const VlExpr*
EiTerm1::elem(
  SizeType index
) const
{
  if ( index != 0 ) {
    throw std::out_of_range{"EiTerm1::elem(index): index is out of range"};
  }
  return mPrimary;
}


//////////////////////////////////////////////////////////////////////
// クラス EiTerm2
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiTerm2::type() const
{
  return VpiObjType::Operation;
}

// @brief ファイル位置の取得
FileRegion
EiTerm2::file_region() const
{
  auto expr1 = mPrimaryList.front();
  auto expr2 = mPrimaryList.back();
  return FileRegion(expr1->file_region(), expr2->file_region());
}

// @brief Verilog-HDL の文字列を得る．
std::string
EiTerm2::decompile() const
{
  std::ostringstream buf;
  buf << "{";
  const char* comma = "";
  for ( auto expr: mPrimaryList ) {
    buf << comma << expr->decompile();
    comma = ", ";
  }
  buf << "}";
  return buf.str();
}

// @brief 式の値のタイプを返す．
VlValueType
EiTerm2::value_type() const
{
  return VlValueType(false, true, bit_size());
}

// @brief 式のビット幅を返す．
SizeType
EiTerm2::bit_size() const
{
  SizeType size = 0;
  for ( auto expr: mPrimaryList ) {
    size += expr->bit_size();
  }
  return size;
}

// @brief 要素数を返す．
SizeType
EiTerm2::elem_num() const
{
  return mPrimaryList.size();
}

// @brief 要素を返す．
const VlExpr*
EiTerm2::elem(
  SizeType index
) const
{
  if ( index >= elem_num() ) {
    throw std::out_of_range{"EiTerm2::elem(index): index is out of range"};
  }
  return mPrimaryList[index];
}

END_NAMESPACE_YM_VERILOG
