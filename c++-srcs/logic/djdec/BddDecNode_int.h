#ifndef BDDDECNODE_INT_H
#define BDDDECNODE_INT_H

/// @file BddDecNode_int.h
/// @brief BddDecNode_int のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "BddDecNode.h"


BEGIN_NAMESPACE_YM_DJDEC

//////////////////////////////////////////////////////////////////////
/// @class BddDecNode_Base BddDecNode_int.h "BddDecNode_int.h"
/// @brief BddDecNode を実装する基底クラス
//////////////////////////////////////////////////////////////////////
class BddDecNode_Base :
  public BddDecNode
{
public:

  /// @brief コンストラクタ
  BddDecNode_Base(
    BddMgr& mgr,             ///< [in] BDDマネージャ
    SizeType id,             ///< [in] ID番号
    const Bdd& f,            ///< [in] グローバル関数
    const BddVarSet& support ///< [in] サポート
  );

  /// @brief デストラクタ
  ~BddDecNode_Base() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  SizeType
  id() const override;

  /// @brief LITタイプの時に true を返す．
  bool
  is_lit() const override;

  /// @brief ORタイプの時に true を返す．
  bool
  is_or() const override;

  /// @brief XORタイプの時に true を返す．
  bool
  is_xor() const override;

  /// @brief CPLXタイプの時に true を返す．
  bool
  is_cplx() const override;

  /// @brief グローバル関数を返す．
  Bdd
  global_func() const override;

  /// @brief 先頭の変数を返す．
  BddVar
  top() const override;

  /// @brief サポートを返す．
  const BddVarSet&
  support() const override;

  /// @brief グローバル関数が1となるパタンを得る．
  BddCube
  pat_1() const override;

  /// @brief グローバル関数が0となるパタンを得る．
  BddCube
  pat_0() const override;

  /// @brief subfunction の数を得る．
  SizeType
  child_num() const override;

  /// @brief subfunction を表す枝を得る．
  BddDecEdge
  child(
    SizeType pos ///< [in] 位置 ( 0 <= pos < child_num() )
  ) const override;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスで用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief BDDマネージャを返す．
  BddMgr&
  mgr() const
  {
    return mBddMgr;
  }

  /// @brief print の共通部分
  void
  print_base(
    std::ostream& s,        ///< [in] 出力ストリーム
    const std::string& type ///< [in] 種類を表す文字列
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BDDマネージャ
  // 本体は BddDecMgr が持っている．
  BddMgr& mBddMgr;

  // ID番号
  SizeType mId;

  // グローバル関数
  Bdd mGlobalFunc;

  // サポート
  BddVarSet mSupport;

  // 出力が1となる入力パタン
  BddCube mPat1;

  // 出力が0となる入力パタン
  BddCube mPat0;

};


//////////////////////////////////////////////////////////////////////
/// @class BddDecNode_Lit BddDecNode_int.h "BddDecNode_int.h"
/// @brief LITタイプの BddDecNode
//////////////////////////////////////////////////////////////////////
class BddDecNode_Lit :
  public BddDecNode_Base
{
public:

  /// @brief コンストラクタ
  BddDecNode_Lit(
    BddMgr& mgr,             ///< [in] BDDマネージャ
    SizeType id,             ///< [in] ID番号
    const Bdd& f,            ///< [in] グローバル関数
    const BddVarSet& support ///< [in] サポート
  ) : BddDecNode_Base(mgr, id, f, support)
  {
  }

  /// @brief デストラクタ
  ~BddDecNode_Lit() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief LITタイプの時に true を返す．
  bool
  is_lit() const override;

  /// @brief ローカル関数を求める．
  Bdd
  local_func() const override;

  /// @brief 内容を出力する．
  void
  print(
    std::ostream& s ///< [in] 出力ストリーム
  ) const override;

};


//////////////////////////////////////////////////////////////////////
/// @class BddDecNode_Mid BddDecNode_int.h "BddDecNode_int.h"
/// @brief 子ノードを持つ共通の親クラス
//////////////////////////////////////////////////////////////////////
class BddDecNode_Mid :
  public BddDecNode_Base
{
public:

  /// @brief コンストラクタ
  BddDecNode_Mid(
    BddMgr& mgr,                          ///< [in] BDDマネージャ
    SizeType id,                          ///< [in] ID番号
    const Bdd& f,                         ///< [in] グローバル関数
    const BddVarSet& support,             ///< [in] サポート
    const std::vector<BddDecEdge>& child_list ///< [in] 子ノードの枝のリスト
  ) : BddDecNode_Base(mgr, id, f, support),
      mChildList{child_list}
  {
  }

  /// @brief デストラクタ
  ~BddDecNode_Mid() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief subfunction の数を得る．
  SizeType
  child_num() const override;

  /// @brief subfunction を表す枝を得る．
  BddDecEdge
  child(
    SizeType pos ///< [in] 位置 ( 0 <= pos < child_num() )
  ) const override;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief pint() の下請け処理を行う．
  void
  print_sub(
    std::ostream& s,        ///< [in] 出力ストリーム
    const std::string& type ///< [in] 種類を表す文字列
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 子ノードの枝のリスト
  std::vector<BddDecEdge> mChildList;

};


//////////////////////////////////////////////////////////////////////
/// @class BddDecNode_Or BddDecNode_int.h "BddDecNode_int.h"
/// @brief ORノードを表すクラス
//////////////////////////////////////////////////////////////////////
class BddDecNode_Or :
  public BddDecNode_Mid
{
public:

  /// @brief コンストラクタ
  BddDecNode_Or(
    BddMgr& mgr,                          ///< [in] BDDマネージャ
    SizeType id,                          ///< [in] ID番号
    const Bdd& f,                         ///< [in] グローバル関数
    const BddVarSet& support,             ///< [in] サポートリスト
    const std::vector<BddDecEdge>& child_list ///< [in] 子ノードの枝のリスト
  ) : BddDecNode_Mid(mgr, id, f, support, child_list)
  {
  }

  /// @brief デストラクタ
  ~BddDecNode_Or() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ORタイプの時に true を返す．
  bool
  is_or() const override;

  /// @brief ローカル関数を求める．
  Bdd
  local_func() const override;

  /// @brief 内容を出力する．
  void
  print(
    std::ostream& s ///< [in] 出力ストリーム
  ) const override;

};


//////////////////////////////////////////////////////////////////////
/// @class BddDecNode_Xor BddDecNode_int.h "BddDecNode_int.h"
/// @brief XORノードを表すクラス
//////////////////////////////////////////////////////////////////////
class BddDecNode_Xor :
  public BddDecNode_Mid
{
public:

  /// @brief コンストラクタ
  BddDecNode_Xor(
    BddMgr& mgr,                          ///< [in] BDDマネージャ
    SizeType id,                          ///< [in] ID番号
    const Bdd& f,                         ///< [in] グローバル関数
    const BddVarSet& support,             ///< [in] サポートリスト
    const std::vector<BddDecEdge>& child_list ///< [in] 子ノードの枝のリスト
  ) : BddDecNode_Mid(mgr, id, f, support, child_list)
  {
  }

  /// @brief デストラクタ
  ~BddDecNode_Xor() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief XORタイプの時に true を返す．
  bool
  is_xor() const override;

  /// @brief ローカル関数を求める．
  Bdd
  local_func() const override;

  /// @brief 内容を出力する．
  void
  print(
    std::ostream& s ///< [in] 出力ストリーム
  ) const override;

};


//////////////////////////////////////////////////////////////////////
/// @class BddDecNode_Cplx BddDecNode_int.h "BddDecNode_int.h"
/// @brief CPLXノードを表すクラス
//////////////////////////////////////////////////////////////////////
class BddDecNode_Cplx :
  public BddDecNode_Mid
{
public:

  /// @brief コンストラクタ
  BddDecNode_Cplx(
    BddMgr& mgr,                          ///< [in] BDDマネージャ
    SizeType id,                          ///< [in] ID番号
    const Bdd& f,                         ///< [in] グローバル関数
    const BddVarSet& support,             ///< [in] サポートリスト
    const std::vector<BddDecEdge>& child_list ///< [in] 子ノードの枝のリスト
  ) : BddDecNode_Mid(mgr, id, f, support, child_list)
  {
  }

  /// @brief デストラクタ
  ~BddDecNode_Cplx() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief CPLXタイプの時に true を返す．
  bool
  is_cplx() const override;

  /// @brief ローカル関数を求める．
  Bdd
  local_func() const override;

  /// @brief 内容を出力する．
  void
  print(
    std::ostream& s ///< [in] 出力ストリーム
  ) const override;

};

END_NAMESPACE_YM_DJDEC

#endif // BDDDECNODE_INT_H
