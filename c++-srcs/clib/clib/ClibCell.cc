
/// @file ClibCell.cc
/// @brief ClibCell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ClibCell.h"
#include "ym/ClibCellLibrary.h"
#include "ym/ClibPin.h"
#include "ym/ClibBus.h"
#include "ym/ClibBundle.h"
#include "ym/ClibSeqAttr.h"
#include "ym/ClibTiming.h"
#include "ci/CiCell.h"
#include "ci/CiCellGroup.h"
#include "ci/CiCellClass.h"
#include "ci/CiCellLibrary.h"


BEGIN_NAMESPACE_YM_CLIB


//////////////////////////////////////////////////////////////////////
// クラス ClibCellPtr
//////////////////////////////////////////////////////////////////////

// @brief 内容を指定したコンストラクタ
ClibCellPtr::ClibCellPtr(
  const CiCell* impl
) : mImpl{impl}
{
  if ( mImpl != nullptr ) {
    mImpl->inc_ref();
  }
}

// @brief コピーコンストラクタ
ClibCellPtr::ClibCellPtr(
  const ClibCellPtr& src
) : mImpl{src.mImpl}
{
  if ( mImpl != nullptr ) {
    mImpl->inc_ref();
  }
}

// @brief デストラクタ
ClibCellPtr::~ClibCellPtr()
{
  if ( mImpl != nullptr ) {
    mImpl->dec_ref();
  }
}


//////////////////////////////////////////////////////////////////////
// クラス ClibCell
//////////////////////////////////////////////////////////////////////

// @brief 親のセルライブラリの取得
ClibCellLibrary
ClibCell::library() const
{
  return ClibCellLibrary{_impl()->library()};
}

// @brief 親のセルグループの取得
ClibCellGroup
ClibCell::group() const
{
  return ClibCellGroup{_impl()->group()};
}

// @brief 名前の取得
std::string
ClibCell::name() const
{
  _check_valid();
  return _impl()->name();
}

// @brief 面積の取得
ClibArea
ClibCell::area() const
{
  _check_valid();
  return _impl()->area();
}

// @brief ピン数の取得
SizeType
ClibCell::pin_num() const
{
  _check_valid();
  return _impl()->pin_num();
}

// @brief ピンの取得
ClibPin
ClibCell::pin(
  SizeType pos
) const
{
  _check_valid();
  auto pin = _impl()->pin(pos);
  return ClibPin{_impl(), pin};
}

// @brief 名前からピンの取得
ClibPin
ClibCell::pin(
  const std::string& name
) const
{
  _check_valid();
  auto pin = _impl()->find_pin(ShString{name});
  return ClibPin{_impl(), pin};
}

// @brief ピンのリストを返す．
ClibPinList
ClibCell::pin_list() const
{
  _check_valid();
  return ClibPinList{_impl(), _impl()->pin_list()};
}

// @brief 入力ピン数の取得
SizeType
ClibCell::input_num() const
{
  _check_valid();
  return _impl()->input_num();
}

// @brief 入力ピンの取得
ClibPin
ClibCell::input(
  SizeType pos
) const
{
  _check_valid();
  auto pin = _impl()->input(pos);
  return ClibPin{_impl(), pin};
}

// @brief 入力ピンのリストの取得
ClibPinList
ClibCell::input_list() const
{
  _check_valid();
  auto& pin_list = _impl()->input_list();
  return ClibPinList{_impl(), pin_list};
}

// @brief 出力ピン数の取得
SizeType
ClibCell::output_num() const
{
  _check_valid();
  return _impl()->output_num();
}

// @brief 出力ピンの取得
ClibPin
ClibCell::output(
  SizeType pos
) const
{
  _check_valid();
  auto pin = _impl()->output(pos);
  return ClibPin{_impl(), pin};
}

// @brief 出力ピンのリストの取得
ClibPinList
ClibCell::output_list() const
{
  _check_valid();
  auto& pin_list = _impl()->output_list();
  return ClibPinList{_impl(), pin_list};
}

