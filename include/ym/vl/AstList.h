#ifndef YM_VL_ASTLIST_H
#define YM_VL_ASTLIST_H

/// @file ym/vl/AstList.h
/// @brief Ast の基底クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class AstListIter AstList.h "ym/vl/AstList.h"
/// @brief AstList 用の反復子
//////////////////////////////////////////////////////////////////////
template <typename T>
class AstListIter
{
public:

  /// @brief コンストラクタ
  AstListIter(
    T* ptr = nullptr
  ) : mPtr{ptr}
  {
  }

  /// @brief デストラクタ
  ~AstListIter() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を取り出す．
  T*
  operator*() const
  {
    return mPtr;
  }

  /// @brief 次の要素に移動する．
  AstListIter&
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
    const AstListIter& right
  ) const
  {
    return mPtr == right.mPtr;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const AstListIter& right
  ) const
  {
    return mPtr != right.mPtr;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素のポインタ
  T* mPtr;

};


//////////////////////////////////////////////////////////////////////
/// @class AstList AstList.h "ym/vl/AstList.h"
/// @brief AstXXX のリストを表すクラス
///
/// AstXXX が link() という関数を持っていると仮定している．
//////////////////////////////////////////////////////////////////////
template <typename T>
class AstList
{
public:

  /// @brief 反復子
  using const_iterator = AstListIter<T>;

public:

  /// @brief 空のコンストラクタ
  AstList() = default;

  /// @brief 先頭の要素を指定したコンストラクタ
  AstList(
    const T* top
  ) : mTop{top}
  {
  }

  /// @brief デストラクタ
  ~AstList() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数を返す．
  ///
  /// O(n) なので多用しないこと．
  SizeType
  size() const
  {
    SizeType n = 0;
    for ( auto x = mTop; x != nullptr; x = x->link() ) {
      ++n;
    }
    return n;
  }

  /// @brief 先頭の要素を返す．
  T*
  front() const
  {
    return mTop;
  }

  /// @brief 先頭の反復子を返す．
  const_iterator
  begin() const
  {
    return const_iterator(mTop);
  }

  /// @brief 末尾の反復子を返す．
  const_iterator
  end() const
  {
    return const_iterator(nullptr);
  }

  /// @brief 内容を std::vector<T*> に変換する．
  std::vector<T*>
  to_vector() const
  {
    auto n = size();
    std::vector<T*> vec;
    vec.reserve(n);
    for ( auto x = mTop; x != nullptr; x = x->link() ) {
      vec.push_back(x);
    }
    return vec;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  T* mTop;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_ASTBASE_H
