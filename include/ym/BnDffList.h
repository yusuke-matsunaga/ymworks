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
#include "ym/BnListBase.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
/// @class BnDffIter BnDffList.h "ym/BnDffList.h"
/// @brief BnDffList の反復子
//////////////////////////////////////////////////////////////////////
class BnDffIter :
  public BnIterBase
{
public:

  /// @brief 空のコンストラクタ
  BnDffIter() = default;

  /// @brief 値を指定したコンストラクタ
  explicit
  BnDffIter(
    const std::shared_ptr<ModelImpl>& model,
    IdIter iter
  ) : BnIterBase(model, iter)
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
    return BnBase::_id2dff(get_id());
  }

};


//////////////////////////////////////////////////////////////////////
/// @class BnDffIter2 BnDffList.h "ym/BnDffList.h"
/// @brief BnDffList の反復子2(Python用)
//////////////////////////////////////////////////////////////////////
class BnDffIter2 :
  public BnIterBase2
{
public:

  /// @brief 空のコンストラクタ
  BnDffIter2() = default;

  /// @brief 値を指定したコンストラクタ
  explicit
  BnDffIter2(
    const std::shared_ptr<ModelImpl>& model,
    IdIter iter,
    IdIter end
  ) : BnIterBase2(model, iter, end)
  {
  }

  /// @brief デストラクタ
  ~BnDffIter2() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を返す．
  ///
  /// has_next() == true と仮定している．
  BnDff
  next()
  {
    auto id = get_id();
    _next();
    return BnBase::_id2dff(id);
  }

};


//////////////////////////////////////////////////////////////////////
/// @class BnDffList BnDffList.h "BnDffList.h"
/// @brief BnDff のリスト
//////////////////////////////////////////////////////////////////////
class BnDffList :
  public BnListBase
{
public:

  using iterator = BnDffIter;
  using iterator2 = BnDffIter2;

public:

  /// @brief 空のコンストラクタ
  BnDffList() = default;

  /// @brief 値を指定したコンストラクタ
  explicit
  BnDffList(
    const std::shared_ptr<ModelImpl>& model, ///< [in] 親のモデル
    const IdList& id_list                    ///< [in] ID番号のリスト
    = {}
  ) : BnListBase(model, id_list)
  {
  }

  /// @brief 値を指定したコンストラクタ
  explicit
  BnDffList(
    const std::vector<BnDff>& dff_list ///< [in] ノードのリスト
  )
  {
    reserve(dff_list.size());
    for ( auto& dff: dff_list ) {
      push_back(dff);
    }
  }

  /// @brief デストラクタ
  ~BnDffList() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を返す．
  BnDff
  operator[](
    SizeType index ///< [in] インデックス ( 0 <= index < size() )
  ) const
  {
    return BnBase::_id2dff(get_id(index));
  }

  /// @brief 先頭の反復子を返す．
  iterator
  begin() const
  {
    return iterator(_model(), id_list().begin());
  }

  /// @brief 末尾の反復子を返す．
  iterator
  end() const
  {
    return iterator(_model(), id_list().end());
  }

  /// @brief Python 用の反復子を返す．
  iterator2
  iter() const
  {
    return iterator2(_model(), id_list().begin(), id_list().end());
  }

  /// @brief 要素を末尾に追加する．
  void
  push_back(
    const BnDff& dff
  )
  {
    put_id(dff, dff.id());
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const BnDffList& right
  ) const
  {
    return check(right);
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const BnDffList& right
  ) const
  {
    return !operator==(right);
  }

};

END_NAMESPACE_YM_BN

#endif // BNDFFLIST_H
