#ifndef BDNODE4_H
#define BDNODE4_H

/// @file BdNode4.h
/// @brief BdNode4 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "BdNode.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class BdNode4 BdNode4.h "BdNode4.h"
/// @brief BD用の OpNode4 の継承クラス
//////////////////////////////////////////////////////////////////////
class BdNode4 :
  public BdNode
{
public:

  /// @brief コンストラクタ
  BdNode4(
    const TemplNode* templ_node,          ///< [in] テンプレートノード
    SizeType input_num,                   ///< [in] 入力数
    SizeType op_id,                       ///< [in] 演算ノード番号
    const std::vector<SizeType>& opr_list ///< [in] オペランドのリスト
  ): BdNode(templ_node, input_num, op_id, opr_list)
  {
  }

  /// @brief デストラクタ
  ~BdNode4() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
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

#endif // BDNODE4_H
