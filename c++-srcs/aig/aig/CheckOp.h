#ifndef CHECKOP_H
#define CHECKOP_H

/// @file CheckOp.h
/// @brief CheckOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class CheckOp CheckOp.h "CheckOp.h"
/// @brief 出力側から DFS を行い適正かチェックする．
///
/// 具体的には以下の項目をチェックする．
/// - 2つのファンインが共に定数ではない．
/// - 2つのファンインが同一ではない．
/// - 参照が循環していない．
///
/// 2つのファンインが逆相で同一のノードを指す枝は適正とみなす．
/// ただし，そのようなノードは簡単化で0に縮退する．
//////////////////////////////////////////////////////////////////////
class CheckOp
{
public:

  /// @brief コンストラクタ
  CheckOp(
    std::vector<std::string>& err_list ///< [in] エラーメッセージを格納するリスト
  ) : mErrList{err_list}
  {
  }

  /// @brief デストラクタ
  ~CheckOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief DFS を行いチェックする．
  void
  dfs(
    AigEdge edge ///< [in] 対象の枝
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 訪問中フラグ
  std::unordered_map<SizeType, bool> mVisiting;

  // エラーメッセージを格納するリスト
  std::vector<std::string>& mErrList;

};

END_NAMESPACE_YM_AIG

#endif // CHECKOP_H
