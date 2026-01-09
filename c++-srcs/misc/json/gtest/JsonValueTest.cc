
/// @file JsonValueTest.cc
/// @brief JsonValueTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM

class JsonValueTest:
  public ::testing::Test
{
public:

  void
  null_test(
    const JsonValue& json_obj,
    const std::string& type
  );

  void
  string_test(
    const JsonValue& json_obj,
    const std::string& type,
    const std::string& exp_value = {}
  );

  void
  int_test(
    const JsonValue& json_obj,
    const std::string& type,
    int exp_value = 0
  );

  void
  float_test(
    const JsonValue& json_obj,
    const std::string& type,
    double exp_value = 0.0
  );

  void
  bool_test(
    const JsonValue& json_obj,
    const std::string& type,
    bool exp_value = false
  );

  void
  object_test(
    const JsonValue& json_obj,
    const std::string& type,
    const std::unordered_map<std::string, JsonValue>& exp_value = {}
  );

  void
  array_test(
    const JsonValue& json_obj,
    const std::string& type,
    const std::vector<JsonValue>& exp_value = {}
  );

};

void
JsonValueTest::null_test(
  const JsonValue& json_obj,
  const std::string& type
)
{
  if ( type == "null" ) {
    EXPECT_TRUE( json_obj.is_null() );
  }
  else {
    EXPECT_FALSE( json_obj.is_null() );
  }
}

void
JsonValueTest::string_test(
  const JsonValue& json_obj,
  const std::string& type,
  const std::string& exp_value
)
{
  if ( type == "string" ) {
    EXPECT_TRUE( json_obj.is_string() );
    EXPECT_EQ( exp_value, json_obj.get_string() );
  }
  else {
    EXPECT_FALSE( json_obj.is_string() );
    EXPECT_THROW( json_obj.get_string(),
		  std::logic_error );
  }
}

void
JsonValueTest::int_test(
  const JsonValue& json_obj,
  const std::string& type,
  int exp_value
)
{
  if ( type == "int" ) {
    EXPECT_TRUE( json_obj.is_number() );
    EXPECT_TRUE( json_obj.is_int() );
    EXPECT_EQ( exp_value, json_obj.get_int() );
  }
  else {
    if ( type != "float" ) {
      EXPECT_FALSE( json_obj.is_number() );
    }
    EXPECT_FALSE( json_obj.is_int() );
    EXPECT_THROW( json_obj.get_int(),
		  std::logic_error );
  }
}

void
JsonValueTest::float_test(
  const JsonValue& json_obj,
  const std::string& type,
  double exp_value
)
{
  if ( type == "float" ) {
    EXPECT_TRUE( json_obj.is_number() );
    EXPECT_TRUE( json_obj.is_float() );
    EXPECT_EQ( exp_value, json_obj.get_float() );
  }
  else {
    if ( type != "int" ) {
      EXPECT_FALSE( json_obj.is_number() );
    }
    EXPECT_FALSE( json_obj.is_float() );
    EXPECT_THROW( json_obj.get_float(),
		  std::logic_error );
  }
}

void
JsonValueTest::bool_test(
  const JsonValue& json_obj,
  const std::string& type,
  const bool exp_value
)
{
  if ( type == "bool" ) {
    EXPECT_TRUE( json_obj.is_bool() );
    EXPECT_EQ( exp_value, json_obj.get_bool() );
  }
  else {
    EXPECT_FALSE( json_obj.is_bool() );
    EXPECT_THROW( json_obj.get_bool(),
		  std::logic_error );
  }
}

void
JsonValueTest::object_test(
  const JsonValue& json_obj,
  const std::string& type,
  const std::unordered_map<std::string, JsonValue>& exp_value
)
{
  if ( type == "object" ) {
    EXPECT_TRUE( json_obj.is_object() );
    EXPECT_EQ( exp_value.size(), json_obj.size() );
    for ( auto& p: exp_value ) {
      auto key = p.first;
      auto value = p.second;
      EXPECT_TRUE( json_obj.has_key(key) );
      EXPECT_EQ( value, json_obj[key] );
      EXPECT_EQ( value, json_obj.at(key) );
    }
    auto key_list = json_obj.key_list();
    for ( auto key: key_list ) {
      EXPECT_TRUE( exp_value.count(key) > 0 );
    }
    auto item_list = json_obj.item_list();
    for ( auto& p: item_list ) {
      auto key = p.first;
      auto value = p.second;
      ASSERT_TRUE( exp_value.count(key) > 0 );
      EXPECT_EQ( value, exp_value.at(key) );
    }
  }
  else {
    EXPECT_FALSE( json_obj.is_object() );
    EXPECT_THROW( json_obj.has_key("abc"),
		  std::logic_error );
    EXPECT_THROW( json_obj.key_list(),
		  std::logic_error );
    EXPECT_THROW( json_obj.item_list(),
		  std::logic_error );
    EXPECT_THROW( json_obj["abc"],
		  std::logic_error );
    EXPECT_THROW( json_obj.at("abc"),
		  std::logic_error );
  }
}

