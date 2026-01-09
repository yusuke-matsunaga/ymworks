#ifndef TVDECMGR_H
#define TVDECMGR_H

/// @file TvDecMgr.h
/// @brief TvDecMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/DjDec.h"
#include "TvDecEdge.h"


BEGIN_NAMESPACE_YM_DJDEC

class TvDecNode;
class TvDecMark;

//////////////////////////////////////////////////////////////////////
/// @class TvDecMgr TvDecMgr.h "TvDecMgr.h"
/// @brief TvDecMgr の実装クラス
//////////////////////////////////////////////////////////////////////
class TvDecMgr
{
public:

  /// @brief コンストラクタ
  TvDecMgr();

  /// @brief デストラクタ
  ~TvDecMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 与えられた関数の DgGraph を得る．
  TvDecEdge
  decompose(
    const TvFunc& func
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief decomp の下請け関数
  TvDecEdge
  decomp_step(
    const TvFunc& func ///< [in] 分解を行う関数
  );

  /// @brief 境界条件のチェックを行う．
  /// @return 結果の枝を返す．
  ///
  /// 自明な解が求まらなかった場合は0の枝を返す．
  /// 通常は0にならない．
  TvDecEdge
  trivial_check(
    SizeType var,        ///< [in] コファクターを行った変数
    const TvFunc& func0, ///< [in] 0のコファクター
    const TvFunc& func1  ///< [in] 1のコファクター
  );

  /// @brief コファクターの結果をマージする．
  TvDecEdge
  merge(
    SizeType var, ///< [in] コファクターを行った変数
    TvDecEdge r0,    ///< [in] 0のコファクターの結果
    TvDecEdge r1     ///< [in] 1のコファクターの結果
  );

  /// @brief 共通な子ノードを求める．
  std::vector<TvDecEdge>
  find_common_inputs(
    const TvDecNode* node0, ///< [in] ノード0
    const TvDecNode* node1  ///< [in] ノード1
  );

  /// @brief 共通な子ノードを求める．
  std::vector<TvDecEdge>
  find_common_inputs(
    const TvDecNode* node0, ///< [in] ノード0
    SizeType pos,        ///< [in] ノード0のうち除外する入力の番号
    const TvDecNode* node1  ///< [in] ノード1
  );

  /// @brief 共通でないファンインを求める．
  void
  find_uncommon_inputs(
    const std::vector<const TvDecNode*>& node_list, ///< [in] ノードリスト
    TvDecMark& mark,                            ///< [in] ノードマーク
    int mval,                                    ///< [in] マークの値
    std::vector<TvDecEdge>& inputs                  ///< [out] 境界ノードを格納するリスト
  );

  /// @brief 共通なノードリストをマージする．
  void
  merge_common_inputs(
    const std::vector<const TvDecNode*>& node_list1, ///< [in] ノードリスト1
    const std::vector<const TvDecNode*>& node_list2, ///< [in] ノードリスト2
    TvDecMark& mark,                             ///< [in] ノードマーク
    std::vector<TvDecEdge>& inputs                   ///< [out] 境界ノードを格納するリスト
  );

  /// @brief Case1-OR のマージを行う．
  TvDecEdge
  case1_or(
    SizeType var,                           ///< [in] コファクターを行った変数
    const std::vector<TvDecEdge>& common_list, ///< [in] 共通の子ノードのリスト
    const std::vector<TvDecEdge>& rest0_list,  ///< [in] node0のみの子ノードのリスト
    const std::vector<TvDecEdge>& rest1_list   ///< [in] node1のみの子ノードのリスト
  );

  /// @brief Case1-XOR のマージを行う．
  TvDecEdge
  case1_xor(
    SizeType var,                           ///< [in] コファクターを行った変数
    const std::vector<TvDecEdge>& common_list, ///< [in] 共通の子ノードのリスト
    const std::vector<TvDecEdge>& rest0_list,  ///< [in] node0のみの子ノードのリスト
    const std::vector<TvDecEdge>& rest1_list,  ///< [in] node1のみの子ノードのリスト
    bool inv0,                              ///< [in] node0の反転フラグ
    bool inv1                               ///< [in] node1の反転フラグ
  );

  /// @brief Case1-CPLX のマージを行う．
  TvDecEdge
  case1_cplx(
    SizeType var,                          ///< [in] コファクターを行った変数
    const TvFunc& lf0,                     ///< [in] ローカル関数0
    const TvFunc& lf1,                     ///< [in] ローカル関数1
    const std::vector<TvDecEdge>& child_list, ///< [in] 子ノードのリスト
    TvDecEdge rest0,                          ///< [in] node0のみの子ノード
    TvDecEdge rest1                           ///< [in] node1のみの子ノード
  );

  /// @brief CASE1-CPLX(Type 2) のマージを行う．
  TvDecEdge
  case1_cplx2(
    SizeType var,                          ///< [in] コファクターを行った変数
    const TvFunc& lf0,                     ///< [in] ローカル関数0
    const TvFunc& lf1,                     ///< [in] ローカル関数1
    const std::vector<TvDecEdge>& child_list, ///< [in] 子ノードのリスト
    SizeType pos                           ///< [in] マージ対象の子ノードの位置
  );

  /// @brief CASE2-OR のマージを行う．
  TvDecEdge
  case2_or(
    SizeType var,        ///< [in] コファクターを行った変数
    bool inv,            ///< [in] インデックスの極性
    const TvDecNode* node0, ///< [in] ORノード
    bool inv0,           ///< [in] node0 の極性
    SizeType pos         ///< [in] 子ノードの位置
  );

  /// @brief CASE2-XOR のマージを行う．
  TvDecEdge
  case2_xor(
    SizeType var,        ///< [in] コファクターを行った変数
    bool inv,            ///< [in] インデックスの極性
    const TvDecNode* node0, ///< [in] XORノード
    bool inv0,           ///< [in] node0 の極性
    SizeType pos,        ///< [in] 子ノードの位置
    bool inv1            ///< [in] 他方の枝の極性
  );

  /// @brief CASE2-CPLX のマージを行う．
  TvDecEdge
  case2_cplx(
    SizeType var,      ///< [in] コファクターを行った変数
    const TvFunc& lf,  ///< [in] ローカル関数
    SizeType pos,      ///< [in] ピボットの位置
    bool lit_inv,
    bool is_and,
    const TvDecNode* node
  );

  /// @brief LITノードを作る．
  TvDecEdge
  make_lit(
    SizeType var, ///< [in] コファクターを行った変数
    bool inv      ///< [in] 反転フラグ
  );

  /// @brief ORノードを作る．
  TvDecEdge
  make_or(
    const std::vector<TvDecEdge>& child_list ///< [in] 子ノードのリスト
  )
  {
    return make_or(child_list, TvDecEdge::zero());
  }

  /// @brief ORノードを作る．
  TvDecEdge
  make_or(
    const std::vector<TvDecEdge>& child_list, ///< [in] 子ノードのリスト
    TvDecEdge ex_edge                         ///< [in] 追加する枝
  );

  /// @brief ORノードを作る．
  TvDecEdge
  make_xor(
    const std::vector<TvDecEdge>& child_list ///< [in] 子ノードのリスト
  )
  {
    return make_xor(child_list, TvDecEdge::zero());
  }

  /// @brief ORノードを作る．
  TvDecEdge
  make_xor(
    const std::vector<TvDecEdge>& child_list, ///< [in] 子ノードのリスト
    TvDecEdge ex_edge                         ///< [in] 追加する枝
  );

  /// @brief LITノードとのANDを作る．
  TvDecEdge
  make_lit_and(
    SizeType var, ///< [in] コファクターを行った変数
    bool inv,     ///< [in] 反転属性
    TvDecEdge e      ///< [in] もう一方の枝
  )
  {
    auto lit = make_lit(var, inv);
    return ~make_or({~lit, ~e});
  }

  /// @brief LITノードとのORを作る．
  TvDecEdge
  make_lit_or(
    SizeType var, ///< [in] コファクターを行った変数
    bool inv,     ///< [in] 反転属性
    TvDecEdge e      ///< [in] もう一方の枝
  )
  {
    auto lit = make_lit(var, inv);
    return make_or({lit, e});
  }

  /// @brief LITノードとのXORを作る．
  TvDecEdge
  make_lit_xor(
    SizeType var, ///< [in] コファクターを行った変数
    bool inv,     ///< [in] 反転属性
    TvDecEdge e      ///< [in] もう一方の枝
  )
  {
    auto lit = make_lit(var, inv);
    return make_xor({lit, e});
  }

  /// @brief CPLXノードを作る．
  TvDecEdge
  make_cplx(
    const TvFunc& lf,                     ///< [in] ローカル関数
    const std::vector<TvDecEdge>& child_list ///< [in] 子ノードのリスト
  );

  /// @brief ORノードを探す
  ///
  /// 見つからない場合には nullptr を返す．
  TvDecNode*
  find_or_node(
    const std::vector<TvDecEdge>& child_list ///< [in] 子ノードのリスト
  ) const;

  /// @brief ORノードを登録する．
  void
  put_or_node(
    const std::vector<TvDecEdge>& child_list, ///< [in] 子ノードのリスト
    TvDecNode* node                           ///< [in] ORノード
  );

  /// @brief XORノードを探す
  ///
  /// 見つからない場合には nullptr を返す．
  TvDecNode*
  find_xor_node(
    const std::vector<TvDecEdge>& child_list ///< [in] 子ノードのリスト
  ) const;

  /// @brief ORノードを登録する．
  void
  put_xor_node(
    const std::vector<TvDecEdge>& child_list, ///< [in] 子ノードのリスト
    TvDecNode* node                           ///< [in] XORノード
  );

  /// @brief CPLXノードを探す．
  ///
  /// 見つからない場合には nullptr を返す．
  TvDecNode*
  find_cplx_node(
    const TvFunc& local_func,             ///< [in] ローカル関数
    const std::vector<TvDecEdge>& child_list ///< [in] 子ノードのリスト
  ) const;

  /// @brief CPLXノードを登録する．
  void
  put_cplx_node(
    const TvFunc& local_func,              ///< [in] ローカル関数
    const std::vector<TvDecEdge>& child_list, ///< [in] 子ノードのリスト
    TvDecNode* node                           ///< [in] CPLXノード
  );

  /// @brief 関数に対応する TvDecEdge を探す．
  /// @retval true 存在した．
  /// @retval false 存在しなかった．
  bool
  find_edge(
    const TvFunc& f, ///< [in] 対象の関数
    TvDecEdge& result   ///< [out] 結果の枝を格納する変数
  ) const;

  /// @brief 関数に対応する TvDecEdge を登録する．
  void
  put_edge(
    const TvFunc& f, ///< [in] 対象の関数
    TvDecEdge result    ///< [in] 分解結果の枝
  );

  /// @brief 内容を出力する．
  static
  void
  print(
    std::ostream& s, ///< [in] 出力ストリーム
    TvDecEdge root      ///< [in] 根の枝
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // TvDecNode のリスト
  std::vector<std::unique_ptr<TvDecNode>> mNodeList;

  // LITノードのリスト
  std::vector<TvDecNode*> mLitList;

  // ノードのシグネチャをキーにして TvDecNode を保持する辞書
  std::unordered_map<std::string, TvDecNode*> mNodeDict;

  // 関数(TvFunc)をキーにしてTvDecEdgeを記録する辞書
  std::unordered_map<TvFunc, TvDecEdge> mEdgeDict;

};

END_NAMESPACE_YM_DJDEC

#endif // TVDECMGR_H
