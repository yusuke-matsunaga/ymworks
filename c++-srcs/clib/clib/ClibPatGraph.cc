
/// @file ClibPatGraph.cc
/// @brief ClibPatGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ClibPatGraph.h"
#include "ym/ClibCellClass.h"
#include "ci/CiPatGraph.h"
#include "ci/CiCellLibrary.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
// クラス ClibPatGraph
//////////////////////////////////////////////////////////////////////

// @brief 代表クラスを返す．
ClibCellClass
ClibPatGraph::rep_class() const
{
  _check_valid();
  auto pg = mLibrary->_pat_graph(mId);
  return ClibCellClass{pg->rep_class()};
}

// @brief 根のノード番号を返す．
SizeType
ClibPatGraph::root_id() const
{
  _check_valid();
  auto pg = mLibrary->_pat_graph(mId);
  return pg->root_id();
}

// @brief 根の反転属性を返す．
bool
ClibPatGraph::root_inv() const
{
  _check_valid();
  auto pg = mLibrary->_pat_graph(mId);
  return pg->root_inv();
}

// @brief 入力数を返す．
SizeType
ClibPatGraph::input_num() const
{
  _check_valid();
  auto pg = mLibrary->_pat_graph(mId);
  return pg->input_num();
}

// @brief 枝数を返す．
SizeType
ClibPatGraph::edge_num() const
{
  _check_valid();
  auto pg = mLibrary->_pat_graph(mId);
  return pg->edge_num();
}

// @brief 枝(の番号)を返す．
SizeType
ClibPatGraph::edge(
  SizeType pos
) const
{
  _check_valid();
  auto pg = mLibrary->_pat_graph(mId);
  return pg->edge(pos);
}

END_NAMESPACE_YM_CLIB
