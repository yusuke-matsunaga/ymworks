#ifndef BDDDECMGR_H
#define BDDDECMGR_H

/// @file BddDecMgr.h
/// @brief BddDecMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/DjDec.h"
#include "ym/Bdd.h"
#include "ym/BddMgr.h"


BEGIN_NAMESPACE_YM_DJDEC

class BddDecNode;
class BddDecEdge;
class BddDecMark;

//////////////////////////////////////////////////////////////////////
/// @class BddDecMgr BddDecMgr.h "BddDecMgr.h"
/// @brief BddDecMgr の実装クラス
//////////////////////////////////////////////////////////////////////
class BddDecMgr
{
public:

  /// @brief コンストラクタ
  BddDecMgr(
    const Bdd& func ///< [in] 分解を行う関数
  );

  /// @brief デストラクタ
  ~BddDecMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 与えられた関数の BddDecGraph を得る．
  BddDecEdge
  decompose();

  /// @brief BddMgr を得る．
  BddMgr&
  bdd_mgr()
  {
    return mBddMgr;
  }

  /// @brief BDD を返す．
  Bdd
  bdd() const
  {
    return mBdd;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief decomp の下請け関数
  BddDecEdge
  decomp_step(
    const Bdd& func ///< [in] 分解を行う関数
  );

  /// @brief コファクターの結果をマージする．
  BddDecEdge
  merge(
    const BddVar& var, ///< [in] コファクターを行った変数
    BddDecEdge r0,         ///< [in] 0のコファクターの結果
    BddDecEdge r1          ///< [in] 1のコファクターの結果
  );

  /// @brief 共通でないファンインを求める．
  void
  find_uncommon_inputs(
    const std::vector<const BddDecNode*>& node_list, ///< [in] ノードリスト
    BddDecMark& mark,                                ///< [in] ノードマーク
    int mval,                                        ///< [in] マークの値
    std::vector<BddDecEdge>& inputs ///< [out] 境界ノードを格納するリスト
  );

  /// @brief 共通なファンインを求める．
  void
  find_common_inputs(
    const std::vector<const BddDecNode*>& node_list1, ///< [in] ノードリスト1
    const std::vector<const BddDecNode*>& node_list2, ///< [in] ノードリスト2
    BddDecMark& mark,                                 ///< [in] ノードマーク
    std::vector<BddDecEdge>& inputs ///< [out] 境界ノードを格納するリスト
  );

  /// @brief Case1-OR のマージを行う．
  BddDecEdge
  case1_or(
    const BddVar& var,                      ///< [in] 変数
    const std::vector<BddDecEdge>& common_list, ///< [in] 共通の子ノードのリスト
    const std::vector<BddDecEdge>& rest0_list,  ///< [in] node0のみの子ノードのリスト
    const std::vector<BddDecEdge>& rest1_list   ///< [in] node1のみの子ノードのリスト
  );

  /// @brief Case1-XOR のマージを行う．
  BddDecEdge
  case1_xor(
    const BddVar& var,                      ///< [in] 変数
    const std::vector<BddDecEdge>& common_list, ///< [in] 共通の子ノードのリスト
    const std::vector<BddDecEdge>& rest0_list,  ///< [in] node0のみの子ノードのリスト
    const std::vector<BddDecEdge>& rest1_list,  ///< [in] node1のみの子ノードのリスト
    bool inv0,                              ///< [in] node0の反転フラグ
    bool inv1                               ///< [in] node1の反転フラグ
  );

  /// @brief Case1-CPLX のマージを行う．
  BddDecEdge
  case1_cplx(
    const Bdd& f,                               ///< [in] グローバル関数
    const BddVar& var,                          ///< [in] 変数
    const BddDecNode* node0,                    ///< [in] ノード0
    const BddDecNode* node1,                    ///< [in] ノード1
    const std::vector<BddDecEdge>& common_list, ///< [in] 共通の子ノードのリスト
    BddDecEdge rest0,                           ///< [in] node0のみの子ノード
    BddDecEdge rest1                            ///< [in] node1のみの子ノード
  );

  /// @brief CASE1-CPLX(Type 2) のマージを行う．
  BddDecEdge
  case1_cplx2(
    const Bdd& f,                          ///< [in] グローバル関数
    const BddVar& var,                     ///< [in] 変数
    const std::vector<BddDecEdge>& child_list, ///< [in] 子ノードのリスト
    SizeType pos                           ///< [in] マージ対象の子ノードの位置
  );

  /// @brief CASE2-OR のマージを行う．
  BddDecEdge
  case2_or(
    const BddVar& var,       ///< [in] 変数
    bool inv,                ///< [in] インデックスの極性
    const BddDecNode* node0, ///< [in] ORノード
    bool inv0,               ///< [in] node0 の極性
    SizeType pos             ///< [in] 子ノードの位置
  );

  /// @brief CASE2-XOR のマージを行う．
  BddDecEdge
  case2_xor(
    const BddVar& var,       ///< [in] 変数
    bool inv,                ///< [in] インデックスの極性
    const BddDecNode* node0, ///< [in] XORノード
    bool inv0,               ///< [in] node0 の極性
    SizeType pos,            ///< [in] 子ノードの位置
    bool inv1                ///< [in] 他方の枝の極性
  );

  /// @brief CASE2-CPLX のマージを行う．
  BddDecEdge
  case2_cplx(
    const Bdd& f,      ///< [in] グローバル関数
    const BddVar& var, ///< [in] 変数
    BddDecEdge cedge,  ///< [in] マージ対象の枝
    bool inv1,
    bool inv2,
    const BddDecNode* node
  );

  /// @brief LITノードを作る．
  BddDecEdge
  make_lit(
    const BddVar& var, ///< [in] 変数
    bool inv = false   ///< [in] 反転フラグ
  );

  /// @brief ORノードを作る．
  BddDecEdge
  make_or(
    const std::vector<BddDecEdge>& child_list ///< [in] 子ノードのリスト
  );

  /// @brief ORノードを作る．
  BddDecEdge
  make_xor(
    const std::vector<BddDecEdge>& child_list ///< [in] 子ノードのリスト
  );

  /// @brief LITノードとのANDを作る．
  BddDecEdge
  make_lit_and(
    const BddVar& var, ///< [in] 変数
    bool inv,          ///< [in] 反転属性
    BddDecEdge e           ///< [in] もう一方の枝
  );

  /// @brief LITノードとのORを作る．
  BddDecEdge
  make_lit_or(
    const BddVar& var, ///< [in] 変数
    bool inv,          ///< [in] 反転属性
    BddDecEdge e           ///< [in] もう一方の枝
  );

  /// @brief LITノードとのXORを作る．
  BddDecEdge
  make_lit_xor(
    const BddVar& var, ///< [in] 変数
    bool inv,          ///< [in] 反転属性
    BddDecEdge e           ///< [in] もう一方の枝
  );

  /// @brief CPLXノードを作る．
  BddDecEdge
  make_cplx(
    const Bdd& f,                         ///< [in] グローバル関数
    const std::vector<BddDecEdge>& child_list ///< [in] 子ノードのリスト
  );

  /// @brief 子供が全てリテラルの CPLXノードを作る．
  BddDecEdge
  make_simple_cplx(
    const Bdd& f,
    const std::vector<SizeType>& sup_list
  );

  /// @brief 関数に対応する BddDecEdge を探す．
  /// @retval true 存在した．
  /// @retval false 存在しなかった．
  bool
  find_node(
    const Bdd& f,  ///< [in] 対象の関数
    BddDecEdge& result ///< [out] 結果の枝を格納する変数
  ) const;

  /// @brief 関数に対応する BddDecEdge を登録する．
  void
  put_node(
    const Bdd& f, ///< [in] 対象の関数
    BddDecEdge result ///< [in] 分解結果の枝
  );

  /// @brief 内容を出力する．
  static
  void
  print(
    std::ostream& s, ///< [in] 出力ストリーム
    BddDecEdge root      ///< [in] 根の枝
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BDDマネージャ
  BddMgr mBddMgr;

  // 対象のBDD
  Bdd mBdd;

  // BddDecNode のリスト
  std::vector<std::unique_ptr<BddDecNode>> mNodeList;

  // 関数(BDD)をキーにしてBddDecEdgeを記録する辞書
  std::unordered_map<Bdd, BddDecEdge> mEdgeDict;

};

END_NAMESPACE_YM_DJDEC

#endif // BDDDECMGR_H
