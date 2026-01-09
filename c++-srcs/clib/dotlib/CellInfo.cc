
/// @file CellInfo.cc
/// @brief CellInfo の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/CellInfo.h"
#include "dotlib/AstExpr.h"
#include "dotlib/AstValue.h"
#include "ci/CiCellLibrary.h"


BEGIN_NAMESPACE_YM_DOTLIB

// @brief 内容を設定する．
void
CellInfo::set(
  const AstValue* cell_val
)
{
  GroupInfo::set(cell_val);

  // セル名
  {
    auto& header_val = cell_val->group_header_value();
    auto& val1 = header_val.complex_elem_value(0);
    mName = val1.string_value();
  }

  // 面積
  set_area();

  // FF
  set_FF();

  // ラッチ
  set_Latch();

  // statetable(FSM)
  set_FSM();

  // ピンの属性情報を取り出す．
  set_pin();

  mInputId = 0;
  mOutputId = 0;

  // phase-1: 入力ピンの名前と入力ピン番号の辞書を作る．
  // phase-2: 出力ピンの名前と出力ピン番号の辞書を作る．
  //
  // 入力ピン番号
  //   0 - (ni - 1)      : 入力ピン
  //  ni - (ni + nb - 1) : 入出力ピン
  //  ni + nb            : Q
  //  ni + nb + 1        : Qvar
  //
  // 出力ピン番号
  //   0 - (no - 1)      : 出力ピン
  //  no - (no + nb - 1) : 入出力ピン
  for ( const auto& pininfo: mPinInfoList ) {
    if ( pininfo.direction() == ClibDirection::input ) {
      for ( auto name: pininfo.name_list() ) {
	mIpinMap.emplace(name, mInputId);
	++ mInputId;
      }
    }
    else if ( pininfo.direction() == ClibDirection::output ) {
      for ( auto name: pininfo.name_list() ) {
	mOpinMap.emplace(name, mOutputId);
	++ mOutputId;
      }
    }
  }

  // phase-3: 入出力ピンの名前と入力ピン番号/出力ピン番号の辞書を作る．
  for ( const auto& pininfo: mPinInfoList ) {
    if ( pininfo.direction() == ClibDirection::inout ) {
      for ( auto name: pininfo.name_list() ) {
	mIpinMap.emplace(name, mInputId);
	++ mInputId;
	mOpinMap.emplace(name, mOutputId);
	++ mOutputId;
      }
    }
  }

  // FF の内部状態変数を入力ピンの辞書に加える．
  if ( mHasFF ) {
    auto var1 = mFFInfo.var1();
    auto var2 = mFFInfo.var2();
    mIpinMap.emplace(var1, mInputId);
    ++ mInputId;
    mIpinMap.emplace(var2, mInputId);
    ++ mInputId;
  }

  // ラッチの内部状態変数を入力ピンの辞書に加える．
  if ( mHasLatch ) {
    auto var1 = mLatchInfo.var1();
    auto var2 = mLatchInfo.var2();
    mIpinMap.emplace(var1, mInputId);
    ++ mInputId;
    mIpinMap.emplace(var2, mInputId);
    ++ mInputId;
  }

  // FSM の内部状態変数を入力ピンの辞書に加える．
  if ( mHasFSM ) {
    auto var1 = mFSMInfo.var1();
    auto var2 = mFSMInfo.var2();
    mIpinMap.emplace(var1, mInputId);
    ++ mInputId;
    mIpinMap.emplace(var2, mInputId);
    ++ mInputId;
  }
}

// @brief セルを作る．
void
CellInfo::add_cell()
{
  CiCell* cell{nullptr};
  if ( mHasFF ) {
    // FF タイプ
    cell = add_ff_cell();
  }
  else if ( mHasLatch ) {
    // ラッチタイプ
    cell = add_latch_cell();
  }
  else if ( mHasFSM ) {
    // FSM タイプ
    cell = add_fsm_cell();
  }
  else {
    // 論理タイプ
    cell = library()->add_logic_cell(mName, mArea);
  }
  ASSERT_COND( cell != nullptr );

  // ピンを作る．
  for ( auto& pininfo: mPinInfoList ) {
    pininfo.add_pin(cell, mIpinMap);
  }

  // タイミングを作る．
  cell->init_timing_map();
  for ( auto& pininfo: mPinInfoList ) {
    pininfo.add_timing(cell, mIpinMap);
  }
}

