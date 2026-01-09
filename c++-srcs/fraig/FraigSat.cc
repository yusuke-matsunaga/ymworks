
/// @file FraigSat.cc
/// @brief FraigSat の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "FraigSat.h"
#include "FraigNode.h"
#include "ym/Timer.h"
#include "ym/SatStats.h"


#if defined(YM_DEBUG)
#define DEBUG_FLAG 1
#endif

#if !defined(DEBUG_FLAG)
#define DEBUG_FLAG 0
#endif


BEGIN_NAMESPACE_YM_FRAIG

BEGIN_NONAMESPACE

const int debug = DEBUG_FLAG;

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// FraigSat
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FraigSat::FraigSat(
  const SatInitParam& init_param
) : mSolver{init_param},
    mLogLevel{0},
    mLogStream{new std::ofstream("/dev/null")}
{
}

// @brief デストラクタ
FraigSat::~FraigSat()
{
  if ( mLogStream != &std::cout ) {
    delete mLogStream;
  }
}

// @brief ノードを登録する．
void
FraigSat::reg_node(
  FraigNode* node
)
{
  auto var = mSolver.new_variable(true);
  mLiteralDict.emplace(node->id(), var);
}

// @brief ノードの入出力の関係を表す CNF 式を作る．
void
FraigSat::make_cnf(
  FraigNode* node
)
{
  auto lito = node_lit(node);
  auto lit1 = node_lit(node->fanin0()) * node->fanin0_inv();
  auto lit2 = node_lit(node->fanin1()) * node->fanin1_inv();
  mSolver.add_clause(~lit1, ~lit2, lito);
  mSolver.add_clause( lit1, ~lito);
  mSolver.add_clause( lit2, ~lito);
}

// @brief ログレベルを設定する．
void
FraigSat::set_loglevel(
  SizeType level
)
{
  mLogLevel = level;
}

// @brief ログ出力用ストリームを設定する．
void
FraigSat::set_logstream(
  std::ostream* out
)
{
  if ( mLogStream != &std::cout ) {
    delete mLogStream;
  }
  mLogStream = out;
}

// node が定数かどうか調べる．
SatBool3
FraigSat::check_const(
  FraigNode* node,
  bool inv
)
{
  if ( debug ) {
    std::cout << "CHECK CONST";
    if ( inv ) {
      std::cout << "1";
    }
    else {
      std::cout << "0";
    }
    std::cout << " " << std::setw(6) << node_lit(node)
	      << "       ";
    std::cout.flush();
  }

  Timer timer;
  timer.start();

  auto lit = node_lit(node) * inv;

  // この関数の戻り値
  auto code = SatBool3::X;

  // lit = 1 が成り立つか調べる
  auto stat = check_condition(lit);
  if ( stat == SatBool3::False ) {
    // 成り立たないということは lit = 0
    mSolver.add_clause(~lit);
    if ( debug ) {
      std::cout << "\tSUCCEED"
		<< std::endl;
    }
    code = SatBool3::True;
  }
  else if ( stat == SatBool3::True ) {
    // 成り立ったということは ~lit が常に成り立つわけではない．
    if ( debug ) {
      std::cout << "\tFAILED"
		<< std::endl;
    }
    code = SatBool3::False;
  }
  else {
    // 分からない．
    if ( debug ) {
      std::cout << "\tABORTED"
		<< std::endl;
    }
  }
  mCheckConstInfo.set_result(code, timer.get_time());
  return code;
}

