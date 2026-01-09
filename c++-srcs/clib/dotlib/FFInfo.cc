
/// @file FFInfo.cc
/// @brief FFInfo の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/FFInfo.h"


BEGIN_NAMESPACE_YM_DOTLIB

// @brief 情報をセットする．
void
FFInfo::set(
  const AstValue* ast_val
)
{
  // 属性の辞書を作る．
  set_common(ast_val);

  if ( !get_expr("clocked_on", mClockedOn) ) {
    // FF に clocked_on は必須
    auto label = "'clocked_on' is missing.";
    parse_error(label);
  }

  if ( !get_expr("clocked_on_also", mClockedOnAlso) ) {
    // これはエラーではない．
    mClockedOnAlso = nullptr;
  }

  if ( !get_expr("next_state", mNextState) ) {
    auto label = "'next_state' is missing.";
    parse_error(label);
  }
}

END_NAMESPACE_YM_DOTLIB
