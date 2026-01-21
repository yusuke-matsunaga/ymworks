#ifndef CLIBLIST2_H
#define CLIBLIST2_H

/// @file ClibList2.h
/// @brief ClibList2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ClibCellElem.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
/// @class ClibList2Iter ClibList2.h "ClibList2.h"
/// @brief ClibList2 の反復子
///
/// - T1 が実装の本体で T2 がそのスマートポインタクラス
/// - T2{const CiCell*, T1} の形のコンストラクタが定義されていると仮定する．
//////////////////////////////////////////////////////////////////////
template<class T1, class T2>
class ClibList2Iter :
  private ClibCellPtr
{
  using impl_iter = typename std::vector<const T1*>::const_iterator;

public:

  /// @brief コンストラクタ
  ClibList2Iter(
    const CiCell* cell, ///< [in] 親のセル
    impl_iter iter      ///< [in] 実体のリストの反復子
  ) : ClibCellPtr{cell},
      mIter{iter}
  {
  }

  /// @brief デストラクタ
  ~ClibList2Iter() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を取り出す．
  T2
  operator*() const
  {
    return T2{ClibCellPtr::_impl(), *mIter};
  }

  /// @brief 一つ進める．
  ClibList2Iter&
  operator++()
  {
    ++ mIter;
    return *this;
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const ClibList2Iter& right
  ) const
  {
    return mIter == right.mIter;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const ClibList2Iter& right
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
/// @class ClibList2 ClibList2.h "ClibList2.h"
/// @brief ClibXXX のリストを表すクラス
///
/// - T1 が実装の本体で T2 がそのスマートポインタクラス
/// - T2{const CiCell*, T1} の形のコンストラクタが定義されていると仮定する．
//////////////////////////////////////////////////////////////////////
template<class T1, class T2>
class ClibList2 :
  private ClibCellPtr
{
public:

  using impl_iter = typename std::vector<const T1*>::const_iterator;
  using iterator = ClibList2Iter<T1, T2>;

public:

  /// @brief 空のコンストラクタ
  ClibList2() = default;

  /// @brief コンストラクタ
  ClibList2(
    const CiCell* cell, ///< [in] 親のセル
    impl_iter begin,    ///< [in] リストの先頭
    impl_iter end       ///< [in] リストの末尾
  ) : ClibCellPtr{cell},
      mList{begin, end}
  {
  }

  /// @brief コンストラクタ
  ClibList2(
    const CiCell* cell,                     ///< [in] 親のセル
    const std::vector<const T1*>& impl_list ///< [in] 実体のリスト
  ) : ClibCellPtr{cell},
      mList{impl_list}
  {
  }

  /// @brief デストラクタ
  ~ClibList2() = default;


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
    return T2{ClibCellPtr::_impl(), ptr};
  }

  /// @brief 先頭の反復子を返す．
  iterator
  begin() const
  {
    return iterator{ClibCellPtr::_impl(), mList.begin()};
  }

  /// @brief 末尾の反復子を返す．
  iterator
  end() const
  {
    return iterator{ClibCellPtr::_impl(), mList.end()};
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

class CiPin;
using ClibPinList = ClibList2<CiPin, ClibPin>;
class CiBus;
using ClibBusList = ClibList2<CiBus, ClibBus>;
class CiBundle;
using ClibBundleList = ClibList2<CiBundle, ClibBundle>;
class CiTiming;
using ClibTimingList = ClibList2<CiTiming, ClibTiming>;

END_NAMESPACE_YM_CLIB

BEGIN_NAMESPACE_YM

using nsClib::ClibPinList;
using nsClib::ClibBusList;
using nsClib::ClibBundleList;
using nsClib::ClibTimingList;

END_NAMESPACE_YM

#endif // CLIBLIST2_H
