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
/// @class BnNodeIter BnNodeList.h "ym/BnNodeList.h"
/// @brief BnNodeList の反復子
//////////////////////////////////////////////////////////////////////
class BnNodeIter
{
public:

  using PtrIter = std::vector<const NodeImpl*>::const_iterator;

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
    const PtrList& ptr_list
  ) : mPtrList{ptr_list},
      mCurIter{mPtrList.begin()},
      mEndIter{mPtrList.end()}
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

  // ポインタのリスト
  PtrList mPtrList;

  // 現在の反復子
  PtrIter mCurIter;

  // 末尾の反復子
  PtrIter mEndIter;

};



//////////////////////////////////////////////////////////////////////
/// @class BnNodeList BnNodeList.h "BnNodeList.h"
/// @brief BnNode のリスト
//////////////////////////////////////////////////////////////////////
class BnNodeList
{
public:

  friend class BnModel;
  using iterator = BnNodeIter;
  using iterator2 = BnNodeIter2;

public:

  /// @brief 空のコンストラクタ
  BnNodeList() = default;

  /// @brief ポインタのリストを指定したコンストラクタ
  explicit
  BnNodeList(
    const std::vector<const NodeImpl*> ptr_list ///< [in] ポインタのリスト
  ) : mPtrList{ptr_list}
  {
  }

  /// @brief ノードのリストを指定したコンストラクタ
  explicit
  BnNodeList(
    const std::vector<BnNode>& node_list ///< [in] ノードのリスト
  )
  {
    mPtrList.reserve(node_list.size());
    for ( auto& node: node_list ) {
      push_back(node);
    }
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
      throw std::out_of_range{"index is out of range"};
    }
    return BnNode(mPtrList[index]);
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
    return iterator2(mPtrList);
  }

  /// @brief 要素を末尾に追加する．
  void
  push_back(
    const BnNode& node
  );
#if 0
  {
    {
      std::cout << "BnNodeList::push_back(node)" << std::endl
		<< "model = " << std::hex << node.mPtr->_model()
		<< std::dec << std::endl
		<< "input_num() = " << node.mPtr->_model()->input_num()
		<< std::endl;
    }
    mPtrList.push_back(node.mPtr);
  }
#endif

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
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////
#if 0
  /// @brief ポインタのリストを返す．
  const std::vector<const NodeImpl*>&
  ptr_list() const
  {
    return mPtrList;
  }
#endif

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ポインタのリスト
  std::vector<const NodeImpl*> mPtrList;

};

END_NAMESPACE_YM_BN

#endif // BNNODELIST_H
