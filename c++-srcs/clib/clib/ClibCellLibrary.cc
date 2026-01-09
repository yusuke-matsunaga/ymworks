
/// @file ClibCellLibrary.cc
/// @brief ClibCellLibrary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ClibCellLibrary.h"
#include "ym/ClibBusType.h"
#include "ym/ClibCell.h"
#include "ym/ClibCellClass.h"
#include "ym/ClibCellGroup.h"
#include "ym/ClibPatGraph.h"
#include "ym/ClibSeqAttr.h"
#include "ci/CiCellLibrary.h"
#include "Writer.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
// クラス ClibCellLibrary
//////////////////////////////////////////////////////////////////////

// @brief mislib 形式のファイルを読み込んでライブラリに設定する．
ClibCellLibrary
ClibCellLibrary::read_mislib(
  const std::string& filename
)
{
  ClibCellLibrary lib;
  lib.mImpl = CiCellLibrary::read_mislib(filename);
  return lib;
}

// @brief liberty 形式のファイルを読み込んでライブラリに設定する．
ClibCellLibrary
ClibCellLibrary::read_liberty(
  const std::string& filename
)
{
  ClibCellLibrary lib;
  lib.mImpl = CiCellLibrary::read_liberty(filename);
  return lib;
}

// @brief 名前の取得
std::string
ClibCellLibrary::name() const
{
  if ( mImpl ) {
    return mImpl->name();
  }
  return {};
}

// @brief テクノロジの取得
ClibTechnology
ClibCellLibrary::technology() const
{
  if ( mImpl ) {
    return mImpl->technology();
  }
  // デフォルトは CMOS
  return ClibTechnology::cmos;
}

// @brief 遅延モデルの取得
ClibDelayModel
ClibCellLibrary::delay_model() const
{
  if ( mImpl ) {
    return mImpl->delay_model();
  }
  // デフォルト値
  return ClibDelayModel::generic_cmos;
}

// @brief 区間のタイプの取得
ClibVarType
ClibCellLibrary::piece_type() const
{
  if ( mImpl ) {
    return mImpl->piece_type();
  }
  // デフォルト値
  return ClibVarType::none;
}

// @brief 区間のリストの取得
std::vector<double>
ClibCellLibrary::piece_define() const
{
  if ( mImpl ) {
    return mImpl->piece_define();
  }
  // デフォルト値
  return {};
}

// @brief バス命名規則の取得
std::string
ClibCellLibrary::bus_naming_style() const
{
  if ( mImpl ) {
    return mImpl->bus_naming_style();
  }
  // デフォルト値
  return {};
}

// @brief 日付情報の取得
std::string
ClibCellLibrary::date() const
{
  if ( mImpl ) {
    return mImpl->date();
  }
  // デフォルト値
  return {};
}

// @brief リビジョン情報の取得
std::string
ClibCellLibrary::revision() const
{
  if ( mImpl ) {
    return mImpl->revision();
  }
  // デフォルト値
  return {};
}

// @brief コメント情報の取得
std::string
ClibCellLibrary::comment() const
{
  if ( mImpl ) {
    return mImpl->comment();
  }
  // デフォルト値
  return {};
}

// @brief 時間単位の取得
std::string
ClibCellLibrary::time_unit() const
{
  if ( mImpl ) {
    return mImpl->time_unit();
  }
  // デフォルト値
  return {};
}

// @brief 電圧単位の取得
std::string
ClibCellLibrary::voltage_unit() const
{
  if ( mImpl ) {
    return mImpl->voltage_unit();
  }
  // デフォルト値
  return {};
}

// @brief 電流単位の取得
std::string
ClibCellLibrary::current_unit() const
{
  if ( mImpl ) {
    return mImpl->current_unit();
  }
  // デフォルト値
  return {};
}

// @brief 抵抗単位の取得
std::string
ClibCellLibrary::pulling_resistance_unit() const
{
  if ( mImpl ) {
    return mImpl->pulling_resistance_unit();
  }
  // デフォルト値
  return {};
}

