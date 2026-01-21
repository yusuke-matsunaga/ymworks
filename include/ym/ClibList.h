#ifndef CLIBLIST_H
#define CLIBLIST_H

/// @file ClibList.h
/// @brief ClibList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
/// @class ClibListIter ClibList.h "ClibList.h"
/// @brief ClibList の反復子
///
/// - T1 が実装の本体で T2 がそのスマートポインタクラス
/// - T2{T1} の形のコンストラクタが定義されていると仮定する．
//////////////////////////////////////////////////////////////////////
template<class T1, class T2>
class ClibListIter
{
  using impl_iter = typename std::vector<const T1*>::const_iterator;

public:

  /// @brief コンストラクタ
  ClibListIter(
    impl_iter iter ///< [in] 実体のリストの反復子
  ) : mIter{iter}
  {
  }

  /// @brief デストラクタ
  ~ClibListIter() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を取り出す．
  T2
  operator*() const
  {
    return T2{*mIter};
  }

  /// @brief 一つ進める．
  ClibListIter&
  operator++()
  {
    ++ mIter;
    return *this;
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const ClibListIter& right
  ) const
  {
    return mIter == right.mIter;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const ClibListIter& right
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実体のリストの反復子
  impl_iter mIter;

};


//////////////////////////////////////////////////////////////////////
/// @class ClibList ClibList.h "ClibList.h"
/// @brief ClibXXX のリストを表すクラス
///
/// - T1 が実装の本体で T2 がそのスマートポインタクラス
/// - T2{T1} の形のコンストラクタが定義されていると仮定する．
//////////////////////////////////////////////////////////////////////
template<class T1, class T2>
class ClibList
{
public:

  using impl_iter = typename std::vector<const T1*>::const_iterator;
  using iterator = ClibListIter<T1, T2>;

public:

  /// @brief 空のコンストラクタ
  ClibList() = default;

  /// @brief コンストラクタ
  ClibList(
    impl_iter begin, ///< [in] リストの先頭
    impl_iter end    ///< [in] リストの末尾
  ) : mList{begin, end}
  {
  }

  /// @brief コンストラクタ
  ClibList(
    const std::vector<const T1*>& impl_list ///< [in] 実体のリスト
  ) : mList{impl_list}
  {
  }

  /// @brief デストラクタ
  ~ClibList() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数を返す．
  SizeType
  size() const
  {
    return mList.size();
  }

  /// @brief 要素を返す．
  T2
  operator[](
    SizeType pos
  ) const
  {
    if ( pos < 0 || size() <= pos ) {
      throw std::out_of_range("out of range");
    }
    auto ptr = mList[pos];
    return T2{ptr};
  }

  /// @brief 先頭の反復子を返す．
  iterator
  begin() const
  {
    return iterator{mList.begin()};
  }

  /// @brief 末尾の反復子を返す．
  iterator
  end() const
  {
    return iterator{mList.end()};
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実体のリスト
  std::vector<const T1*> mList;

};


//////////////////////////////////////////////////////////////////////
// ClibList を用いたリスト型
//////////////////////////////////////////////////////////////////////

class CiCell;
using ClibCellList = ClibList<CiCell, ClibCell>;
class CiCellGroup;
using ClibCellGroupList = ClibList<CiCellGroup, ClibCellGroup>;
class CiCellClass;
using ClibCellClassList = ClibList<CiCellClass, ClibCellClass>;
class CiLut;
using ClibLutList = ClibList<CiLut, ClibLut>;

END_NAMESPACE_YM_CLIB

BEGIN_NAMESPACE_YM

using nsClib::ClibCellList;
using nsClib::ClibCellGroupList;
using nsClib::ClibCellClassList;
using nsClib::ClibLutList;

END_NAMESPACE_YM

#endif // CLIBLIST_H
