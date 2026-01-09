
/// @file CiCellLibrary.cc
/// @brief CiCellLibrary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ci/CiCellLibrary.h"
#include "ci/CiCellClass.h"
#include "ci/CiCellGroup.h"
#include "ci/CiCell.h"
#include "ci/CiPin.h"
#include "ci/CiBus.h"
#include "ci/CiBundle.h"
#include "ci/CiTiming.h"
#include "ci/CiLutTemplate.h"
#include "ci/CiLut.h"
#include "ci/CiBusType.h"
#include "ci/CiPatGraph.h"
#include "ci/CiCell.h"
#include "cgmgr/CgMgr.h"
#include "cgmgr/CgSignature.h"
#include "cgmgr/PatMgr.h"
#include "cgmgr/PatNode.h"
#include "ym/ClibSeqAttr.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
// クラス CiCellLibrary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiCellLibrary::CiCellLibrary()
{
}

// @brief デストラクタ
CiCellLibrary::~CiCellLibrary()
{
}

// @brief 参照回数を増やす．
void
CiCellLibrary::inc_ref() const
{
  ++ mRefCount;
}

// @brief 参照回数を減らす．
void
CiCellLibrary::dec_ref() const
{
  -- mRefCount;
  if ( mRefCount == 0 ) {
    // 自殺する．
    delete this;
  }
}

// @brief FFクラスを返す．
std::vector<const CiCellClass*>
CiCellLibrary::find_ff_class(
  ClibSeqAttr seq_attr
) const
{
  auto index = seq_attr.index();
  if ( mFFClassDict.count(index) > 0 ) {
    return mFFClassDict.at(index);
  }
  return {};
}

// @brief ラッチクラスを返す．
std::vector<const CiCellClass*>
CiCellLibrary::find_latch_class(
  ClibSeqAttr seq_attr
) const
{
  auto index = seq_attr.index();
  if ( mLatchClassDict.count(index) > 0 ) {
    return mLatchClassDict.at(index);
  }
  return {};
}

// @brief 属性を設定する(浮動小数点型)
void
CiCellLibrary::set_attr(
  const std::string& attr_name,
  double value
)
{
  if ( attr_name == "em_temp_degradation_factor" ) {
  }
  else if ( attr_name == "nom_process" ) {
  }
  else if ( attr_name == "nom_temperature" ) {
  }
  else if ( attr_name == "nom_voltage" ) {
  }
  else if ( attr_name == "default_cell_leakage_power" ) {
  }
  else if ( attr_name == "default_fall_delay_intercept" ) {
  }
  else if ( attr_name == "default_fall_pin_resistance" ) {
  }
  else if ( attr_name == "default_fanout_load" ) {
  }
  else if ( attr_name == "default_inout_pin_cap" ) {
  }
  else if ( attr_name == "default_inout_pin_fall_res" ) {
  }
  else if ( attr_name == "default_inout_pin_rise_res" ) {
  }
  else if ( attr_name == "default_inout_pin_cap" ) {
  }
  else if ( attr_name == "default_intrinsic_fall" ) {
  }
  else if ( attr_name == "default_intrinsic_rise" ) {
  }
  else if ( attr_name == "default_leakage_power_density" ) {
  }
  else if ( attr_name == "default_max_capacitance" ) {
  }
  else if ( attr_name == "default_max_fanout" ) {
  }
  else if ( attr_name == "default_max_transition" ) {
  }
  else if ( attr_name == "default_max_utilization" ) {
  }
  else if ( attr_name == "default_min_porosity" ) {
  }
  else if ( attr_name == "default_output_pin_cap" ) {
  }
  else if ( attr_name == "default_output_pin_fall_res" ) {
  }
  else if ( attr_name == "default_output_pin_rise_res" ) {
  }
  else if ( attr_name == "default_rise_delay_intercept" ) {
  }
  else if ( attr_name == "default_slope_fall" ) {
  }
  else if ( attr_name == "default_slope_rise" ) {
  }
  else if ( attr_name == "default_wire_load_area" ) {
  }
  else if ( attr_name == "default_wire_load_capacitance" ) {
  }
  else if ( attr_name == "default_wire_load_resistance" ) {
  }
}

