
/// @file LibraryInfo.cc
/// @brief LibraryInfo の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/Parser.h"
#include "dotlib/AstAttr.h"
#include "dotlib/LibraryInfo.h"
#include "dotlib/AstValue.h"
#include "dotlib/LuTemplInfo.h"
#include "dotlib/CellInfo.h"
#include "ym/split.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス LibraryInfo
//////////////////////////////////////////////////////////////////////

// @brief 内容を設定する．
void
LibraryInfo::set(
  const AstValue& lib_val
)
{
  GroupInfo::set(&lib_val);

  { // ライブラリ名の設定
    auto& header_val = lib_val.group_header_value();
    auto& val1 = header_val.complex_elem_value(0);
    mLibrary->set_name(val1.string_value());
  }

  SizeType nerrs = 0;

  // 属性の設定

  // 'technology' の設定
  set_technology();

  // 'delay_model' の設定
  set_delay_model();

  // 'piece_type' と 'piece_define' の設定
  set_piece_params();

  // 'bus_naming_style' の設定
  set_str_attr("bus_naming_style");

  // 'comment' の設定
  set_str_attr("comment");

  // 'date' の設定
  set_str_attr("date");

  // 'revision' の設定
  set_str_attr("revision");

  // 'time_unit' の設定
  set_str_attr("time_unit");

  // 'voltage_unit' の設定
  set_str_attr("voltage_unit");

  // 'current_unit' の設定
  set_str_attr("current_unit");

  // 'pulling_resistance_unit' の設定
  set_str_attr("pulling_resistance_unit");

  // 'capacitive_load_unit' の設定
  set_capacitive_load_unit();

  // 'leakage_power_unit' の設定
  set_str_attr("leakage_power_unit");

  // lu_table_template の設定
  if ( elem_dict().count("lu_table_template") > 0 ) {
    auto& vec = elem_dict().at("lu_table_template");
    for ( auto ast_templ: vec ) {
      try {
	LuTemplInfo info{*this};
	info.set(ast_templ);
	auto tid = info.add_lu_template();
	mLutDict.emplace(info.name(), tid);
      }
      catch ( std::invalid_argument ) {
	++ nerrs;
      }
    }
  }

  // セルの内容の設定
  if ( elem_dict().count("cell") > 0 ) {
    auto& v = elem_dict().at("cell");
    for ( auto ast_cell: v ) {
      try {
	CellInfo cell_info{*this};
	cell_info.set(ast_cell);
	cell_info.add_cell();
      }
      catch ( std::invalid_argument ) {
	++ nerrs;
      }
    }
  }

  if ( nerrs > 0 ) {
    throw std::invalid_argument{"syntax error"};
  }

  mLibrary->compile();
  mLibrary->wrap_up();
}

// @brief technology の属性をセットする．
void
LibraryInfo::set_technology()
{
  const char* keyword{"technology"};
  auto val = get_value(keyword);
  if ( val != nullptr ) {
    auto technology = val->technology_value();
    mLibrary->set_technology(technology);
  }
  else {
    // デフォルト値
    mLibrary->set_technology(ClibTechnology::cmos);
  }
}

// @brief delay_model の属性をセットする．
void
LibraryInfo::set_delay_model()
{
  const char* keyword{"delay_model"};
  auto val = get_value(keyword);
  if ( val != nullptr ) {
    auto delay_model = val->delay_model_value();
    mLibrary->set_delay_model(delay_model);
  }
}

// @brief piece_type と piece_define の設定
void
LibraryInfo::set_piece_params()
{
  const char* keyword1{"piece_type"};
  const char* keyword2{"piece_define"};
  if ( mLibrary->delay_model() == ClibDelayModel::piecewise_cmos ) {
    ClibVarType piece_type{ClibVarType::output_net_length}; // デフォルト値
    auto val = get_value(keyword1);
    if ( val != nullptr ) {
      piece_type = val->piece_type_value();
    }
    ShString tmp_str;
    if ( !get_string(keyword2, tmp_str) ) {
      auto label = "'piece_define' is missing.";
      parse_error(label);
    }
    auto str_list = split(tmp_str);
    std::vector<double> piece_list;
    piece_list.reserve(str_list.size());
    for ( auto str: str_list ) {
      piece_list.push_back(std::stod(str));
    }
    mLibrary->set_piece_params(piece_type, piece_list);
  }
  else {
    if ( get_value(keyword1) ) {
      auto label = "'piece_type' has no effects on this delay model, ignored";
      warning(label);
    }
    if ( get_value(keyword2) ) {
      auto label = "'piece_define' has no effects on this delay model, ignored";
      warning(label);
    }
  }
}

// 文字列型の属性をセットする．
void
LibraryInfo::set_str_attr(
  const char* keyword
)
{
  ShString tmp_str;
  if ( get_string(keyword, tmp_str) ) {
    mLibrary->set_attr(keyword, tmp_str);
  }
}

// @brief capacitive_load_unit の属性をセットする．
void
LibraryInfo::set_capacitive_load_unit()
{
  const char* keyword{"capacitive_load_unit"};
  auto val = get_value(keyword);
  if ( val ) {
    ASSERT_COND( val->complex_elem_size() == 2 );
    auto u = val->complex_elem_value(0).float_value();
    auto ustr = val->complex_elem_value(1).string_value();
    mLibrary->set_capacitive_load_unit(u, ustr);
  }
}

END_NAMESPACE_YM_DOTLIB

BEGIN_NAMESPACE_YM_CLIB

// @brief liberty 形式のファイルを読み込んでライブラリに設定する．
CiCellLibrary*
CiCellLibrary::read_liberty(
  const std::string& filename
)
{
  using namespace nsDotlib;

  // ファイルを開く
  std::ifstream fin{filename};
  if ( !fin ) {
    std::ostringstream buf;
    buf << filename << ": Could not open.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    FileRegion(),
		    MsgType::Failure,
		    "DOTLIB_PARSER",
		    buf.str());
    // ファイルが開けなかった．
    throw std::invalid_argument{buf.str()};
  }

  // 読み込んでASTを作る．
  Parser parser{fin, {filename}, false};
  auto ast_library = parser.parse();

  ASSERT_COND( ast_library->kwd() == "library" );

  auto lib_ptr = std::unique_ptr<CiCellLibrary>{new CiCellLibrary{}};

  LibraryInfo lib_info{lib_ptr.get()};

  // AstValue の内容をライブラリに設定する．
  lib_info.set(ast_library->value());

  auto lib = lib_ptr.get();
  lib_ptr.release();
  return lib;
}

END_NAMESPACE_YM_CLIB
