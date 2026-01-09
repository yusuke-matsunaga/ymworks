
/// @file LbCalc.cc
/// @brief LbCalc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "mincov/LbCalc.h"
#include "ym/JsonValue.h"
#include "LbCS.h"
#include "LbMIS1.h"
#include "LbMIS2.h"
#include "LbMIS3.h"
#include "LbMAX.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス LbCalc
//////////////////////////////////////////////////////////////////////

// @brief インスタンスを生成するクラスメソッド
std::unique_ptr<LbCalc>
LbCalc::new_obj(
  const JsonValue& opt_obj
)
{
  auto type_str = std::string{"CS"};
  if ( opt_obj.has_key("type") ) {
    auto type_obj = opt_obj.at("type");
    type_str = opt_obj.get_string();
  }
  if ( type_str == "CS" ) {
    return std::unique_ptr<LbCalc>{new LbCS};
  }
  if ( type_str == "MIS1" ) {
    return std::unique_ptr<LbCalc>{new LbMIS1};
  }
  if ( type_str == "MIS2" ) {
    return std::unique_ptr<LbCalc>{new LbMIS2};
  }
  if ( type_str == "MIS3" ) {
    return std::unique_ptr<LbCalc>{new LbMIS3};
  }
  if ( type_str == "MAX" ) {
    if ( !opt_obj.has_key("child_list") ) {
      throw std::invalid_argument{"'MAX' type requires 'child_list'"};
    }
    auto childs_obj = opt_obj.at("child_list");
    if ( !childs_obj.is_array() ) {
      throw std::invalid_argument{"'child_list' should be an array"};
    }
    auto n = childs_obj.size();
    auto child_list = std::vector<std::unique_ptr<LbCalc>>(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      child_list[i] = new_obj(childs_obj[i]);
    }
    return std::unique_ptr<LbCalc>{new LbMAX{std::move(child_list)}};
  }
  {
    std::ostringstream buf;
    buf << type_str << ": unknown type name for LbCalc";
    throw std::invalid_argument{buf.str()};
  }
  return nullptr;
}


END_NAMESPACE_YM_MINCOV
