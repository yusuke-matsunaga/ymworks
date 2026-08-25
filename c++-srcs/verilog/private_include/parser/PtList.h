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
/// @class PtList PtHeadList.h "parser/PtHeadList.h"
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

END_NAMESPACE_YM_VERILOG

#endif // PTHEADLIST_H
