#ifndef BDDDECNODE_H
#define BDDDECNODE_H

/// @file BddDecNode.h
/// @brief BddDecNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/DjDec.h"
#include "ym/Bdd.h"
#include "ym/BddCube.h"
#include "ym/BddVarSet.h"


BEGIN_NAMESPACE_YM_DJDEC

class BddDecEdge;

//////////////////////////////////////////////////////////////////////
/// @class BddDecNode BddDecNode.h "ym/BddDecNode.h"
/// @brief Disjoint Graph のノードを表すクラス
///
/// このクラスはインターフェイス定義のみの純粋仮想基底クラス
//////////////////////////////////////////////////////////////////////
class BddDecNode
{
public:

  /// @brief デストラクタ
  virtual
  ~BddDecNode() = default;

  /// @brief LITタイプのオブジェクトを生成する．
  static
  BddDecNode*
  new_lit(
    BddMgr& mgr,             ///< [in] BDDマネージャ
    SizeType id,             ///< [in] ID番号
    const Bdd& f,            ///< [in] グローバル関数
    const BddVarSet& support ///< [in] サポート
  );

  /// @brief ORタイプのオブジェクトを生成する．
  static
  BddDecNode*
  new_or(
    BddMgr& mgr,                              ///< [in] BDDマネージャ
    SizeType id,                              ///< [in] ID番号
    const Bdd& f,                             ///< [in] グローバル関数
    const BddVarSet& support,                 ///< [in] サポートリスト
    const std::vector<BddDecEdge>& child_list ///< [in] 子ノードの枝のリスト
  );

  /// @brief XORタイプのオブジェクトを生成する．
  static
  BddDecNode*
  new_xor(
    BddMgr& mgr,                              ///< [in] BDDマネージャ
    SizeType id,                              ///< [in] ID番号
    const Bdd& f,                             ///< [in] グローバル関数
    const BddVarSet& support,                 ///< [in] サポートリスト
    const std::vector<BddDecEdge>& child_list ///< [in] 子ノードの枝のリスト
  );

  /// @brief CPLXタイプのオブジェクトを生成する．
  static
  BddDecNode*
  new_cplx(
    BddMgr& mgr,                              ///< [in] BDDマネージャ
    SizeType id,                              ///< [in] ID番号
    const Bdd& f,                             ///< [in] グローバル関数
    const BddVarSet& support,                 ///< [in] サポートリスト
    const std::vector<BddDecEdge>& child_list ///< [in] 子ノードの枝のリスト
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  virtual
  SizeType
  id() const = 0;

  /// @brief LITタイプの時に true を返す．
  virtual
  bool
  is_lit() const = 0;

  /// @brief ORタイプの時に true を返す．
  virtual
  bool
  is_or() const = 0;

  /// @brief XORタイプの時に true を返す．
  virtual
  bool
  is_xor() const = 0;

  /// @brief CPLXタイプの時に true を返す．
  virtual
  bool
  is_cplx() const = 0;

  /// @brief グローバル関数を返す．
  virtual
  Bdd
  global_func() const = 0;

  /// @brief 先頭の変数を返す．
  virtual
  BddVar
  top() const = 0;

  /// @brief サポートを返す．
  virtual
  const BddVarSet&
  support() const = 0;

  /// @brief グローバル関数が1となるパタンを得る．
  virtual
  BddCube
  pat_1() const = 0;

  /// @brief グローバル関数が0となるパタンを得る．
  virtual
  BddCube
  pat_0() const = 0;

  /// @brief ローカル関数を求める．
  virtual
  Bdd
  local_func() const = 0;

  /// @brief subfunction の数を得る．
  virtual
  SizeType
  child_num() const = 0;

  /// @brief subfunction を表す枝を得る．
  virtual
  BddDecEdge
  child(
    SizeType pos ///< [in] 位置 ( 0 <= pos < child_num() )
  ) const = 0;

  /// @brief 内容を出力する．
  virtual
  void
  print(
    std::ostream& s ///< [in] 出力ストリーム
  ) const = 0;

};

END_NAMESPACE_YM_DJDEC

#endif // BDDDECNODE_H
