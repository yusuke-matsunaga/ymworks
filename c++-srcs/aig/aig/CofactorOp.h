#ifndef COFACTOROP_H
#define COFACTOROP_H

/// @file CofactorOp.h
/// @brief CofactorOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"
#include "AigEdge.h"
#include "AigMgrImpl.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class CofactorOp CofactorOp.h "CofactorOp.h"
/// @brief cofactor 演算を行うクラス
///
/// 実体は std::unordered_map<SizeType, AigEdge> だが
/// AigEdge をキーのように扱うことができる．
/// 反転した枝の結果も反転する．
//////////////////////////////////////////////////////////////////////
class CofactorOp
{
public:

  /// @brief コンストラクタ
  CofactorOp(
    AigMgrImpl* mgr,                       ///< [in] マネージャ
    const std::vector<AigEdge>& cedge_list ///< [in] コファクター用のキューブを表すリスト
  );

  /// @brief デストラクタ
  ~CofactorOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief コファクター演算を行う．
  AigEdge
  call(
    AigEdge edge ///< [in] 対象の枝
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 登録する．
  void
  put(
    const AigNode* node, ///< [in] キーのノード
    AigEdge edge         ///< [in] 結果の枝
  )
  {
    mDict.emplace(node->id(), edge);
  }

  /// @brief 登録されているか調べる．
  bool
  check(
    AigEdge key ///< [in] キーの枝
  ) const
  {
    auto node = key.node();
    return mDict.count(node->id()) > 0;
  }

  /// @brief 結果を得る．
  AigEdge
  get(
    AigEdge key ///< [in] キーの枝
  ) const
  {
    auto node = key.node();
    auto inv = key.inv();
    auto edge = mDict.at(node->id());
    if ( inv ) {
      edge = ~edge;
    }
    return edge;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャ
  AigMgrImpl* mMgr;

  // 結果を保持する辞書
  std::unordered_map<SizeType, AigEdge> mDict;

};

END_NAMESPACE_YM_AIG

#endif // COFACTOROP_H
