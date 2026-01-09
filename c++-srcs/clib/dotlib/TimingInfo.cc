
/// @file TimingInfo.cc
/// @brief TimingInfo の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/TimingInfo.h"
#include "dotlib/AstValue.h"
#include "dotlib/AstExpr.h"
#include "ci/CiCell.h"
#include "ym/TvFunc.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス TimingInfo
//////////////////////////////////////////////////////////////////////

// @brief 内容を設定する．
void
TimingInfo::set(
  const AstValue* timing_val
)
{
  GroupInfo::set(timing_val);

  mDelayModel = library()->delay_model();

  // 名前を得る．
  auto& header = timing_val->group_header_value();
  SizeType n = header.complex_elem_size();
  ASSERT_COND( n <= 1 );
  if ( n == 1 ) {
    mName = header.complex_elem_value(0).string_value();
  }

  // 共通なパラメータの取得
  set_common_params();

  switch ( mDelayModel ) {
  case ClibDelayModel::generic_cmos:
    set_generic_params();
    break;

  case ClibDelayModel::piecewise_cmos:
    set_piecewise_params();
    break;

  case ClibDelayModel::table_lookup:
    mLutType = set_table_lookup_params();
    break;

  case ClibDelayModel::cmos2:
#warning "TODO: cmos2 モデル"
    break;

  case ClibDelayModel::dcm:
#warning "TODO: dcm モデル"
    break;

  case ClibDelayModel::polynomial:
#warning "TODO: polynomial モデル"
    break;

  case ClibDelayModel::none:
    ASSERT_NOT_REACHED;
    break;
  }
}

// @brief タイミング情報を作る．
void
TimingInfo::add_timing(
  CiCell* cell,
  const Expr& function_expr,
  const Expr& tristate_expr,
  const std::vector<SizeType>& opin_list,
  const std::unordered_map<ShString, SizeType>& ipin_map
) const
{
  auto when = Expr::one();
  if ( mWhen != nullptr ) {
    when = mWhen->to_expr(ipin_map);
  }

  CiTiming* timing;
  switch ( mDelayModel ) {
  case ClibDelayModel::generic_cmos:
    timing = cell->add_timing_generic(mTimingType, when,
				      mIntrinsicRise, mIntrinsicFall,
				      mSlopeRise, mSlopeFall,
				      mRiseResistance, mFallResistance);
    break;

  case ClibDelayModel::piecewise_cmos:
    {
      auto& rp_res = mRisePinResistance;
      auto& fp_res = mFallPinResistance;
      auto& rd_int = mRiseDelayIntercept;
      auto& fd_int = mFallDelayIntercept;
      timing = cell->add_timing_piecewise(mTimingType, when,
					  mIntrinsicRise, mIntrinsicFall,
					  mSlopeRise, mSlopeFall,
					  rp_res, fp_res,
					  rd_int, fd_int);
    }
    break;

  case ClibDelayModel::table_lookup:
    switch ( mLutType ) {
    case 1:
      {
	auto cr_lut = mCellRise.gen_stlut();
	auto cf_lut = mCellFall.gen_stlut();
	auto rt_lut = mRiseTransition.gen_stlut();
	auto ft_lut = mFallTransition.gen_stlut();
	timing = cell->add_timing_lut_cell(mTimingType, when,
					   std::move(cr_lut),
					   std::move(cf_lut),
					   std::move(rt_lut),
					   std::move(ft_lut));
      }
      break;
    case 2:
      {
	auto rt_lut = mRiseTransition.gen_stlut();
	auto ft_lut = mFallTransition.gen_stlut();
	auto rp_lut = mRisePropagation.gen_stlut();
	auto fp_lut = mFallPropagation.gen_stlut();
	timing = cell->add_timing_lut_prop(mTimingType, when,
					   std::move(rt_lut),
					   std::move(ft_lut),
					   std::move(rp_lut),
					   std::move(fp_lut));
      }
      break;
    case 3:
      {
	auto cr_lut = mCellRise.gen_lut();
	auto cf_lut = mCellFall.gen_lut();
	auto rt_lut = mRiseTransition.gen_lut();
	auto ft_lut = mFallTransition.gen_lut();
	timing = cell->add_timing_lut_cell(mTimingType, when,
					   std::move(cr_lut),
					   std::move(cf_lut),
					   std::move(rt_lut),
					   std::move(ft_lut));
      }
      break;
    case 4:
      {
	auto rt_lut = mRiseTransition.gen_lut();
	auto ft_lut = mFallTransition.gen_lut();
	auto rp_lut = mRisePropagation.gen_lut();
	auto fp_lut = mFallPropagation.gen_lut();
	timing = cell->add_timing_lut_prop(mTimingType, when,
					   std::move(rt_lut),
					   std::move(ft_lut),
					   std::move(rp_lut),
					   std::move(fp_lut));
      }
      break;
    default:
      ASSERT_NOT_REACHED;
      break;
    }
    break;

  case ClibDelayModel::cmos2:
#warning "TODO: cmos2 モデル"
    break;

  case ClibDelayModel::dcm:
#warning "TODO: dcm モデル"
    break;

  case ClibDelayModel::polynomial:
#warning "TODO: polynomial モデル"
    break;

  case ClibDelayModel::none:
    ASSERT_NOT_REACHED;
    break;
  }

  // timing_type が combinational の場合には実際の論理関数を調べて
  // timing_sense を設定する．
  SizeType ni = cell->input2_num();
  TvFunc function{ni};
  TvFunc tristate{ni};
  switch ( mTimingType ) {
  case ClibTimingType::combinational:
  case ClibTimingType::combinational_rise:
    function = function_expr.tvfunc(ni);
    break;

  case ClibTimingType::combinational_fall:
    function = ~function_expr.tvfunc(ni);
    break;

  case ClibTimingType::three_state_enable:
  case ClibTimingType::three_state_enable_rise:
  case ClibTimingType::three_state_disable:
  case ClibTimingType::three_state_disable_rise:
    tristate = tristate_expr.tvfunc(ni);
    break;

  case ClibTimingType::three_state_enable_fall:
  case ClibTimingType::three_state_disable_fall:
    tristate = ~tristate_expr.tvfunc(ni);
    break;

  default:
    break;
  }

  // タイミングマップの設定
  std::vector<SizeType> ipin_list;
  ipin_list.reserve(mRelatedPin.size());
  for ( auto name: mRelatedPin ) {
    ASSERT_COND( ipin_map.count(name) > 0 );
    auto id = ipin_map.at(name);
    ipin_list.push_back(id);
  }

  // timing_sense の抽出
  // TODO: いろいろ確かめる必要がある．
  for ( auto opin: opin_list ) {
    for ( auto ipin: ipin_list ) {
      bool p_unate{false};
      bool n_unate{false};
      if ( mTimingType == ClibTimingType::combinational ) {
	auto p_func = function.cofactor(ipin, false);
	auto n_func = function.cofactor(ipin, true);
	auto sense = mTimingSense;
	if ( p_func && ~n_func ) {
	  p_unate = true;
	}
	if ( ~p_func && n_func ) {
	  n_unate = true;
	}
      }
      else if ( mTimingType == ClibTimingType::three_state_enable ||
		mTimingType == ClibTimingType::three_state_disable ) {
	auto p_func = tristate.cofactor(ipin, false);
	auto n_func = tristate.cofactor(ipin, true);
	auto sense = mTimingSense;
	if ( p_func && ~n_func ) {
	  p_unate = true;
	}
	if ( ~p_func && n_func ) {
	  n_unate = true;
	}
      }
      else if ( mTimingType == ClibTimingType::rising_edge ||
		mTimingType == ClibTimingType::falling_edge ) {
	// クロックと出力値の rise/fall は無関係
	p_unate = true;
	n_unate = true;
      }
      else {
	if ( mTimingSense == ClibTimingSense::positive_unate ||
	     mTimingSense == ClibTimingSense::non_unate ) {
	  p_unate = true;
	}
	if ( mTimingSense == ClibTimingSense::negative_unate ||
	     mTimingSense == ClibTimingSense::non_unate ) {
	  n_unate = true;
	}
      }
      if ( p_unate ) {
	cell->set_timing(ipin, opin, ClibTimingSense::positive_unate, timing);
      }
      if ( n_unate ) {
	cell->set_timing(ipin, opin, ClibTimingSense::negative_unate, timing);
      }
    }
  }
}

