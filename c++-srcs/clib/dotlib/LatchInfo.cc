
/// @file LatchInfo.cc
/// @brief LatchInfo の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/LatchInfo.h"


BEGIN_NAMESPACE_YM_DOTLIB

// @brief 情報をセットする．
void
LatchInfo::set(
  const AstValue* ast_val
)
{
  set_common(ast_val);

  if ( !get_expr("enable_on", mEnableOn) ) {
    // ラッチの場合はエラーではない．
    mEnableOn = nullptr;
  }

  if ( !get_expr("enable_on_also", mEnableOnAlso) ) {
    // これはエラーではない．
    mEnableOnAlso = nullptr;
  }

  if ( !get_expr("data_in", mDataIn) ) {
    // これはエラーではない．
    mDataIn = nullptr;
  }
}

END_NAMESPACE_YM_DOTLIB
