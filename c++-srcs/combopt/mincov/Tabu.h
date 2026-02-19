#ifndef TABU_H
#define TABU_H

/// @file Tabu.h
/// @brief Tabu のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "Solver.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class Tabu Tabu.h "Tabu.h"
/// @brief Minimum Covering 問題を tabu search で解くクラス
//////////////////////////////////////////////////////////////////////
class Tabu:
  public Solver
{
public:

  /// @brief コンストラクタ
  Tabu(
    McMatrix& matrix,        ///< [in] 対象の行列
    const JsonValue& opt_obj ///< [in] オプションを表す JSON オブジェクト
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 最小被覆問題を解く．
  /// @return 解のコストを返す．
  SizeType
  solve(
    std::vector<SizeType>& solution ///< [out] 選ばれた列集合
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 列を選択するクラス
  std::unique_ptr<Selector> mSelector;

};

END_NAMESPACE_YM_MINCOV

#endif // TABU_H
