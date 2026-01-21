
/// @file ItvlMgr.cc
/// @brief ItvlMgrの実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2010, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ItvlMgr.h"
#include "ItvlTree.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス ItvlMgr
//////////////////////////////////////////////////////////////////////

// コンストラクタ
ItvlMgr::ItvlMgr(
) : mImpl{new ItvlTree}
{
}

// @brief デストラクタ
ItvlMgr::~ItvlMgr()
{
  // ItvlTree をここのローカルにしておくために
  // この関数は inline 化できない．
}

// 内容をクリアして全区間を使用可能とする．
void
ItvlMgr::clear()
{
  mImpl->clear();
}

// 使用可能な数字を得る．
SizeType
ItvlMgr::avail_num() const
{
  return mImpl->get_min();
}

// d を使用可能な区間から削除する．
void
ItvlMgr::erase(
  SizeType d
)
{
  mImpl->add(d);
}

// d を使用可能区間に追加する．
void
ItvlMgr::add(
  SizeType d
)
{
  mImpl->del(d);
}

END_NAMESPACE_YM
