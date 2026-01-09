
/// @file JsonParser.cc
/// @brief JsonParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "JsonParser.h"
#include "JsonScanner.h"
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM_JSON

// @brief コンストラクタ
JsonParser::JsonParser(
  std::istream& s
) : mScanner{s}
{
}

// @brief デストラクタ
JsonParser::~JsonParser()
{
}

// @brief 読み込む．
JsonValue
JsonParser::read()
{
  auto obj = read_value();
  auto tk = mScanner.read_token();
  if ( tk != JsonToken::End ) {
    error("syntax error");
  }
  return obj;
}

// @brief 値を読み込む．
JsonValue
JsonParser::read_value()
{
  auto tk = mScanner.read_token();
  switch ( tk ) {
  case JsonToken::String:
    return JsonValue(mScanner.cur_string());

  case JsonToken::Int:
    return JsonValue(mScanner.cur_int());

  case JsonToken::Float:
    return JsonValue(mScanner.cur_float());

  case JsonToken::LCB:
    return read_object();

  case JsonToken::LBK:
    return read_array();

  case JsonToken::True:
    return JsonValue(true);

  case JsonToken::False:
    return JsonValue(false);

  case JsonToken::Null:
    return JsonValue::null();

  default:
    // シンタックスエラー
    {
      std::ostringstream buf;
      buf << "'" << mScanner.cur_string() << "': unexpected token";
      error(buf.str());
    }
  }
  // ダミー
  return JsonValue{};
}

// @brief オブジェクトを読み込む．
JsonValue
JsonParser::read_object()
{
  auto obj = JsonValue::object();
  auto tk = mScanner.read_token();
  if ( tk == JsonToken::RCB ) {
    // 空のオブジェクト
    return obj;
  }
  mScanner.unget_token(tk);
  for ( ; ; ) {
    auto tk = mScanner.read_token();
    if ( tk == JsonToken::String ) {
      auto key = mScanner.cur_string();
      tk = mScanner.read_token();
      if ( tk != JsonToken::Colon ) {
	// ':' ではなかった．
	error("':' is expected");
      }
      auto value = read_value();
      obj.add(key, value);
    }
    else {
      // シンタックスエラー
      std::ostringstream buf;
      buf << mScanner.cur_string()
	  << ": illegal token, string is expected";
      error(buf.str());
    }

    tk = mScanner.read_token();
    if ( tk == JsonToken::RCB ) {
      break;
    }

    if ( tk != JsonToken::Comma ) {
      // シンタックスエラー
      std::ostringstream buf;
      buf << mScanner.cur_string()
	  << ": illegal token, ',' is expected";
      error(buf.str());
    }
  }
  return obj;
}

// @brief 配列を読み込む．
JsonValue
JsonParser::read_array()
{
  auto obj = JsonValue::array();
  auto tk = mScanner.read_token();
  if ( tk == JsonToken::RBK ) {
    // 空の配列
    return obj;
  }
  if ( tk == JsonToken::End ) {
    // シンタックスエラー
    error("unexpected EOF");
  }

  mScanner.unget_token(tk);
  for ( ; ; ) {
    auto value = read_value();
    obj.add(value);
    tk = mScanner.read_token();
    if ( tk == JsonToken::RBK ) {
      break;
    }
    if ( tk != JsonToken::Comma ) {
      // シンタックスエラー
      std::ostringstream buf;
      buf << mScanner.cur_string()
	  << ": illegal token, ',' is expected";
      error(buf.str());
    }
  }
  return obj;
}

// @brief エラーを出力する．
void
JsonParser::error(
  const std::string& msg
)
{
  std::ostringstream buf;
  buf << mScanner.cur_loc()
      << ": " << msg;
  throw std::invalid_argument(buf.str());
}

END_NAMESPACE_YM_JSON