// @brief 容量単位の取得
//
// なぜかここだけインターフェイスが異なる．
double
ClibCellLibrary::capacitive_load_unit() const
{
  if ( mImpl ) {
    return mImpl->capacitive_load_unit();
  }
  // デフォルト値
  return 0.0;
}

// @brief 容量単位文字列の取得
//
// なぜかここだけインターフェイスが異なる．
std::string
ClibCellLibrary::capacitive_load_unit_str() const
{
  if ( mImpl ) {
    return mImpl->capacitive_load_unit_str();
  }
  // デフォルト値
  return {};
}

// @brief 電力単位の取得
std::string
ClibCellLibrary::leakage_power_unit() const
{
  if ( mImpl ) {
    return mImpl->leakage_power_unit();
  }
  // デフォルト値
  return {};
}

// @brief このライブラリの持つセル数の取得
SizeType
ClibCellLibrary::cell_num() const
{
  if ( mImpl ) {
    return mImpl->cell_num();
  }
  // デフォルト値
  return 0;
}

// @brief セル情報の取得
ClibCell
ClibCellLibrary::cell(
  SizeType cell_id
) const
{
  if ( mImpl ) {
    auto cell = mImpl->cell(cell_id);
    return ClibCell{cell};
  }
  // デフォルト値
  return {};
}

// @brief 名前からのセルの取得
ClibCell
ClibCellLibrary::cell(
  const std::string& name
) const
{
  if ( mImpl ) {
    auto cell = mImpl->cell(name);
    return ClibCell{cell};
  }
  // デフォルト値
  return {};
}

// @brief 全セルのリストの取得
ClibCellList
ClibCellLibrary::cell_list() const
{
  if ( mImpl ) {
    return ClibCellList{mImpl->cell_list()};
  }
  // エラー
  return {};
}

// @brief セルグループ数の取得
SizeType
ClibCellLibrary::cell_group_num() const
{
  if ( mImpl ) {
    return mImpl->cell_group_num();
  }
  return 0;
}

// @brief セルグループの取得
ClibCellGroup
ClibCellLibrary::cell_group(
  SizeType id
) const
{
  if ( mImpl ) {
    auto cg = mImpl->cell_group(id);
    return ClibCellGroup{cg};
  }
  return {};
}

// @brief セルグループのリストの取得
ClibCellGroupList
ClibCellLibrary::cell_group_list() const
{
  if ( mImpl ) {
    return ClibCellGroupList{mImpl->cell_group_list()};
  }
  // エラー
  return {};
}

// @brief NPN同値クラス数の取得
SizeType
ClibCellLibrary::npn_class_num() const
{
  if ( mImpl ) {
    return mImpl->npn_class_num();
  }
  return 0;
}

// @brief NPN同値クラスの取得
ClibCellClass
ClibCellLibrary::npn_class(
  SizeType id
) const
{
  if ( mImpl ) {
    auto cc = mImpl->npn_class(id);
    return ClibCellClass{cc};
  }
  return {};
}

// @brief NPN同値クラスのリストの取得
ClibCellClassList
ClibCellLibrary::npn_class_list() const
{
  if ( mImpl ) {
    return ClibCellClassList{mImpl->npn_class_list()};
  }
  // エラー
  return {};
}

// @brief 定数0セルのグループを返す．
ClibCellGroup
ClibCellLibrary::const0_func() const
{
  if ( mImpl ) {
    return ClibCellGroup{mImpl->const0_func()};
  }
  // デフォルト値
  return {};
}

// @brief 定数1セルのグループを返す．
ClibCellGroup
ClibCellLibrary::const1_func() const
{
  if ( mImpl ) {
    return ClibCellGroup{mImpl->const1_func()};
  }
  // デフォルト値
  return {};
}

// @brief バッファセルのグループを返す．
ClibCellGroup
ClibCellLibrary::buf_func() const
{
  if ( mImpl ) {
    return ClibCellGroup{mImpl->buf_func()};
  }
  // デフォルト値
  return {};
}

