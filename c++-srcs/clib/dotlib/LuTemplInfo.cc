
/// @file LuTemplInfo.cc
/// @brief LuTemplInfo の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/LuTemplInfo.h"
#include "dotlib/AstValue.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス LuTemplInfo
//////////////////////////////////////////////////////////////////////

// @brief 内容を設定する．
void
LuTemplInfo::set(
  const AstValue* lut_val
)
{
  GroupInfo::set(lut_val);

  // 名前を得る．
  auto& header = lut_val->group_header_value();
  SizeType n = header.complex_elem_size();
  ASSERT_COND( n == 1 );
  mName = header.complex_elem_value(0).string_value();

  get_variable_type("variable_1", mVar1);
  get_variable_type("variable_2", mVar2);
  get_variable_type("variable_3", mVar3);
  get_complex_float_vector("index_1", mIndex1);
  get_complex_float_vector("index_2", mIndex2);
  get_complex_float_vector("index_3", mIndex3);

  if ( mVar1 == ClibVarType::none ) {
    // variable_1 が未定義
    auto label = "variable_1 is missing.";
    parse_error(label);
  }
  if ( mIndex1.size() == 0 ) {
    // index_1 が未定義
    auto label = "index_1 is missing.";
    parse_error(label);
  }

  mDimension = 1;
  if ( mVar2 == ClibVarType::none ) {
    if ( mIndex2.size() > 0 ) {
      // variable_2 が定義されていないのに index_2 が定義されている．
      auto label = "variable_2 is missing.";
      parse_error(label);
    }
    return;
  }

  if ( mIndex2.size() == 0 ) {
    // variable_2 が定義されているのに index_2 が未定義
    auto label = "index_2 is missing.";
    parse_error(label);
  }

  mDimension = 2;
  if ( mVar3 == ClibVarType::none ) {
    if ( mIndex3.size() > 0 ) {
      // variable_3 が定義されていないのに index_3 が定義されている．
      auto label = "variable_3 is missing.";
      parse_error(label);
    }
    return;
  }

  if ( mIndex3.size() == 0 ) {
    // variable_3 が定義されているのに index_3 が未定義
    auto label = "index_3 is missing.";
    parse_error(label);
  }

  mDimension = 3;
}

// @brief テンプレートを作る．
const CiLutTemplate*
LuTemplInfo::add_lu_template()
{
  const CiLutTemplate* templ{nullptr};
  switch ( mDimension ) {
  case 1:
    templ = library()->add_lut_template1(mVar1, mIndex1);
    break;

  case 2:
    templ = library()->add_lut_template2(mVar1, mIndex1,
					 mVar2, mIndex2);
    break;

  case 3:
    templ = library()->add_lut_template3(mVar1, mIndex1,
					 mVar2, mIndex2,
					 mVar3, mIndex3);
    break;

  default:
    ASSERT_NOT_REACHED;
    break;
  }
  return templ;
}

END_NAMESPACE_YM_DOTLIB
