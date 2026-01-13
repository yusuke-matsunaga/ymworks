#ifndef BNLISTBASE_H
#define BNLISTBASE_H

/// @file BnListBase.h
/// @brief BnListBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/BnBase.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
/// @class BnIterBase BnListBase.h "ym/BnListBase.h"
/// @brief BnListBase の反復子
//////////////////////////////////////////////////////////////////////
class BnIterBase :
  public BnBase
{
public:

  using IdIter = std::vector<SizeType>::const_iterator;

public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  BnIterBase() = default;

  /// @brief 値を指定したコンストラクタ
  explicit
  BnIterBase(
    const std::shared_ptr<ModelImpl>& model,
    IdIter iter
  ) : BnBase(model),
      mIdIter{iter}
  {
  }

  /// @brief デストラクタ
  ~BnIterBase() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素に進む．
  void
  operator++()
  {
    ++ mIdIter;
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const BnIterBase& right
  ) const
  {
    return BnBase::operator==(right) && mIdIter == right.mIdIter;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const BnIterBase& right
  ) const
  {
    return !operator==(right);
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスで用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ID の値を得る．
  SizeType
  get_id() const
  {
    return *mIdIter;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // mIdList の反復子
  IdIter mIdIter;

};


//////////////////////////////////////////////////////////////////////
/// @class BnIterBase2 BnListBase.h "ym/BnListBase.h"
/// @brief BnListBase の反復子2(Python用)
//////////////////////////////////////////////////////////////////////
class BnIterBase2 :
  public BnBase
{
public:

  using IdIter = std::vector<SizeType>::const_iterator;

public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  BnIterBase2() = default;

  /// @brief 値を指定したコンストラクタ
  explicit
  BnIterBase2(
    const std::shared_ptr<ModelImpl>& model,
    IdIter iter,
    IdIter end
  ) : BnBase(model),
      mIdIter{iter},
      mEnd{end}
  {
  }

  /// @brief デストラクタ
  ~BnIterBase2() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 有効な値を持っているか調べる．
  bool
  has_next() const
  {
    return mIdIter != mEnd;
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスで用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素に進む．
  void
  _next()
  {
    ++ mIdIter;
  }

  /// @brief ID の値を得る．
  SizeType
  get_id() const
  {
    return *mIdIter;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // mIdList の反復子
  IdIter mIdIter;

  // 末尾の反復子
  IdIter mEnd;

};


//////////////////////////////////////////////////////////////////////
/// @class BnListBase BnListBase.h "BnListBase.h"
/// @brief BnBase のリスト
//////////////////////////////////////////////////////////////////////
class BnListBase :
  public BnBase
{
public:

  using IdList = std::vector<SizeType>;
  using IdIter = IdList::const_iterator;

public:

  /// @brief 空のコンストラクタ
  BnListBase() = default;

  /// @brief 値を指定したコンストラクタ
  explicit
  BnListBase(
    const std::shared_ptr<ModelImpl>& model, ///< [in] 親のモデル．
    const IdList& id_list                    ///< [in] ノード番号のリスト
    = {}
  ) : BnBase(model),
      mIdList{id_list}
  {
  }

  /// @brief 値を指定したコンストラクタ
  explicit
  BnListBase(
    const BnBase& base,   ///< [in] 親のモデル．
    const IdList& id_list ///< [in] ノード番号のリスト
    = {}
  ) : BnBase(base),
      mIdList{id_list}
  {
  }

  /// @brief デストラクタ
  ~BnListBase() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 空の時 true を返す．
  bool
  empty() const
  {
    return mIdList.empty();
  }

  /// @brief 要素数を返す．
  SizeType
  size() const
  {
    return mIdList.size();
  }

  /// @brief ID番号のリストを返す．
  const IdList&
  id_list() const
  {
    return mIdList;
  }

  /// @brief 空リストにする．
  void
  clear()
  {
    mIdList.clear();
  }

  /// @brief 容量を確保する．
  void
  reserve(
    SizeType size
  )
  {
    mIdList.reserve(size);
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスで用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素のID番号を取り出す．
  SizeType
  get_id(
    SizeType index ///< [in] インデックス ( 0 <= index < size() )
  ) const
  {
    if ( index >= size() ) {
      throw std::out_of_range{"index is out of range"};
    }
    return mIdList[index];
  }

  /// @brief IDを末尾に追加する．
  void
  put_id(
    const BnBase& base,
    SizeType id
  )
  {
    if ( is_valid() ) {
      if ( !BnBase::operator==(base) ) {
	throw std::invalid_argument{"model mismatch"};
      }
    }
    else {
      *(static_cast<BnBase*>(this)) = base;
    }
    mIdList.push_back(id);
  }

  /// @brief 等価かどうか調べる．
  bool
  check(
    const BnListBase& right
  ) const
  {
    return _check_impl(right) && mIdList == right.mIdList;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号のリスト
  IdList mIdList;

};

END_NAMESPACE_YM_BN

#endif // BNLISTBASE_H