// node1 と node2 機能的に等価かどうか調べる．
SatBool3
FraigSat::check_equiv(
  FraigNode* node1,
  FraigNode* node2,
  bool inv
)
{
  auto id1 = node_lit(node1);
  auto id2 = node_lit(node2);

  if ( debug ) {
    std::cout << "CHECK EQUIV  "
	      << std::setw(6) << id1 << " "
	      << std::setw(6) << id2;
    if ( inv ) {
      std::cout << " N";
    }
    std::cout.flush();
  }

  Timer timer;
  timer.start();

  auto lit1 = id1;
  auto lit2 = id2 * inv;

  // この関数の戻り値
  auto code = SatBool3::X;

  // 等価でない条件
  // - lit1 = 0 かつ lit2 = 1 が成り立つ
  // - lit0 = 1 かつ lit2 = 0 が成り立つ
  auto stat = check_condition(~lit1,  lit2);
  if ( stat == SatBool3::False ) {
    stat = check_condition( lit1, ~lit2);
    if ( stat == SatBool3::False ) {
      // どの条件も成り立たなかったので等しい
      mSolver.add_clause(~lit1,  lit2);
      mSolver.add_clause( lit1, ~lit2);

      if ( debug ) {
	std::cout << "\tSUCCEED"
		  << std::endl;
      }
      code = SatBool3::True;
      goto end;
    }
  }
  if ( stat == SatBool3::True ) {
    if ( debug ) {
      std::cout << "\tFAILED"
		<< std::endl;
    }
    code = SatBool3::False;
  }
  else {
    if ( debug ) {
      std::cout << "\tABORTED"
		<< std::endl;
    }
  }

 end:
  mCheckEquivInfo.set_result(code, timer.get_time());
  return code;
}

// @brief ノードに対応するリテラルを得る．
SatLiteral
FraigSat::node_lit(
  FraigNode* node
)
{
  ASSERT_COND( mLiteralDict.count(node->id()) > 0 );
  return mLiteralDict.at(node->id());
}

// lit1 が成り立つか調べる．
SatBool3
FraigSat::check_condition(
  SatLiteral lit1
)
{
  auto assumptions = std::vector<SatLiteral>{lit1};
  auto ans1 = mSolver.solve(assumptions);

#if defined(VERIFY_SATSOLVER)
  SatSolver solver{SatInitParam{"minisat2"}};
  for ( auto node: mAllNodes ) {
    auto lit = solver.new_variable();
    ASSERT_COND( lit.varid() == node_lit(node).varid() );
    if ( node->is_and() ) {
      auto lito = lit;
      auto lit1 = node_lit(node->fanin0()) * node->fanin0_inv();
      auto lit2 = node_lit(node->fanin1()) * node->fanin1_inv();
      solver.add_clause(~lit1, ~lit2, lito);
      solver.add_clause( lit1, ~lito);
      solver.add_clause( lit2, ~lito);
    }
  }
  auto ans2 = solver.solve(assumptions);
  if ( ans1 != ans2 ) {
    std::cout << std::endl
	      << "ERROR!"
	      << std::endl
	      << "check_condition(" << lit1 << ")"
	      << std::endl
	      << " ans1 = " << ans1
	      << std::endl
	      << " ans2 = " << ans2
	      << std::endl
	      << " clauses"
	      << std::endl;
    for ( auto node: mAllNodes ) {
      if ( node->is_and() ) {
	auto lito = node_lit(node);
	auto lit1 = node_lit(node->fanin0()) * node->fanin0_inv();
	auto lit2 = node_lit(node->fanin1()) * node->fanin1_inv();
	std::cout << "   " << ~lit1 << " + " << ~lit2 << " + " << lito
		  << std::endl
		  << "   " << lit1 << " + " << ~lito
		  << std::endl
		  << "   " << lit2 << " + " << ~lito
		  << std::endl;
      }
    }
  }
#endif
  return ans1;
}

