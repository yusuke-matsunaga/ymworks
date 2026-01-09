
/// @file FenceGen.cc
/// @brief FenceGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "FenceGen.h"


BEGIN_NAMESPACE_YM_EXSYN

// @brief Fence を生成する．
std::vector<Fence>
FenceGen::operator()(
  SizeType k, ///< [in] ノード数
  SizeType l, ///< [in] 最大レベル
  SizeType i, ///< [in] 演算のオペランド数
  SizeType o  ///< [in] 出力数
)
{
  if ( l < 1 ) {
    return {};
  }
  if ( k < l ) {
    throw std::invalid_argument{"k < l"};
  }
  mK = k;
  mL = l;
  mI = i;
  mO = o;
  mFenceList.clear();
  // 各レベルに1つのノードは必ず割り当てられるので
  // k - l のノードのみに自由度がある．
  SizeType r = k - l;
  // 現在の割り当て結果のリスト
  std::vector<SizeType> cur_num_list(mL, 1);
  recur_sub(cur_num_list, 0, r);
  return mFenceList;
}

// @brief 下請け関数
void
FenceGen::recur_sub(
  const std::vector<SizeType>& cur_num_list,
  SizeType pos,
  SizeType r
)
{
  if ( pos == mL ) {
    // cur_num_list を解に加える．
    mFenceList.push_back(Fence(cur_num_list));
  }
  else {
    // r の一部を pos に割り当てる．
    SizeType begin = 0;
    if ( pos == mL - 1 ) {
      // 最後の要素なら全て割り当てる．
      begin = r;
    }
    for ( SizeType n = begin; n <= r; ++ n ) {
      auto tmp_num_list = cur_num_list; // コピー
      tmp_num_list[pos] += n;
      if ( check_feasibility(tmp_num_list) ) {
	recur_sub(tmp_num_list, pos + 1, r - n);
      }
    }
  }
}

// @brief 割り当てが実現可能か調べる．
bool
FenceGen::check_feasibility(
  const std::vector<SizeType>& num_list
) const
{
  if ( num_list.size() != mL ) {
    throw std::logic_error{"num_list.size() != mL"};
  }
  SizeType limit = mO;
  for ( SizeType i = 0; i < mL; ++ i ) {
    SizeType l = mL - i - 1;
    if ( num_list[l] > limit ) {
      return false;
    }
    limit *= mI;
  }
  return true;
}

END_NAMESPACE_YM_EXSYN
