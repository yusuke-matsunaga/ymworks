
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
#include "Engine_Naive.h"


BEGIN_NAMESPACE_YM_EXSYN

void
check(
  const TruthTable& tt,
  const BcGraph& g,
  const char* exp_str
)
{
  EXPECT_TRUE( ExSyn::check_boolchain(tt, g) );

  std::ostringstream buf;
  g.print(buf);
  EXPECT_EQ( exp_str, buf.str() );
}

TEST(ExSynTest, test2)
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

  std::unordered_map<std::string, JsonValue> option_dict;
  option_dict.emplace("selecter", JsonValue("SSV"));
  auto option = JsonValue(option_dict);

  XformInfo xf(tt);
  auto& tt2 = xf.normal_func();
  for ( SizeType k = tt2.output_num(); k < 6; ++ k ) {
    Engine_Naive engine(tt2, k, 2, option);
    auto g = engine.solve();
    if ( g.input_num() == tt2.input_num() ) {
      auto g0 = xf.denormalize(g);
      g0.print(std::cout);
      break;
    }
    std::cout << "k = " << k << " failed" << std::endl;
  }
}

TEST(ExSynTest, test0660_SSV)
{
  auto tt = TruthTable::from_hex("4:1:0660");

  std::unordered_map<std::string, JsonValue> option_dict;
  option_dict.emplace("selecter", JsonValue("SSV"));
  auto option = JsonValue(option_dict);

  XformInfo xf(tt);
  auto& tt2 = xf.normal_func();
  for ( SizeType k = tt2.output_num(); k < 10; ++ k ) {
    Engine_Naive engine(tt2, k, 2, option);
    auto g = engine.solve();
    if ( g.input_num() == tt2.input_num() ) {
      auto g0 = xf.denormalize(g);
      g0.print(std::cout);
      break;
    }
    std::cout << "k = " << k << " failed" << std::endl;
  }
}

TEST(ExSynTest, test0660_OSV)
{
  auto tt = TruthTable::from_hex("4:1:0660");

  std::unordered_map<std::string, JsonValue> option_dict;
  option_dict.emplace("selecter", JsonValue("OSV"));
  auto option = JsonValue(option_dict);

  XformInfo xf(tt);
  auto& tt2 = xf.normal_func();
  for ( SizeType k = tt2.output_num(); k < 10; ++ k ) {
    Engine_Naive engine(tt2, k, 2, option);
    auto g = engine.solve();
    if ( g.input_num() == tt2.input_num() ) {
      auto g0 = xf.denormalize(g);
      g0.print(std::cout);
      break;
    }
    std::cout << "k = " << k << " failed" << std::endl;
  }
}

END_NAMESPACE_YM_EXSYN
