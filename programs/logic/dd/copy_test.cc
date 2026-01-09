
/// @file copy_test.cc
/// @brief copy_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "ym/BddVar.h"
#include "ym/BddMgr.h"


BEGIN_NAMESPACE_YM

void
test()
{
  BddMgr mgr;
  auto bdd = mgr.from_truth("1101");

  auto bdd1 = bdd;

  display(std::cout, bdd1);

  SizeType ni = 2;
  std::vector<BddVar> var_list(ni);
  for ( SizeType i = 0; i < ni; ++ i ) {
    std::cout << "Var#" << i << std::endl;
    auto var = mgr.variable(i);
    auto var_bdd = var.bdd();
    display(std::cout, var_bdd);
    var_list[i] = var;
  }
  auto str = bdd.to_truth(var_list);
  std::cout << str << std::endl;
}

END_NAMESPACE_YM


int
main(
  int argc,
  char** argv
)
{
  YM_NAMESPACE::test();

  return 0;
}