// @brief インバータセルのグループを返す．
ClibCellGroup
ClibCellLibrary::inv_func() const
{
  if ( mImpl ) {
    return ClibCellGroup{mImpl->inv_func()};
  }
  // デフォルト値
  return {};
}

// @brief ANDセルのグループを返す．
ClibCellGroup
ClibCellLibrary::and_func(
  SizeType ni
) const
{
  if ( mImpl ) {
    return ClibCellGroup{mImpl->and_func(ni)};
  }
  // デフォルト値
  return {};
}

// @brief NANDセルのグループを返す．
ClibCellGroup
ClibCellLibrary::nand_func(
  SizeType ni
) const
{
  if ( mImpl ) {
    return ClibCellGroup{mImpl->nand_func(ni)};
  }
  // デフォルト値
  return {};
}

// @brief ORセルのグループを返す．
ClibCellGroup
ClibCellLibrary::or_func(
  SizeType ni
) const
{
  if ( mImpl ) {
    return ClibCellGroup{mImpl->or_func(ni)};
  }
  // デフォルト値
  return {};
}

// @brief NORセルのグループを返す．
ClibCellGroup
ClibCellLibrary::nor_func(
  SizeType ni
) const
{
  if ( mImpl ) {
    return ClibCellGroup{mImpl->nor_func(ni)};
  }
  // デフォルト値
  return {};
}

// @brief XORセルのグループを返す．
ClibCellGroup
ClibCellLibrary::xor_func(
  SizeType ni
) const
{
  if ( mImpl ) {
    return ClibCellGroup{mImpl->xor_func(ni)};
  }
  // デフォルト値
  return {};
}

// @brief XNORセルのグループを返す．
ClibCellGroup
ClibCellLibrary::xnor_func(
  SizeType ni
) const
{
  if ( mImpl ) {
    return ClibCellGroup{mImpl->xnor_func(ni)};
  }
  // デフォルト値
  return {};
}

// @brief MUX2セルのグループを返す．
ClibCellGroup
ClibCellLibrary::mux2_func() const
{
  if ( mImpl ) {
    return ClibCellGroup{mImpl->mux2_func()};
  }
  // デフォルト値
  return {};
}

// @brief MUX4セルのグループを返す．
ClibCellGroup
ClibCellLibrary::mux4_func() const
{
  if ( mImpl ) {
    return ClibCellGroup{mImpl->mux4_func()};
  }
  // デフォルト値
  return {};
}

// @brief FFクラスを返す．
ClibCellClassList
ClibCellLibrary::find_ff_class(
  ClibSeqAttr seq_attr
) const
{
  if ( mImpl ) {
    auto class_list = mImpl->find_ff_class(seq_attr);
    return ClibCellClassList{class_list};
  }
  // デフォルト値
  return {};
}

// @brief ラッチクラスを返す．
ClibCellClassList
ClibCellLibrary::find_latch_class(
  ClibSeqAttr seq_attr
) const
{
  if ( mImpl ) {
    auto class_list = mImpl->find_latch_class(seq_attr);
    return ClibCellClassList{class_list};
  }
  // デフォルト値
  return {};
}

// @brief 総パタン数を返す．
SizeType
ClibCellLibrary::pg_pat_num() const
{
  if ( mImpl ) {
    return mImpl->pg_pat_num();
  }
  // デフォルト値
  return 0;
}

// @brief パタンを返す．
ClibPatGraph
ClibCellLibrary::pg_pat(
  SizeType id
) const
{
  if ( mImpl ) {
    return ClibPatGraph{mImpl, id};
  }
  // デフォルト値
  return {};
}

// @brief パタンの最大の入力数を得る．
SizeType
ClibCellLibrary::pg_max_input() const
{
  if ( mImpl ) {
    return mImpl->pg_max_input();
  }
  // デフォルト値
  return 0;
}

