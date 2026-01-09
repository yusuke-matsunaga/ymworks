
/// @file CiCell.cc
/// @brief CiCell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ci/CiCell.h"
#include "ci/CiCellGroup.h"
#include "ci/CiCellClass.h"
#include "ci/CiCellLibrary.h"
#include "ci/CiPin.h"
#include "ci/CiBus.h"
#include "ci/CiBundle.h"
#include "ci/CiTiming.h"
#include "cgmgr/CgSignature.h"
#include "ci/Serializer.h"
#include "ci/Deserializer.h"
#include "ym/Range.h"
#include "CiLogicCell.h"
#include "CiFFCell.h"
#include "CiLatchCell.h"
#include "CiFsmCell.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
// クラス CiCell
//////////////////////////////////////////////////////////////////////

// @brief 親のライブラリを返す．
const CiCellLibrary*
CiCell::library() const
{
  return mGroup->library();
}

// @brief 参照回数を増やす．
void
CiCell::inc_ref() const
{
  library()->inc_ref();
}

// @brief 参照回数を減らす．
void
CiCell::dec_ref() const
{
  library()->dec_ref();
}

// @brief セルの種類を返す．
ClibCellType
CiCell::type() const
{
  return ClibCellType::Logic;
}

// @brief 組み合わせ論理タイプの時 true を返す．
bool
CiCell::is_logic() const
{
  return true;
}

// @brief FFタイプの時 true を返す．
bool
CiCell::is_ff() const
{
  return false;
}

// @brief ラッチタイプの時 true を返す．
bool
CiCell::is_latch() const
{
  return false;
}

// @brief FSMタイプの時 true を返す．
bool
CiCell::is_fsm() const
{
  return false;
}

// @brief 内部変数1の名前を返す．
std::string
CiCell::qvar1() const
{
  return std::string{};
}

// @brief 内部変数1の名前を返す．
std::string
CiCell::qvar2() const
{
  return std::string{};
}

// @brief FFセル/ラッチセルの場合にクリア条件を表す論理式を返す．
Expr
CiCell::clear_expr() const
{
  return Expr::invalid();
}

// @brief FFセル/ラッチセルの場合にプリセット条件を表す論理式を返す．
Expr
CiCell::preset_expr() const
{
  return Expr::invalid();
}

// @brief 順序セルの属性を返す．
ClibSeqAttr
CiCell::seq_attr() const
{
  return ClibSeqAttr{};
}

// @brief FFセルの場合にクロックのアクティブエッジを表す論理式を返す．
Expr
CiCell::clock_expr() const
{
  return Expr::invalid();
}

// @brief FFセルの場合にスレーブクロックのアクティブエッジを表す論理式を返す．
Expr
CiCell::clock2_expr() const
{
  return Expr::invalid();
}

// @brief FFセルの場合に次状態関数を表す論理式を返す．
Expr
CiCell::next_state_expr() const
{
  return Expr::invalid();
}

// @brief ラッチセルの場合にイネーブル条件を表す論理式を返す．
Expr
CiCell::enable_expr() const
{
  return Expr::invalid();
}

// @brief ラッチセルの場合に2つめのイネーブル条件を表す論理式を返す．
Expr
CiCell::enable2_expr() const
{
  return Expr::invalid();
}

// @brief ラッチセルの場合にデータ入力関数を表す論理式を返す．
Expr
CiCell::data_in_expr() const
{
  return Expr::invalid();
}

// @brief 名前からピンの取得
const CiPin*
CiCell::find_pin(
  const ShString& name
) const
{
  auto lib = library();
  if ( lib != nullptr ) {
    return lib->find_pin(this, name);
  }
  return nullptr;
}

// @brief 名前からバスの取得
const CiBus*
CiCell::find_bus(
  const ShString& name
) const
{
  auto lib = library();
  if ( lib != nullptr ) {
    return lib->find_bus(this, name);
  }
  return nullptr;
}

// @brief 名前からバスの取得
const CiBundle*
CiCell::find_bundle(
  const ShString& name
) const
{
  auto lib = library();
  if ( lib != nullptr ) {
    return lib->find_bundle(this, name);
  }
  return nullptr;
}

