#ifndef YM_VL_ASTARRAY_H
#define YM_VL_ASTARRAY_H

/// @file ym/vl/AstArray.h
/// @brief AstArray のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class AstArrayIter AstArray.h "ym/vl/AstArray.h"
/// @brief AstArray の反復子
//////////////////////////////////////////////////////////////////////
template <typename T>
class AstArrayIter
{
public:

  /// @brief コンストラクタ
  AstArrayIter(
    T* ptr
  ) : mPtr{ptr}
  {
  }

  /// @brief デストラクタ
  ~AstArrayIter() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief dereference 演算子
  T
  operator*() const
  {
    return *mPtr;
  }

  /// @brief 次の要素に進む．
  AstArrayIter&
  operator++()
  {
    ++ mPtr;
    return *this;
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const AstArrayIter& right
  ) const
  {
    return mPtr == right.mPtr;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const AstArrayIter& right
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 位置
  T* mPtr;

};


//////////////////////////////////////////////////////////////////////
/// @class AstArray AstArray.h "AstArray.h"
/// @brief Ast系の配列を表すクラス
///
/// 単純なポインタ配列だがメモリ管理を Alloc クラスが行う．
//////////////////////////////////////////////////////////////////////
template <typename T>
class AstArray
{
public:

  /// @brief 反復子の型
  using iterator = AstArrayIter<T>;

public:

  /// @brief 空のコンストラクタ
  AstArray() = default;

  /// @brief 内容を指定したコンストラクタ
  AstArray(
    SizeType size,
    T* array
  ) : mSize{size},
      mArray{array}
  {
  }

  /// @brief コピーコンストラクタは禁止
  AstArray(
    const AstArray& src
  ) = delete;

  /// @brief ムーブコンストラクタ
  AstArray(
    AstArray&& src
  ) : mSize{src.mSize},
      mArray{src.mArray}
  {
    src.mSize = 0;
    src.mArray = nullptr;
  }

  /// @brief コピー代入演算子は禁止
  AstArray&
  operator=(
    const AstArray& src
  ) = delete;

  /// @brief ムーブ代入演算子
  AstArray&
  operator=(
    AstArray&& src
  )
  {
    mSize = src.mSize;
    mArray = src.mArray;
    src.mSize = 0;
    src.mArray = nullptr;
    return *this;
  }

  /// @brief デストラクタ
  ~AstArray() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数を返す．
  SizeType
  size() const
  {
    return mSize;
  }

  /// @brief 内容にアクセスする．
  const T&
  operator[](
    SizeType index ///< [in] 位置 ( 0 <= index < size() )
  ) const
  {
    if ( index >= size() ) {
      throw std::out_of_range{"index is out of range"};
    }
    return mArray[index];
  }

  /// @brief 先頭の反復子を返す．
  iterator
  begin() const
  {
    return mArray;
  }

  /// @brief 末尾の反復子を返す．
  iterator
  end() const
  {
    return mArray + size();
  }

  /// @brief std::vector<T> に変換する．
  std::vector<T>
  to_vector() const
  {
    std::vector<T> vec;
    vec.reserve(mSize);
    for ( SizeType i = 0; i < mSize; ++ i ) {
      auto v = mArray[i];
      vec.push_back(v);
    }
    return vec;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  SizeType mSize{0};

  // 配列本体
  T* mArray{nullptr};

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_ASTARRAY_H
