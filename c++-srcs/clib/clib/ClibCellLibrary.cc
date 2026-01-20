
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
  return mImpl->name();
}

// @brief テクノロジの取得
ClibTechnology
ClibCellLibrary::technology() const
{
  return mImpl->technology();
}

// @brief 遅延モデルの取得
ClibDelayModel
ClibCellLibrary::delay_model() const
{
  return mImpl->delay_model();
}

// @brief 区間のタイプの取得
ClibVarType
ClibCellLibrary::piece_type() const
{
  return mImpl->piece_type();
}

// @brief 区間のリストの取得
std::vector<double>
ClibCellLibrary::piece_define() const
{
  return mImpl->piece_define();
}

// @brief バス命名規則の取得
std::string
ClibCellLibrary::bus_naming_style() const
{
  return mImpl->bus_naming_style();
}

// @brief 日付情報の取得
std::string
ClibCellLibrary::date() const
{
  return mImpl->date();
}

// @brief リビジョン情報の取得
std::string
ClibCellLibrary::revision() const
{
  return mImpl->revision();
}

// @brief コメント情報の取得
std::string
ClibCellLibrary::comment() const
{
  return mImpl->comment();
}

// @brief 時間単位の取得
ClibTimeUnit
ClibCellLibrary::time_unit() const
{
  return ClibTimeUnit(mImpl->time_unit());
}

// @brief 電圧単位の取得
ClibVoltageUnit
ClibCellLibrary::voltage_unit() const
{
  return ClibVoltageUnit(mImpl->voltage_unit());
}

// @brief 電流単位の取得
ClibCurrentUnit
ClibCellLibrary::current_unit() const
{
  return ClibCurrentUnit(mImpl->current_unit());
}

// @brief 抵抗単位の取得
ClibResistanceUnit
ClibCellLibrary::pulling_resistance_unit() const
{
  return ClibResistanceUnit(mImpl->pulling_resistance_unit());
}

// @brief 容量単位の取得
ClibCapacitanceUnit
ClibCellLibrary::capacitive_load_unit() const
{
  return ClibCapacitanceUnit(mImpl->capacitive_load_unit(),
			     mImpl->capacitive_load_unit_str());
}

// @brief 電力単位の取得
ClibPowerUnit
ClibCellLibrary::leakage_power_unit() const
{
  return ClibPowerUnit(mImpl->leakage_power_unit());
}

// @brief このライブラリの持つセル数の取得
SizeType
ClibCellLibrary::cell_num() const
{
  return mImpl->cell_num();
}

// @brief セル情報の取得
ClibCell
ClibCellLibrary::cell(
  SizeType cell_id
) const
{
  return ClibCell(mImpl->cell(cell_id));
}

// @brief 名前からのセルの取得
ClibCell
ClibCellLibrary::cell(
  const std::string& name
) const
{
  return ClibCell(mImpl->cell(name));
}

// @brief 全セルのリストの取得
ClibCellList
ClibCellLibrary::cell_list() const
{
  return ClibCellList(mImpl->cell_list());
}

// @brief セルグループ数の取得
SizeType
ClibCellLibrary::cell_group_num() const
{
  return mImpl->cell_group_num();
}

// @brief セルグループの取得
ClibCellGroup
ClibCellLibrary::cell_group(
  SizeType id
) const
{
  return ClibCellGroup(mImpl->cell_group(id));
}

// @brief セルグループのリストの取得
ClibCellGroupList
ClibCellLibrary::cell_group_list() const
{
  return ClibCellGroupList(mImpl->cell_group_list());
}

// @brief NPN同値クラス数の取得
SizeType
ClibCellLibrary::npn_class_num() const
{
  return mImpl->npn_class_num();
}

// @brief NPN同値クラスの取得
ClibCellClass
ClibCellLibrary::npn_class(
  SizeType id
) const
{
  return ClibCellClass(mImpl->npn_class(id));
}

// @brief NPN同値クラスのリストの取得
ClibCellClassList
ClibCellLibrary::npn_class_list() const
{
  return ClibCellClassList(mImpl->npn_class_list());
}

// @brief 定数0セルのグループを返す．
ClibCellGroup
ClibCellLibrary::const0_func() const
{
  return ClibCellGroup(mImpl->const0_func());
}

// @brief 定数1セルのグループを返す．
ClibCellGroup
ClibCellLibrary::const1_func() const
{
  return ClibCellGroup(mImpl->const1_func());
}

// @brief バッファセルのグループを返す．
ClibCellGroup
ClibCellLibrary::buf_func() const
{
  return ClibCellGroup(mImpl->buf_func());
}