// @brief 入力ピンを追加する．
CiPin*
CiCell::add_input(
  const ShString& name,
  ClibCapacitance capacitance,
  ClibCapacitance rise_capacitance,
  ClibCapacitance fall_capacitance
)
{
  SizeType pid = mPinList.size();
  SizeType iid = mInputList.size();
  auto ptr = CiPin::new_Input(pid, iid, name,
			      capacitance,
			      rise_capacitance,
			      fall_capacitance);
  auto pin = ptr.get();
  mPinList.push_back(std::move(ptr));
  mInputList.push_back(pin);
  ++ mInputNum;
  return pin;
}

// @brief 出力ピンを追加する．
CiPin*
CiCell::add_output(
  const ShString& name,
  ClibCapacitance max_fanout,
  ClibCapacitance min_fanout,
  ClibCapacitance max_capacitance,
  ClibCapacitance min_capacitance,
  ClibTime max_transition,
  ClibTime min_transition,
  const Expr& function,
  const Expr& tristate
)
{
  SizeType pid = mPinList.size();
  SizeType oid = mOutputList.size();
  auto ptr = CiPin::new_Output(pid, oid, name,
			       max_fanout, min_fanout,
			       max_capacitance, min_capacitance,
			       max_transition, min_transition,
			       function, tristate);
  auto pin = ptr.get();
  mPinList.push_back(std::move(ptr));
  mOutputList.push_back(pin);
  ++ mOutputNum;
  return pin;
}

// @brief 入出力ピンを追加する．
CiPin*
CiCell::add_inout(
  const ShString& name,
  ClibCapacitance capacitance,
  ClibCapacitance rise_capacitance,
  ClibCapacitance fall_capacitance,
  ClibCapacitance max_fanout,
  ClibCapacitance min_fanout,
  ClibCapacitance max_capacitance,
  ClibCapacitance min_capacitance,
  ClibTime max_transition,
  ClibTime min_transition,
  const Expr& function,
  const Expr& tristate
)
{
  SizeType pid = mPinList.size();
  SizeType iid = mInputList.size();
  SizeType oid = mOutputList.size();
  auto ptr = CiPin::new_Inout(pid, iid, oid, name,
			      capacitance,
			      rise_capacitance,
			      fall_capacitance,
			      max_fanout, min_fanout,
			      max_capacitance, min_capacitance,
			      max_transition, min_transition,
			      function, tristate);
  auto pin = ptr.get();
  mPinList.push_back(std::move(ptr));
  mInputList.push_back(pin);
  mOutputList.push_back(pin);
  ++ mInoutNum;
  return pin;
}

// @brief 内部ピンを追加する．
CiPin*
CiCell::add_internal(
  const ShString& name
)
{
  // mPinList には含まれない．
  auto iid = mInternalList.size();
  auto ptr = CiPin::new_Internal(iid, name);
  auto pin = ptr.get();
  mInternalList.push_back(std::move(ptr));
  return pin;
}

// @brief バスを追加する．
CiBus*
CiCell::add_bus(
  const ShString& name,
  const CiBusType* bus_type,
  const std::vector<const CiPin*>& pin_list
)
{
  auto bus = new CiBus{name, bus_type, pin_list};
  mBusList.push_back(std::unique_ptr<CiBus>{bus});
  return bus;
}

// @brief バンドルを追加する．
CiBundle*
CiCell::add_bundle(
  const ShString& name,
  const std::vector<const CiPin*>& pin_list
)
{
  auto bundle = new CiBundle{name, pin_list};
  mBundleList.push_back(std::unique_ptr<CiBundle>{bundle});
  return bundle;
}

// @brief タイミング情報を作る(ジェネリック遅延モデル)．
CiTiming*
CiCell::add_timing_generic(
  ClibTimingType type,
  const Expr& cond,
  ClibTime intrinsic_rise,
  ClibTime intrinsic_fall,
  ClibTime slope_rise,
  ClibTime slope_fall,
  ClibResistance rise_resistance,
  ClibResistance fall_resistance
)
{
  auto ptr = CiTiming::new_Generic(type, cond,
				   intrinsic_rise,
				   intrinsic_fall,
				   slope_rise,
				   slope_fall,
				   rise_resistance,
				   fall_resistance);
  auto timing = ptr.get();
  mTimingList.push_back(std::move(ptr));
  return timing;
}