// @brief タイミング情報の共通なパラメータを得る．
void
TimingInfo::set_common_params()
{
  if ( !get_timing_type("timing_type", mTimingType) ) {
    // 省略時は combinational を指定する．要確認
    mTimingType = ClibTimingType::combinational;
  }

  if ( !get_expr("when", mWhen) ) {
    mWhen = nullptr;
  }

  if ( !get_timing_sense("timing_sense", mTimingSense) ) {
    mTimingSense = ClibTimingSense::none;
  }

  const char* keyword{"related_pin"};
  if ( elem_dict().count(keyword) == 0 ) {
    auto label = "'related_pin' is missing.";
    parse_error(label);
  }
  auto& vec = elem_dict().at(keyword);
  mRelatedPin.clear();
  mRelatedPin.reserve(vec.size());
  for ( auto v: vec ) {
    auto name = v->string_value();
    mRelatedPin.push_back(name);
  }
}

// @brief generic_cmos/piecewise_cmos に共通なパラメータを得る．
void
TimingInfo::set_gp_common_params()
{
  if ( !get_time("intrinsic_rise", mIntrinsicRise) ) {
    auto label = "'intrinsic_rise' is missing.";
    parse_error(label);
  }
  if ( !get_time("intrinsic_fall", mIntrinsicFall) ) {
    auto label = "'intrinsic_fall' is missing.";
    parse_error(label);
  }
  if ( !get_time("slope_rise", mSlopeRise) ) {
    auto label = "'slope_rise' is missing.";
    parse_error(label);
  }
  if ( !get_time("slope_fall", mSlopeFall) ) {
    auto label = "'slope_fall' is missing.";
    parse_error(label);
  }
}

