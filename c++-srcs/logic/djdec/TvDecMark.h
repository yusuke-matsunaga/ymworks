#ifndef TVDECMARK_H
#define TVDECMARK_H

/// @file TvDecMark.h
/// @brief TvDecMark のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/DjDec.h"
#include "TvDecNode.h"


BEGIN_NAMESPACE_YM_DJDEC

class TvDecEdge;

//////////////////////////////////////////////////////////////////////
/// @class TvDecMark TvDecMark.h "TvDecMark.h"
/// @brief TvDecNode のマークを管理するクラス
//////////////////////////////////////////////////////////////////////
class TvDecMark
{
public:

  /// @brief コンストラクタ
  TvDecMark() = default;

  /// @brief デストラクタ
  ~TvDecMark() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 再帰的にマークをつける．
  void
  mark_recur(
    TvDecEdge edge, ///< [in] 枝
    int mval       ///< [in] マークの値
  );

  /// @brief 再帰的にTFIマークをつける．
  int
  tfimark_recur(
    TvDecEdge edge ///< [in] 枝
  );

  /// @brief マークに基づいて境界ノードに印をつける．
  int
  get_boundary(
    TvDecEdge edge,                         ///< [in] 枝
    std::vector<const TvDecNode*>& or_list, ///< [out] 境界ノードをファンインに持つ OR ノードのリスト
    std::vector<const TvDecNode*>& xor_list ///< [out] 境界ノードをファンインに持つ XOR ノードのリスト
  );

  /// @brief 境界ノードを求める．
  void
  find_bnode(
    const TvDecNode* node,         ///< [in] ノード
    std::vector<TvDecEdge>& inputs ///< [out] 結果のノードを格納するリスト
  );

  /// @brief マークを返す．
  int
  mark(
    const TvDecNode* node ///< [in] ノード
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
    const TvDecNode* node, ///< [in] ノード
    int val               ///< [in] 値
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
    const TvDecNode* node ///< [in] ノード
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
    const TvDecNode* node, ///< [in] ノード
    int val               ///< [in] 値
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
    const TvDecNode* node ///< [in] ノード
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
    const TvDecNode* node ///< [in] ノード
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
    const TvDecNode* node, ///< [in] ノード
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
  std::unordered_map<const TvDecNode*, Info> mMarkDict;

};

END_NAMESPACE_YM_DJDEC

#endif // TVDECMARK_H
