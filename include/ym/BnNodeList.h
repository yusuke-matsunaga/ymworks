#ifndef BNNODELIST_H
#define BNNODELIST_H

/// @file BnNodeList.h
/// @brief BnNodeList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/BnNode.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
/// @ingroup BnGroup
/// @class BnNodeIter BnNodeList.h "ym/BnNodeList.h"
/// @brief BnNodeList の反復子
//////////////////////////////////////////////////////////////////////
class BnNodeIter
{
public:

  using PtrList = std::vector<const NodeImpl*>;
  using PtrIter = PtrList::const_iterator;

public:

  /// @brief 空のコンストラクタ
  BnNodeIter() = default;

  /// @brief 値を指定したコンストラクタ
  explicit
  BnNodeIter(
    PtrIter ptr_iter
  ) : mPtrIter{ptr_iter}
  {
  }

  /// @brief デストラクタ
  ~BnNodeIter() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief dereference 演算子
  BnNode
  operator*() const
  {
    return BnNode(*mPtrIter);
  }

  /// @brief 次の要素に進む．
  BnNodeIter&
  operator++()
  {
    ++ mPtrIter;
    return *this;
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const BnNodeIter& right ///< [in] オペランド
  ) const
  {
    return mPtrIter == right.mPtrIter;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const BnNodeIter& right ///< [in] オペランド
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ポインタの反復子
  PtrIter mPtrIter;

};


//////////////////////////////////////////////////////////////////////
/// @ingroup BnGroup
/// @class BnNodeIter2 BnNodeList.h "ym/BnNodeList.h"
/// @brief BnNodeList の反復子2(Python用)
//////////////////////////////////////////////////////////////////////
class BnNodeIter2
{
public:

  using PtrList = std::vector<const NodeImpl*>;
  using PtrIter = PtrList::const_iterator;

public:

  /// @brief 空のコンストラクタ
  BnNodeIter2() = default;

  /// @brief 対象のリストを指定したコンストラクタ
  explicit
  BnNodeIter2(
    PtrIter cur,
    PtrIter end
  ) : mCurIter{cur},
      mEndIter{end}
  {
  }

  /// @brief デストラクタ
  ~BnNodeIter2() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 有効な値を持っているか調べる．
  bool
  has_next() const
  {
    return mCurIter != mEndIter;
  }

  /// @brief 次の要素を返す．
  ///
  /// has_next() == true と仮定している．
  BnNode
  next()
  {
    auto ptr = *mCurIter;
    ++ mCurIter;
    return BnNode(ptr);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 現在の反復子
  PtrIter mCurIter;

  // 末尾の反復子
  PtrIter mEndIter;

};


//////////////////////////////////////////////////////////////////////
/// @ingroup BnGroup
/// @class BnNodeList BnNodeList.h "BnNodeList.h"
/// @brief BnNode のリスト
///
/// 意味的には std::vector<BnNode> と同様に BnNode のリストを表すが，
/// こちらのほうが効率がよい．
/// ただし，内容を変更することはできない．
/// BnModel や BnNode が持っているノードのリストの情報を表すために用いられる．
///
/// std::vector<> と同様の反復子を用意しているので範囲for文で用いる
/// ことができる．
///
/// @code
/// ...
/// BnNodeList node_list(...);
/// for ( auto node: node_list ) {
///   // node には node_list の要素の BnNode が入っている．
///   ...
/// }
/// @endcode
///
/// `[]` 演算子も使用可能
///
/// @code
/// for ( SizeType i = 0; i < node_list.size(); ++ i ) {
///   auto node = node_list[i];
///   ...
/// }
/// @endcode
//////////////////////////////////////////////////////////////////////
class BnNodeList
{
public:

  friend class BnModel;
  using iterator = BnNodeIter;
  using iterator2 = BnNodeIter2;

public:

  /// @brief コンストラクタ
  explicit
  BnNodeList(
    const std::vector<const NodeImpl*>& ptr_list ///< [in] ノードのポインタのリスト
  ) : mPtrList{ptr_list}
  {
  }

  /// @brief デストラクタ
  ~BnNodeList() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数を返す．
  SizeType
  size() const
  {
    return mPtrList.size();
  }

  /// @brief 要素を返す．
  BnNode
  operator[](
    SizeType index ///< [in] インデックス ( 0 <= index < size() )
  ) const
  {
    if ( index >= size() ) {
      throw std::out_of_range{"id is out of range"};
    }
    auto ptr = mPtrList[index];
    return BnNode(ptr);
  }

  /// @brief 先頭の反復子を返す．
  iterator
  begin() const
  {
    return iterator(mPtrList.begin());
  }

  /// @brief 末尾の反復子を返す．
  iterator
  end() const
  {
    return iterator(mPtrList.end());
  }

  /// @brief Python 用の反復子を返す．
  iterator2
  iter() const
  {
    return iterator2(mPtrList.begin(), mPtrList.end());
  }

  /// @brief std::vector<BnNode> に変換する．
  std::vector<BnNode>
  to_vector() const
  {
    std::vector<BnNode> node_list;
    node_list.reserve(size());
    for ( auto ptr: mPtrList ) {
      node_list.push_back(BnNode(ptr));
    }
    return node_list;
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const BnNodeList& right
  ) const
  {
    return mPtrList == right.mPtrList;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const BnNodeList& right
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードのポインタのリストへの参照
  const std::vector<const NodeImpl*>& mPtrList;

};

END_NAMESPACE_YM_BN

#endif // BNNODELIST_H
