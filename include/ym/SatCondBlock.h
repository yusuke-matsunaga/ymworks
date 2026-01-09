#ifndef SATCONDBLOCK_H
#define SATCONDBLOCK_H

/// @file SatCondBlock.h
/// @brief SatCondBlock のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/sat.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
/// @class SatCondBlock SatCondBlock.h "SatCondBlock.h"
/// @brief conditional_literals をセットするためのクラス
///
/// 基本的な操作は SatSolver::set_conditional_literals() と
/// SatSolver::clear_conditional_literals() で行うが，
/// 対で行う必要があるので C++ のコンストラクタ，デストラクタ
/// の機能を利用してより簡単に記述できるようにする．
///
/// 通常，デストラクタは必ず起動されるので clear_conditional_literals()
/// が呼ばれないバグを防ぐことができる．
//////////////////////////////////////////////////////////////////////
class SatCondBlock
{
public:

  /// @brief コンストラクタ
  SatCondBlock(
    SatSolver& solver, ///< [in] SATソルバ
    SizeType num       ///< [in] 取り除く変数の数
  ) : mSolver{solver},
      mNum{num}
  {
  }

  /// @brief コピーコンストラクタは禁止
  SatCondBlock(
    const SatCondBlock& src
  ) = delete;

  /// @brief ムーブコンストラクタはあり
  SatCondBlock(
    SatCondBlock&& src
  ) : mSolver{src.mSolver},
      mNum{src.mNum}
  {
    src.mNum = 0;
  }

  /// @brief デストラクタ
  ~SatCondBlock();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SATソルバ
  SatSolver& mSolver;

  // 追加した要素数
  SizeType mNum;

};

END_NAMESPACE_YM_SAT

#endif // SATCONDBLOCK_H
