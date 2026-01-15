#ifndef BNDFFLIST_H
#define BNDFFLIST_H

/// @file BnDffList.h
/// @brief BnDffList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/BnDff.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
/// @class BnDffIter BnDffList.h "ym/BnDffList.h"
/// @brief BnDffList の反復子
//////////////////////////////////////////////////////////////////////
class BnDffIter
{
public:

  using PtrList = std::vector<const DffImpl*>;
  using PtrIter = PtrList::const_iterator;

public:

  /// @brief 空のコンストラクタ
  BnDffIter() = default;

  /// @brief 値を指定したコンストラクタ
  explicit
  BnDffIter(
    PtrIter ptr_iter
  ) : mPtrIter{ptr_iter}
  {
  }

  /// @brief デストラクタ
  ~BnDffIter() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief dereference 演算子
  BnDff
  operator*() const
  {
    return BnDff(*mPtrIter);
  }

  /// @brief 次の要素に進む．
  BnDffIter&
  operator++()
  {
    ++ mPtrIter;
    return *this;
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const BnDffIter& right ///< [in] オペランド
  ) const
  {
    return mPtrIter == right.mPtrIter;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const BnDffIter& right ///< [in] オペランド
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
/// @class BnDffIter2 BnDffList.h "ym/BnDffList.h"
/// @brief BnDffList の反復子2(Python用)
//////////////////////////////////////////////////////////////////////
class BnDffIter2
{
public:

  using PtrList = std::vector<const DffImpl*>;
  using PtrIter = PtrList::const_iterator;

public:

  /// @brief 空のコンストラクタ
  BnDffIter2() = default;

  /// @brief 値を指定したコンストラクタ
  explicit
  BnDffIter2(
    PtrIter cur,
    PtrIter end
  ) : mCurIter{cur},
      mEndIter{end}
  {
  }

  /// @brief デストラクタ
  ~BnDffIter2() = default;


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
  BnDff
  next()
  {
    auto ptr = *mCurIter;
    ++ mCurIter;
    return BnDff(ptr);
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
/// @class BnDffList BnDffList.h "BnDffList.h"
/// @brief BnDff のリスト
//////////////////////////////////////////////////////////////////////
class BnDffList
{
public:

  using iterator = BnDffIter;
  using iterator2 = BnDffIter2;

public:

  /// @brief コンストラクタ
  explicit
  BnDffList(
    const std::vector<const DffImpl*>& ptr_list ///< [in] ポインタのリスト
  ) : mPtrList{ptr_list}
  {
  }

  /// @brief デストラクタ
  ~BnDffList() = default;


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
  BnDff
  operator[](
    SizeType index ///< [in] インデックス ( 0 <= index < size() )
  ) const
  {
    if ( index >= size() ) {
      throw std::out_of_range{"index is out of range"};
    }
    auto ptr = mPtrList[index];
    return BnDff(ptr);
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

  /// @brief std::vector<BnDff> に変換する．
  std::vector<BnDff>
  to_vector() const
  {
    std::vector<BnDff> dff_list;
    dff_list.reserve(size());
    for ( auto ptr: mPtrList ) {
      dff_list.push_back(BnDff(ptr));
    }
    return dff_list;
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const BnDffList& right
  ) const
  {
    return mPtrList == right.mPtrList;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const BnDffList& right
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ポインタのリスト
  const std::vector<const DffImpl*>& mPtrList;

};

END_NAMESPACE_YM_BN

#endif // BNDFFLIST_H
