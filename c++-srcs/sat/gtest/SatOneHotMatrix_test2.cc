
/// @file SatOneHotMatrix_test.cc
/// @brief SatOneHotMatrix_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/SatSolver.h"
#include "ym/PermGen.h"


BEGIN_NAMESPACE_YM

TEST(SatOneHotMatrixTest2, test1)
{
  SatSolver solver;
  SizeType elem_size = 2;
  SizeType sel_size = 1;
  auto var = solver.new_onehot_matrix(elem_size, sel_size);
  ASSERT_EQ( SatBool3::True, solver.solve() );
  for ( PermGen pg(elem_size, sel_size); !pg.is_end(); ++ pg ) {
    std::vector<SatLiteral> assumptions;
    assumptions.reserve(sel_size);
    for ( SizeType i = 0; i < sel_size; ++ i ) {
      auto lit = var.var(i, pg(i));
      assumptions.push_back(lit);
    }
    auto ans = solver.solve(assumptions);
    ASSERT_EQ( SatBool3::True, ans );
    auto& model = solver.model();
    for ( SizeType sel1 = 0; sel1 < sel_size; ++ sel1 ) {
      for ( SizeType elem1 = 0; elem1 < elem_size; ++ elem1 ) {
	auto lit1 = var.var(sel1, elem1);
	auto exp_ans = elem1 == pg(sel1) ? SatBool3::True : SatBool3::False;
	std::ostringstream buf;
	buf << ", sel1 = " << sel1
	    << ", elem1 = " << elem1;
	EXPECT_EQ( exp_ans, model[lit1] ) << buf.str();
      }
    }
  }
}
END_NAMESPACE_YM