// @brief タイミング情報を作る(折れ線近似)．
CiTiming*
CiCell::add_timing_piecewise(
  ClibTimingType timing_type,
  const Expr& cond,
  ClibTime intrinsic_rise,
  ClibTime intrinsic_fall,
  ClibTime slope_rise,
  ClibTime slope_fall,
  const std::vector<ClibResistance>& rise_pin_resistance,
  const std::vector<ClibResistance>& fall_pin_resistance,
  const std::vector<ClibTime>& rise_delay_intercept,
  const std::vector<ClibTime>& fall_delay_intercept
)
{
  auto ptr = CiTiming::new_Piecewise(timing_type, cond,
				     intrinsic_rise,
				     intrinsic_fall,
				     slope_rise,
				     slope_fall,
				     rise_pin_resistance,
				     fall_pin_resistance,
				     rise_delay_intercept,
				     fall_delay_intercept);
  auto timing = ptr.get();
  mTimingList.push_back(std::move(ptr));
  return timing;
}

// @brief タイミング情報を作る(非線形タイプ1)．
CiTiming*
CiCell::add_timing_lut_cell(
  ClibTimingType timing_type,
  const Expr& cond,
  std::unique_ptr<CiLut>&& cell_rise,
  std::unique_ptr<CiLut>&& cell_fall,
  std::unique_ptr<CiLut>&& rise_transition,
  std::unique_ptr<CiLut>&& fall_transition
)
{
  auto ptr = CiTiming::new_Lut_cell(timing_type, cond,
				    std::move(cell_rise),
				    std::move(cell_fall),
				    std::move(rise_transition),
				    std::move(fall_transition));
  auto timing = ptr.get();
  mTimingList.push_back(std::move(ptr));
  return timing;
}

// @brief タイミング情報を作る(非線形タイプ2)．
CiTiming*
CiCell::add_timing_lut_prop(
  ClibTimingType timing_type,
  const Expr& cond,
  std::unique_ptr<CiLut>&& rise_transition,
  std::unique_ptr<CiLut>&& fall_transition,
  std::unique_ptr<CiLut>&& rise_propagation,
  std::unique_ptr<CiLut>&& fall_propagation
)
{
  auto ptr = CiTiming::new_Lut_prop(timing_type, cond,
				    std::move(rise_transition),
				    std::move(fall_transition),
				    std::move(rise_propagation),
				    std::move(fall_propagation));
  auto timing = ptr.get();
  mTimingList.push_back(std::move(ptr));
  return timing;
}

// @brief タイミング情報を作る(非線形タイプ1)．
CiTiming*
CiCell::add_timing_lut_cell(
  ClibTimingType timing_type,
  const Expr& cond,
  std::unique_ptr<CiStLut>&& cell_rise,
  std::unique_ptr<CiStLut>&& cell_fall,
  std::unique_ptr<CiStLut>&& rise_transition,
  std::unique_ptr<CiStLut>&& fall_transition
)
{
  auto ptr = CiTiming::new_Lut_cell(timing_type, cond,
				    std::move(cell_rise),
				    std::move(cell_fall),
				    std::move(rise_transition),
				    std::move(fall_transition));
  auto timing = ptr.get();
  mTimingList.push_back(std::move(ptr));
  return timing;
}

// @brief タイミング情報を作る(非線形タイプ2)．
CiTiming*
CiCell::add_timing_lut_prop(
  ClibTimingType timing_type,
  const Expr& cond,
  std::unique_ptr<CiStLut>&& rise_transition,
  std::unique_ptr<CiStLut>&& fall_transition,
  std::unique_ptr<CiStLut>&& rise_propagation,
  std::unique_ptr<CiStLut>&& fall_propagation
)
{
  auto ptr = CiTiming::new_Lut_prop(timing_type, cond,
				    std::move(rise_transition),
				    std::move(fall_transition),
				    std::move(rise_propagation),
				    std::move(fall_propagation));
  auto timing = ptr.get();
  mTimingList.push_back(std::move(ptr));
  return timing;
}

// @brief タイミング情報用のデータ構造を初期化する．
void
CiCell::init_timing_map()
{
  SizeType map_size = input2_num() * output2_num() * 2;
  mTimingMap.clear();
  mTimingMap.resize(map_size, {});
}

