#ifndef AIGWRITER_H
#define AIGWRITER_H

/// @file AigWriter.h
/// @brief AigWriter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"
#include "AigEdge.h"
#include "AigNode.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class AigWriter AigWriter.h "AigWriter.h"
/// @brief aag/aig 形式の出力を行うクラス
///
/// aag 形式の場合はそのままの内容を出力すればよいが，
/// aig 形式では自身のID番号がファンインのノードのID番号より
/// 大きくなければならないという制約があるので出力用に
/// 見かけのID番号を付け替える必要がある．
//////////////////////////////////////////////////////////////////////
class AigWriter
{
public:

  /// @brief コンストラクタ
  AigWriter(
    AigMgrImpl* mgr
  );

  /// @brief デストラクタ
  ~AigWriter() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief aag 形式で出力する．
  void
  write_aag(
    std::ostream& s  ///< [in] 出力ストリーム
  );

  /// @brief aig 形式で出力する．
  void
  write_aig(
    std::ostream& s  ///< [in] 出力ストリーム
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief DFSを用いて mAndList を作る．
  void
  mark_dfs(
    AigEdge edge
  );

  /// @brief シンボルとコメントを出力する．
  void
  write_symbols(
    std::ostream& s ///< [in] 出力ストリーム
  );

  /// @brief 符号化を行って数値を書き込む．
  void
  encode(
    std::ostream& s, ///< [in] 出力ストリーム
    SizeType val     ///< [in] 値
  );

  /// @brief ノード番号をリテラルに変換する．
  SizeType
  id2lit(
    SizeType id
  )
  {
    auto id1 = mIdMap.at(id);
    return (id1 + 1) * 2;
  }

  /// @brief 枝の情報をリテラルに変換する．
  SizeType
  edge2lit(
    AigEdge edge
  )
  {
    if ( edge.is_zero() ) {
      return 0;
    }
    if ( edge.is_one() ) {
      return 1;
    }
    auto node = edge.node();
    auto lit = id2lit(node->id());
    if ( edge.inv() ) {
      ++ lit;
    }
    return lit;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のマネージャ
  AigMgrImpl* mMgr{nullptr};

  // 処理の対象となるノードの印
  std::unordered_set<SizeType> mMark;

  // 処理の対象となる入力ノードのリスト
  std::vector<AigNode*> mInputList;

  // トポロジカル順に整列したANDノードのリスト
  std::vector<AigNode*> mAndList;

  // IDの変換辞書
  std::unordered_map<SizeType, SizeType> mIdMap;

};

END_NAMESPACE_YM_AIG

#endif // AIGWRITER_H
