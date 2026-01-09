
/// @file xor3.cc
/// @brief xor3 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "ym/BddMgr.h"


BEGIN_NAMESPACE_YM

void
test()
{
  BddMgr mgr;
  auto v0 = mgr.variable_bdd(0);
  auto v1 = mgr.variable_bdd(1);
  auto v2 = mgr.variable_bdd(2);

  std::cout << "v0: variable(0)" << std::endl;
  display(std::cout, v0);
  std::cout << "v1: variable(1)" << std::endl;
  display(std::cout, v1);
  std::cout << "v2: variable(2)" << std::endl;
  display(std::cout, v2);

  auto xor01 = v0 ^ v1;
  std::cout << "v0 ^ v1" << std::endl;
  display(std::cout, xor01);

  //auto bdd = v0 ^ v1 ^ v2;
  auto bdd = xor01 ^ v2;
  std::cout << "v0 ^ v1 ^ v2" << std::endl;
  display(std::cout, bdd);
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
