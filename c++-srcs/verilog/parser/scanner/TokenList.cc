
/// @file TokenList.cc
/// @brief TokenList の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "TokenList.h"
#include "TokenInfo.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// トークンのリストを表すクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TokenList::TokenList() :
  mTop(nullptr),
  mEnd(nullptr)
{
}

// @brief コピーコンストラクタ
TokenList::TokenList(
  const TokenList& src
) : mTop(nullptr),
    mEnd(nullptr)
{
}


// @brief デストラクタ
TokenList::~TokenList()
{
  for ( TokenInfo* token = mTop; token; ) {
    TokenInfo* next = token->mLink;
    delete token;
    token = next;
  }
}

// @brief 要素を追加する (位置パラメータ)
void
TokenList::add(
  int pos
)
{
  add(new TokenInfo(pos));
}

// @brief 要素を追加する (トークン)
void
TokenList::add(
  const FileRegion& loc,
  int id,
  const std::string& str
)
{
  add(new TokenInfo(loc, id, str));
}

// @brief add() の下請
void
TokenList::add(
  TokenInfo* token
)
{
  if ( mEnd ) {
    mEnd->mLink = token;
  }
  else {
    mTop = token;
  }
  mEnd = token;
}

// @brief 先頭のトークンを取り出す．
const TokenInfo*
TokenList::top() const
{
  return mTop;
}

END_NAMESPACE_YM_VERILOG
