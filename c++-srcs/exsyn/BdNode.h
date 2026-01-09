#ifndef BDNODE_H
#define BDNODE_H

/// @file BdNode2.h
/// @brief BdNode2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ExSyn.h"
#include "TemplNode.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class BdNode BdNode.h "BdNode.h"
/// @brief Engine_BD 用の演算ノード
///
/// OpNode の派生クラスと異なり，複数の BdNode が一つの TemplNode を共有する．
//////////////////////////////////////////////////////////////////////
class BdNode
{
public:

  using MapInfo = TemplNode::MapInfo;

public:

  /// @brief コンストラクタ
  BdNode(
    const TemplNode* templ_node,          ///< [in] テンプレートノード
    SizeType input_num,                   ///< [in] 入力数
    SizeType op_id,                       ///< [in] 演算ノード番号
    const std::vector<SizeType>& opr_list ///< [in] オペランドのリスト
  ) : mTemplate{templ_node},
      mInputNum{input_num},
      mOpId{op_id},
      mOprList{opr_list}
  {
  }

  /// @brief デストラクタ
  virtual
  ~BdNode() = default;

  /// @brief 実際の派生クラスを作る．
  static
  const BdNode*
  new_obj(
    const TemplNode* templ_node,          ///< [in] テンプレートノード
    SizeType input_num,                   ///< [in] 入力数
    SizeType op_id,                       ///< [in] 演算ノード番号
    const std::vector<SizeType>& opr_list ///< [in] オペランドのリスト
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を返す．
  SizeType
  input_num() const
  {
    return mInputNum;
  }

  /// @brief 演算ノード番号を返す．
  SizeType
  op_id() const
  {
    return mOpId;
  }

  /// @brief オペランド数を返す．
  SizeType
  opr_num() const
  {
    return mOprList.size();
  }

  /// @brief オペランドの値を返す．
  SizeType
  opr(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < opr_num )
  ) const
  {
    if ( pos >= opr_num() ) {
      throw std::out_of_range{"pos is out of range"};
    }
    return mOprList[pos];
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 制約を生成する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力値に関する制約を追加する．
  /// @return 出力値を表す変数を返す．
  virtual
  SatLiteral
  add_ovar_cond(
    SatSolver& solver,                        ///< [in] SATソルバ
    const std::vector<bool>& ival_list,       ///< [in] 入力値のリスト
    const std::vector<SatLiteral>& opvar_list ///< [in] 演算ノードの出力の変数のリスト
  ) const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // SAT モデルから値を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief SATモデルから演算タイプを得る．
  MapInfo
  get_map_info(
    const SatModel& model ///< [in] SATモデル
  ) const
  {
    return mTemplate->get_map_info(model);
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // 派生クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief テンプレートノードを返す．
  const TemplNode*
  _template() const
  {
    return mTemplate;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // テンプレートノード
  const TemplNode* mTemplate;

  // 入力数
  SizeType mInputNum;

  // 演算ノード番号
  SizeType mOpId;

  // オペランドのリスト
  std::vector<SizeType> mOprList;

};

END_NAMESPACE_YM_EXSYN

#endif // BDNODE_H
