#ifndef AIG2CNF_H
#define AIG2CNF_H

/// @file Aig2Cnf.h
/// @brief Aig2Cnf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SatSolver.h"
#include "ym/AigMgr.h"
#include "ym/AigLiteral.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
/// @class Aig2Cnf Aig2Cnf.h "Aig2Cnf.h"
/// @brief AIG を CNF に変換する補助クラス
//////////////////////////////////////////////////////////////////////
class Aig2Cnf
{
public:

  /// @brief コンストラクタ
  Aig2Cnf(
    const AigMgr& aig_mgr,        ///< [in] AIGマネージャ
    SatSolver& solver,            ///< [in] SATソルバ
    const SatLiteralMap& lit_map  ///< [in] AIG の入力番号とリテラルの対応関係を表す辞書
  ) : mAigMgr{aig_mgr},
      mSolver{solver},
      mLitMap{lit_map}
  {
  }

  /// @brief デストラクタ
  ~Aig2Cnf() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief AIG を CNF に変換する．
  /// @return リテラルのリストを返す．
  ///
  /// - 与えられた AIG の値が 1 となる条件を表すリテラルを返す．
  /// - 否定しても 0 となる条件にはならないことに注意
  std::vector<SatLiteral>
  make_cnf(
    const AigLiteral& aig
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // AigMgr
  const AigMgr& mAigMgr;

  // SATソルバ
  SatSolver& mSolver;

  // 入力番号とリテラルの対応関係を表す辞書
  SatLiteralMap mLitMap;

  // AigLiteral をキーにして対応する SatLiteral のリストを記憶する辞書
  std::unordered_map<AigLiteral, std::vector<SatLiteral>> mAigDict;

};

END_NAMESPACE_YM_SAT

#endif // AIG2CNF_H
