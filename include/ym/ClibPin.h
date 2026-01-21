#ifndef YM_CLIBPIN_H
#define YM_CLIBPIN_H

/// @file ym/ClibPin.h
/// @brief ClibPin のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ClibCellElem.h"
#include "ym/ClibCapacitance.h"
#include "ym/Expr.h"


BEGIN_NAMESPACE_YM_CLIB

class CiPin;

//////////////////////////////////////////////////////////////////////
/// @ingroup ClibGroup
/// @class ClibPin ClibPin.h "ym/ClibPin.h"
/// @brief セルのピンを表すクラス
//////////////////////////////////////////////////////////////////////
class ClibPin :
  public ClibCellElem<CiPin>
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// - 不正値となる．
  ClibPin() = default;

  /// @brief 内容を指定したコンストラクタ
  ClibPin(
    const CiCell* cell, ///< [in] 親のセル
    const CiPin* impl   ///< [in] 本体
  ) : ClibCellElem{cell, impl}
  {
  }

  /// @brief デストラクタ
  ~ClibPin() = default;


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 共通属性
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief ピン名を返す．
  /// @exception std::logic_error is_valid() = false の場合
  std::string
  name() const;

  /// @brief 方向を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @sa ClibDirection
  ClibDirection
  direction() const;

  /// @brief 入力ピンの時に true を返す．
  /// @exception std::logic_error is_valid() = false の場合
  ///
  /// - direction() == ClibDirection::input と等価
  bool
  is_input() const;

  /// @brief 出力ピンの時に true を返す．
  /// @exception std::logic_error is_valid() = false の場合
  ///
  /// - direction() == ClibDirection::output と等価
  bool
  is_output() const;

  /// @brief 入出力ピンの時に true を返す．
  /// @exception std::logic_error is_valid() = false の場合
  ///
  /// - direction() == ClibDirection::inout と等価
  bool
  is_inout() const;

  /// @brief 内部ピンの時に true を返す．
  /// @exception std::logic_error is_valid() = false の場合
  ///
  /// - direction() == ClibDirection::internal と等価
  bool
  is_internal() const;

  /// @brief ピン番号を返す．
  /// @exception std::logic_error is_valid() = false の場合
  ///
  /// このピンを持つセルを cell とすると．
  /// @code
  ///   pin = cell.pin(pin_id);
  ///   pin_id = pin.pin_id()
  /// @endcode
  /// が成り立つ．
  SizeType
  pin_id() const;

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 入力ピンの属性
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ピン番号を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::logic_error is_input() = false && is_inout() = false の場合
  ///
  /// このピンを持つセルを cell とすると．
  /// @code
  ///   pin = cell.input(iid);
  ///   iid = pin.input_id()
  /// @endcode
  /// が成り立つ．
  SizeType
  input_id() const;

  /// @brief 負荷容量を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::logic_error is_input() = false && is_inout() = false の場合
  /// @sa ClibCapacitance
  ClibCapacitance
  capacitance() const;

  /// @brief 立ち上がり時の負荷容量を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::logic_error is_input() = false && is_inout() = false の場合
  /// @sa ClibCapacitance
  ClibCapacitance
  rise_capacitance() const;

  /// @brief 立ち下がり時の負荷容量を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::logic_error is_input() = false && is_inout() = false の場合
  /// @sa ClibCapacitance
  ClibCapacitance
  fall_capacitance() const;

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 出力ピンの属性
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力ピン番号を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::logic_error is_output() = false && is_inout() = false の場合
  ///
  /// このピンを持つセルを cell とすると．
  /// @code
  ///   pin = cell.output(oid);
  ///   oid = pin.output_id()
  /// @endcode
  /// が成り立つ．
  SizeType
  output_id() const;

  /// @brief 最大ファンアウト容量を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::logic_error is_output() = false && is_inout() = false の場合
  /// @sa ClibCapacitance
  ClibCapacitance
  max_fanout() const;

  /// @brief 最小ファンアウト容量を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::logic_error is_output() = false && is_inout() = false の場合
  /// @sa ClibCapacitance
  ClibCapacitance
  min_fanout() const;

  /// @brief 最大負荷容量を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::logic_error is_output() = false && is_inout() = false の場合
  /// @sa ClibCapacitance
  ClibCapacitance
  max_capacitance() const;

  /// @brief 最小負荷容量を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::logic_error is_output() = false && is_inout() = false の場合
  /// @sa ClibCapacitance
  ClibCapacitance
  min_capacitance() const;

  /// @brief 最大遷移時間を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::logic_error is_output() = false && is_inout() = false の場合
  /// @sa ClibTime
  ClibTime
  max_transition() const;

  /// @brief 最小遷移時間を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::logic_error is_output() = false && is_inout() = false の場合
  /// @sa ClibTime
  ClibTime
  min_transition() const;

  /// @brief 論理式を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::logic_error is_output() = false && is_inout() = false の場合
  /// @sa Expr
  ///
  /// - 定義されていない場合には Expr::is_invalid() == true となる式を返す．
  Expr
  function() const;

  /// @brief tristate 条件式を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::logic_error is_output() = false && is_inout() = false の場合
  /// @sa Expr
  ///
  /// - 定義されていない場合には Expr::is_invalid() == true となる式を返す．
  Expr
  tristate() const;

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 内部ピンの属性
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部ピン番号を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::logic_error is_internal() = false の場合
  SizeType
  internal_id() const;

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////

};

END_NAMESPACE_YM_CLIB

#endif // YM_CLIBCELLPIN_H
