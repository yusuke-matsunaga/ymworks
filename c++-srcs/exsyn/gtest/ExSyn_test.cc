
/// @file ExSyn_test.cc
/// @brief ExSyn_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/ExSyn.h"
#include "ym/JsonValue.h"
#include "XformInfo.h"


BEGIN_NAMESPACE_YM_EXSYN

class ExSynTest :
public ::testing::TestWithParam<std::tuple<std::string, // sat
					   std::string, // selecter
					   std::string, // mode
					   int,         // max_input
					   bool>>       // cegar

{
public:

  void
  do_test(
    const TruthTable& tt
  )
  {
    auto& tuple_val = GetParam();
    auto sat_str = std::get<0>(tuple_val);
    auto sel_str = std::get<1>(tuple_val);
    auto mode = std::get<2>(tuple_val);
    auto max_input = std::get<3>(tuple_val);
    auto cegar = std::get<4>(tuple_val);
    if ( mode == "" && max_input == 3 ) {
      return;
    }
    std::unordered_map<std::string, JsonValue> opt_dict;
    opt_dict.emplace("sat", JsonValue(sat_str));
    opt_dict.emplace("selecter", JsonValue(sel_str));
    opt_dict.emplace("mode", JsonValue(mode));
    opt_dict.emplace("max_input", JsonValue(max_input));
    opt_dict.emplace("cegar", JsonValue(cegar));
    auto option = JsonValue(opt_dict);
    {
      std::cout << tt.bin_str() << std::endl
		<< "sat:              " << sat_str << std::endl
		<< "selecter:         " << sel_str << std::endl
		<< "mode:             " << mode << std::endl
		<< "max_input:        " << max_input << std::endl
		<< "cegar:            " << cegar << std::endl;
      XformInfo xf(tt);
      std::cout << "normal func:      " << xf.normal_func().bin_str() << std::endl
		<< "xform info:       " << xf << std::endl;
    }
    auto g = ExSyn::gen_boolchain(tt, option);
    if ( !ExSyn::check_boolchain(tt, g) ) {
      g.print(std::cout);
      tt.print(std::cout);
    }
    EXPECT_TRUE( ExSyn::check_boolchain(tt, g) );
  }

};

TEST_P(ExSynTest, test0000)
{
  TruthTable tt(2, 1);

  tt.set_value(0, 0, Val3::ZERO);
  tt.set_value(1, 0, Val3::ZERO);
  tt.set_value(2, 0, Val3::ZERO);
  tt.set_value(3, 0, Val3::ZERO);

  do_test(tt);
}

TEST_P(ExSynTest, test0001)
{
  TruthTable tt(2, 1);

  tt.set_value(0, 0, Val3::ZERO);
  tt.set_value(1, 0, Val3::ZERO);
  tt.set_value(2, 0, Val3::ZERO);
  tt.set_value(3, 0, Val3::ONE);

  do_test(tt);
}

TEST_P(ExSynTest, test0010)
{
  TruthTable tt(2, 1);

  tt.set_value(0, 0, Val3::ZERO);
  tt.set_value(1, 0, Val3::ZERO);
  tt.set_value(2, 0, Val3::ONE);
  tt.set_value(3, 0, Val3::ZERO);

  do_test(tt);
}

TEST_P(ExSynTest, test0011)
{
  TruthTable tt(2, 1);

  tt.set_value(0, 0, Val3::ZERO);
  tt.set_value(1, 0, Val3::ZERO);
  tt.set_value(2, 0, Val3::ONE);
  tt.set_value(3, 0, Val3::ONE);

  do_test(tt);
}

TEST_P(ExSynTest, test0100)
{
  TruthTable tt(2, 1);

  tt.set_value(0, 0, Val3::ZERO);
  tt.set_value(1, 0, Val3::ONE);
  tt.set_value(2, 0, Val3::ZERO);
  tt.set_value(3, 0, Val3::ZERO);

  do_test(tt);
}

TEST_P(ExSynTest, test0101)
{
  TruthTable tt(2, 1);

  tt.set_value(0, 0, Val3::ZERO);
  tt.set_value(1, 0, Val3::ONE);
  tt.set_value(2, 0, Val3::ZERO);
  tt.set_value(3, 0, Val3::ONE);

  do_test(tt);
}

TEST_P(ExSynTest, test0110)
{
  TruthTable tt(2, 1);

  tt.set_value(0, 0, Val3::ZERO);
  tt.set_value(1, 0, Val3::ONE);
  tt.set_value(2, 0, Val3::ONE);
  tt.set_value(3, 0, Val3::ZERO);

  do_test(tt);
}

TEST_P(ExSynTest, test0111)
{
  TruthTable tt(2, 1);

  tt.set_value(0, 0, Val3::ZERO);
  tt.set_value(1, 0, Val3::ONE);
  tt.set_value(2, 0, Val3::ONE);
  tt.set_value(3, 0, Val3::ONE);

  do_test(tt);
}

TEST_P(ExSynTest, test1000)
{
  TruthTable tt(2, 1);

  tt.set_value(0, 0, Val3::ONE);
  tt.set_value(1, 0, Val3::ZERO);
  tt.set_value(2, 0, Val3::ZERO);
  tt.set_value(3, 0, Val3::ZERO);

  do_test(tt);
}

