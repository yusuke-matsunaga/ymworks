#ifndef BDDCUBEOP_H
#define BDDCUBEOP_H

/// @file BddCubeOp.h
/// @brief BddCubeOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "BddOpBase.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class BddCubeOp BddCubeOp.h "BddCubeOp.h"
/// @brief キューブを表すBDDに関する演算
//////////////////////////////////////////////////////////////////////
class BddCubeOp :
  public BddOpBase
{
public:

  /// @brief コンストラクタ
  BddCubeOp(
    BddMgrImpl* mgr ///< [in] マネージャ
  ) : BddOpBase{mgr}
  {
  }

  /// @brief デストラクタ
  ~BddCubeOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理積を求める．
  DdEdge
  and_step(
    DdEdge edge0,
    DdEdge edge1
  );

  /// @brief 集合差を求める．
  DdEdge
  diff_step(
    DdEdge edge0,
    DdEdge edge1
  );

};

END_NAMESPACE_YM_DD

#endif // BDDCUBEOP_H
