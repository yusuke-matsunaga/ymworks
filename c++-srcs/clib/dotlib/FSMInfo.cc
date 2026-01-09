
/// @file FSMInfo.cc
/// @brief FSMInfo の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/FSMInfo.h"
#include "dotlib/AstValue.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス FSMInfo
//////////////////////////////////////////////////////////////////////

// @brief 情報をセットする．
void
FSMInfo::set(
  const AstValue* ast_val
)
{
  set_common(ast_val);
}

END_NAMESPACE_YM_DOTLIB
