#ifndef FENCEGEN_H
#define FENCEGEN_H

/// @file FenceGen.h
/// @brief FenceGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ExSyn.h"
#include "Fence.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class FenceGen FenceGen.h "FenceGen.h"
/// @brief Fence を生成するクラス
//////////////////////////////////////////////////////////////////////
class FenceGen
{
public:

  /// @brief コンストラクタ
  FenceGen() = default;

  /// @brief デストラクタ
  ~FenceGen() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief Fence を生成する．
  std::vector<Fence>
  operator()(
    SizeType k,     ///< [in] ノード数
    SizeType l,     ///< [in] 最大レベル
    SizeType i = 2, ///< [in] 演算のオペランド数
    SizeType o = 1  ///< [in] 出力数
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 下請け関数
  void
  recur_sub(
    const std::vector<SizeType>& cur_num_list,
    SizeType pos,
    SizeType r
  );

  /// @brief 割り当てが実現可能か調べる．
  ///
  /// 具体的には
  /// - num_list[mL - 1] <= mO
  /// - num_list[mL - 2] <= mO * mI
  /// - num_list[mL - 3] <= mO * mI * mI
  /// というチェックを行う．
  bool
  check_feasibility(
    const std::vector<SizeType>& num_list
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード数
  SizeType mK;

  // 最大レベル
  SizeType mL;

  // 演算のオペランド数
  SizeType mI;

  // 出力数
  SizeType mO;

  // 生成された Fence のリスト
  std::vector<Fence> mFenceList;

};

END_NAMESPACE_YM_EXSYN

#endif // FENCEGEN_H
