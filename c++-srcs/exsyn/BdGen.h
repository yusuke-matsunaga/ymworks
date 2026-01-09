#ifndef BDGEN_H
#define BDGEN_H

/// @file BdGen.h
/// @brief BdGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ExSyn.h"
#include "Fence.h"
#include "PD.h"
#include "BD.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class BdGen BdGen.h "BdGen.h"
/// @brief Bound DAG を生成するクラス
//////////////////////////////////////////////////////////////////////
class BdGen
{
public:

  using Opr = std::vector<int>;
  using OprList = std::vector<Opr>;

public:

  /// @brief コンストラクタ
  BdGen(
    const PD& pd,      ///< [in] Partial DAG
    SizeType input_num ///< [in] 入力数
  ) : mPD{pd},
      mInputNum{input_num}
  {
  }

  /// @brief デストラクタ
  ~BdGen() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief Bound DAG を生成する．
  std::vector<BD>
  operator()();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 下請け関数
  void
  recur_sub(
    const OprList& cur_pd,
    SizeType node_id
  );

  /// @brief 適切な BD か調べる．
  bool
  check_bd(
    const OprList& opr_list
  ) const;

  /// @biref オペランド間の順序関係を調べる．
  bool
  check_ord(
    const OprList& opr_list
  ) const;

  /// @brief オペランドの番号の比較を行う．
  bool
  check_le(
    const BdGen::Opr& op1,
    const BdGen::Opr& op2
  ) const;

  /// @brief no reapplication のチェック
  bool
  check_noreapplication(
    const OprList& opr_list
  ) const;

  /// @brief use_ll_inputs のチェック
  bool
  check_all_inputs(
    const OprList& opr_list
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // Partial DAG
  PD mPD;

  // 入力数
  SizeType mInputNum;

  // 結果を格納するリスト
  std::vector<BD> mBdList;

};

END_NAMESPACE_YM_EXSYN

#endif // BDGEN_H
