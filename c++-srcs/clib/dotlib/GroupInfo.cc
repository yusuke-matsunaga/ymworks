
/// @file GroupInfo.cc
/// @brief GroupInfo の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/GroupInfo.h"
#include "dotlib/AstAttr.h"
#include "dotlib/AstValue.h"
#include "ym/ClibArea.h"
#include "ym/ClibCapacitance.h"
#include "ym/ClibResistance.h"
#include "ym/ClibTime.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス GroupInfo
//////////////////////////////////////////////////////////////////////

// @brief キーワードの値を返す．
const AstValue*
GroupInfo::get_value(
  const char* keyword
) const
{
  if ( mElemDict.count(keyword) == 0 ) {
    // なし
    return nullptr;
  }

  auto& vec = mElemDict.at(keyword);
  if ( vec.size() > 1 ) {
    // 2回以上指定されている．
    std::ostringstream buf;
    buf << keyword << " apears more than once."
	<< " previous occurance is " << vec[0]->loc();
    auto label = buf.str();
    parse_error(label);
  }

  return vec[0];
}

// @brief string の値を取り出す．
bool
GroupInfo::get_string(
  const char* keyword,
  ShString& val
) const
{
  auto _val = get_value(keyword);
  if ( _val != nullptr ) {
    val = _val->string_value();
    return true;
  }
  return false;
}

// @brief area の値を取り出す．
bool
GroupInfo::get_area(
  const char* keyword,
  ClibArea& val
) const
{
  auto _val = get_value(keyword);
  if ( _val != nullptr ) {
    val = ClibArea{_val->float_value()};
    return true;
  }
  return false;
}

// @brief capacitance の値を取り出す．
bool
GroupInfo::get_capacitance(
  const char* keyword,
  ClibCapacitance& val
) const
{
  auto _val = get_value(keyword);
  if ( _val != nullptr ) {
    val = ClibCapacitance{_val->float_value()};
    return true;
  }
  return false;
}

// @brief resistance の値を取り出す．
bool
GroupInfo::get_resistance(
  const char* keyword,
  ClibResistance& val
) const
{
  auto _val = get_value(keyword);
  if ( _val != nullptr ) {
    val = ClibResistance{_val->float_value()};
    return true;
  }
  return false;
}

// @brief time の値を取り出す．
bool
GroupInfo::get_time(
  const char* keyword,
  ClibTime& val
) const
{
  auto _val = get_value(keyword);
  if ( _val != nullptr ) {
    val = ClibTime{_val->float_value()};
    return true;
  }
  return false;
}

// @brief ClibCPV の値を取り出す．
bool
GroupInfo::get_cpv(
  const char* keyword,
  ClibCPV& val
) const
{
  auto _val = get_value(keyword);
  if ( _val != nullptr ) {
    auto tmp_str = _val->string_value();
    if ( tmp_str == "L" || tmp_str == "l" ) {
      val = ClibCPV::L;
    }
    else if ( tmp_str == "H" || tmp_str == "h" ) {
      val = ClibCPV::H;
    }
    else if ( tmp_str == "N" || tmp_str == "n" ) {
      val = ClibCPV::N;
    }
    else if ( tmp_str == "T" || tmp_str == "t" ) {
      val = ClibCPV::T;
    }
    else if ( tmp_str == "X" || tmp_str == "x" ) {
      val = ClibCPV::X;
    }
    else {
      std::ostringstream buf;
      buf << tmp_str << ": Illegal string. Only 'H', 'L', 'N', 'T', or 'X' are allowed";
      auto label = buf.str();
      parse_error(label);
    }
    return true;
  }
  return false;
}

// @brief ClibVarType の値を取り出す．
bool
GroupInfo::get_variable_type(
  const char* keyword,
  ClibVarType& val
) const
{
  auto _val= get_value(keyword);
  if ( _val != nullptr ) {
    val = _val->variable_type_value();
    return true;
  }
  return false;
}

// @brief expr の値を取り出す．
bool
GroupInfo::get_expr(
  const char* keyword,
  const AstExpr*& val
) const
{
  auto _val = get_value(keyword);
  if ( _val != nullptr ) {
    val = &(_val->expr_value());
    return true;
  }
  return false;
}

// @brief direction の値を取り出す．
bool
GroupInfo::get_direction(
  const char* keyword,
  ClibDirection& val
) const
{
  auto _val = get_value(keyword);
  if ( _val != nullptr ) {
    val = _val->direction_value();
    return true;
  }
  return false;
}

// @brief timing_type の値を取り出す．
bool
GroupInfo::get_timing_type(
  const char* keyword,
  ClibTimingType& val
) const
{
  auto _val = get_value(keyword);
  if ( _val != nullptr ) {
    val = _val->timing_type_value();
    return true;
  }
  return false;
}

// @brief timing_sense の値を取り出す．
bool
GroupInfo::get_timing_sense(
  const char* keyword,
  ClibTimingSense& val
) const
{
  auto _val = get_value(keyword);
  if ( _val != nullptr ) {
    val = _val->timing_sense_value();
    return true;
  }
  return false;
}

// @brief float_vector の値を取り出す．
bool
GroupInfo::get_float_vector(
  const char* keyword,
  std::vector<double>& val
) const
{
  auto _val = get_value(keyword);
  if ( _val != nullptr ) {
    val = _val->float_vector_value();
    return true;
  }
  return false;
}

// @brief complex 形式の float_vector の値を取り出す．
bool
GroupInfo::get_complex_float_vector(
  const char* keyword,
  std::vector<double>& val
) const
{
  auto _val = get_value(keyword);
  if ( _val != nullptr ) {
    SizeType n = _val->complex_elem_size();
    for ( SizeType i = 0; i < n; ++ i ) {
      auto tmp = _val->complex_elem_value(i).float_vector_value();
      val.insert(val.end(), tmp.begin(), tmp.end());
    }
    return true;
  }
  return false;
}

// @brief 内容を設定する．
void
GroupInfo::set(
  const AstValue* val
)
{
  mLoc = val->loc();

  SizeType n = val->group_elem_size();
  for ( SizeType i = 0; i < n; ++ i ) {
    auto& elem = val->group_elem_attr(i);
    auto kwd = elem.kwd();
    auto val = &elem.value();
    if ( mElemDict.count(kwd) == 0 ) {
      mElemDict.emplace(kwd, std::vector<const AstValue*>{val});
    }
    else {
      mElemDict.at(kwd).push_back(val);
    }
  }
}

// @brief エラー処理を行う．
void
GroupInfo::parse_error(
  const std::string& err_msg
) const
{
  MsgMgr::put_msg(__FILE__, __LINE__,
		  loc(),
		  MsgType::Error,
		  "DOTLIB_PARSER",
		  err_msg);
  throw std::invalid_argument{err_msg};
}

// @brief 警告処理を行う．
void
GroupInfo::warning(
  const std::string& warn_msg
) const
{
  MsgMgr::put_msg(__FILE__, __LINE__,
		  loc(),
		  MsgType::Warning,
		  "DOTLIB_PARSER",
		  warn_msg);
}

END_NAMESPACE_YM_DOTLIB
