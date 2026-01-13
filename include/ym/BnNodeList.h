#ifndef BNNODELIST_H
#define BNNODELIST_H

/// @file BnNodeList.h
/// @brief BnNodeList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/BnNode.h"
#include "ym/BnListBase.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
/// @class BnNodeIter BnNodeList.h "ym/BnNodeList.h"
/// @brief BnNodeList の反復子
//////////////////////////////////////////////////////////////////////
class BnNodeIter :
  public BnIterBase
{
public:

  /// @brief 空のコンストラクタ
  BnNodeIter() = default;

  /// @brief 値を指定したコンストラクタ
  explicit
  BnNodeIter(
    const std::shared_ptr<ModelImpl>& model,
    IdIter iter
  ) : BnIterBase(model, iter)
  {
  }

  /// @brief デストラクタ
  ~BnNodeIter() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief dereference 演算子
  BnNode
  operator*() const
  {
    return BnBase::_id2node(get_id());
  }

};


//////////////////////////////////////////////////////////////////////
/// @class BnNodeIter2 BnNodeList.h "ym/BnNodeList.h"
/// @brief BnNodeList の反復子2(Python用)
//////////////////////////////////////////////////////////////////////
class BnNodeIter2 :
  public BnIterBase2
{
public:

  /// @brief 空のコンストラクタ
  BnNodeIter2() = default;

  /// @brief 値を指定したコンストラクタ
  explicit
  BnNodeIter2(
    const std::shared_ptr<ModelImpl>& model,
    IdIter iter,
    IdIter end
  ) : BnIterBase2(model, iter, end)
  {
  }

  /// @brief デストラクタ
  ~BnNodeIter2() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を返す．
  ///
  /// has_next() == true と仮定している．
  BnNode
  next()
  {
    auto id = get_id();
    _next();
    return BnBase::_id2node(id);
  }

};


//////////////////////////////////////////////////////////////////////
/// @class BnNodeList BnNodeList.h "BnNodeList.h"
/// @brief BnNode のリスト
//////////////////////////////////////////////////////////////////////
class BnNodeList :
  public BnListBase
{
public:

  using iterator = BnNodeIter;
  using iterator2 = BnNodeIter2;

public:

  /// @brief 空のコンストラクタ
  BnNodeList() = default;

  /// @brief 値を指定したコンストラクタ
  explicit
  BnNodeList(
    const std::shared_ptr<ModelImpl>& model, ///< [in] 親のモデル
    const IdList& id_list                    ///< [in] ID番号のリスト
    = {}
  ) : BnListBase(model, id_list)
  {
  }

  /// @brief 値を指定したコンストラクタ
  explicit
  BnNodeList(
    const std::vector<BnNode>& node_list ///< [in] ノードのリスト
  )
  {
    reserve(node_list.size());
    for ( auto& node: node_list ) {
      push_back(node);
    }
  }

  /// @brief デストラクタ
  ~BnNodeList() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を返す．
  BnNode
  operator[](
    SizeType index ///< [in] インデックス ( 0 <= index < size() )
  ) const
  {
    return BnBase::_id2node(get_id(index));
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
    const BnNode& node
  )
  {
    put_id(node, node.id());
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const BnNodeList& right
  ) const
  {
    return check(right);
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const BnNodeList& right
  ) const
  {
    return !operator==(right);
  }

};

END_NAMESPACE_YM_BN

#endif // BNNODELIST_H
