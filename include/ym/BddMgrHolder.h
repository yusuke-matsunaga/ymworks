#ifndef BDDMGRHOLDER_H
#define BDDMGRHOLDER_H

/// @file BddMgrHolder.h
/// @brief BddMgrHolder のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/dd.h"
#include "ym/BinDec.h"
#include "ym/BinEnc.h"
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM_DD

class BddMgrImpl;
class DdEdge;
class DdInfoMgr;

//////////////////////////////////////////////////////////////////////
/// @class BddMgrHolder BddMgrHolder.h "ym/BddMgrHolder.h"
/// @brief BddMgrImpl のスマートポインタ
///
/// 中身は std::shared_ptr<BddMgrImpl> だが意味的には BddMgrImpl
/// の delegate クラスとなっている．
//////////////////////////////////////////////////////////////////////
class BddMgrHolder
{
public:

  /// @brief 生のポインタを指定したコンストラクタ
  explicit
  BddMgrHolder(
    BddMgrImpl* ptr = nullptr ///< [in] マネージャのポインタ
  );

  /// @brief コピーコンストラクタ
  BddMgrHolder(
    const BddMgrHolder& src ///< [in] コピー元のオブジェクト
  );

  /// @brief コピー代入演算子
  BddMgrHolder&
  operator=(
    const BddMgrHolder& src ///< [in] コピー元のオブジェクト
  );

  /// @brief デストラクタ
  ~BddMgrHolder();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のマネージャを返す．
  BddMgr
  mgr() const;

  /// @brief 同じマネージャのオブジェクトなら true を返す．
  static
  bool
  has_same_mgr(
    const BddMgrHolder& left,
    const BddMgrHolder& right
  )
  {
    return left.mPtr == right.mPtr;
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスで用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 適正な値を持っている時に true を返す．
  bool
  _has_valid_mgr() const
  {
    return mPtr != nullptr;
  }

  /// @brief BddMgrImpl のポインタを取り出す(チェックなし)．
  ///
  /// nullptr の場合もある．
  BddMgrImpl*
  _get() const
  {
    return mPtr.get();
  }

  /// @brief BddMgrImpl のポインタを取り出す(チェックあり)．
  ///
  /// nullptr の場合は例外を送出する．
  BddMgrImpl*
  get() const
  {
    if ( !_has_valid_mgr() ) {
      throw std::invalid_argument{"mPtr is nullptr"};
    }
    return mPtr.get();
  }

  /// @brief get() の別名
  BddMgrImpl*
  operator->() const
  {
    return get();
  }

  /// @brief DdEdge を Bdd に変換する．
  Bdd
  _bdd(
    DdEdge edge
  ) const;

  /// @brief DdEdge を BddVar に変換する．
  BddVar
  _var(
    DdEdge edge
  ) const;

  /// @brief レベルを BddVar に変換する．
  BddVar
  _var(
    SizeType level ///< [in] レベル
  ) const;

  /// @brief DdEdge を BddLit に変換する．
  BddLit
  _lit(
    DdEdge edge
  ) const;

  /// @brief DdEdge を BddVarSet に変換する．
  BddVarSet
  _varset(
    DdEdge edge
  ) const;

  /// @brief DdEdge を BddCube に変換する．
  BddCube
  _cube(
    DdEdge edge
  ) const;

  /// @brief BddVar のレベルを返す．
  static
  SizeType
  _var2level(
    const BddVar& var
  );

  /// @brief BddVar のリストから DdEdge のリストに変換する．
  static
  std::vector<DdEdge>
  _var2edge_list(
    const std::vector<BddVar>& var_list
  );

  /// @brief BddVar のリストからレベルのリストに変換する．
  static
  std::vector<SizeType>
  _var2level_list(
    const std::vector<BddVar>& var_list
  );

  /// @brief 適正な値を持っているかチェックする．
  void
  _check_valid_mgr() const
  {
    if ( !_has_valid_mgr() ) {
      throw std::invalid_argument{"no BddMgr"};
    }
  }

  /// @brief 同じマネージャの要素かチェックする．
  ///
  /// 異なる場合には std::invalid_argument 例外を送出する．
  static
  void
  _check_mgr(
    const BddMgrHolder& left,
    const BddMgrHolder& right
  )
  {
    left._check_valid_mgr();
    right._check_valid_mgr();
    if ( !has_same_mgr(left, right) ) {
      throw std::invalid_argument{"BddMgr mismatch"};
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャの本体
  std::shared_ptr<BddMgrImpl> mPtr;

};

END_NAMESPACE_YM_DD

#endif // BDDMGRHOLDER_H
