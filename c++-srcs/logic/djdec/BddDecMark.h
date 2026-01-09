#ifndef BDDDECMARK_H
#define BDDDECMARK_H

/// @file BddDecMark.h
/// @brief BddDecMark のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/DjDec.h"


BEGIN_NAMESPACE_YM_DJDEC

class BddDecNode;
class BddDecEdge;

//////////////////////////////////////////////////////////////////////
/// @class BddDecMark BddDecMark.h "BddDecMark.h"
/// @brief BddDecNode のマークを管理するクラス
//////////////////////////////////////////////////////////////////////
class BddDecMark
{
public:

  /// @brief コンストラクタ
  BddDecMark() = default;

  /// @brief デストラクタ
  ~BddDecMark() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 再帰的にマークをつける．
  void
  mark_recur(
    BddDecEdge edge, ///< [in] 枝
    int mval     ///< [in] マークの値
  );

  /// @brief 再帰的にTFIマークをつける．
  int
  tfimark_recur(
    BddDecEdge edge ///< [in] 枝
  );

  /// @brief マークに基づいて境界ノードに印をつける．
  int
  get_boundary(
    BddDecEdge edge,                   ///< [in] 枝
    std::vector<const BddDecNode*>& or_list, ///< [out] 境界ノードをファンインに持つ OR ノードのリスト
    std::vector<const BddDecNode*>& xor_list ///< [out] 境界ノードをファンインに持つ XOR ノードのリスト
  );

  /// @brief 境界ノードを求める．
  void
  find_bnode(
    const BddDecNode* node,         ///< [in] ノード
    std::vector<BddDecEdge>& inputs ///< [out] 結果のノードを格納するリスト
  );

  /// @brief マークを返す．
  int
  mark(
    const BddDecNode* node ///< [in] ノード
  )
  {
    if ( mMarkDict.count(node) == 0 ) {
      mMarkDict.emplace(node, Info{});
    }
    auto& info = mMarkDict.at(node);
    return info.mark;
  }

  /// @brief マークをつける．
  void
  set_mark(
    const BddDecNode* node, ///< [in] ノード
    int val                 ///< [in] 値
  )
  {
    if ( mMarkDict.count(node) == 0 ) {
      mMarkDict.emplace(node, Info{});
    }
    auto& info = mMarkDict.at(node);
    info.mark = val;
  }

  /// @brief TFIマークを返す．
  int
  tfimark(
    const BddDecNode* node ///< [in] ノード
  )
  {
    if ( mMarkDict.count(node) == 0 ) {
      mMarkDict.emplace(node, Info{});
    }
    auto& info = mMarkDict.at(node);
    return info.tfimark;
  }

  /// @brief TFIマークをつける．
  void
  set_tfimark(
    const BddDecNode* node, ///< [in] ノード
    int val                 ///< [in] 値
  )
  {
    if ( mMarkDict.count(node) == 0 ) {
      mMarkDict.emplace(node, Info{});
    }
    auto& info = mMarkDict.at(node);
    info.tfimark = val;
  }

  /// @brief マークと TFIマークが等しい時に true を返す．
  bool
  is_boundary(
    const BddDecNode* node ///< [in] ノード
  )
  {
    if ( mMarkDict.count(node) == 0 ) {
      mMarkDict.emplace(node, Info{});
    }
    auto& info = mMarkDict.at(node);
    return info.mark == info.tfimark;
  }

  /// @brief 境界マークを返す．
  int
  bmark(
    const BddDecNode* node ///< [in] ノード
  )
  {
    if ( mMarkDict.count(node) == 0 ) {
      mMarkDict.emplace(node, Info{});
    }
    auto& info = mMarkDict.at(node);
    return info.bmark;
  }

  /// @brief 境界マークをつける．
  void
  set_bmark(
    const BddDecNode* node, ///< [in] ノード
    int val
  )
  {
    if ( mMarkDict.count(node) == 0 ) {
      mMarkDict.emplace(node, Info{});
    }
    auto& info = mMarkDict.at(node);
    info.bmark = val;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マークの情報
  struct Info
  {
    int mark{0};
    int tfimark{0};
    int bmark{0};
  };

  // ノードのポインタをキーにしてマークを格納する辞書
  std::unordered_map<const BddDecNode*, Info> mMarkDict;

};

END_NAMESPACE_YM_DJDEC

#endif // BDDDECMARK_H