// lit1 & lit2 が成り立つか調べる．
SatBool3
FraigSat::check_condition(
  SatLiteral lit1,
  SatLiteral lit2
)
{
  auto assumptions = std::vector<SatLiteral>{lit1, lit2};
  auto ans1 = mSolver.solve(assumptions);

#if defined(VERIFY_SATSOLVER)
  SatSolver solver{SatInitParam{"minisat2"}};
  for ( auto node: mAllNodes ) {
    auto lit = solver.new_variable();
    ASSERT_COND( lit.varid() == node_lit(node).varid() );
    if ( node->is_and() ) {
      auto lito = lit;
      auto lit1 = node_lit(node->fanin0()) * node->fanin0_inv();
      auto lit2 = node_lit(node->fanin1()) * node->fanin1_inv();
      solver.add_clause(~lit1, ~lit2, lito);
      solver.add_clause(lit1, ~lito);
      solver.add_clause(lit2, ~lito);
    }
  }
  auto ans2 = solver.solve(assumptions);
  if ( ans1 != ans2 ) {
    std::cout << std::endl
	      << "ERROR!"
	      << std::endl
	      << "check_condition("
	      << lit1 << " & " << lit2 << ")"
	      << std::endl
	      << " ans1 = " << ans1
	      << std::endl
	      << " ans2 = " << ans2
	      << std::endl
	      << " clauses"
	      << std::endl;
    for ( auto node: mAllNodes ) {
      if ( node->is_and() ) {
	auto lito = node_lit(node);
	auto lit1 = node_lit(node->fanin0()) * node->fanin0_inv();
	auto lit2 = node_lit(node->fanin1()) * node->fanin1_inv();
	std::cout << "   " << ~lit1 << " + " << ~lit2 << " + " << lito
		  << std::endl
		  << "   " << lit1 << " + " << ~lito
		  << std::endl
		  << "   " << lit2 << " + " << ~lito
		  << std::endl;
      }
    }
  }
#endif
  return ans1;
}

// @brief 直前の sat_sweep に関する統計情報を出力する．
void
FraigSat::dump_stats(
  std::ostream& s
)
{
  s << "=====<< AigMgr Statistics >> ====="
    << std::endl
    << "check_const:"
    << std::endl;
  mCheckConstInfo.dump(s);
  s << "----------------------------------"
    << std::endl
    << "check_equiv:"
    << std::endl;
  mCheckEquivInfo.dump(s);
  s << std::endl;

  auto stats = mSolver.get_stats();
  s << "----------------------------------"
    << std::endl
    << "sat stat:"
    << std::endl
    << "  restarts          : " << stats.mRestart
    << std::endl
    << "  conflicts         : " << stats.mConflictNum
    << std::endl
    << "  decisions         : " << stats.mDecisionNum
    << std::endl
    << "  propagations      : " << stats.mPropagationNum
    << std::endl
    << "  conflict literals : " << stats.mLearntLitNum
    << std::endl;
}

FraigSat::SatStat::SatStat()
{
  mTotalCount = 0;
  for ( auto i: { 0, 1, 2 } ) {
    mTimeStat[i].mCount = 0;
    mTimeStat[i].mTotalTime = 0.0;
    mTimeStat[i].mMaxTime = 0.0;
  }
}

void
FraigSat::SatStat::set_result(
  SatBool3 code,
  double t
)
{
  ++ mTotalCount;

  int idx = 0;
  if ( code == SatBool3::True ) {
    idx = 1;
  }
  else if ( code == SatBool3::False ) {
    idx = 2;
  }

  ++ mTimeStat[idx].mCount;
  mTimeStat[idx].mTotalTime += t;
  if ( mTimeStat[idx].mMaxTime < t ) {
    mTimeStat[idx].mMaxTime = t;
  }
}

void
FraigSat::SatStat::dump(
  std::ostream& s
) const
{
  s << mTimeStat[1].mCount << " / " << mTotalCount
    << std::endl;
  if ( mTimeStat[1].mCount > 0 ) {
    s << " In success(total/ave./max): "
      << mTimeStat[1].mTotalTime << " / "
      << mTimeStat[1].mTotalTime / mTimeStat[1].mCount << " / "
      << mTimeStat[1].mMaxTime
      << std::endl;
  }
  if ( mTimeStat[2].mCount > 0 ) {
    s << " In failure(total/ave./max): "
      << mTimeStat[2].mTotalTime << " / "
      << mTimeStat[2].mTotalTime / mTimeStat[2].mCount << " / "
      << mTimeStat[2].mMaxTime
      << std::endl;
  }
  if ( mTimeStat[0].mCount > 0 ) {
    s << " In abort(total/ave./max): "
      << mTimeStat[0].mTotalTime << " / "
      << mTimeStat[0].mTotalTime / mTimeStat[0].mCount << " / "
      << mTimeStat[0].mMaxTime
      << std::endl;
  }
}

END_NAMESPACE_YM_FRAIG
