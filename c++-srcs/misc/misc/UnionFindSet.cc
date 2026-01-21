
/// @file UnionFindSet.cc
/// @brief UnionFindSet の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2010, 2014, 2018, 2019, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/UnionFindSet.h"


BEGIN_NAMESPACE_YM

/// @class UFCell UnionFindSet.cc
/// @brief UnionFindSet 内部で用いられるクラス
class UFCell
{
  friend class UnionFindSet;
private:

  // コンストラクタ
  UFCell(
    SizeType id
  ) : mId{id},
      mParent{this},
      mRank{0}
  {
  }

  // この要素の属する集合の代表元を返す．
  UFCell*
  find()
  {
    auto tmp = this;

    // 検索の途中で道の圧縮を行なう．
    auto parent = tmp->mParent;
    while ( parent->mParent != parent ) {
      tmp = tmp->mParent = parent->mParent;
      parent = tmp->mParent;
    }

    return parent;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 一つの母集合の中でユニークなID番号
  SizeType mId;

  // 親を指すポインタ
  UFCell* mParent;

  // 根本までのレベル
  SizeType mRank{0};

};


/// @brief コンストラクタ
UnionFindSet::UnionFindSet(
  SizeType n
) : mCellArray(n)
{
  for ( SizeType i = 0; i < n; ++ i ) {
    mCellArray[i] = new UFCell(i);
  }
}

// デストラクタ
UnionFindSet::~UnionFindSet()
{
  for ( auto cell: mCellArray ) {
    delete cell;
  }
}

// id を含む集合の代表元を返す．
SizeType
UnionFindSet::find(
  SizeType id
)
{
  auto x = _get(id);
  x = x->find();
  return x->mId;
}

// x_id を含む集合と y_id を含む集合を併合し，新しい集合の代表元を返す．
SizeType
UnionFindSet::merge(
  SizeType x_id,
  SizeType y_id
)
{
  auto x = _get(x_id);
  if ( x->mParent != x ) {
    // xは代表点ではない．
    x = x->find();
  }

  auto y = _get(y_id);
  if ( y->mParent != y ) {
    // yは代表点ではない．
    y = y->find();
  }

  if ( x == y ) {
    // 同一の集合のマージはできない．
    // ただし，違反ではないのでその代表元を返す．
    return x->mId;
  }

  // ランクによるマージ
  if ( x->mRank > y->mRank ) {
    y->mParent = x;
    return x->mId;
  }
  if ( x->mRank == y->mRank ) {
    ++ x->mRank;
    y->mParent = x;
    return x->mId;
  }
  x->mParent = y;
  return y->mId;
}

END_NAMESPACE_YM
