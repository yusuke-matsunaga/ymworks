#ifndef YM_VL_ASTLIST_H
#define YM_VL_ASTLIST_H

/// @file ym/vl/AstList.h
/// @brief Ast の基底クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class AstListIter AstList.h "ym/vl/AstList.h"
/// @brief AstList 用の反復子
//////////////////////////////////////////////////////////////////////
template <typename T>
class AstListIter
{
public:

  /// @brief 空のコンストラクタ
  AstListIter() = default;

  /// @brief コンストラクタ
  explicit
  AstListIter(
    const T& val
  ) : mVal{val}
  {
  }

  /// @brief デストラクタ
  ~AstListIter() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を取り出す．
  T
  operator*() const
  {
    return mVal;
  }

  /// @brief 次の要素に移動する．
  AstListIter&
  operator++()
  {
    if ( mVal.is_valid() ) {
      mVal = mVal.next();
    }
    return *this;
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const AstListIter& right
  ) const
  {
    return mVal == right.mVal;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const AstListIter& right
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  T mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class AstList AstList.h "ym/vl/AstList.h"
/// @brief AstXXX のリストを表すクラス
///
/// T が is_valid(), link() という関数を持っていると仮定している．
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
    const T& top
  ) : mTop{top}
  {
  }

  /// @brief デストラクタ
  ~AstList() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 空リストの時 true を返す．
  bool
  empty() const
  {
    return mTop.is_invalid();
  }

  /// @brief 要素数を返す．
  ///
  /// O(n) なので多用しないこと．
  SizeType
  size() const
  {
    SizeType n = 0;
    for ( auto x = mTop; x.is_valid(); x = x.next() ) {
      ++n;
    }
    return n;
  }

  /// @brief 先頭の要素を返す．
  T
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
    return const_iterator();
  }

  /// @brief 内容を std::vector<T> に変換する．
  std::vector<T>
  to_vector() const
  {
    auto n = size();
    std::vector<T> vec;
    vec.reserve(n);
    for ( auto x = mTop; x.is_valid(); x = x.next() ) {
      vec.push_back(x);
    }
    return vec;
  }

  /// @brief 内容を JsonValue に変換する．
  ///
  /// T::json_obj() というメンバ関数が実装されていると仮定している．
  JsonValue
  json_obj() const
  {
    auto jobj = JsonValue::array();
    for ( auto x = mTop; x.is_valid(); x = x.next() ) {
      jobj.add(x.json_obj());
    }
    return jobj;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  T mTop;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_ASTBASE_H
