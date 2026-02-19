
/// @file Selector.cc
/// @brief Selector の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "mincov/Selector.h"
#include "SelSimple.h"
#include "SelNaive.h"
#include "SelCS.h"
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM_MINCOV

// @brief インスタンスを生成するクラスメソッド
std::unique_ptr<Selector>
Selector::new_obj(
  const JsonValue& option
)
{
  auto type_str = get_string(option, "type", "simple");
  if ( type_str == "simple" ) {
    return std::unique_ptr<Selector>{new SelSimple};
  }
  if ( type_str == "naive" ) {
    return std::unique_ptr<Selector>{new SelNaive};
  }
  if ( type_str == "cs" ) {
    return std::unique_ptr<Selector>{new SelCS};
  }
  std::ostringstream buf;
  buf << type_str << ": unknown value for selector.type";
  throw std::invalid_argument{buf.str()};
  return nullptr;
}

END_NAMESPACE_YM_MINCOV