// @brief 入出力ピン数の取得
SizeType
ClibCell::inout_num() const
{
  _check_valid();
  return _impl()->inout_num();
}

// @brief 入出力ピンの取得
ClibPin
ClibCell::inout(
  SizeType pos
) const
{
  _check_valid();
  auto pin = _impl()->inout(pos);
  return ClibPin{_impl(), pin};
}

// @brief 入出力ピンのリストの取得
ClibPinList
ClibCell::inout_list() const
{
  _check_valid();
  return ClibPinList{_impl(),
		     _impl()->inout_begin(),
		     _impl()->input_list().end()};
}

// @brief 入力ピン+入出力ピン数の取得
SizeType
ClibCell::input2_num() const
{
  _check_valid();
  return _impl()->input2_num();
}

// @brief 入力ピンの取得
ClibPin
ClibCell::input2(
  SizeType pos
) const
{
  _check_valid();
  auto pin = _impl()->input(pos);
  return ClibPin{_impl(), pin};
}

// @brief 出力ピン+入出力ピン数の取得
SizeType
ClibCell::output2_num() const
{
  _check_valid();
  return _impl()->output2_num();
}

// @brief 出力ピンの取得
ClibPin
ClibCell::output2(
  SizeType pos
) const
{
  _check_valid();
  auto pin = _impl()->output(pos);
  return ClibPin{_impl(), pin};
}

// @brief 内部ピン数の取得
SizeType
ClibCell::internal_num() const
{
  _check_valid();
  return _impl()->internal_num();
}

// @brief 内部ピンの取得
ClibPin
ClibCell::internal(
  SizeType pos
) const
{
  _check_valid();
  auto pin = _impl()->internal(pos);
  return ClibPin{_impl(), pin};
}

// @brief 内部ピンのリストの取得
ClibPinList
ClibCell::internal_list() const
{
  _check_valid();
  return ClibPinList{_impl(), _impl()->internal_list()};
}

// @brief バス数の取得
SizeType
ClibCell::bus_num() const
{
  _check_valid();
  return _impl()->bus_num();
}

// @brief バスの取得
ClibBus
ClibCell::bus(
  SizeType pos
) const
{
  _check_valid();
  auto pin = _impl()->bus(pos);
  return ClibBus{_impl(), pin};
}

// @brief 名前からバスの取得
ClibBus
ClibCell::bus(
  const std::string& name
) const
{
  _check_valid();
  auto bus = _impl()->find_bus(ShString{name});
  return ClibBus{_impl(), bus};
}

// @brief バスのリストの取得
ClibBusList
ClibCell::bus_list() const
{
  _check_valid();
  return ClibBusList{_impl(), _impl()->bus_list()};
}

// @brief バンドル数の取得
SizeType
ClibCell::bundle_num() const
{
  _check_valid();
  return _impl()->bundle_num();
}

// @brief バンドルの取得
ClibBundle
ClibCell::bundle(
  SizeType pos
) const
{
  _check_valid();
  auto bundle = _impl()->bundle(pos);
  return ClibBundle{_impl(), bundle};
}

// @brief 名前からバンドルの取得
ClibBundle
ClibCell::bundle(
  const std::string& name
) const
{
  _check_valid();
  auto bundle = _impl()->find_bundle(ShString{name});
  return ClibBundle{_impl(), bundle};
}

// @brief バンドルのリストの取得
ClibBundleList
ClibCell::bundle_list() const
{
  _check_valid();
  return ClibBundleList{_impl(), _impl()->bundle_list()};
}

// @brief 条件に合致するタイミング情報のリストを返す．
ClibTimingList
ClibCell::timing_list(
  SizeType ipos,
  SizeType opos,
  ClibTimingSense sense
) const
{
  _check_valid();
  auto& timing_list = _impl()->timing_list(ipos, opos, sense);
  return ClibTimingList{_impl(), timing_list};
}

// @brief セルの種類を返す．
ClibCellType
ClibCell::type() const
{
  _check_valid();
  return _impl()->type();
}

// @brief 組み合わせ論理タイプの時 true を返す．
bool
ClibCell::is_logic() const
{
  _check_valid();
  return _impl()->is_logic();
}