void
JsonValueTest::array_test(
  const JsonValue& json_obj,
  const std::string& type,
  const std::vector<JsonValue>& exp_value
)
{
  if ( type == "array" ) {
    EXPECT_TRUE( json_obj.is_array() );
    SizeType n = exp_value.size();
    EXPECT_EQ( n, json_obj.size() );
    for ( SizeType i = 0; i < n; ++ i ) {
      EXPECT_EQ( exp_value[i], json_obj[i] );
      EXPECT_EQ( exp_value[i], json_obj.at(i) );
    }
    EXPECT_THROW( json_obj[n],
		  std::out_of_range );
    EXPECT_THROW( json_obj.at(n),
		  std::out_of_range );
  }
  else {
    if ( type != "object" ) {
      EXPECT_THROW( json_obj.size(),
		    std::logic_error );
    }
    EXPECT_FALSE( json_obj.is_array() );
    EXPECT_THROW( json_obj[0],
		  std::logic_error );
    EXPECT_THROW( json_obj.at(0),
		  std::logic_error );
  }
}

TEST_F(JsonValueTest, null)
{
  JsonValue json_obj;

  auto type = std::string("null");
  null_test(json_obj, type);
  string_test(json_obj, type);
  int_test(json_obj, type);
  float_test(json_obj, type);
  bool_test(json_obj, type);
  object_test(json_obj, type);
  array_test(json_obj, type);

  // to_json()
  EXPECT_EQ( "null", json_obj.to_json() );

  // parse()
  auto copy_obj = JsonValue::parse(json_obj.to_json());
  EXPECT_EQ( copy_obj, json_obj );

  // eq
  JsonValue another_obj;
  EXPECT_TRUE( another_obj == json_obj );
  JsonValue int_obj{0};
  EXPECT_TRUE( int_obj != json_obj );
}

TEST_F(JsonValueTest, null2)
{
  auto json_obj = JsonValue::null();

  auto type = std::string("null");
  null_test(json_obj, type);
  string_test(json_obj, type);
  int_test(json_obj, type);
  float_test(json_obj, type);
  bool_test(json_obj, type);
  object_test(json_obj, type);
  array_test(json_obj, type);

  // to_json()
  EXPECT_EQ( "null", json_obj.to_json() );

  // parse()
  auto copy_obj = JsonValue::parse(json_obj.to_json());
  EXPECT_EQ( copy_obj, json_obj );

  // eq
  JsonValue another_obj;
  EXPECT_TRUE( another_obj == json_obj );
  JsonValue int_obj{0};
  EXPECT_TRUE( int_obj != json_obj );
}

TEST_F(JsonValueTest, array0)
{
  auto json_obj = JsonValue::array();

  auto type = std::string("array");
  std::vector<JsonValue> value{};
  null_test(json_obj, type);
  string_test(json_obj, type);
  int_test(json_obj, type);
  float_test(json_obj, type);
  bool_test(json_obj, type);
  object_test(json_obj, type);
  array_test(json_obj, type, value);

  // to_json()
  EXPECT_EQ( "[]", json_obj.to_json() );

  // parse()
  auto copy_obj = JsonValue::parse(json_obj.to_json());
  EXPECT_EQ( copy_obj, json_obj );

  // eq
  auto another_obj = JsonValue::array();
  EXPECT_TRUE( another_obj == json_obj );
  auto int_obj = JsonValue(0);
  EXPECT_TRUE( int_obj != json_obj );
}

