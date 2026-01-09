#ifndef BDNODE3_H
#define BDNODE3_H

/// @file BdNode3.h
/// @brief BdNode3 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "OpNode3.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class BdNode3 BdNode3.h "BdNode3.h"
/// @brief BD用の OpNode3 の継承クラス
//////////////////////////////////////////////////////////////////////
class BdNode3 :
  public BdNode
{
public:

  /// @brief コンストラクタ
  BdNode3(
    const TemplNode* templ_node,          ///< [in] テンプレートノード
    SizeType input_num,                   ///< [in] 入力数
    SizeType op_id,                       ///< [in] 演算ノード番号
    const std::vector<SizeType>& opr_list ///< [in] オペランドのリスト
  ): BdNode(templ_node, input_num, op_id, opr_list)
  {
  }

  /// @brief デストラクタ
  ~BdNode3() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 制約を生成する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力値に関する制約を追加する．
  /// @return 出力値を表す変数を返す．
  SatLiteral
  add_ovar_cond(
    SatSolver& solver,                        ///< [in] SATソルバ
    const std::vector<bool>& ival_list,       ///< [in] 入力値のリスト
    const std::vector<SatLiteral>& opvar_list ///< [in] 演算ノードの出力の変数のリスト
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

};

END_NAMESPACE_YM_EXSYN

#endif // BDNODE3_H
