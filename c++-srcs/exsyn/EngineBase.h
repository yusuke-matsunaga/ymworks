#ifndef ENGINEBASE_H
#define ENGINEBASE_H

/// @file EngineBase.h
/// @brief EngineBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ExSyn.h"
#include "ym/SatSolver.h"
#include "ym/JsonValue.h"
#include "ym/bc.h"


BEGIN_NAMESPACE_YM_EXSYN

class OpNode;

//////////////////////////////////////////////////////////////////////
/// @class EngineBase EngineBase.h "EngineBase.h"
/// @brief ExSyn 用のエンコーダーの基底クラス
///
/// コンストラクタで生成した後に solver() で計算を行う．
/// 関数を２つに分けている理由は EngineBase のコンストラクタ中では
/// 派生クラスの仮想関数が使えないから．
//////////////////////////////////////////////////////////////////////
class EngineBase
{
public:

  /// @brief コンストラクタ
  EngineBase(
    const TruthTable& tt,   ///< [in] 対象の論理関数
    SizeType op_num,        ///< [in] 演算ノード数
    const JsonValue& option ///< [in] オプション
  );

  /// @brief デストラクタ
  ~EngineBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 問題を解く
  ///
  /// 解けなかった場合は空のグラフを返す．
  BcGraph
  solve();

  /// @brief 対象の論理関数を返す．
  const TruthTable&
  func() const
  {
    return mFunc;
  }

  /// @brief オプションを返す．
  const JsonValue&
  option() const
  {
    return mOption;
  }

  /// @brief SATソルバを返す．
  SatSolver&
  solver()
  {
    return mSolver;
  }

  /// @brief 入力数を返す．
  SizeType
  input_num() const
  {
    return mInputNum;
  }

  /// @brief 出力数を返す．
  SizeType
  output_num() const
  {
    return mOutputNum;
  }

  /// @brief 演算ノード数を返す．
  SizeType
  op_num() const
  {
    return mOpList.size();
  }

  /// @brief 演算ノードを返す．
  const OpNode*
  get_op(
    SizeType op_id ///< [in] 演算ノード番号
  ) const
  {
    if ( op_id >= mOpList.size() ) {
      throw std::out_of_range{"op_id is out of range"};
    }
    return mOpList[op_id].get();
  }

  /// @brief op_id の演算を出力が参照している条件を求める．
  ///
  /// 結果は dst_lits に「追加」される．
  void
  add_ref_cond(
    SizeType op_id,                   ///< [in] 演算ノード番号
    std::vector<SatLiteral>& dst_lits ///< [out] 結果を格納する変数
  );

  /// @brief 入力値に対応する条件を追加する．
  void
  add_cond(
    SizeType input_index ///< [in] 入力インデックス
  );

  /// @brief 出力の選択結果を返す．
  SizeType
  output_sel(
    SizeType o_id,        ///< [in] 出力番号
    const SatModel& model ///< [in] SATモデル
  ) const;

  /// @brief 出力の反転属性結果を返す．
  ///
  /// もともと双極性でない場合も false が返される．
  bool
  output_inv(
    SizeType o_id,        ///< [in] 出力番号
    const SatModel& model ///< [in] SATモデル
  ) const;

  /// @brief 演算ノードを設定する．
  void
  set_op(
    SizeType op_id,  ///< [in] 演算番号
    const OpNode* op ///< [in] 演算ノード
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 派生クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算ノードの制約を作る．
  virtual
  void
  make_op_list() = 0;

  /// @brief use_all_inputs 制約を作る．
  virtual
  void
  add_use_all_inputs() = 0;

  /// @brief use_all_steps 制約を作る．
  virtual
  void
  add_use_all_steps() = 0;

  /// @brief no_reapplication 制約を作る．
  virtual
  void
  add_no_reapplication() = 0;

  /// @brief op_order 制約を作る．
  virtual
  void
  add_op_order() = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の論理関数
  TruthTable mFunc;

  // オプション
  JsonValue mOption;

  // SATソルバ
  SatSolver mSolver;

  // 入力数
  SizeType mInputNum;

  // 出力数
  SizeType mOutputNum;

  // 演算ノードのリスト
  std::vector<std::unique_ptr<const OpNode>> mOpList;

  // 出力の選択用変数
  SatOneHotMatrix mOutSelMatrix;

  // 出力の反転属性
  // 極性が決まっている場合は SatLiteral::X を持つ．
  std::vector<SatLiteral> mOutInvArray;

};

END_NAMESPACE_YM_EXSYN

#endif // ENGINEBASE_H