TEST_F(JsonValueTest, object0)
{
  auto json_obj = JsonValue::object();

  auto type = std::string("object");
  std::unordered_map<std::string, JsonValue> value{};
  null_test(json_obj, type);
  string_test(json_obj, type);
  int_test(json_obj, type);
  float_test(json_obj, type);
  bool_test(json_obj, type);
  object_test(json_obj, type, value);
  array_test(json_obj, type);

  EXPECT_FALSE( json_obj.has_key("abc") );
  EXPECT_THROW( json_obj["abc"],
		std::out_of_range );
  EXPECT_THROW( json_obj.at("abc"),
		std::out_of_range );

  // to_json()
  EXPECT_EQ( "{}", json_obj.to_json() );

  // parse()
  auto copy_obj = JsonValue::parse(json_obj.to_json());
  EXPECT_EQ( copy_obj, json_obj );

  // eq
  auto another_obj = JsonValue::object();
  EXPECT_TRUE( another_obj == json_obj );
  auto int_obj = JsonValue(0);
  EXPECT_TRUE( int_obj != json_obj );
}

TEST_F(JsonValueTest, string1)
{
  const char* src_value = "abcde";
  auto json_obj = JsonValue(src_value);

  auto type = std::string("string");
  auto value = std::string(src_value);
  null_test(json_obj, type);
  string_test(json_obj, type, value);
  int_test(json_obj, type);
  float_test(json_obj, type);
  bool_test(json_obj, type);
  object_test(json_obj, type);
  array_test(json_obj, type);

  // to_json()
  std::ostringstream buf;
  buf << '"' << value << '"';
  EXPECT_EQ( buf.str(), json_obj.to_json() );

  // parse()
  auto copy_obj = JsonValue::parse(json_obj.to_json());
  EXPECT_EQ( copy_obj, json_obj );

  // eq
  JsonValue another_obj{value};
  EXPECT_TRUE( another_obj == json_obj );
  JsonValue int_obj{0};
  EXPECT_TRUE( int_obj != json_obj );
}

TEST_F(JsonValueTest, string2)
{
  auto value = std::string("abcde");
  auto json_obj = JsonValue(value);

  auto type = std::string("string");
  null_test(json_obj, type);
  string_test(json_obj, type, value);
  int_test(json_obj, type);
  float_test(json_obj, type);
  bool_test(json_obj, type);
  object_test(json_obj, type);
  array_test(json_obj, type);

  // to_json()
  std::ostringstream buf;
  buf << '"' << value << '"';
  EXPECT_EQ( buf.str(), json_obj.to_json() );

  // parse()
  auto copy_obj = JsonValue::parse(json_obj.to_json());
  EXPECT_EQ( copy_obj, json_obj );

  // eq
  JsonValue another_obj{value};
  EXPECT_TRUE( another_obj == json_obj );
  JsonValue int_obj{0};
  EXPECT_TRUE( int_obj != json_obj );
}

TEST_F(JsonValueTest, string_dq)
{
  auto value = std::string("\"abcde\"");
  auto json_obj = JsonValue(value);

  auto type = std::string("string");
  null_test(json_obj, type);
  string_test(json_obj, type, value);
  int_test(json_obj, type);
  float_test(json_obj, type);
  bool_test(json_obj, type);
  object_test(json_obj, type);
  array_test(json_obj, type);

  // double-quote を含む文字列を表す json 文字列は
  // single-quote を用いる．
  EXPECT_EQ( "'\"abcde\"'", json_obj.to_json() );
}

TEST_F(JsonValueTest, string_sq)
{
  auto value = std::string("'abcde'");
  auto json_obj = JsonValue(value);

  auto type = std::string("string");
  null_test(json_obj, type);
  string_test(json_obj, type, value);
  int_test(json_obj, type);
  float_test(json_obj, type);
  bool_test(json_obj, type);
  object_test(json_obj, type);
  array_test(json_obj, type);

  // single-quote を含む文字列を表す json 文字列は
  // double-quote を用いる．
  EXPECT_EQ( "\"'abcde'\"", json_obj.to_json() );
}

TEST_F(JsonValueTest, string_dqsq)
{
  auto value = std::string("\"'abcde'\"");
  auto json_obj = JsonValue(value);

  auto type = std::string("string");
  null_test(json_obj, type);
  string_test(json_obj, type, value);
  int_test(json_obj, type);
  float_test(json_obj, type);
  bool_test(json_obj, type);
  object_test(json_obj, type);
  array_test(json_obj, type);

  // single-quote と double-quote を含む文字列を表す json 文字列は
  // double-quote を用い，double-quote をエスケープする．
  EXPECT_EQ( R"("\"'abcde'\"")", json_obj.to_json() );
}

