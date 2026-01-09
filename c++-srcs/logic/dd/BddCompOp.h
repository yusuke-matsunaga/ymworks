#ifndef BDDCOMPOP_H
#define BDDCOMPOP_H

/// @file BddCompOp.h
/// @brief BddCompOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "BddOpBase.h"
#include "BddIteOp.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class BddCompOp BddCompOp.h "BddCompOp.h"
/// @brief compose演算を行うクラス
///
/// remap_vars() 演算にも対応している．
//////////////////////////////////////////////////////////////////////
class BddCompOp :
  public BddOpBase
{
public:

  /// @brief コンストラクタ
  BddCompOp(
    BddMgrImpl* mgr,                                     ///< [in] マネージャ
    const std::unordered_map<SizeType, DdEdge>& comp_map ///< [in] 変換マップ
  );

  /// @brief デストラクタ
  ~BddCompOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 複数のcompose演算を行う．
  DdEdge
  comp_op(
    DdEdge edge
  )
  {
    return comp_step(edge, 0);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 複数のcompose演算を行う．
  DdEdge
  comp_step(
    DdEdge edge,
    SizeType pos
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変換リスト
  std::vector<std::pair<SizeType, DdEdge>> mCompList;

  // 演算結果テーブル
  std::unordered_map<const DdNode*, DdEdge> mTable;

  // ITE演算
  BddIteOp mIteOp;

};

END_NAMESPACE_YM_DD

#endif // BDDCOMPOP_H
