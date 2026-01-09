#ifndef BDDINTSECTOP_H
#define BDDINTSECTOP_H

/// @file BddIntsectOp.h
/// @brief BddIntsectOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "BddOpBase.h"
#include "DdEdge.h"
#include "Apply2Key.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class BddIntsectOp BddIntsectOp.h "BddIntsectOp.h"
/// @brief 交差チェックを行うクラス
//////////////////////////////////////////////////////////////////////
class BddIntsectOp :
  public BddOpBase
{
public:

  /// @brief コンストラクタ
  BddIntsectOp(
    BddMgrImpl* mgr ///< [in] マネージャ
  ) : BddOpBase{mgr}
  {
  }

  /// @brief デストラクタ
  ~BddIntsectOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief コファクターを計算する．
  bool
  op_step(
    DdEdge edge0, ///< [in] 枝0
    DdEdge edge1  ///< [in] 枝1
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算結果テーブル
  std::unordered_map<Apply2Key, bool> mTable;

};

END_NAMESPACE_YM_DD

#endif // BDDINTSECTOP_H
