#ifndef PDGEN_H
#define PDGEN_H

/// @file PdGen.h
/// @brief PdGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ExSyn.h"
#include "Fence.h"
#include "PD.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class PdGen PdGen.h "PdGen.h"
/// @brief Partial DAG を生成するクラス
//////////////////////////////////////////////////////////////////////
class PdGen
{
public:

  using Opr = std::vector<int>;
  using OprList = std::vector<Opr>;

public:

  /// @brief コンストラクタ
  PdGen(
    bool use_all_steps = true
  )
  {
    mFlags[0] = use_all_steps;
  }

  /// @brief デストラクタ
  ~PdGen() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief Partial DAG を生成する．
  std::vector<PD>
  operator()(
    const Fence& fence,      ///< [in] fence
    SizeType operand_num = 2 ///< [in] 演算の項数
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief use_all_steps のチェックを行う時 true
  bool
  use_all_steps() const
  {
    return mFlags[0];
  }

  /// @brief 下請け関数
  void
  recur_sub(
    const OprList& cur_pd,
    SizeType node_id
  );

  /// @brief 適切な PD か調べる．
  bool
  check_pd(
    const OprList& opr_list
  ) const;

  /// @biref オペランド間の順序関係を調べる．
  bool
  check_ord(
    const OprList& opr_list
  ) const;

  /// @brief no reapplication のチェック
  bool
  check_noreapplication(
    const OprList& opr_list
  ) const;

  /// @brief use_all_steps のチェック
  bool
  check_all_use(
    const OprList& opr_list
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 種々のフラグ
  std::bitset<1> mFlags{0};

  // fence
  Fence mFence;

  // 演算の項数
  SizeType mOperandNum;

  // 結果を格納するリスト
  std::vector<PD> mPdList;

};

END_NAMESPACE_YM_EXSYN

#endif // PDGEN_H