TEST_F(JsonValueTest, int)
{
  int value = 99;
  auto json_obj = JsonValue(value);

  auto type = std::string("int");
  null_test(json_obj, type);
  string_test(json_obj, type);
  int_test(json_obj, type, value);
  float_test(json_obj, type);
  bool_test(json_obj, type);
  object_test(json_obj, type);
  array_test(json_obj, type);

  // to_json()
  std::ostringstream buf;
  buf << value;
  EXPECT_EQ( buf.str(), json_obj.to_json() );

  // parse()
  auto copy_obj = JsonValue::parse(json_obj.to_json());
  EXPECT_EQ( copy_obj, json_obj );

  // eq
  JsonValue another_obj{value};
  EXPECT_TRUE( another_obj == json_obj );
  JsonValue str_obj{"abc"};
  EXPECT_TRUE( str_obj != json_obj );
}

TEST_F(JsonValueTest, float)
{
  double value = 1.2345;
  auto json_obj = JsonValue(value);

  auto type = std::string("float");
  null_test(json_obj, type);
  string_test(json_obj, type);
  int_test(json_obj, type);
  float_test(json_obj, type, value);
  bool_test(json_obj, type);
  object_test(json_obj, type);
  array_test(json_obj, type);

  // to_json()
  std::ostringstream buf;
  buf << value;
  EXPECT_EQ( buf.str(), json_obj.to_json() );

  // parse()
  auto copy_obj = JsonValue::parse(json_obj.to_json());
  EXPECT_EQ( copy_obj, json_obj );

  // eq
  JsonValue another_obj{value};
  EXPECT_TRUE( another_obj == json_obj );
  JsonValue str_obj{"abc"};
  EXPECT_TRUE( str_obj != json_obj );
}

TEST_F(JsonValueTest, bool_true)
{
  bool value = true;
  auto json_obj = JsonValue(value);

  auto type = std::string("bool");
  null_test(json_obj, type);
  string_test(json_obj, type);
  int_test(json_obj, type);
  float_test(json_obj, type);
  bool_test(json_obj, type, value);
  object_test(json_obj, type);
  array_test(json_obj, type);

  // to_json()
  EXPECT_EQ( "true", json_obj.to_json() );

  // parse()
  auto copy_obj = JsonValue::parse(json_obj.to_json());
  EXPECT_EQ( copy_obj, json_obj );

  // eq
  JsonValue another_obj{true};
  EXPECT_TRUE( another_obj == json_obj );
  JsonValue str_obj{"abc"};
  EXPECT_TRUE( str_obj != json_obj );
}

TEST_F(JsonValueTest, bool_false)
{
  bool value = false;
  JsonValue json_obj{value};

  auto type = std::string("bool");
  null_test(json_obj, type);
  string_test(json_obj, type);
  int_test(json_obj, type);
  float_test(json_obj, type);
  bool_test(json_obj, type, value);
  object_test(json_obj, type);
  array_test(json_obj, type);

  // to_json()
  EXPECT_EQ( "false", json_obj.to_json() );

  // parse()
  auto copy_obj = JsonValue::parse(json_obj.to_json());
  EXPECT_EQ( copy_obj, json_obj );

  // eq
  JsonValue another_obj{false};
  EXPECT_TRUE( another_obj == json_obj );
  JsonValue str_obj{"abc"};
  EXPECT_TRUE( str_obj != json_obj );
}

TEST_F(JsonValueTest, array1)
{
  std::string value1 = "xyz";
  JsonValue json1{value1};

  int value2 = 2;
  JsonValue json2{value2};

  float value3 = 0.99;
  JsonValue json3{value3};

  auto value = std::vector<JsonValue>{json1, json2, json3};
  auto json_obj = JsonValue(value);

  auto type = std::string("array");
  null_test(json_obj, type);
  string_test(json_obj, type);
  int_test(json_obj, type);
  float_test(json_obj, type);
  bool_test(json_obj, type);
  object_test(json_obj, type);
  array_test(json_obj, type, value);

  // to_json()
  const char* exp_str = "[\"xyz\",2,0.99]";
  EXPECT_EQ( exp_str, json_obj.to_json() );

  // parse()
  auto copy_obj = JsonValue::parse(json_obj.to_json());
  EXPECT_EQ( exp_str, copy_obj.to_json() );

  // eq
  JsonValue another_obj{value};
  EXPECT_TRUE( another_obj == json_obj );
  JsonValue str_obj{"abc"};
  EXPECT_TRUE( str_obj != json_obj );
}

