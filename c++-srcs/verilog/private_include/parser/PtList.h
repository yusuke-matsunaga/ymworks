#ifndef PTLIST_H
#define PTLIST_H

/// @file PtList.h
/// @brief PtList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtListIter PtListIter.h "PtListIter.h"
/// @brief PtList<T> 用の反復子
//////////////////////////////////////////////////////////////////////
template <typename T>
class PtListIter
{
public:

  /// @brief コンストラクタ
  PtListIter(
    T* ptr = nullptr
  ) : mPtr{ptr}
  {
  }

  /// @brief デストラクタ
  ~PtListIter() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を返す．
  T*
  operator*() const
  {
    return mPtr;
  }

  /// @brief 次の要素に移動する．
  PtListIter&
  operator++()
  {
    if ( mPtr != nullptr ) {
      mPtr = mPtr->link();
    }
    return *this;
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const PtListIter& right
  ) const
  {
    return mPtr == right.mPtr;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const PtListIter& right
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 内容を指すポインタ
  T* mPtr;

};


//////////////////////////////////////////////////////////////////////
/// @class PtList PtList.h "parser/PtList.h"
/// @brief T* のリストを表す構造体
///
/// 先頭と末尾のポインタを持つ．
/// T は set_link(T*) というメンバ関数を持つと仮定する．
///
/// Yacc/Bison 中ではコンストラクタが使えないため
/// 明示的な初期化関数の呼び出しが必要となる．
///
/// @code
/// auto ptlist = PtList<T>::new_obj(...)
/// @endcode
/// という形で初期化する．
//////////////////////////////////////////////////////////////////////
template <typename T>
struct PtList
{
  using const_iterator = PtListIter<T>;

  /// @brief 先頭の要素
  T* top;

  /// @brief 末尾の要素
  T* tail;

  /// @brief 初期化されたインスタンスを返す．
  ///
  static
  PtList
  new_obj(
    T* elem = nullptr
  )
  {
    PtList ptlist;
    ptlist._init(elem);
    return ptlist;
  }

  /// @brief 初期化関数
  void
  _init(
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

  /// @brief 先頭の反復子を返す．
  const_iterator
  begin() const
  {
    return const_iterator(top);
  }

  /// @brief 末尾の反復子を返す．
  const_iterator
  end() const
  {
    return const_iterator(nullptr);
  }

};


//////////////////////////////////////////////////////////////////////
/// @class PtDList PtList.h "parser/PtList.h"
/// @brief 親子関係の2つのクラスの二重構造のリスト
///
/// 親クラスのリストと子クラスの末尾の要素のポインタを持つ．
//////////////////////////////////////////////////////////////////////
template <typename T1, typename T2>
struct PtDList
{
  /// @brief 親クラスのリスト
  PtList<T1> head_list;

  /// @brief 末尾の子供
  T2* item_tail;

  /// @brief 初期化されたインスタンスを返す．
  ///
  /// Yacc/Bison 中ではコンストラクタが使えないため
  /// 明示的な初期化関数の呼び出しが必要となる．
  static
  PtDList
  new_obj(
  )
  {
    PtDList ptdlist;
    ptdlist._init();
    return ptdlist;
  }

  /// @brief 初期化されたインスタンスを返す．
  ///
  /// Yacc/Bison 中ではコンストラクタが使えないため
  /// 明示的な初期化関数の呼び出しが必要となる．
  static
  PtDList
  new_obj(
    T1* head,
    T2* item
  )
  {
    PtDList ptdlist;
    ptdlist._init(head, item);
    return ptdlist;
  }

  /// @brief 初期化関数
  void
  _init(
  )
  {
    head_list._init();
    item_tail = nullptr;
  }

  /// @brief 初期化関数
  void
  _init(
    T1* head,
    T2* item
  )
  {
    head_list._init(head);
    item_tail = item;
  }

  /// @brief ヘッダを追加する．
  void
  add_head(
    T1* head,
    T2* item
  )
  {
    if ( head != nullptr ) {
      head_list.add(head);
      item_tail = item;
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


//////////////////////////////////////////////////////////////////////
/// @class PtListElem PtList.h "parser/PtList.h"
/// @brief PtList<> の要素用のクラス
///
/// 実際にはこのクラスを継承する必要がある．
//////////////////////////////////////////////////////////////////////
template <typename T>
class PtListElem :
  public PtBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief リンクを返す．
  T*
  link() const
  {
    return mLink;
  }

  /// @brief リンクを設定する．
  void
  set_link(
    T* link
  )
  {
    mLink = link;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素
  T* mLink{nullptr};

};

END_NAMESPACE_YM_VERILOG

#endif // PTHEADLIST_H
