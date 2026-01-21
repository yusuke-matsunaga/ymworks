#ifndef BCGRAPH_H
#define BCGRAPH_H

/// @file BcGraph.h
/// @brief BcGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bc.h"
#include "ym/BcEdge.h"
#include "ym/BcOp.h"


BEGIN_NAMESPACE_YM_BC

//////////////////////////////////////////////////////////////////////
/// @class BcGraph BcGraph.h "ym/BcGraph.h"
/// @ingroup BcGroup
/// @brief Boolean chain を表すグラフ
///
/// Boolean chain は N入力M出力の論理関数を表す論理式の集合
///
/// ただし，論理式の間に順序関係があり，論理式の右辺には
/// 自分より前の論理式もしくは入力しか使えない．
///
/// 特殊なケースとして定数を表す入力がある．
/// ただし，定数は出力の右辺としてのみ用いられる．
///
/// この Boolean chain を DAG とみなしてグラフ構造で表す．
///
/// ただし，ノードとして情報を持つのは演算のみ．
/// 入力や定数の情報は枝に持たせる．
///
/// なお，この Boolean chain では各ノードの出力は正規形になっている．
///
/// 正規形とは全ての入力が0の時の値が0であること．
/// そのため元々の関数が正規形でなかった場合には出力に反転属性をつける．
//////////////////////////////////////////////////////////////////////
class BcGraph
{
public:

  /// @brief 空のコンストラクタ
  BcGraph();

  /// @brief 入力サイズだけ指定したコンストラクタ
  explicit
  BcGraph(
    SizeType input_num ///< [in] 入力数
  );

  /// @brief デストラクタ
  ~BcGraph();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を得る．
  SizeType
  input_num() const
  {
    return mInputNum;
  }

  /// @brief 出力数を得る．
  SizeType
  output_num() const
  {
    return mOutputList.size();
  }

  /// @brief 演算数を得る．
  SizeType
  op_num() const
  {
    return mOpList.size();
  }

  /// @brief 演算ノードを得る．
  const BcOp&
  op(
    SizeType op_id ///< [in] 演算番号 ( 0 <= op_id < op_num() )
  ) const
  {
    check_op_id(op_id);
    return mOpList[op_id];
  }

  /// @brief 出力の枝を得る．
  BcEdge
  output(
    SizeType output_id ///< [in] 出力番号 ( 0 <= output_id < output_num() )
  ) const
  {
    check_output_id(output_id);
    return mOutputList[output_id];
  }

  /// @brief 出力のレベルを得る．
  SizeType
  output_level(
    SizeType output_id ///< [in] 出力番号 ( 0 <= output_id < output_num() )
  ) const
  {
    check_output_id(output_id);
    return mOutputLevelList[output_id];
  }

  /// @brief 最大レベルを得る．
  SizeType
  max_level() const
  {
    return mMaxLevel;
  }

  /// @brief 枝のレベルを得る．
  SizeType
  edge_level(
    const BcEdge& edge ///< [in] 枝
  ) const;

  /// @brief シミュレーションを行う(bool値版)．
  std::vector<bool>
  simulate(
    const std::vector<bool>& input_vals ///< [in] 入力ベクタ
  ) const;

  /// @brief シミュレーションを行う(BcBvType版)．
  std::vector<BcBvType>
  simulate(
    const std::vector<BcBvType>& input_vals ///< [in] 入力ベクタ
  ) const;

  /// @brief 内容を出力する．
  void
  print(
    std::ostream& s ///< [in] 出力ストリーム
  ) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算を追加する．
  /// @return 追加された演算番号を返す．
  SizeType
  add_op(
    BcOpType type,                      ///< [in] 演算の種類
    const std::vector<BcEdge>& opr_list ///< [in] オペランドのリスト
  )
  {
    return add_op_sub(type, {}, opr_list);
  }

  /// @brief 真理値表タイプの演算を追加する．
  /// @return 追加された演算番号を返す．
  SizeType
  add_tvop(
    const std::vector<BcBvType>& tv,    ///< [in] 真理値表ベクタ
    const std::vector<BcEdge>& opr_list ///< [in] オペランドのリスト
  )
  {
    return add_op_sub(BcOpType::TV, tv, opr_list);
  }

  /// @brief 出力を追加する．
  /// @return 追加された出力番号を返す．
  SizeType
  add_output(
    BcEdge src ///< [in] ソースの枝
  );

  /// @brief 定数0の出力を追加する．
  /// @return 追加された出力番号を返す．
  SizeType
  add_zero_output();

  /// @brief 定数1の出力を追加する．
  /// @return 追加された出力番号を返す．
  SizeType
  add_one_output();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief add_op(), add_tvop() の下請け関数
  SizeType
  add_op_sub(
    BcOpType type,
    const std::vector<BcBvType>& tv,
    const std::vector<BcEdge>& opr_list
  );

  /// @brief 出力番号の範囲をチェックする．
  void
  check_output_id(
    SizeType output_id
  ) const
  {
    if ( output_id >= output_num() ) {
      throw std::out_of_range{"output_id is out of range"};
    }
  }

  /// @brief 演算番号の範囲をチェックする．
  void
  check_op_id(
    SizeType op_id
  ) const
  {
    if ( op_id >= op_num() ) {
      throw std::out_of_range{"op_id is out of range"};
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  SizeType mInputNum{0};

  // 演算のリスト
  std::vector<BcOp> mOpList;

  // 出力の枝のリスト
  std::vector<BcEdge> mOutputList;

  // 出力のレベルのリスト
  std::vector<SizeType> mOutputLevelList;

  // 最大レベル
  SizeType mMaxLevel{0};

};

END_NAMESPACE_YM_BC

#endif // BCGRAPH_H
