
/// @file AstNameBranch.cc
/// @brief AstNameBranch の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstNameBranch.h"
#include "parser/PtNameBranch.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstNameBranch
//////////////////////////////////////////////////////////////////////

// @brief 名前の取得
const char*
AstNameBranch::name() const
{
  _check_ptr();
  return mPtr->name();
}

// @brief インデックスの有無のチェック
bool
AstNameBranch::has_index() const
{
  _check_ptr();
  return mPtr->has_index();
}

// @brief インデックスの取得
int
AstNameBranch::index() const
{
  _check_ptr();
  return mPtr->index();
}

// @brief インデックスを含めた名前を返す．
std::string
AstNameBranch::decompile() const
{
  std::ostringstream buf;
  buf << name();
  if ( has_index() ) {
    buf << "[" << index() << "]";
  }
  return buf.str();
}

// @brief 内容を JsonValue に変換する．
JsonValue
AstNameBranch::json_obj() const
{
  auto jobj = JsonValue::object();
  jobj.add("name", JsonValue(name()));
  if ( has_index() ) {
    jobj.add("index", JsonValue(index()));
  }
  return jobj;
}

// @brief 次の要素を返す．
AstNameBranch
AstNameBranch::next() const
{
  _check_ptr();
  return AstNameBranch(mPtr->link());
}

END_NAMESPACE_YM_VERILOG