// @brief 属性を設定する(文字列型)．
void
CiCellLibrary::set_attr(
  const std::string& attr_name,
  const std::string& value
)
{
  if ( attr_name == "bus_naming_style" ) {
    mBusNamingStyle = value;
  }
  else if ( attr_name == "date" ) {
    mDate = value;
  }
  else if ( attr_name == "revision" ) {
    mRevision = value;
  }
  else if ( attr_name == "comment" ) {
    mComment = value;
  }
  else if ( attr_name == "time_unit" ) {
    mTimeUnit = value;
  }
  else if ( attr_name == "voltage_unit" ) {
    mVoltageUnit = value;
  }
  else if ( attr_name == "current_unit" ) {
    mCurrentUnit = value;
  }
  else if ( attr_name == "pulling_resistance_unit" ) {
    mPullingResistanceUnit = value;
  }
  else if ( attr_name == "leakage_power_unit" ) {
    mLeakagePowerUnit = value;
  }
}

// @brief 1次元の LUT のテンプレートを作る．
CiLutTemplate*
CiCellLibrary::add_lut_template1(
  ClibVarType var_type1,
  const std::vector<double>& index_list1
)
{
  auto ptr = CiLutTemplate::new_1D(var_type1, index_list1);
  auto tmpl = ptr.get();
  mLutTemplateList.push_back(std::move(ptr));
  return tmpl;
}

// @brief 2次元の LUT のテンプレートを作る．
CiLutTemplate*
CiCellLibrary::add_lut_template2(
  ClibVarType var_type1,
  const std::vector<double>& index_list1,
  ClibVarType var_type2,
  const std::vector<double>& index_list2
)
{
  auto ptr = CiLutTemplate::new_2D(var_type1, index_list1,
				   var_type2, index_list2);
  auto tmpl = ptr.get();
  mLutTemplateList.push_back(std::move(ptr));
  return tmpl;
}

// @brief 3次元の LUT のテンプレートを作る．
CiLutTemplate*
CiCellLibrary::add_lut_template3(
  ClibVarType var_type1,
  const std::vector<double>& index_list1,
  ClibVarType var_type2,
  const std::vector<double>& index_list2,
  ClibVarType var_type3,
  const std::vector<double>& index_list3
)
{
  auto ptr = CiLutTemplate::new_3D(var_type1, index_list1,
				   var_type2, index_list2,
				   var_type3, index_list3);
  auto tmpl = ptr.get();
  mLutTemplateList.push_back(std::move(ptr));
  return tmpl;
}

// @brief セルクラスを作る．
CiCellClass*
CiCellLibrary::add_cell_class(
  ClibCellType cell_type,
  ClibSeqAttr seq_attr,
  const std::vector<ClibIOMap>& idmap_list
)
{
  auto cc = new CiCellClass{this, cell_type, seq_attr, idmap_list};
  mCellClassList.push_back(std::unique_ptr<CiCellClass>(cc));
  return cc;
}

// @brief セルグループを作る．
CiCellGroup*
CiCellLibrary::add_cell_group(
  const CiCellClass* rep_class,
  const ClibIOMap& iomap
)
{
  auto cg = new CiCellGroup{rep_class, iomap};
  mCellGroupList.push_back(std::unique_ptr<CiCellGroup>(cg));
  return cg;
}

// @brief 論理セルを追加する．
CiCell*
CiCellLibrary::add_logic_cell(
  const ShString& name,
  ClibArea area
)
{
  auto ptr = CiCell::new_Logic(name, area);
  return reg_cell(ptr);
}

// @brief FFセルを生成する．
CiCell*
CiCellLibrary::add_ff_cell(
  const ShString& name,
  ClibArea area,
  const ShString& var1,
  const ShString& var2,
  const Expr& clock,
  const Expr& clock2,
  const Expr& next_state,
  const Expr& clear,
  const Expr& preset,
  ClibSeqAttr seq_attr
)
{
  auto ptr = CiCell::new_FF(name, area,
			    var1, var2,
			    clock,
			    clock2,
			    next_state,
			    clear,
			    preset,
			    seq_attr);
  return reg_cell(ptr);
}

// @brief ラッチセルを追加する．
CiCell*
CiCellLibrary::add_latch_cell(
  const ShString& name,
  ClibArea area,
  const ShString& var1,
  const ShString& var2,
  const Expr& enable,
  const Expr& enable2,
  const Expr& data_in,
  const Expr& clear,
  const Expr& preset,
  ClibSeqAttr seq_attr
)
{
  auto ptr = CiCell::new_Latch(name, area,
			       var1, var2,
			       enable,
			       enable2,
			       data_in,
			       clear,
			       preset,
			       seq_attr);
  return reg_cell(ptr);
}

// @brief FSMセルを追加する．
CiCell*
CiCellLibrary::add_fsm_cell(
  const ShString& name,
  ClibArea area
)
{
  auto ptr = CiCell::new_FSM(name, area);
  return reg_cell(ptr);
}