TEST_P(ExSynTest, test1001)
{
  TruthTable tt(2, 1);

  tt.set_value(0, 0, Val3::ONE);
  tt.set_value(1, 0, Val3::ZERO);
  tt.set_value(2, 0, Val3::ZERO);
  tt.set_value(3, 0, Val3::ONE);

  do_test(tt);
}

TEST_P(ExSynTest, test1010)
{
  TruthTable tt(2, 1);

  tt.set_value(0, 0, Val3::ONE);
  tt.set_value(1, 0, Val3::ZERO);
  tt.set_value(2, 0, Val3::ONE);
  tt.set_value(3, 0, Val3::ZERO);

  do_test(tt);
}

TEST_P(ExSynTest, test1011)
{
  TruthTable tt(2, 1);

  tt.set_value(0, 0, Val3::ONE);
  tt.set_value(1, 0, Val3::ZERO);
  tt.set_value(2, 0, Val3::ONE);
  tt.set_value(3, 0, Val3::ONE);

  do_test(tt);
}

TEST_P(ExSynTest, test1100)
{
  TruthTable tt(2, 1);

  tt.set_value(0, 0, Val3::ONE);
  tt.set_value(1, 0, Val3::ONE);
  tt.set_value(2, 0, Val3::ZERO);
  tt.set_value(3, 0, Val3::ZERO);

  do_test(tt);
}

TEST_P(ExSynTest, test1101)
{
  TruthTable tt(2, 1);

  tt.set_value(0, 0, Val3::ONE);
  tt.set_value(1, 0, Val3::ONE);
  tt.set_value(2, 0, Val3::ZERO);
  tt.set_value(3, 0, Val3::ONE);

  do_test(tt);
}

TEST_P(ExSynTest, test1110)
{
  TruthTable tt(2, 1);

  tt.set_value(0, 0, Val3::ONE);
  tt.set_value(1, 0, Val3::ONE);
  tt.set_value(2, 0, Val3::ONE);
  tt.set_value(3, 0, Val3::ZERO);

  do_test(tt);
}

TEST_P(ExSynTest, test1111)
{
  TruthTable tt(2, 1);

  tt.set_value(0, 0, Val3::ONE);
  tt.set_value(1, 0, Val3::ONE);
  tt.set_value(2, 0, Val3::ONE);
  tt.set_value(3, 0, Val3::ONE);

  do_test(tt);
}

TEST_P(ExSynTest, test00000001)
{
  TruthTable tt(3, 1);

  tt.set_value(0, 0, Val3::ZERO);
  tt.set_value(1, 0, Val3::ZERO);
  tt.set_value(2, 0, Val3::ZERO);
  tt.set_value(3, 0, Val3::ZERO);
  tt.set_value(4, 0, Val3::ZERO);
  tt.set_value(5, 0, Val3::ZERO);
  tt.set_value(6, 0, Val3::ZERO);
  tt.set_value(7, 0, Val3::ONE);

  do_test(tt);
}

TEST_P(ExSynTest, test01111111)
{
  TruthTable tt(3, 1);

  tt.set_value(0, 0, Val3::ZERO);
  tt.set_value(1, 0, Val3::ONE);
  tt.set_value(2, 0, Val3::ONE);
  tt.set_value(3, 0, Val3::ONE);
  tt.set_value(4, 0, Val3::ONE);
  tt.set_value(5, 0, Val3::ONE);
  tt.set_value(6, 0, Val3::ONE);
  tt.set_value(7, 0, Val3::ONE);

  do_test(tt);
}

TEST_P(ExSynTest, test00000111)
{
  TruthTable tt(3, 1);

  tt.set_value(0, 0, Val3::ZERO);
  tt.set_value(1, 0, Val3::ZERO);
  tt.set_value(2, 0, Val3::ZERO);
  tt.set_value(3, 0, Val3::ZERO);
  tt.set_value(4, 0, Val3::ZERO);
  tt.set_value(5, 0, Val3::ONE);
  tt.set_value(6, 0, Val3::ONE);
  tt.set_value(7, 0, Val3::ONE);

  do_test(tt);
}

TEST_P(ExSynTest, test2)
{
  TruthTable tt(2, 16);
  for ( SizeType o = 0; o < 16; ++ o ) {
    for ( SizeType b = 0; b < 4; ++ b ) {
      if ( o & (1 << (3 - b)) ) {
	tt.set_value(b, o, Val3::ONE);
      }
      else {
	tt.set_value(b, o, Val3::ZERO);
      }
    }
  }

  do_test(tt);
}

INSTANTIATE_TEST_SUITE_P(ExSynTest, ExSynTest,
			 ::testing::Combine(
			   ::testing::Values("lingeling", "minisat2", "ymsat2"),
			   ::testing::Values("SSV", "MSV", "OSV"),
			   ::testing::Values("", "PD"),
			   ::testing::Values(2, 3),
			   ::testing::Bool()));

END_NAMESPACE_YM_EXSYN
