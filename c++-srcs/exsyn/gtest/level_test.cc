
/// @file level_test.cc
/// @brief level_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SatSolver.h"
#include "ym/SatCondBlock.h"
#include "ym/SatLiteral.h"
#include "ym/SatOrderedSet.h"


BEGIN_NAMESPACE_YM

void
level_test()
{
  SatSolver solver;

  auto svar = solver.new_variable(true);
  auto level1 = solver.new_ordered_set(1, 3);
  auto level2 = solver.new_ordered_set(0, 3);

  {
    auto _ = solver.condblock(svar);
    solver.add_le(level1, level2);
  }

  for ( SizeType i = 0; i <= 3; ++ i ) {
    std::vector<SatLiteral> assumptions(1);
    assumptions[0] = level2.var(i);
    auto res = solver.solve(assumptions);
    std::cout << "level = " << i << ": ";
    if ( res == SatBool3::True ) {
      std::cout << "SAT";
    }
    else {
      std::cout << "UNSAT";
    }
    std::cout << std::endl;
  }

  for ( SizeType i = 0; i <= 3; ++ i ) {
    std::vector<SatLiteral> assumptions(2);
    assumptions[0] = level2.var(i);
    assumptions[1] = svar;
    auto res = solver.solve(assumptions);
    std::cout << "with svar, level = " << i << ": ";
    if ( res == SatBool3::True ) {
      std::cout << "SAT";
    }
    else {
      std::cout << "UNSAT";
    }
    std::cout << std::endl;
  }
}

END_NAMESPACE_YM

int
main(
  int argc,
  char** argv
)
{
  nsYm::level_test();
  return 0;
}