// @brief generic タイプのタイミング情報のパラメータを得る．
void
TimingInfo::set_generic_params()
{
  set_gp_common_params();
  if ( !get_resistance("rise_resistance", mRiseResistance) ) {
    auto label = "'rise_resistance' is missing.";
    parse_error(label);
  }
  if ( !get_resistance("fall_resistance", mFallResistance) ) {
    auto label = "'fall_resistance' is missing.";
    parse_error(label);
  }
}

BEGIN_NONAMESPACE

template<class T>
void
set_param_list(
  const std::vector<std::pair<SizeType, double>>& src_list,
  SizeType piece_num,
  std::vector<T>& dst_list
)
{
  SizeType nelem = src_list.size();
  dst_list.clear();
  dst_list.resize(piece_num);
  SizeType src_i = 0;
  for ( SizeType index = 0; index < piece_num; ++ index ) {
    if ( src_i < nelem - 1 && src_list[src_i + 1].first <= index ) {
      ++ src_i;
    }
    dst_list[index] = T{src_list[src_i].second};
  }
}

END_NONAMESPACE

// @brief piecewise_cmos タイプのタイミング情報のパラメータを得る．
void
TimingInfo::set_piecewise_params()
{
  set_gp_common_params();

  SizeType piece_num = library()->piece_define().size();
  set_param_list(extract_piecewise_params("rise_pin_resistance"),
		 piece_num, mRisePinResistance);
  set_param_list(extract_piecewise_params("fall_pin_resistance"),
		 piece_num, mFallPinResistance);
  set_param_list(extract_piecewise_params("rise_delay_intercept"),
		 piece_num, mRiseDelayIntercept);
  set_param_list(extract_piecewise_params("fall_delay_intercept"),
		 piece_num, mFallDelayIntercept);
}

// @brief piecewise_cmos タイプのパラメータ の値を取り出す．
std::vector<std::pair<SizeType, double>>
TimingInfo::extract_piecewise_params(
  const char* keyword
)
{
  std::vector<std::pair<SizeType, double>> tmp_list;
  if ( elem_dict().count(keyword) > 0 ) {
    auto& elem_list = elem_dict().at(keyword);
    tmp_list.reserve(elem_list.size());
    for ( auto elem: elem_list ) {
      auto pid = static_cast<SizeType>(elem->complex_elem_value(0).int_value());
      auto val = elem->complex_elem_value(1).float_value();
      tmp_list.push_back(std::make_pair(pid, val));
    }
    sort(tmp_list.begin(), tmp_list.end(),
	 [&](const std::pair<SizeType, double>& a,
	     const std::pair<SizeType, double>& b) {
	   return a.first < b.first;
	 });
  }
  return tmp_list;
}

// @brief table-lookup タイプのタイミング情報のパラメータを得る．
int
TimingInfo::set_table_lookup_params()
{
  bool standard = false;
  auto rt_val = get_value("rise_transition");
  auto ft_val = get_value("fall_transition");
  if ( rt_val == nullptr && ft_val == nullptr ) {
    // rise|fall_transition のどちらかは必須
    auto label = ": Neigther 'rise_transition' nor 'fall_transition' is not defined.";
    parse_error(label);
  }

  if ( rt_val ) {
    mRiseTransition.set(rt_val);
    if ( !mRiseTransition.is_standard() ) {
      standard = false;
    }
  }
  if ( ft_val ) {
    mFallTransition.set(ft_val);
    if ( !mFallTransition.is_standard() ) {
      standard = false;
    }
  }

  auto cr_val = get_value("cell_rise");
  auto cf_val = get_value("cell_fall");

  auto rp_val = get_value("rise_propagation");
  auto fp_val = get_value("fall_propagation");

  int type = 0;
  if ( cr_val ) {
    if ( rp_val ) {
      // cell_rise と rise_propagation は同時に指定できない．
      auto label = "'cell_rise' and 'rise_propagation' are mutually exclusive.";
      parse_error(label);
    }
    mCellRise.set(cr_val);
    if ( !mCellRise.is_standard() ) {
      standard = false;
    }
    type = 1;
  }
  else if ( rp_val ) {
    mRisePropagation.set(rp_val);
    if ( !mRisePropagation.is_standard() ) {
      standard = false;
    }
    type = 2;
  }

  if ( cf_val ) {
    if ( fp_val ) {
      // cell_fall と fall_propagation は同時に指定できない．
      auto label = "'cell_fall' and 'fall_propagation' are mutually exclusive.";
      parse_error(label);
    }
    if ( type == 2 ) {
      auto label = "'cell_fall' and 'rise_propagation' are mutually exclusive.";
      parse_error(label);
    }
    mCellFall.set(cf_val);
    if ( !mCellFall.is_standard() ) {
      standard = false;
    }
    type = 1;
  }
  else if ( fp_val ) {
    if ( type == 1 ) {
      auto label = "'cell_rise' and 'fall_propagation' are mutually exclusive.";
      parse_error(label);
    }
    mFallPropagation.set(fp_val);
    if ( !mFallPropagation.is_standard() ) {
      standard = false;
    }
    type = 2;
  }

  if ( !standard ) {
    type += 2;
  }

  return type;
}

END_NAMESPACE_YM_DOTLIB
