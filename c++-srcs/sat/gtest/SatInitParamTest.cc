
/// @file SatInitParamTest.cc
/// @brief SatInitParamTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "ym/SatInitParam.h"


BEGIN_NAMESPACE_YM

TEST(SatInitParamTest, const1)
{
  auto type = std::string{"ymsat2"};
  SatInitParam param{type};

  EXPECT_EQ( type, param.type() );
}

TEST(SatInitParamTest, const2)
{
  const char* type = "ymsat2";
  SatInitParam param{type};

  EXPECT_EQ( std::string{type}, param.type() );
}

TEST(SatInitParamTest, const3)
{
  const char* conf_str = "{'type': 'ymsat2'}";
  auto js_obj = JsonValue::parse(conf_str);
  SatInitParam param{js_obj};

  EXPECT_EQ( "ymsat2", param.type() );
}

TEST(SatInitParamTest, const4)
{
  auto type = std::string{"ymsat2"};
  auto tmp_dict = std::unordered_map<std::string, JsonValue>{
    {"type", JsonValue{type}},
    {"foo", JsonValue{"foo"}}
  };
  JsonValue js_obj{tmp_dict};
  SatInitParam param{js_obj};

  EXPECT_EQ( std::string{type}, param.type() );

  auto js_obj2 = param.js_obj();
  EXPECT_EQ( js_obj, js_obj2 );
}

TEST(SatInitParamTest, bad_const1)
{
  auto type = std::string{"badsat"};
  ASSERT_THROW( SatInitParam{type}, std::invalid_argument );
}

TEST(SatInitParamTest, bad_const2)
{
  const char* type = "badsat";
  ASSERT_THROW( SatInitParam{type}, std::invalid_argument );
}

TEST(SatInitParamTest, bad_const3)
{
  auto type = std::string{"badsat"};
  auto tmp_dict = std::unordered_map<std::string, JsonValue>{
    {"type", JsonValue{type}}
  };
  JsonValue js_obj{tmp_dict};
  ASSERT_THROW( SatInitParam param{js_obj}, std::invalid_argument );
}

TEST(SatInitParamTest, from_json1)
{
  auto data_dir = std::filesystem::path{TESTDATA_DIR} / "sat";
  auto filename = data_dir / "ymsat.json";
  auto param = SatInitParam::from_json(filename);

  EXPECT_EQ( "glueminisat2", param.type() );

  auto js_obj = param.js_obj();
  EXPECT_EQ( "bar", js_obj["foo"].get_string() );
}

TEST(SatInitParamTest, from_json2)
{
  ASSERT_THROW( SatInitParam::from_json("/foo/bar"), std::invalid_argument);
}

END_NAMESPACE_YM
