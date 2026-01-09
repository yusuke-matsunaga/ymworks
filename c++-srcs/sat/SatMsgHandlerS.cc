
/// @file SatMsgHandlerS.cc
/// @brief SatMsgHandlerS の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SatMsgHandlerS.h"
#include "ym/SatStats.h"


BEGIN_NAMESPACE_YM_SAT

// @brief コンストラクタ
SatMsgHandlerS::SatMsgHandlerS(
  std::ostream& s
) : mS(s)
{
}

// @brief デストラクタ
SatMsgHandlerS::~SatMsgHandlerS()
{
}

// @brief ヘッダの出力
void
SatMsgHandlerS::print_header()
{
  mS << "====================[ Problem Statistics ] ========================" << std::endl
     << "|                                                                 |" << std::endl
     << "|  Number of variables:                                           |" << std::endl
     << "|  Number of clauses:                                             |" << std::endl
     << "====================[ Search Statistics ] =========================" << std::endl
     << "| conflicts |       ORIGINAL      |             LEARNT            |" << std::endl
     << "|           |   Clauses      Lits |     limit   Clauses    Lit/Cl |" << std::endl
     << "===================================================================" << std::endl;
}

// @brief メッセージ出力関数
void
SatMsgHandlerS::print_message(
  const SatStats& stats
)
{
  mS << "| "
     << std::setw(9) << stats.mConflictNum
     << " | "
     << std::setw(9) << stats.mConstrClauseNum
     << " "
     << std::setw(9) << stats.mConstrLitNum
     << " | "
     << std::setw(9) << stats.mLearntLimit
     << " "
     << std::setw(9) << stats.mLearntClauseNum
     << " " << std::setw(9);
  if ( stats.mLearntClauseNum == 0 ) {
    mS << 0;
  }
  else {
    mS << static_cast<int>(static_cast<double>(stats.mLearntLitNum) / stats.mLearntClauseNum);
  }
  mS << " |"
     << std::endl;
}

// @brief フッタの出力
void
SatMsgHandlerS::print_footer(
  const SatStats& stats
)
{
  mS << "===================================================================" << std::endl
     << "restarts          : " << stats.mRestart << std::endl
     << "conflicts         : " << stats.mConflictNum << std::endl
     << "decisions         : " << stats.mDecisionNum << std::endl
     << "propagations      : " << stats.mPropagationNum << std::endl
     << "conflict literals : " << stats.mLearntLitNum << std::endl
     << "CPU time(ms)      : " << stats.mTime.count() << std::endl;
}

END_NAMESPACE_YM_SAT
