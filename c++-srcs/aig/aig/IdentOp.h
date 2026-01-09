#ifndef IDENTOP_H
#define IDENTOP_H

/// @file IdentOp.h
/// @brief IdentOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class IdentOp IdentOp.h "IdentOp.h"
/// @brief 同一の構造を持つかどうかのチェックを行う．
///
/// 名前やコメントは比較しない．
//////////////////////////////////////////////////////////////////////
class IdentOp
{
public:

  /// @brief コンストラクタ
  IdentOp(
    AigMgrImpl* mgr1, ///< [in] マネージャ1
    AigMgrImpl* mgr2  ///< [in] マネージャ2
  ) : mMgr1{mgr1},
      mMgr2{mgr2}
  {
  }

  /// @brief デストラクタ
  ~IdentOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 構造が等しければ true を返す．
  bool
  check();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 枝野対応付けが取れているか調べる．
  bool
  check_edge(
    AigEdge edge1, ///< [in] マネージャ1の枝
    AigEdge edge2  ///< [in] マネージャ2の枝
  );

  /// @brief 対応するノードを返す．
  ///
  /// なければ nullptr を返す．
  AigNode*
  node_match(
    AigNode* node1 ///< [in] マネージャ1のノード
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャ1
  AigMgrImpl* mMgr1;

  // マネージャ2
  AigMgrImpl* mMgr2;

  // マネージャ1のノード番号をキーにして対応するマネージャ2の
  // ノードを格納する辞書
  std::unordered_map<SizeType, AigNode*> mNodeMap;

};

END_NAMESPACE_YM_AIG

#endif // IDENTOP_H
