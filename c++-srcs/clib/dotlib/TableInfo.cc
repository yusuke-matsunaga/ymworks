
/// @file TableInfo.cc
/// @brief TableInfo の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/TableInfo.h"
#include "dotlib/LibraryInfo.h"
#include "dotlib/AstValue.h"
#include "ci/CiLut.h"
#include "ci/CiLutTemplate.h"
#include "ci/CiStLut.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス TableInfo
//////////////////////////////////////////////////////////////////////

// @brief 内容をセットする．
void
TableInfo::set(
  const AstValue* val
)
{
  GroupInfo::set(val);

  // テンプレート名
  auto& header = val->group_header_value();
  SizeType n = header.complex_elem_size();
  ASSERT_COND( n == 1 );
  auto name = header.complex_elem_value(0).string_value();
  mTemplate = library_info().find_lut(name);
  if ( mTemplate == nullptr ) {
    std::ostringstream buf;
    buf << name << ": No such lu_template";
    auto label = buf.str();
    parse_error(label);
  }

  get_complex_float_vector("index_1", mIndex1);

  get_complex_float_vector("index_2", mIndex2);

  get_complex_float_vector("index_3", mIndex3);

  get_complex_float_vector("values", mValues);

  mDomain = get_value("domain");
}

// @brief 標準タイプの時 true を返す．
bool
TableInfo::is_standard() const
{
  return mTemplate->is_standard_type();
}

std::unique_ptr<CiLut>
TableInfo::gen_lut() const
{
  if ( mValues.size() > 0 ) {
    return CiLut::new_lut(mTemplate, mValues, mIndex1, mIndex2, mIndex3);
  }
  return {};
}

std::unique_ptr<CiStLut>
TableInfo::gen_stlut() const
{
  if ( mValues.size() > 0 ) {
    return CiLut::new_stlut(mTemplate, mValues, mIndex1, mIndex2);
  }
  return {};
}

END_NAMESPACE_YM_DOTLIB
