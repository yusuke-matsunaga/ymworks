#ifndef FRAIGMGR_H
#define FRAIGMGR_H

/// @file FraigMgr.h
/// @brief FraigMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/fraig.h"
#include "ym/FraigLiteral.h"
#include "ym/SatInitParam.h"
#include "ym/SatBool3.h"


BEGIN_NAMESPACE_YM_FRAIG

class FraigMgrImpl;

//////////////////////////////////////////////////////////////////////
/// @class FraigMgr FraigMgr.h "ym/FraigMgr.h"
/// @brief FRAIG マネージャ
/// @sa FraigLiteral, SatInitParam
///
/// AIG とよく似た2入力ANDを基本としたDAG構造．
/// 目的は純粋に論理関数を表すこと．
///
/// AIG は構造が同一のノードを共有していたのに対して
/// FRAIG では論理関数が同一のノードも共有する．
///
/// ただし，BDDのようなカノニカル性はないのでグラフの
/// 作り方によって異なった構造のグラフが作られる可能性がある．
///
/// 論理関数の同一判定には SAT ソルバを用いる．
///
/// 大まかにはグラフ中のノードについて処理を行うが，
/// 場合によってはそのノードの否定を扱う場合もあるので
/// ノードと反転属性を合わせたリテラルを対象とする．
/// リテラルは FraigLiteral で表す．
///
/// 実装は FraigMgrImpl が行う．
/// このクラスは FraigMgrImpl へのユニークポインタを持つだけ．
//////////////////////////////////////////////////////////////////////
class FraigMgr
{
public:

  /// @brief コンストラクタ
  FraigMgr(
    SizeType sig_size,                  ///< [in] シグネチャのサイズ
    const SatInitParam& init_param = {} ///< [in] SATソルバの初期化パラメータ
  );

  /// @brief デストラクタ
  ~FraigMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 構造(FraigNode)を作成するメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 外部入力を作る．
  /// @return 作成した入力を表すリテラルを返す．
  /// @sa FraigLiteral
  FraigLiteral
  new_input();

  /// @brief 2つのノードの AND を作る．
  /// @return 作成したノードを表すリテラルを返す．
  /// @sa FraigLiteral
  FraigLiteral
  new_and(
    FraigLiteral edge1, ///< [in] 入力1のリテラル
    FraigLiteral edge2  ///< [in] 入力2のリテラル
  );

  /// @brief 複数ノードの AND を作る．
  /// @return 作成したノードを表すリテラルを返す．
  /// @sa FraigLiteral
  FraigLiteral
  new_and(
    const std::vector<FraigLiteral>& edge_list ///< [in] 入力のリテラルのリスト
  );

  /// @brief ２つのノードの OR を作る．
  /// @return 作成したノードを表すリテラルを返す．
  /// @sa FraigLiteral
  FraigLiteral
  new_or(
    FraigLiteral edge1, ///< [in] 入力1のリテラル
    FraigLiteral edge2  ///< [in] 入力2のリテラル
  );

  /// @brief 複数ノードの OR を作る．
  /// @return 作成したノードを表すリテラルを返す．
  /// @sa FraigLiteral
  FraigLiteral
  new_or(
    const std::vector<FraigLiteral>& edge_list ///< [in] 入力のリテラルのリスト
  );

  /// @brief ２つのノードの XOR を作る．
  /// @return 作成したノードを表すリテラルを返す．
  /// @sa FraigLiteral
  FraigLiteral
  new_xor(
    FraigLiteral edge1, ///< [in] 入力1のリテラル
    FraigLiteral edge2  ///< [in] 入力2のリテラル
  );

  /// @brief 複数ノードの XOR を作る．
  /// @return 作成したノードを表すリテラルを返す．
  /// @sa FraigLiteral
  FraigLiteral
  new_xor(
    const std::vector<FraigLiteral>& edge_list ///< [in] 入力のリテラルのリスト
  );

  /// @brief If-Then-Else演算
  /// @return 作成したノードを表すリテラルを返す．
  /// @sa FraigLiteral
  FraigLiteral
  ite(
    FraigLiteral if_edge,   ///< [in] 選択信号線
    FraigLiteral then_edge, ///< [in] if_edge == 1 の時選ばれる入力
    FraigLiteral else_edge  ///< [in] if_edge == 0 の時選ばれる入力
  );

  /// @brief コファクター演算
  /// @return 作成したノードを表すリテラルを返す．
  /// @sa FraigLiteral
  FraigLiteral
  cofactor(
    FraigLiteral edge, ///< [in] 対象のリテラル
    SizeType input_id, ///< [in] 固定する入力番号
    bool val           ///< [in] 固定する値(true: 1, false: 0)
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 検証用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 2つのリテラルが等価かどうか調べる．
  /// @sa FraigLiteral
  SatBool3
  check_equiv(
    FraigLiteral lit1, ///< [in] 入力1のリテラル
    FraigLiteral lit2  ///< [in] 入力2のリテラル
  );

  /// @brief ログレベルを設定する．
  void
  set_loglevel(
    SizeType level ///< [in] ログレベル
  );

  /// @brief ログ出力用ストリームを設定する．
  void
  set_logstream(
    std::ostream* out ///< [in] 出力ストリーム
  );

  /// @brief 内部の統計情報を出力する．
  void
  dump_stats(
    std::ostream& s ///< [in] 出力ストリーム
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief new_and() の下請け関数
  FraigLiteral
  _and_sub(
    const std::vector<FraigLiteral>& edge_list, ///< [in] 入力のリスト
    SizeType start_pos,                         ///< [in] 開始位置
    SizeType end_pos,                           ///< [in] 終了位置
    bool iinv                                   ///< [in] 入力の反転フラグ
  );

  /// @brief new_xor() の下請け関数
  FraigLiteral
  _xor_sub(
    const std::vector<FraigLiteral>& edge_list, ///< [in] 入力のリスト
    SizeType start_pos,                        ///< [in] 開始位置
    SizeType end_pos                           ///< [in] 終了位置
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実装クラスへのポインタ
  std::unique_ptr<FraigMgrImpl> mPtr;

};

END_NAMESPACE_YM_FRAIG

#endif // FRAIGMGR_H
