#ifndef BDDVARSET_H
#define BDDVARSET_H

/// @file BddVarSet.h
/// @brief BddVarSet のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BddBase.h"
#include "ym/BddVar.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class BddVarSet BddVarSet.h "BddVarSet.h"
/// @ingroup DdGroup
/// @brief Bdd を用いて変数集合を表すクラス
///
/// - 実際には変数を表す関数の論理積を用いている．
/// - 空集合を表す場合には定数1を用いる．
/// - 定数0は用いない．
//////////////////////////////////////////////////////////////////////
class BddVarSet :
  public BddBase
{
  friend class BddMgrHolder;

public:

  /// @brief 要素のリストを指定したコンストラクタ
  /// @sa BddVar
  ///
  /// - var_set を省略した場合には空集合となる．
  /// - var_set の要素に不正な BddVar が含まれている場合，
  ///   std::invalid_argument が送出される．
  /// - var_set の要素に相異なる BddMgr の BddVar が含まれている場合，
  ///   std::invalid_argument が送出される．
  explicit
  BddVarSet(
    const std::vector<BddVar>& var_set ///< [in] 変数のリスト
    = std::vector<BddVar>{}
  );

  /// @brief BddVar からの変換コンストラクタ
  /// @sa BddVar
  ///
  /// var のみを要素とする singleton を作る．
  explicit
  BddVarSet(
    const BddVar& var
  );

  /// @brief デストラクタ
  ~BddVarSet() = default;

  /// @brief 空集合を返す．
  static
  BddVarSet
  empty_set();

  /// @brief 不正値を返す．
  static
  BddVarSet
  invalid();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 空集合の時 true を返す．
  bool
  is_empty_set() const;

  /// @brief 変数の数を返す．
  SizeType
  size() const;

  /// @brief 変数のリストに変換する．
  /// @sa BddVar
  std::vector<BddVar>
  var_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 演算
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数を追加する．
  /// @sa BddVar
  BddVarSet
  operator+(
    const BddVar& right ///< [in] 変数
  ) const
  {
    return _varset_cup(*this, right);
  }

  /// @brief 和集合を求める．
  BddVarSet
  operator+(
    const BddVarSet& right ///< [in] オペランド
  ) const
  {
    return _varset_cup(*this, right);
  }

  /// @brief 変数を自身に追加する．
  /// @sa BddVar
  BddVarSet&
  operator+=(
    const BddVar& right ///< [in] 変数
  )
  {
    *this = _varset_cup(*this, right);
    return *this;
  }

  /// @brief 和集合を求め代入する．
  BddVarSet&
  operator+=(
    const BddVarSet& right ///< [in] オペランド
  )
  {
    *this = _varset_cup(*this, right);
    return *this;
  }

  /// @brief 変数を取り除く
  /// @sa BddVar
  BddVarSet
  operator-(
    const BddVar& right ///< [in] 変数
  ) const
  {
    return _varset_diff(*this, right);
  }

  /// @brief 変数を自身の集合から取り除く
  /// @sa BddVar
  BddVarSet&
  operator-=(
    const BddVar& right ///< [in] 変数
  )
  {
    *this = _varset_diff(*this, right);
    return *this;
  }

  /// @brief 差集合を求める．
  BddVarSet
  operator-(
    const BddVarSet& right ///< [in] オペランド
  ) const
  {
    return _varset_diff(*this, right);
  }

  /// @brief 差集合を求め代入する．
  BddVarSet&
  operator-=(
    const BddVarSet& right ///< [in] オペランド
  )
  {
    *this = _varset_diff(*this, right);
    return *this;
  }

  /// @brief 共通集合を求める．
  /// @sa BddVar
  ///
  /// 結果は空集合が singleton になる．
  BddVarSet
  operator&(
    const BddVar& right ///< [in] オペランド
  ) const
  {
    return _varset_cap(*this, right);
  }

  /// @brief 共通集合を求め代入する．
  /// @sa BddVar
  ///
  /// 結果は空集合が singleton になる．
  BddVarSet&
  operator&=(
    const BddVar& right ///< [in] オペランド
  )
  {
    *this = _varset_cap(*this, right);
    return *this;
  }

  /// @brief 共通集合を求める．
  BddVarSet
  operator&(
    const BddVarSet& right ///< [in] オペランド
  ) const
  {
    return _varset_cap(*this, right);
  }

  /// @brief 共通集合を求め代入する．
  BddVarSet&
  operator&=(
    const BddVarSet& right ///< [in] オペランド
  )
  {
    *this = _varset_cap(*this, right);
    return *this;
  }

  /// @brief 変数を含むかチェックする．
  /// @sa BddVar
  bool
  operator&&(
    const BddVar& right ///< [in] 変数
  ) const
  {
    return _varset_check(*this, right);
  }

  /// @brief 共通部分を持つかチェックする．
  bool
  operator&&(
    const BddVarSet& right ///< [in] オペランド
  ) const
  {
    return _varset_check(*this, right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // BddMgrHolder から用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  BddVarSet(
    const BddMgrHolder& holder,
    DdEdge root
  );

};


/// @brief ストリーム出力演算子
std::ostream&
operator<<(
  std::ostream& s,
  const BddVarSet& varset
);

END_NAMESPACE_YM_DD

BEGIN_NAMESPACE_STD

// BddVarSet をキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::BddVarSet>
{
  SizeType
  operator()(
    const YM_NAMESPACE::BddVarSet& varset
  ) const
  {
    return varset.hash();
  }
};

END_NAMESPACE_STD

#endif // BDDVARSET_H
