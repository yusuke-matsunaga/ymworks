
/// @file CptNameBranch.cc
/// @brief CptNameBranch の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptNameBranch.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptNameBranch
//////////////////////////////////////////////////////////////////////

// 名前を取り出す．
const char*
CptNameBranch::name() const
{
  return mName;
}

// @brief インデックスの有無のチェック
bool
CptNameBranch::has_index() const
{
  return false;
}

// インデックスの取得
int
CptNameBranch::index() const
{
  throw std::logic_error{"has_index() == false"};
}

// @brief 次の要素の取得
const AstNameBranch*
CptNameBranch::link() const
{
  return mLink;
}

// @brief link を設定する．
void
CptNameBranch::set_link(
  PtNameBranch* link
)
{
  mLink = link;
}

// @brief PtNameBranch* 型のリンクを返す．
PtNameBranch*
CptNameBranch::_link() const
{
  return mLink;
}

// @brief リンクトリストを逆順にする．
PtNameBranch*
CptNameBranch::reverse()
{
  return make_reverse<PtNameBranch>(this);
}


//////////////////////////////////////////////////////////////////////
// クラス CptNameBranchI
//////////////////////////////////////////////////////////////////////

// インデックスを持っている時 true を返す．
bool
CptNameBranchI::has_index() const
{
  return true;
}

// インデックスを取り出す．
int
CptNameBranchI::index() const
{
  return mIndex;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// 階層名を生成する．
PtNameBranch*
PtFactory::new_NameBranch(
  const char* name
)
{
  void* p = mAlloc.get_memory(sizeof(CptNameBranch));
  return new (p) CptNameBranch(name);
}

// 階層名を生成する．
PtNameBranch*
PtFactory::new_NameBranch(
  const char* name,
  int index
)
{
  void* p = mAlloc.get_memory(sizeof(CptNameBranchI));
  return new (p) CptNameBranchI(name, index);
}

END_NAMESPACE_YM_VERILOG
