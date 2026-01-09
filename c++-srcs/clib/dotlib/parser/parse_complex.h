#ifndef PARSE_COMPLEX_H
#define PARSE_COMPLEX_H

/// @file parse_complex.h
/// @brief complex attribute 用のパース関数のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/dotlib_nsdef.h"
#include "dotlib/Parser.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @{
/// @name complex attribute を読み込む関数
//////////////////////////////////////////////////////////////////////

/// @brief ( float, float ) の形式の Complex attribute のパースを行う．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
complex_float_float(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_complex_attribute(kwd, kwd_loc, Parser::sFloatFloatHeader);
}

/// @brief ( float, float, string ) の形式の Complex attribute のパースを行う．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
complex_float_float_string(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_complex_attribute(kwd, kwd_loc, Parser::sFloatFloatStrHeader);
}

/// @brief ( float, string ) 型の Complex attribute のパースを行う．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
complex_float_string(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_complex_attribute(kwd, kwd_loc, Parser::sFloatStrHeader);
}

/// @brief ( "float, float, ... " ) の形式の Complex attribute のパースを行う．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
complex_float_vector(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_complex_attribute(kwd, kwd_loc, Parser::sFloatVectorHeader);
}

/// @brief ( integer, float ) の形式の Complex attribute のパースを行う．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
complex_int_float(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_complex_attribute(kwd, kwd_loc, Parser::sIntFloatHeader);
}

/// @brief ( integer, string ) の形式の Complex attribute のパースを行う．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
complex_int_string(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_complex_attribute(kwd, kwd_loc, Parser::sIntStrHeader);
}

/// @brief ( integer, "float, float, ... " ) の形式の Complex attribute のパースを行う．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
complex_int_float_vector(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_complex_attribute(kwd, kwd_loc, Parser::sIntFloatVectorHeader);
}

/// @brief ( integer, integer, ...  ) の形式の Complex attribute のパースを行う．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
complex_int_list(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_complex_attribute(kwd, kwd_loc, Parser::sIntListHeader);
}

/// @brief ( "integer, integer, ... " ) の形式の Complex attribute のパースを行う．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
complex_int_vector(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_complex_attribute(kwd, kwd_loc, Parser::sIntVectorHeader);
}

/// @brief ( string ) 型の Complex attribute のパースを行う．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
complex_string(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_complex_attribute(kwd, kwd_loc, Parser::sStrHeader);
}

/// @brief ( string, float ) の形式の Complex attribute のパースを行う．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
complex_string_float(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_complex_attribute(kwd, kwd_loc, Parser::sStrFloatHeader);
}

/// @brief ( string, integer ) の形式の Complex attribute のパースを行う．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
complex_string_int(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_complex_attribute(kwd, kwd_loc, Parser::sStrIntHeader);
}

/// @brief ( string, string, ... ) の形式の Complex attribute のパースを行う．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
complex_string_list(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_complex_attribute(kwd, kwd_loc, Parser::sStrListHeader);
}

/// @brief ( string, string ) の形式の Complex attribute のパースを行う．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
complex_string_string(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_complex_attribute(kwd, kwd_loc, Parser::sStrStrHeader);
}

/// @brief ( string, string, string ) の形式の Complex attribute のパースを行う．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
complex_string_string_string(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_complex_attribute(kwd, kwd_loc, Parser::sStrStrStrHeader);
}

/// @brief 'fanout_length' Complex Attribute のパースを行う．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
complex_fanout_length(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_complex_attribute(kwd, kwd_loc, Parser::sFanoutLengthHeader);
}

/// @brief 'Technology' Complex Attribute のパースを行う．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
complex_technology(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_complex_attribute(kwd, kwd_loc, Parser::sTechnologyHeader);
}

/// @brief 'values' Complex attribute のパースを行う．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
complex_values(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_complex_attribute(kwd, kwd_loc, Parser::sFloatVectorListHeader);
}

/// @brief ( vartype, vartype, ... ) の形式の Complex attribute のパースを行う．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
complex_vartype_list(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_complex_attribute(kwd, kwd_loc, Parser::sVarTypeListHeader);
}

//////////////////////////////////////////////////////////////////////
/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @{
/// @name group statement を読み込む関数
//////////////////////////////////////////////////////////////////////

/// @brief () タイプの group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_empty(
  Parser& parser,            ///< [in] パーサー
  const std::string& kwd,    ///< [in] 属性名
  const FileRegion& kwd_loc, ///< [in] 属性名の位置
  const char* group_name     ///< [in] 親のグループ名
)
{
  return parser.parse_group_statement(kwd, kwd_loc, group_name, Parser::sEmptyHeader);
}

/// @brief ( string ) タイプの group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_string(
  Parser& parser,            ///< [in] パーサー
  const std::string& kwd,    ///< [in] 属性名
  const FileRegion& kwd_loc, ///< [in] 属性名の位置
  const char* group_name     ///< [in] 親のグループ名
)
{
  return parser.parse_group_statement(kwd, kwd_loc, group_name, Parser::sStrHeader);
}

/// @brief ( string, string ) タイプの group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_string_string(
  Parser& parser,            ///< [in] パーサー
  const std::string& kwd,    ///< [in] 属性名
  const FileRegion& kwd_loc, ///< [in] 属性名の位置
  const char* group_name     ///< [in] 親のグループ名
)
{
  return parser.parse_group_statement(kwd, kwd_loc, group_name, Parser::sStrStrHeader);
}

/// @brief ( string, string, int ) タイプの group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_string_string_int(
  Parser& parser,            ///< [in] パーサー
  const std::string& kwd,    ///< [in] 属性名
  const FileRegion& kwd_loc, ///< [in] 属性名の位置
  const char* group_name     ///< [in] 親のグループ名
)
{
  return parser.parse_group_statement(kwd, kwd_loc, group_name, Parser::sStrStrIntHeader);
}

//////////////////////////////////////////////////////////////////////
/// @}
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_YM_DOTLIB

#endif // PARSE_COMPLEX_H
