#ifndef GET_OPT_H
#define GET_OPT_H

/// @file get_opt.h
/// @brief get_bool, get_string のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ExSyn.h"
#include "ym/SatInitParam.h"


BEGIN_NAMESPACE_YM_EXSYN

BEGIN_NONAMESPACE

inline
SatInitParam
sat_param(
  const JsonValue& option
)
{
  if ( option.is_object() && option.has_key("sat") ) {
    auto val = option.at("sat");
    return SatInitParam(val);
  }
  return SatInitParam();
}

END_NONAMESPACE

END_NAMESPACE_YM_EXSYN

#endif // GET_OPT_H
