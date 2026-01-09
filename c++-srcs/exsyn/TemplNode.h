#ifndef TEMPLNODE_H
#define TEMPLNODE_H

/// @file TemplNode.h
/// @brief TemplNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ExSyn.h"
#include "ym/SatModel.h"
#include "ym/bc.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class TemplNode TemplNode.h "TemplNode.h"
/// @brief 演算ノードを表す基底クラス
///
/// 項数の定まっていない演算を表す．
//////////////////////////////////////////////////////////////////////
class TemplNode
{
public:

  /// @brief 結果を表す構造体
  struct MapInfo {
    BcOpType op_type;         ///< 演算のタイプ
    std::vector<BcBvType> tv; ///< 真理値表ベクタ
    std::vector<bool> inv;    ///< 反転フラグ
  };


public:

  /// @brief コンストラクタ
  TemplNode() = default;

  /// @brief デストラクタ
  virtual
  ~TemplNode() = default;

  /// @brief 実際の継承クラスを作る．
  static
  TemplNode*
  new_obj(
    SatSolver& solver, ///< [in] SATソルバ
    SizeType opr_num   ///< [in] オペランド数
  );


public:
  //////////////////////////////////////////////////////////////////////
  // SAT モデルから値を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief SATモデルから演算タイプを得る．
  virtual
  MapInfo
  get_map_info(
    const SatModel& model ///< [in] SATモデル
  ) const = 0;

};

END_NAMESPACE_YM_EXSYN

#endif // TEMPLNODE_H