// @brief タイミング情報をセットする．
void
CiCell::set_timing(
  SizeType ipin_id,
  SizeType opin_id,
  ClibTimingSense timing_sense,
  const std::vector<const CiTiming*>& timing_list
)
{
  SizeType base = (opin_id * input2_num() + ipin_id) * 2;
  switch ( timing_sense ) {
  case ClibTimingSense::positive_unate: base += 0; break;
  case ClibTimingSense::negative_unate: base += 1; break;
  default: ASSERT_NOT_REACHED;
  }
  mTimingMap[base] = timing_list;
}

// @brief シグネチャを返す．
CgSignature
CiCell::make_signature() const
{
  SizeType ni = input_num();
  SizeType no = output_num();
  SizeType nb = inout_num();
  SizeType ni2 = ni + nb;
  SizeType no2 = no + nb;
  if ( no == 1 && nb == 0 ) {
    auto opin = output(0);
    auto expr = opin->function();
    if ( expr.is_valid() ) {
      // 1出力，双方向なし
      auto tri_expr = opin->tristate();
      if ( tri_expr.is_valid() ) {
	// tristate あり
	auto func = expr.tvfunc(ni2);
	auto tristate = tri_expr.tvfunc(ni2);
	return CgSignature::make_logic_sig(func, tristate);
      }
      // tristate なし
      return CgSignature::make_logic_sig(ni2, expr);
    }
  }
  std::vector<TvFunc> logic_list(no2);
  std::vector<TvFunc> tristate_list(no2);
  for ( SizeType i = 0; i < no2; ++ i ) {
    auto opin = output(i);
    logic_list[i] = opin->function().tvfunc(ni2);
    tristate_list[i] = opin->tristate().tvfunc(ni2);
  }
  return CgSignature::make_logic_sig(ni, no, nb, logic_list, tristate_list);
}

// @brief 内容をシリアライズする．
void
CiCell::serialize(
  Serializer& s
) const
{
  s.reg_obj(this);
  for ( auto& pin: mPinList ) {
    pin->serialize(s);
  }
  for ( auto& pin: mInternalList ) {
    pin->serialize(s);
  }
  for ( auto& bus: mBusList ) {
    bus->serialize(s);
  }
  for ( auto& bundle: mBundleList ) {
    bundle->serialize(s);
  }
  for ( auto& timing: mTimingList ) {
    timing->serialize(s);
  }
}

// @brief 共通部分のダンプ
void
CiCell::dump_common(
  Serializer& s
) const
{
  s.dump(mName);
  s.dump(mArea);
  s.dump(mInputNum);
  s.dump(mOutputNum);
  s.dump(mInoutNum);
  s.dump(mPinList);
  s.dump(mInputList);
  s.dump(mOutputList);
  s.dump(mInternalList);
  s.dump(mBusList);
  s.dump(mBundleList);
  s.dump(mTimingList);
  s.dump(mTimingMap.size());
  for ( auto& timing_list: mTimingMap ) {
    s.dump(timing_list);
  }
}

// @brief 内容を読み込む．
std::unique_ptr<CiCell>
CiCell::restore(
  Deserializer& s
)
{
  std::uint8_t type;
  s.restore(type);
  CiCell* cell = nullptr;
  switch ( type ) {
  case 0: cell = new CiLogicCell; break;
  case 1: cell = new CiFFCell; break;
  case 2: cell = new CiFF2Cell; break;
  case 3: cell = new CiLatchCell; break;
  case 4: cell = new CiLatch2Cell; break;
  case 5: cell = new CiFsmCell; break;
  default: ASSERT_NOT_REACHED; break;
  }
  auto ptr = std::unique_ptr<CiCell>{cell};
  ptr->_restore(s);
  return ptr;
}

// @brief _restore() の共通部分
void
CiCell::restore_common(
  Deserializer& s
)
{
  s.restore(mName);
  s.restore(mArea);
  s.restore(mInputNum);
  s.restore(mOutputNum);
  s.restore(mInoutNum);

  // ピンリスト
  s.restore(mPinList);

  // 入力ピンリスト
  s.restore(mInputList);

  // 出力ピンリスト
  s.restore(mOutputList);

  // 内部ピンリスト
  s.restore(mInternalList);

  // バスのリスト
  s.restore(mBusList);

  // バンドルのリスト
  s.restore(mBundleList);

  // タイミングのリスト
  s.restore(mTimingList);

  // タイミングマップ
  s.restore(mTimingMap);
}

END_NAMESPACE_YM_CLIB
