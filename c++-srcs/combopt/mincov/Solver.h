#ifndef SOLVER_H
#define SOLVER_H

/// @file Solver.h
/// @brief Solver のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "mincov/mincov_nsdef.h"
#include "ym/json.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class Solver Solver.h "Solver.h"
/// @brief 最小被覆問題の厳密解を求めるクラス
//////////////////////////////////////////////////////////////////////
class Solver
{
protected:

  /// @brief コンストラクタ
  Solver(
    McMatrix& matrix,        ///< [in] 問題の行列
    const JsonValue& opt_obj ///< [in] オプションを表す JSON オブジェクト
  );


public:
  //////////////////////////////////////////////////////////////////////
  //
  //////////////////////////////////////////////////////////////////////

  /// @brief オプションを解析してインスタンスを生成する関数
  static
  std::unique_ptr<Solver>
  new_obj(
    McMatrix& matrix,        ///< [in] 対象の行列
    const JsonValue& opt_obj ///< [in] オプションを表す JSON オブジェクト
  );

  /// @brief デストラクタ
  virtual
  ~Solver();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 最小被覆問題を解く．
  /// @return 解のコスト
  virtual
  SizeType
  solve(
    std::vector<SizeType>& solution ///< [out] 選ばれた列集合
  ) = 0;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 対象の行列を返す．
  McMatrix&
  matrix()
  {
    return mMatrix;
  }

  /// @brief デバッグレベルを得る．
  int
  debug() const
  {
    return mDebug;
  }

  /// @brief デバッグ用の最大深さを得る．
  int
  debug_depth() const
  {
    return mDebugDepth;
  }

  /// @brief Selector オブジェクトを作る．
  static
  std::unique_ptr<Selector>
  new_Selector(
    const JsonValue&  option
  );

  /// @brief LbCalc オブジェクトを作る．
  static
  std::unique_ptr<LbCalc>
  new_LbCalc(
    const JsonValue&  option
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の行列
  McMatrix& mMatrix;

  // デバッグレベル
  int mDebug;

  // デバッグで表示する最大深さ
  int mDebugDepth;

};

END_NAMESPACE_YM_MINCOV

#endif // SOLVER_H