// @brief FFタイプの時 true を返す．
bool
ClibCell::is_ff() const
{
  _check_valid();
  return _impl()->is_ff();
}

// @brief ラッチタイプの時 true を返す．
bool
ClibCell::is_latch() const
{
  _check_valid();
  return _impl()->is_latch();
}

// @brief 出力の論理式を持っている時に true を返す．
bool
ClibCell::has_logic(
  SizeType pin_id
) const
{
  return logic_expr(pin_id).is_valid();
}

// @brief 全ての出力が論理式を持っているときに true を返す．
bool
ClibCell::has_logic() const
{
  _check_valid();
  for ( auto pin: output_list() ) {
    if ( !pin.function().is_valid() ) {
      return false;
    }
  }
  return true;
}

// @brief 論理セルの場合に出力の論理式を返す．
Expr
ClibCell::logic_expr(
  SizeType pin_id
) const
{
  _check_valid();
  auto pin = output(pin_id);
  return pin.function();
}

// @brief 出力がトライステート条件を持っている時に true を返す．
bool
ClibCell::has_tristate(
  SizeType pin_id
) const
{
  return tristate_expr(pin_id).is_valid();
}

// @brief トライステートセルの場合にトライステート条件式を返す．
Expr
ClibCell::tristate_expr(
  SizeType pin_id
) const
{
  _check_valid();
  auto pin = output(pin_id);
  return pin.tristate();
}

// @brief 内部変数1の名前を返す．
std::string
ClibCell::qvar1() const
{
  _check_valid();
  return _impl()->qvar1();
}

// @brief 内部変数1の名前を返す．
std::string
ClibCell::qvar2() const
{
  _check_valid();
  return _impl()->qvar2();
}

// @brief 非同期 clear を持つ時 true を返す．
bool
ClibCell::has_clear() const
{
  return clear_expr().is_valid();
}

// @brief FFセル/ラッチセルの場合にクリア条件を表す論理式を返す．
Expr
ClibCell::clear_expr() const
{
  _check_valid();
  return _impl()->clear_expr();
}

// @brief 非同期 preset を持つ時 true を返す．
bool
ClibCell::has_preset() const
{
  return preset_expr().is_valid();
}

// @brief FFセル/ラッチセルの場合にプリセット条件を表す論理式を返す．
Expr
ClibCell::preset_expr() const
{
  _check_valid();
  return _impl()->preset_expr();
}

// @brief clear と preset が同時にアクティブになった時の値1
ClibCPV
ClibCell::clear_preset_var1() const
{
  _check_valid();
  return _impl()->seq_attr().cpv1();
}

// @brief clear と preset が同時にアクティブになった時の値1
ClibCPV
ClibCell::clear_preset_var2() const
{
  _check_valid();
  return _impl()->seq_attr().cpv2();
}

// @brief FFセルの場合にクロックのアクティブエッジを表す論理式を返す．
Expr
ClibCell::clock_expr() const
{
  _check_valid();
  return _impl()->clock_expr();
}

// @brief FFセルの場合にスレーブクロックのアクティブエッジを表す論理式を返す．
Expr
ClibCell::clock2_expr() const
{
  _check_valid();
  return _impl()->clock2_expr();
}

// @brief FFセルの場合に次状態関数を表す論理式を返す．
Expr
ClibCell::next_state_expr() const
{
  _check_valid();
  return _impl()->next_state_expr();
}

// @brief ラッチセルの場合にイネーブル条件を表す論理式を返す．
Expr
ClibCell::enable_expr() const
{
  _check_valid();
  return _impl()->enable_expr();
}

// @brief ラッチセルの場合に2つめのイネーブル条件を表す論理式を返す．
Expr
ClibCell::enable2_expr() const
{
  _check_valid();
  return _impl()->enable2_expr();
}

// @brief ラッチセルの場合にデータ入力関数を表す論理式を返す．
Expr
ClibCell::data_in_expr() const
{
  _check_valid();
  return _impl()->data_in_expr();
}

END_NAMESPACE_YM_CLIB