// @brief 総ノード数を返す．
SizeType
ClibCellLibrary::pg_node_num() const
{
  if ( mImpl ) {
    return mImpl->pg_node_num();
  }
  // デフォルト値
  return 0;
}

// @brief ノードの種類を返す．
ClibPatType
ClibCellLibrary::pg_node_type(
  SizeType id
) const
{
  if ( mImpl ) {
    return mImpl->pg_node_type(id);
  }
  // デフォルト値
  return ClibPatType::Input;
}

// @brief ノードが入力ノードの時に入力番号を返す．
SizeType
ClibCellLibrary::pg_input_id(
  SizeType id
) const
{
  if ( mImpl ) {
    return mImpl->pg_input_id(id);
  }
  // デフォルト値
  return 0;
}

// @brief 入力のノード番号を返す．
SizeType
ClibCellLibrary::pg_input_node(
  SizeType input_id
) const
{
  if ( mImpl ) {
    return mImpl->pg_input_node(input_id);
  }
  // デフォルト値
  return 0;
}

// @brief 総枝数を返す．
SizeType
ClibCellLibrary::pg_edge_num() const
{
  if ( mImpl ) {
    return mImpl->pg_edge_num();
  }
  // デフォルト値
  return 0;
}

// @brief 枝のファンイン元のノード番号を返す．
SizeType
ClibCellLibrary::pg_edge_from(
  SizeType id
) const
{
  if ( mImpl ) {
    return mImpl->pg_edge_from(id);
  }
  // デフォルト値
  return 0;
}

// @brief 枝のファンアウト先のノード番号を返す．
SizeType
ClibCellLibrary::pg_edge_to(
  SizeType id
) const
{
  if ( mImpl ) {
    return mImpl->pg_edge_to(id);
  }
  // デフォルト値
  return 0;
}

// @brief 枝のファンアウト先の入力位置( 0 or 1 ) を返す．
SizeType
ClibCellLibrary::pg_edge_pos(
  SizeType id
) const
{
  if ( mImpl ) {
    return mImpl->pg_edge_pos(id);
  }
  // デフォルト値
  return 0;
}

// @brief 枝の反転属性を返す．
// @param[in] id 枝番号 ( 0 <= id < edge_num() )
bool
ClibCellLibrary::pg_edge_inv(
  SizeType id
) const
{
  if ( mImpl ) {
    return mImpl->pg_edge_inv(id);
  }
  // デフォルト値
  return false;
}

// @brief 内容を出力する．
void
ClibCellLibrary::display(
  std::ostream& s
) const
{
  Writer writer{*this};
  writer.run(s);
}

// @brief 内容を表す文字列を返す．
std::string
ClibCellLibrary::to_string() const
{
  std::ostringstream buf;
  display(buf);
  return buf.str();
}

// @brief 内容をバイナリダンプする．
void
ClibCellLibrary::dump(
  std::ostream& s
) const
{
  if ( mImpl ) {
    return mImpl->dump(s);
  }
}

// @brief 内容をバイナリダンプする．
void
ClibCellLibrary::dump(
  const std::string& filename
) const
{
  std::ofstream s(filename);
  if ( !s ) {
    std::ostringstream buf;
    buf << filename << ": Could not create file";
    throw std::invalid_argument{buf.str()};
  }
  dump(s);
}

// @brief バイナリダンプされた内容を読み込む．
ClibCellLibrary
ClibCellLibrary::restore(
  std::istream& s
)
{
  auto impl = CiCellLibrary::restore(s);
  return ClibCellLibrary{impl};
}

// @brief バイナリダンプされた内容を読み込む．
ClibCellLibrary
ClibCellLibrary::restore(
  const std::string& filename
)
{
  std::ifstream s(filename);
  if ( !s ) {
    std::ostringstream buf;
    buf << filename << ": Could not open file";
    throw std::invalid_argument{buf.str()};
  }
  return restore(s);
}

END_NAMESPACE_YM_CLIB
