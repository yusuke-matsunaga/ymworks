
/// @file MacroSource.cc
/// @brief MacroSource の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "MacroSource.h"
#include "TokenList.h"
#include "TokenInfo.h"
#include "print_token.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// define macro の入力切り替えのためのクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MacroSource::MacroSource() :
  mParamArray(nullptr)
{
}

// @brief デストラクタ
MacroSource::~MacroSource()
{
  delete [] mParamArray;
}

// @brief 内容をセットする．
void
MacroSource::set(
  const std::string& name,
  const TokenInfo* top,
  TokenList* param_array
)
{
  mName = name;
  mNextToken1 = top;
  mNextToken2 = nullptr;
  mParamArray = param_array;
}

// @brief 内容を取り出す
const TokenInfo*
MacroSource::get()
{
  const TokenInfo* ans = mNextToken2;
  if ( ans ) {
    mNextToken2 = ans->next();
    return ans;
  }
  ans = mNextToken1;
  if ( ans ) {
    mNextToken1 = ans->next();
    int pos = ans->pos();
    if ( pos >= 0 ) {
      // 置き換えパラメータだった
      ans = mParamArray[pos].top();
      mNextToken2 = ans->next();
    }
  }
  return ans;
}

END_NAMESPACE_YM_VERILOG