// @brief セルを登録する．
CiCell*
CiCellLibrary::reg_cell(
  std::unique_ptr<CiCell>& ptr
)
{
  auto cell = ptr.get();
  mCellList.push_back(std::move(ptr));
  mCellDict.emplace(cell->name(), cell);
  return cell;
}

// @brief 設定後の最終処理を行う．
void
CiCellLibrary::wrap_up()
{
  // ピン名の辞書を作る．
  for ( auto& cell: mCellList ) {
    for ( auto& pin: cell->_pin_list() ) {
      mPinDict.add(cell.get(), pin->_name(), pin.get());
    }
    for ( auto& bus: cell->_bus_list() ) {
      mBusDict.add(cell.get(), bus->_name(), bus.get());
    }
    for ( auto& bundle: cell->_bundle_list() ) {
      mBundleDict.add(cell.get(), bundle->_name(), bundle.get());
    }
  }

  { // sanity-check
    for ( auto& cell: mCellList ) {
      auto group = cell->group();
      ASSERT_COND( group != nullptr );
    }
    for ( auto& group: mCellGroupList ) {
      auto rep_class = group->rep_class();
      ASSERT_COND( rep_class != nullptr );
      for ( auto cell: group->cell_list() ) {
	ASSERT_COND( cell->group() == group.get() );
      }
    }
    for ( auto& cclass: mCellClassList ) {
      auto library = cclass->library();
      ASSERT_COND( library == this );
      for ( auto group: cclass->cell_group_list() ) {
	ASSERT_COND( group->rep_class() == cclass.get() );
      }
    }
  }
}

// @brief セルグループ/セルクラスの設定を行なう．
void
CiCellLibrary::compile()
{
  // シグネチャを用いてセルグループとセルクラスの設定を行う．
  CgMgr cgmgr{*this};
  for ( auto& cell: mCellList ) {
    // シグネチャを作る．
    auto sig = cell->make_signature();
    // sig に対応するグループを求める．
    auto group = cgmgr.find_group(sig);
    // セルを登録する．
    group->add_cell(cell.get());
    cell->set_group(group);
  }

  // パタングラフを作る．
  cgmgr.gen_pat();

  // セルクラスの情報をコピーする．
  for ( SizeType index = 0; index < 24; ++ index ) {
    mLogicGroup[index] = cgmgr.logic_group(index);
  }
  mFFClassDict = cgmgr.ff_class_dict();
  mLatchClassDict = cgmgr.latch_class_dict();

  // パタングラフの情報をコピーする．
  SizeType nn = cgmgr.pat_node_num();
  mPatMgr.set_node_num(nn);
  for ( SizeType i = 0; i < nn; ++ i ) {
    const auto& node = cgmgr.pat_node(i);
    if ( node.is_input() ) {
      mPatMgr.set_node_info(i, node.input_id());
    }
    else {
      auto type = node.is_and() ? ClibPatType::And : ClibPatType::Xor;
      SizeType iid1 = node.fanin(0).id();
      bool iinv1 = node.fanin_inv(0);
      SizeType iid2 = node.fanin(1).id();
      bool iinv2 = node.fanin_inv(1);
      mPatMgr.set_node_info(i, type, iid1, iinv1, iid2, iinv2);
    }
  }
  SizeType np = cgmgr.pat_num();
  mPatMgr.set_pat_num(np);
  for ( SizeType i = 0; i < np; ++ i ) {
    const CiCellClass* rep_class;
    SizeType input_num;
    std::vector<SizeType> edge_list;
    cgmgr.get_pat_info(i, rep_class, input_num, edge_list);
    mPatMgr.set_pat_info(i, rep_class, input_num, edge_list);
  }
}

// @brief 内容をクリアする．
void
CiCellLibrary::clear()
{
  mName = {};
  mBusNamingStyle = {};
  mDate = {};
  mRevision = {};
  mComment = {};
  mTimeUnit = {};
  mVoltageUnit = {};
  mCurrentUnit = {};
  mPullingResistanceUnit = {};
  mCapacitiveLoadUnit = 0.0;
  mCapacitiveLoadUnitStr = {};
  mLeakagePowerUnit = {};
  mLutTemplateList.clear();
  mCellList.clear();
  mCellDict.clear();
  mPinDict.clear();
  mBusDict.clear();
  mBundleDict.clear();
  mCellGroupList.clear();
  mCellClassList.clear();
}

END_NAMESPACE_YM_CLIB
