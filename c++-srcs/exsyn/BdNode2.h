#ifndef BDNODE2_H
#define BDNODE2_H

/// @file BdNode2.h
/// @brief BdNode2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "BdNode.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class BdNode2 BdNode2.h "BdNode2.h"
/// @brief 二項演算の BdNode
///
/// 二項演算の２つのオペランドの情報を持つ．
/// 入力番号のそのままオペランド番号となる．
/// 演算番号は入力数を足したものがオペランド番号となる．
//////////////////////////////////////////////////////////////////////
class BdNode2 :
  public BdNode
{
public:

  /// @brief コンストラクタ
  BdNode2(
    const TemplNode* templ_node,          ///< [in] テンプレートノード
    SizeType input_num,                   ///< [in] 入力数
    SizeType op_id,                       ///< [in] 演算ノード番号
    const std::vector<SizeType>& opr_list ///< [in] オペランドのリスト
  ): BdNode(templ_node, input_num, op_id, opr_list)
  {
  }

  /// @brief デストラクタ
  ~BdNode2() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////


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

#endif // BDNODE2_H
