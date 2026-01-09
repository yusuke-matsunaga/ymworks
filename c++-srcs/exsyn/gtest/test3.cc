
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
#include "FenceGen.h"
#include "PdGen.h"
#include "Engine_PD.h"


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

TEST(ExSynTest, PD_test1)
{
  TruthTable tt(2, 1);
  for ( SizeType b = 0; b < 4; ++ b ) {
    if ( b == 3 ) {
      tt.set_value(b, 0, Val3::ONE);
    }
    else {
      tt.set_value(b, 0, Val3::ZERO);
    }
  }

  std::unordered_map<std::string, JsonValue> option_dict;
  option_dict.emplace("mode", JsonValue("PD"));
  option_dict.emplace("selecter", JsonValue("OSV"));
  auto option = JsonValue(option_dict);

  XformInfo xf(tt);
  auto& tt2 = xf.normal_func();
  FenceGen fg;
  PdGen pg;
  SizeType k = 1;
  auto fence_list = fg(k, 1);
  ASSERT_EQ( 1, fence_list.size() );
  auto pd_list = pg(fence_list[0]);
  ASSERT_EQ( 1, pd_list.size() );
  auto& pd = pd_list[0];
  std::cout << pd << std::endl;
  std::cout << tt2.bin_str() << std::endl;
  Engine_PD engine(tt2, pd, option);
  auto g = engine.solve();
  ASSERT_EQ( tt2.input_num(), g.input_num() );
  auto g0 = xf.denormalize(g);
  g0.print(std::cout);
  EXPECT_TRUE( ExSyn::check_boolchain(tt, g) );
}

TEST(ExSynTest, PD_test2)
{
  TruthTable tt(3, 1);
  for ( SizeType b = 0; b < 8; ++ b ) {
    if ( b == 7 ) {
      tt.set_value(b, 0, Val3::ONE);
    }
    else {
      tt.set_value(b, 0, Val3::ZERO);
    }
  }

  std::unordered_map<std::string, JsonValue> option_dict;
  option_dict.emplace("mode", JsonValue("PD"));
  option_dict.emplace("selecter", JsonValue("OSV"));
  auto option = JsonValue(option_dict);

  XformInfo xf(tt);
  auto& tt2 = xf.normal_func();
  FenceGen fg;
  PdGen pg;
  auto fence_list = fg(2, 2);
  ASSERT_EQ( 1, fence_list.size() );
  auto pd_list = pg(fence_list[0]);
  ASSERT_EQ( 1, pd_list.size() );
  auto& pd = pd_list[0];
  Engine_PD engine(tt2, pd, option);
  auto g = engine.solve();
  ASSERT_EQ( tt2.input_num(), g.input_num() );
  auto g0 = xf.denormalize(g);
  g0.print(std::cout);
  EXPECT_TRUE( ExSyn::check_boolchain(tt, g) );
}

TEST(ExSynTest, PD3_test1)
{
  auto tt = TruthTable::from_hex("3:1:80");

  std::unordered_map<std::string, JsonValue> option_dict;
  option_dict.emplace("selecter", JsonValue("SSV"));
  option_dict.emplace("mode", JsonValue("PD"));
  option_dict.emplace("max_input", JsonValue(3));
  auto option = JsonValue(option_dict);

  auto g = ExSyn::gen_boolchain(tt, option);
  g.print(std::cout);
  EXPECT_TRUE( ExSyn::check_boolchain(tt, g) );
}

TEST(ExSynTest, PD3_test2)
{
  auto tt = TruthTable::from_hex("4:1:8000");

  std::unordered_map<std::string, JsonValue> option_dict;
  option_dict.emplace("selecter", JsonValue("SSV"));
  option_dict.emplace("mode", JsonValue("PD"));
  option_dict.emplace("max_input", JsonValue(3));
  auto option = JsonValue(option_dict);

  auto g = ExSyn::gen_boolchain(tt, option);
  g.print(std::cout);
  EXPECT_TRUE( ExSyn::check_boolchain(tt, g) );
}

TEST(ExSynTest, PD3_test3)
{
  auto tt = TruthTable::from_hex("4:1:E883");

  std::unordered_map<std::string, JsonValue> option_dict;
  option_dict.emplace("selecter", JsonValue("SSV"));
  option_dict.emplace("mode", JsonValue("PD"));
  option_dict.emplace("max_input", JsonValue(3));
  auto option = JsonValue(option_dict);

  auto g = ExSyn::gen_boolchain(tt, option);
  g.print(std::cout);
  EXPECT_TRUE( ExSyn::check_boolchain(tt, g) );
}

END_NAMESPACE_YM_EXSYN