// @brief FF セルを作る．
CiCell*
CellInfo::add_ff_cell() const
{
  auto var1 = mFFInfo.var1();
  auto var2 = mFFInfo.var2();
  auto clocked_on = mFFInfo.clocked_on()->to_expr(mIpinMap);
  auto clocked_on_also = Expr::invalid();
  if ( mFFInfo.clocked_on_also() != nullptr ) {
    clocked_on_also = mFFInfo.clocked_on_also()->to_expr(mIpinMap);
  }
  auto next_state = mFFInfo.next_state()->to_expr(mIpinMap);
  auto clear = Expr::invalid();
  if ( mFFInfo.clear() != nullptr ) {
    clear = mFFInfo.clear()->to_expr(mIpinMap);
  }
  auto preset = Expr::invalid();
  if ( mFFInfo.preset() != nullptr ) {
    preset = mFFInfo.preset()->to_expr(mIpinMap);
  }
  auto seq_attr = mFFInfo.seq_attr();
  auto cell = library()->add_ff_cell(mName, mArea,
				     var1, var2,
				     clocked_on,
				     clocked_on_also,
				     next_state,
				     clear,
				     preset,
				     seq_attr);
  return cell;
}

// @brief ラッチセルを作る．
CiCell*
CellInfo::add_latch_cell() const
{
  auto var1 = mLatchInfo.var1();
  auto var2 = mLatchInfo.var2();
  auto enable_on = Expr::invalid();
  if ( mLatchInfo.enable_on() != nullptr ) {
    enable_on = mLatchInfo.enable_on()->to_expr(mIpinMap);
  }
  auto enable_on_also = Expr::invalid();
  if ( mLatchInfo.enable_on_also() != nullptr ) {
    enable_on_also = mLatchInfo.enable_on_also()->to_expr(mIpinMap);
  }
  auto data_in = Expr::invalid();
  if ( mLatchInfo.data_in() != nullptr ) {
    data_in = mLatchInfo.data_in()->to_expr(mIpinMap);
  }
  auto clear = Expr::invalid();
  if ( mLatchInfo.clear() != nullptr ) {
    clear = mLatchInfo.clear()->to_expr(mIpinMap);
  }
  auto preset = Expr::invalid();
  if ( mLatchInfo.preset() != nullptr ) {
    preset = mLatchInfo.preset()->to_expr(mIpinMap);
  }
  auto seq_attr = mLatchInfo.seq_attr();
  auto cell = library()->add_latch_cell(mName, mArea,
					var1, var2,
					enable_on,
					enable_on_also,
					data_in,
					clear,
					preset,
					seq_attr);
  return cell;
}

// @brief FSM セルを作る．
CiCell*
CellInfo::add_fsm_cell() const
{
  auto cell = library()->add_fsm_cell(mName, mArea);
  return cell;
}

// @brief 面積を取り出す．
void
CellInfo::set_area()
{
  // 面積
  const char* keyword{"area"};
  if ( !get_area(keyword, mArea) ) {
    auto label = "'area' attribute is missing";
    parse_error(label);
  }
}

// @brief FF グループの情報を取り出す．
void
CellInfo::set_FF()
{
  mHasFF = false;
  const char* keyword{"ff"};
  auto ff_val = get_value(keyword);
  if ( ff_val ) {
    mHasFF = true;
    mFFInfo.set(ff_val);
  }
}

// @brief Latch グループの情報を取り出す．
void
CellInfo::set_Latch()
{
  mHasLatch = false;
  const char* keyword{"latch"};
  auto latch_val = get_value(keyword);
  if ( latch_val ) {
    mHasLatch = true;
    mLatchInfo.set(latch_val);
  }
}

// @brief FSM グループの情報を取り出す．
void
CellInfo::set_FSM()
{
  mHasFSM = false;
  const char* keyword{"statetable"};
  auto fsm_val = get_value(keyword);
  if ( fsm_val ) {
    mHasFSM = true;
    mFSMInfo.set(fsm_val);
  }
}

// @brief pin グループの情報を取り出す．
void
CellInfo::set_pin()
{
  const char* keyword{"pin"};
  if ( elem_dict().count(keyword) == 0 ) {
    // pin 情報がないセルはないはず．
    auto label = "No 'pin' attributes";
    parse_error(label);
  }

  auto& vec = elem_dict().at(keyword);
  SizeType npin = vec.size();
  mPinInfoList.clear();
  mPinInfoList.reserve(npin);
  for ( SizeType i = 0; i < npin; ++ i ) {
    auto pin_val = vec[i];
    mPinInfoList.push_back(PinInfo{library_info()});
    auto& pin_info = mPinInfoList.back();
    pin_info.set(pin_val);
  }
}

END_NAMESPACE_YM_DOTLIB