TEST_F(JsonValueTest, array2)
{
  std::string value1 = "xyz";
  JsonValue json1{value1};

  int value2 = 2;
  JsonValue json2{value2};

  float value3 = 0.99;
  JsonValue json3{value3};

  auto json_obj = JsonValue::array();
  json_obj.add(json1);
  json_obj.add(json2);
  json_obj.add(json3);

  auto type = std::string("array");
  auto value = std::vector<JsonValue>{json1, json2, json3};
  null_test(json_obj, type);
  string_test(json_obj, type);
  int_test(json_obj, type);
  float_test(json_obj, type);
  bool_test(json_obj, type);
  object_test(json_obj, type);
  array_test(json_obj, type, value);

  // to_json()
  const char* exp_str = "[\"xyz\",2,0.99]";
  EXPECT_EQ( exp_str, json_obj.to_json() );

  // parse()
  auto copy_obj = JsonValue::parse(json_obj.to_json());
  EXPECT_EQ( exp_str, copy_obj.to_json() );

  // eq
  JsonValue another_obj{value};
  EXPECT_TRUE( another_obj == json_obj );
  JsonValue str_obj{"abc"};
  EXPECT_TRUE( str_obj != json_obj );
}

TEST_F(JsonValueTest, object1)
{
  std::string value1 = "xyz";
  JsonValue json1{value1};

  int value2 = 2;
  JsonValue json2{value2};

  float value3 = 0.99;
  JsonValue json3{value3};

  std::unordered_map<std::string, JsonValue> value{
    {"key1", json1},
    {"key2", json2},
    {"key3", json3}
  };
  auto json_obj = JsonValue(value);

  auto type = std::string("object");
  null_test(json_obj, type);
  string_test(json_obj, type);
  int_test(json_obj, type);
  float_test(json_obj, type);
  bool_test(json_obj, type);
  object_test(json_obj, type, value);
  array_test(json_obj, type);

  EXPECT_THROW( json_obj["abc"],
		std::out_of_range );
  EXPECT_THROW( json_obj.at("abc"),
		std::out_of_range );

  // to_json()
  const char* exp_str = "{\"key1\":\"xyz\",\"key2\":2,\"key3\":0.99}";
  EXPECT_EQ( exp_str, json_obj.to_json() );

  // parse()
  auto copy_obj = JsonValue::parse(json_obj.to_json());
  EXPECT_EQ( exp_str, copy_obj.to_json() );

  // eq
  JsonValue another_obj{value};
  EXPECT_TRUE( another_obj == json_obj );
  JsonValue str_obj{"abc"};
  EXPECT_TRUE( str_obj != json_obj );
}

TEST_F(JsonValueTest, object2)
{
  std::string value1 = "xyz";
  JsonValue json1{value1};

  int value2 = 2;
  JsonValue json2{value2};

  float value3 = 0.99;
  JsonValue json3{value3};

  auto json_obj = JsonValue::object();
  json_obj.add("key1", json1);
  json_obj.add("key2", json2);
  json_obj.add("key3", json3);

  auto type = std::string("object");
  std::unordered_map<std::string, JsonValue> value{
    {"key1", json1},
    {"key2", json2},
    {"key3", json3}
  };
  null_test(json_obj, type);
  string_test(json_obj, type);
  int_test(json_obj, type);
  float_test(json_obj, type);
  bool_test(json_obj, type);
  object_test(json_obj, type, value);
  array_test(json_obj, type);

  EXPECT_THROW( json_obj["abc"],
		std::out_of_range );
  EXPECT_THROW( json_obj.at("abc"),
		std::out_of_range );

  // to_json()
  const char* exp_str = "{\"key1\":\"xyz\",\"key2\":2,\"key3\":0.99}";
  EXPECT_EQ( exp_str, json_obj.to_json() );

  // parse()
  auto copy_obj = JsonValue::parse(json_obj.to_json());
  EXPECT_EQ( exp_str, copy_obj.to_json() );

  // eq
  JsonValue another_obj{value};
  EXPECT_TRUE( another_obj == json_obj );
  JsonValue str_obj{"abc"};
  EXPECT_TRUE( str_obj != json_obj );
}

END_NAMESPACE_YM
