
/// @file CiPatMgr.cc
/// @brief CiPatMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ci/CiPatMgr.h"
#include "ci/CiPatGraph.h"
#include "ci/Serializer.h"
#include "ci/Deserializer.h"
#include "ym/BinEnc.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
// クラス CiPatMgr
//////////////////////////////////////////////////////////////////////

CiPatMgr::CiPatMgr()
{
}

// @brief このセルライブラリに含まれるセルの最大の入力数を得る．
SizeType
CiPatMgr::max_input() const
{
  SizeType ans = 0;
  for ( auto& pat: mPatArray ) {
    auto ni = pat.input_num();
    if ( ans < ni ) {
      ans = ni;
    }
  }
  return ans;
}

// @brief 総パタン数を返す．
SizeType
CiPatMgr::pat_num() const
{
  // CiPatGraph の定義が必要なのでヘッダファイルに書けない．
  return mPatArray.size();
}

// @brief パタンを返す．
const CiPatGraph&
CiPatMgr::pat(
  SizeType pat_id
) const
{
  // CiPatGraph の定義が必要なのでヘッダファイルに書けない．
  if ( pat_id >= pat_num() ) {
    throw std::out_of_range{"'pat_id' is out of range"};
  }
  return mPatArray[pat_id];
}

// @brief ノード数を設定する．
void
CiPatMgr::set_node_num(
  SizeType nn
)
{
  mNodeTypeArray.clear();
  mEdgeArray.clear();
  if ( nn > 0 ) {
    mNodeTypeArray.resize(nn);
    mEdgeArray.resize(nn * 2);
  }
}

// @brief 入力ノードの情報を設定する．
void
CiPatMgr::set_node_info(
  SizeType pos,
  SizeType id
)
{
  SizeType v = static_cast<SizeType>(ClibPatType::Input) | (id << 2);
  mNodeTypeArray[pos] = v;
}

// @brief 論理ノードの情報を設定する．
void
CiPatMgr::set_node_info(
  SizeType pos,
  ClibPatType type,
  SizeType iid1,
  bool iinv1,
  SizeType iid2,
  bool iinv2
)
{
  SizeType v = static_cast<SizeType>(type);
  mNodeTypeArray[pos] = v;
  SizeType fanin_id[] = {iid1, iid2};
  bool fanin_inv[] = {iinv1, iinv2};
  for ( auto i: { 0, 1 } ) {
    SizeType v = 0U;
    v = fanin_id[i] * 2;
    if ( fanin_inv[i] ) {
      v |= 1U;
    }
    mEdgeArray[pos * 2 + i] = v;
  }
}

// @brief パタン数を設定する．
void
CiPatMgr::set_pat_num(
  SizeType np
)
{
  mPatArray.clear();
  mPatArray.resize(np);
}

// @brief パタンの情報を設定する．
void
CiPatMgr::set_pat_info(
  SizeType pos,
  const CiCellClass* rep_class,
  SizeType input_num,
  const std::vector<SizeType>& edge_list
)
{
  CiPatGraph& pg = mPatArray[pos];
  pg.init(rep_class, input_num, edge_list);
}

// @brief バイナリダンプを行う．
void
CiPatMgr::dump(
  Serializer& s
) const
{
  // パタングラフのノード情報のダンプ
  s.dump(node_num());
  for ( auto i: Range(node_num()) ) {
    s.dump(mNodeTypeArray[i]);
    s.dump(mEdgeArray[i * 2 + 0]);
    s.dump(mEdgeArray[i * 2 + 1]);
  }

  // パタングラフの情報のダンプ
  s.dump(pat_num());
  for ( auto& pat: mPatArray ) {
    pat.dump(s);
  }
}

// @brief データを読み込んでセットする．
bool
CiPatMgr::restore(
  Deserializer& s
)
{
  // ノードと枝の情報を読み込む．
  SizeType nn;
  s.restore(nn);
  set_node_num(nn);
  for ( auto i: Range(nn) ) {
    s.restore(mNodeTypeArray[i]);
    s.restore(mEdgeArray[i * 2]);
    s.restore(mEdgeArray[i * 2 + 1]);
  }

  // パタングラフの情報を読み込む．
  SizeType np;
  s.restore(np);
  set_pat_num(np);
  for ( auto id: Range(np) ) {
    mPatArray[id].restore(s);
  }

  return true;
}

END_NAMESPACE_YM_CLIB
