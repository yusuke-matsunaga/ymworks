#ifndef PTLIST_H
#define PTLIST_H

/// @file PtList.h
/// @brief PtList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtList PtList.h "parser/PtList.h"
/// @brief T* のリストを表す構造体
///
/// 先頭と末尾のポインタを持つ．
/// T は set_link(T*) というメンバ関数を持つと仮定する．
//////////////////////////////////////////////////////////////////////
template <typename T>
struct PtList
{
  /// @brief 先頭の要素
  T* top;

  /// @brief 末尾の要素
  T* tail;

  /// @brief 初期化する．
  void
  init(
    T* elem = nullptr
  )
  {
    top = elem;
    tail = elem;
  }

  /// @brief 要素を追加する．
  void
  add(
    T* elem
  )
  {
    if ( tail != nullptr ) {
      tail->set_link(elem);
      tail = elem;
    }
    else {
      top = elem;
      tail = elem;
    }
  }

};


//////////////////////////////////////////////////////////////////////
/// @class PtDList PtList.h "parser/PtList.h"
/// @brief 親子関係の2つのクラスの二重構造のリスト
///
/// 親クラスのリストと子クラスの末尾の要素のポインタを持つ．
/// 親クラスは先頭の子供を得る _item_top() という関数を持つと仮定している．
//////////////////////////////////////////////////////////////////////
template <typename T1, typename T2>
struct PtDList
{
  /// @brief 親クラスのリスト
  PtList<T1> head_list;

  /// @brief 末尾の子供
  T2* item_tail;

  /// @brief 初期化する．
  void
  init(
    T1* head = nullptr
  )
  {
    head_list.init(head);
    if ( head != nullptr ) {
      item_tail = head->_item_top();
    }
    else {
      item_tail = nullptr;
    }
  }

  /// @brief ヘッダを追加する．
  void
  add_head(
    T1* head
  )
  {
    if ( head != nullptr ) {
      head_list.add(head);
      item_tail = head->_item_top();
    }
  }

  /// @brief 要素を追加する．
  void
  add_item(
    T2* item
  )
  {
    if ( item != nullptr ) {
      item_tail->set_link(item);
      item_tail = item;
    }
  }

};

END_NAMESPACE_YM_VERILOG

#endif // PTHEADLIST_H
