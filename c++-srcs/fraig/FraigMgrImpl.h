#ifndef FRAIGMGRIMPL_H
#define FRAIGMGRIMPL_H

/// @file FraigMgrImpl.h
/// @brief FraigMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/fraig.h"
#include "ym/FraigLiteral.h"
#include "StructTable.h"
#include "PatTable.h"
#include "FraigSat.h"
#include <random>

#include "ym/SatBool3.h"
#include "ym/SatInitParam.h"


BEGIN_NAMESPACE_YM_FRAIG

//////////////////////////////////////////////////////////////////////
/// @class FraigMgrImpl FraigMgrImpl.h "FraigMgrImpl.h"
/// @brief FraigMgr の実装クラス
//////////////////////////////////////////////////////////////////////
class FraigMgrImpl
{
public:

  /// @brief コンストラクタ
  FraigMgrImpl(
    SizeType sig_size,                  ///< [in] シグネチャのサイズ
    const SatInitParam& init_param = {} ///< [in] SAT-solver の初期化パラメータ
  );

  /// @brief デストラクタ
  ~FraigMgrImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得するメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ノード数を得る．
  SizeType
  input_num() const
  {
    return mInputNodes.size();
  }

  /// @brief 入力ノードを取り出す．
  FraigNode*
  input_node(
    SizeType pos ///< [in] 入力番号 ( 0 <= pos < input_num() )
  ) const
  {
    if ( pos >= input_num() ) {
      throw std::out_of_range{"pos is out of range"};
    }

    return mInputNodes[pos];
  }

  /// @brief ノード数を得る．
  SizeType
  node_num() const
  {
    return mAllNodes.size();
  }

  /// @brief リテラルからノードを取り出す．
  ///
  /// - 反転属性は無視する．
  /// - ANDノードの他に入力ノードも含まれる．
  FraigNode*
  literal2node(
    FraigLiteral lit ///< [in] リテラル
  ) const
  {
    auto pos = lit.node_id();
    if ( pos >= node_num() ) {
      throw std::out_of_range{"pos is out of range"};
    }
    return mAllNodes[pos].get();
  }

  /// @brief ノードを指すリテラルを作る．
  static
  FraigLiteral
  node2literal(
    FraigNode* node,
    bool inv = false
  )
  {
    return FraigLiteral(node->id(), inv);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 構造(FraigNode)を作成するメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 外部入力を作る．
  FraigLiteral
  new_input();

  /// @brief 2つのノードの AND を作る．
  FraigLiteral
  new_and(
    FraigLiteral edge1, ///< [in] 入力1のハンドル
    FraigLiteral edge2  ///< [in] 入力2のハンドル
  );

  /// @brief コファクター演算
  FraigLiteral
  cofactor(
    FraigLiteral edge, ///< [in] 対象のハンドル
    SizeType input_id, ///< [in] 入力番号
    bool val           ///< [in] 固定する値
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 検証用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 2つのハンドルが等価かどうか調べる．
  SatBool3
  check_equiv(
    FraigLiteral aig1, ///< [in] 入力1のハンドル
    FraigLiteral aig2  ///< [in] 入力2のハンドル
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

  /// @brief make_cofactor() の下請け関数
  FraigLiteral
  _cofactor_sub(
    FraigNode* node,
    SizeType input_id,
    bool val
  );

  /// @brief 縮退検査を行う．
  /// @retval SatBool3::True 定数に縮退していた．
  /// @retval SatBool3::False 定数ではなかった．
  /// @retval SatBool3::X 不明(アボート)
  SatBool3
  verify_const(
    FraigNode* node, ///< [in] 対象のノード
    FraigLiteral& ans ///< [out] 答
  );

  /// @brief パタンハッシュを調べて論理的に等価なノードを探す．
  /// @return 等価なハンドル
  ///
  /// 等価なノードが見つからなかった場合はパタンハッシュに登録する．
  FraigLiteral
  check_pat(
    FraigNode* node ///< [in] 対象のノード
  );

  /// @brief 全ノードのシミュレーションパタン用配列を拡大する．
  void
  resize_pat(
    SizeType size ///< [in] サイズ
  );

  /// @breif 直前の SAT の反例を加えて再ハッシュする．
  void
  add_pat(
    FraigNode* node ///< [in] 対象のノード
  );

  /// @brief ノードを登録する．
  void
  reg_node(
    FraigNode* node
  );

  /// @brief リテラルの内容を表す文字列を作る．
  std::string
  print_literal(
    FraigLiteral lit
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号をキーにしたノードの配列
  // ノードの所有権を持つ．
  std::vector<std::unique_ptr<FraigNode>> mAllNodes;

  // 入力ノードの配列
  std::vector<FraigNode*> mInputNodes;

  // 構造ハッシュ
  StructTable mStructTable;

  // パタンハッシュ
  PatTable mPatTable;

  // 乱数発生器
  std::mt19937 mRandGen;

  // SATソルバ
  FraigSat mSolver;

  // recsolver 用のストリーム
  std::ostream* mOutP;

  // ログレベル
  SizeType mLogLevel;

  // ログ出力用のストリーム
  std::ostream* mLogStream;

};

END_NAMESPACE_YM_FRAIG

#endif // FRAIGMGRIMPL_H