// @brief インバータセルのグループを返す．
ClibCellGroup
ClibCellLibrary::inv_func() const
{
  return ClibCellGroup(mImpl->inv_func());
}

// @brief ANDセルのグループを返す．
ClibCellGroup
ClibCellLibrary::and_func(
  SizeType ni
) const
{
  return ClibCellGroup(mImpl->and_func(ni));
}

// @brief NANDセルのグループを返す．
ClibCellGroup
ClibCellLibrary::nand_func(
  SizeType ni
) const
{
  return ClibCellGroup(mImpl->nand_func(ni));
}

// @brief ORセルのグループを返す．
ClibCellGroup
ClibCellLibrary::or_func(
  SizeType ni
) const
{
  return ClibCellGroup(mImpl->or_func(ni));
}

// @brief NORセルのグループを返す．
ClibCellGroup
ClibCellLibrary::nor_func(
  SizeType ni
) const
{
  return ClibCellGroup(mImpl->nor_func(ni));
}

// @brief XORセルのグループを返す．
ClibCellGroup
ClibCellLibrary::xor_func(
  SizeType ni
) const
{
  return ClibCellGroup(mImpl->xor_func(ni));
}

// @brief XNORセルのグループを返す．
ClibCellGroup
ClibCellLibrary::xnor_func(
  SizeType ni
) const
{
  return ClibCellGroup(mImpl->xnor_func(ni));
}

// @brief MUX2セルのグループを返す．
ClibCellGroup
ClibCellLibrary::mux2_func() const
{
  return ClibCellGroup(mImpl->mux2_func());
}

// @brief MUX4セルのグループを返す．
ClibCellGroup
ClibCellLibrary::mux4_func() const
{
  return ClibCellGroup(mImpl->mux4_func());
}

// @brief FFクラスを返す．
ClibCellClassList
ClibCellLibrary::find_ff_class(
  ClibSeqAttr seq_attr
) const
{
  return ClibCellClassList(mImpl->find_ff_class(seq_attr));
}

// @brief ラッチクラスを返す．
ClibCellClassList
ClibCellLibrary::find_latch_class(
  ClibSeqAttr seq_attr
) const
{
  return ClibCellClassList(mImpl->find_latch_class(seq_attr));
}

// @brief 総パタン数を返す．
SizeType
ClibCellLibrary::pg_pat_num() const
{
  return mImpl->pg_pat_num();
}

// @brief パタンを返す．
ClibPatGraph
ClibCellLibrary::pg_pat(
  SizeType id
) const
{
  return ClibPatGraph(mImpl, id);
}

// @brief パタンの最大の入力数を得る．
SizeType
ClibCellLibrary::pg_max_input() const
{
  return mImpl->pg_max_input();
}

// @brief 総ノード数を返す．
SizeType
ClibCellLibrary::pg_node_num() const
{
  return mImpl->pg_node_num();
}

// @brief ノードの種類を返す．
ClibPatType
ClibCellLibrary::pg_node_type(
  SizeType id
) const
{
  return mImpl->pg_node_type(id);
}

// @brief ノードが入力ノードの時に入力番号を返す．
SizeType
ClibCellLibrary::pg_input_id(
  SizeType id
) const
{
  return mImpl->pg_input_id(id);
}

// @brief 入力のノード番号を返す．
SizeType
ClibCellLibrary::pg_input_node(
  SizeType input_id
) const
{
  return mImpl->pg_input_node(input_id);
}

// @brief 総枝数を返す．
SizeType
ClibCellLibrary::pg_edge_num() const
{
  return mImpl->pg_edge_num();
}

// @brief 枝のファンイン元のノード番号を返す．
SizeType
ClibCellLibrary::pg_edge_from(
  SizeType id
) const
{
  return mImpl->pg_edge_from(id);
}

// @brief 枝のファンアウト先のノード番号を返す．
SizeType
ClibCellLibrary::pg_edge_to(
  SizeType id
) const
{
  return mImpl->pg_edge_to(id);
}

// @brief 枝のファンアウト先の入力位置( 0 or 1 ) を返す．
SizeType
ClibCellLibrary::pg_edge_pos(
  SizeType id
) const
{
  return mImpl->pg_edge_pos(id);
}

// @brief 枝の反転属性を返す．
// @param[in] id 枝番号 ( 0 <= id < edge_num() )
bool
ClibCellLibrary::pg_edge_inv(
  SizeType id
) const
{
  return mImpl->pg_edge_inv(id);
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
  return mImpl->dump(s);
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
