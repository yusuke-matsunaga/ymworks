#ifndef MISNODEHEAP_H
#define MISNODEHEAP_H

/// @file MisNodeHeap.h
/// @brief MisNodeHeap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "mincov/mincov_nsdef.h"
#include "MisNode.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class MisNodeHeap MisNodeHeap.h "MisNodeHeap.h"
/// @brief MisNode のヒープ木
//////////////////////////////////////////////////////////////////////
class MisNodeHeap
{
public:

  /// @brief コンストラクタ
  MisNodeHeap(
    SizeType num ///< [in] ノード数
  );

  /// @brief デストラクタ
  ~MisNodeHeap();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 確保されている配列のサイズを返す．
  SizeType
  node_size() const
  {
    return mNodeSize;
  }

  /// @brief ノードを返す．
  MisNode*
  node(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < node_size() )
  )
  {
    return &mNodeChunk[pos];
  }

  /// @brief ヒープが空の時 true を返す．
  bool
  empty() const
  {
    return mNodeNum == 0;
  }

  /// @brief ノードを追加する．
  void
  put(
    MisNode* node
  )
  {
    ASSERT_COND( mNodeNum < mNodeSize );

    set(mNodeNum, node);
    ++ mNodeNum;
    move_up(node);
  }

  /// @brief ノードを取り去る．
  void
  delete_node(
    MisNode* node
  )
  {
    ASSERT_COND( !empty() );

    int idx = node->mHeapIdx;

    ASSERT_COND( idx > 0 );

    node->mHeapIdx = 0;
    -- mNodeNum;
    auto last = mNodeHeap[mNodeNum];
    if ( last != node ) {
      -- idx;
      set(idx, last);
      move_down(last);
    }
  }

  /// @brief 値が最小の要素を取り出す．
  /// そのノードはヒープから取り除かれる．
  MisNode*
  get_min()
  {
    ASSERT_COND( !empty() );

    auto node = mNodeHeap[0];
    node->mHeapIdx = 0;
    -- mNodeNum;
    if ( mNodeNum > 0 ) {
      auto last = mNodeHeap[mNodeNum];
      set(0, last);
      move_down(last);
    }
    return node;
  }

  /// @brief ノードの値の変更に伴ってヒープ構造を更新する．
  void
  update(
    MisNode* node ///< [in] 値が変更されたノード
  )
  {
    int idx = node->mHeapIdx;
    ASSERT_COND( idx > 0 );

    -- idx;
    ASSERT_COND( mNodeHeap[idx] == node );

    if ( idx > 0 ) {
      int p_idx = (idx - 1) / 2;
      auto p_node = mNodeHeap[p_idx];
      if ( compare(p_node, node) > 0 ) {
	move_up(node);
	goto end;
      }
    }
    move_down(node);
  end:
    ;
  }

  /// @brief 内容を出力する．
  void
  print(
    std::ostream& s
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを適当な位置まで沈める．
  void
  move_down(
    MisNode* node ///< [in] 対象のノード
  );

  /// @brief ノードを適当な位置まで浮かび上がらせる．
  void
  move_up(
    MisNode* node ///< [in] 対象のノード
  );

  /// @brief ノードの比較を行う．
  /// @retval 負の値 node1 が node2 より前にある．
  /// @retval 0 node1 と node2 は同じ
  /// @retval 正の値 node1 が node2 より後ろにある．
  static
  int
  compare(
    MisNode* node1,
    MisNode* node2
  )
  {
    return node1->adj_num() - node2->adj_num();
  }

  /// @brief ノードをセットする．
  void
  set(
    SizeType pos, ///< [in] 位置
    MisNode* node ///< [in] ノード
  )
  {
    mNodeHeap[pos] = node;
    node->mHeapIdx = pos + 1;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 配列のサイズ
  SizeType mNodeSize;

  // ノードの実体の配列
  // サイズは mNodeSize
  MisNode* mNodeChunk;

  // ノードのヒープ木
  MisNode** mNodeHeap;

  // ヒープ木中にあるノード数
  SizeType mNodeNum;

};

END_NAMESPACE_YM_MINCOV

#endif // MISNODEHEAP_H
