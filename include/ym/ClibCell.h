#ifndef YM_CLIBCELL_H
#define YM_CLIBCELL_H

/// @file ym/ClibCell.h
/// @brief ClibCell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga (松永 裕介)
/// All rights reserved.

#include "ym/clib.h"
#include "ym/ClibCellPtr.h"
#include "ym/ClibArea.h"
#include "ym/ClibPin.h"
#include "ym/ClibBus.h"
#include "ym/ClibBundle.h"
#include "ym/ClibTiming.h"
#include "ym/ClibList2.h"
#include "ym/Expr.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
/// @ingroup ClibGroup
/// @class ClibCell ClibCell.h "ym/ClibCell.h"
/// @brief セル本体のクラス
//////////////////////////////////////////////////////////////////////
class ClibCell :
  public ClibCellPtr
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// - 不正値となる．
  ClibCell() = default;

  /// @brief 内容を指定したコンストラクタ
  ClibCell(
    const CiCell* impl ///< [in] 本体
  ) : ClibCellPtr{impl}
  {
  }

  /// @brief デストラクタ
  ~ClibCell() = default;


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 基本情報の取得
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のセルライブラリの取得
  /// @exception std::logic_error is_valid() = false の場合
  /// @sa ClibCellLibrary
  ClibCellLibrary
  library() const;

  /// @brief 親のセルグループの取得
  /// @exception std::logic_error is_valid() = false の場合
  /// @sa ClibCellGroup
  ClibCellGroup
  group() const;

  /// @brief 名前の取得
  /// @exception std::logic_error is_valid() = false の場合
  std::string
  name() const;

  /// @brief 面積の取得
  /// @exception std::logic_error is_valid() = false の場合
  /// @sa ClibArea
  ClibArea
  area() const;

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ピン情報の取得
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief ピン数の取得
  /// @exception std::logic_error is_valid() = false の場合
  SizeType
  pin_num() const;

  /// @brief ピンの取得
  /// @return ピン情報を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::out_of_range 範囲外のアクセス
  /// @sa ClibPin
  ClibPin
  pin(
    SizeType pin_id ///< [in] ピン番号 ( 0 <= pin_id < pin_num() )
  ) const;

  /// @brief 名前からピン番号の取得
  /// @return name という名前のピンを返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::out_of_range name という名前のピンが見つからなかった．
  /// @sa ClibPin
  ClibPin
  pin(
    const std::string& name ///< [in] ピン名
  ) const;

  /// @brief ピンのリストを返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @sa ClibPinList
  ClibPinList
  pin_list() const;

  /// @brief 入力ピン数の取得
  /// @exception std::logic_error is_valid() = false の場合
  SizeType
  input_num() const;

  /// @brief 入力ピンの取得
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::out_of_range 範囲外のアクセス
  /// @sa ClibPin
  ClibPin
  input(
    SizeType pos ///< [in] 番号 ( 0 <= pos < input_num() )
  ) const;

  /// @brief 入力ピンのリストの取得
  /// @exception std::logic_error is_valid() = false の場合
  /// @sa ClibPinList
  ClibPinList
  input_list() const;

  /// @brief 出力ピン数の取得
  /// @exception std::logic_error is_valid() = false の場合
  SizeType
  output_num() const;

  /// @brief 出力ピンの取得
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::out_of_range 範囲外のアクセス
  /// @sa ClibPin
  ClibPin
  output(
    SizeType pos ///< [in] 番号 ( 0 <= pos < output_num() )
  ) const;

  /// @brief 出力ピンのリストの取得
  /// @exception std::logic_error is_valid() = false の場合
  /// @sa ClibPinList
  ClibPinList
  output_list() const;

  /// @brief 入出力ピン数の取得
  /// @exception std::logic_error is_valid() = false の場合
  SizeType
  inout_num() const;

  /// @brief 入出力ピンの取得
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::out_of_range 範囲外のアクセス
  /// @sa ClibPin
  ClibPin
  inout(
    SizeType pos ///< [in] 番号 ( 0 <= pos < inout_num() )
  ) const;

  /// @brief 入出力ピンのリストの取得
  /// @exception std::logic_error is_valid() = false の場合
  /// @sa ClibPinList
  ClibPinList
  inout_list() const;

  /// @brief 入力ピン+入出力ピン数の取得
  /// @exception std::logic_error is_valid() = false の場合
  ///
  /// - input_num() + inout_num() に等しい．
  SizeType
  input2_num() const;

  /// @brief 入力ピンの取得
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::out_of_range 範囲外のアクセス
  /// @sa ClibPin
  ///
  /// - id >= input_num() の場合には入出力ピンが返される．
  ClibPin
  input2(
    SizeType pos ///< [in] 番号 ( 0 <= pos < input_num2() )
  ) const;

  /// @brief 出力ピン+入出力ピン数の取得
  /// @exception std::logic_error is_valid() = false の場合
  ///
  /// - output_num() + inout_num() に等しい．
  SizeType
  output2_num() const;

  /// @brief 出力ピンの取得
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::out_of_range 範囲外のアクセス
  ///
  /// - id >= output_num() の場合には入出力ピンが返される．
  ClibPin
  output2(
    SizeType pos ///< [in] 番号 ( 0 <= pos < output_num2() )
  ) const;

  /// @brief 内部ピン数の取得
  /// @exception std::logic_error is_valid() = false の場合
  SizeType
  internal_num() const;

  /// @brief 内部ピンの取得
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::out_of_range 範囲外のアクセス
  /// @sa ClibPin
  ClibPin
  internal(
    SizeType pos ///< [in] 内部ピン番号 ( 0 <= pos < internal_num() )
  ) const;

  /// @brief 内部ピンのリストの取得
  /// @exception std::logic_error is_valid() = false の場合
  /// @sa ClibPinList
  ClibPinList
  internal_list() const;

  /// @brief バス数の取得
  /// @exception std::logic_error is_valid() = false の場合
  SizeType
  bus_num() const;

  /// @brief バスの取得
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::out_of_range 範囲外のアクセス
  /// @sa ClibBus
  ClibBus
  bus(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < bus_num() )
  ) const;

  /// @brief 名前からバスの取得
  /// @return name という名前のバスを返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::out_of_range name というバスが見つからなかった
  /// @sa ClibBus
  ClibBus
  bus(
    const std::string& name ///< [in] バス名
  ) const;

  /// @brief バスのリストの取得
  /// @exception std::logic_error is_valid() = false の場合
  /// @sa ClibBusList
  ClibBusList
  bus_list() const;

  /// @brief バンドル数の取得
  /// @exception std::logic_error is_valid() = false の場合
  SizeType
  bundle_num() const;

  /// @brief バンドルの取得
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::out_of_range 範囲外のアクセス
  /// @sa ClibBundle
  ClibBundle
  bundle(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < bundle_num() )
  ) const;

  /// @brief 名前からバンドルの取得
  /// @return name という名前のバンドルを返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::out_of_range name というバンドルが見つからなかった．
  /// @sa ClibBundle
  ClibBundle
  bundle(
    const std::string& name ///< [in] バンドル名
  ) const;

  /// @brief バンドルのリストの取得
  /// @exception std::logic_error is_valid() = false の場合
  /// @sa ClibBundleList
  ClibBundleList
  bundle_list() const;

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name タイミング情報の取得
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 条件に合致するタイミング情報のリストを返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::out_of_range 範囲外のアクセス
  /// @sa ClibTimingList
  ClibTimingList
  timing_list(
    SizeType ipos,        ///< [in] 開始ピン番号 ( 0 <= ipos < input_num2() )
    SizeType opos,        ///< [in] 終了ピン番号 ( 0 <= opos < output_num2() )
    ClibTimingSense sense ///< [in] タイミング情報の摘要条件
  ) const;

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 機能情報の取得
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief セルの種類を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @sa ClibCellType
  ClibCellType
  type() const;

  /// @brief 組み合わせ論理タイプの時 true を返す．
  /// @exception std::logic_error is_valid() = false の場合
  bool
  is_logic() const;

  /// @brief FFタイプの時 true を返す．
  /// @exception std::logic_error is_valid() = false の場合
  bool
  is_ff() const;

  /// @brief ラッチタイプの時 true を返す．
  /// @exception std::logic_error is_valid() = false の場合
  bool
  is_latch() const;

  /// @brief 出力の論理式を持っている時に true を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::out_of_range 範囲外のアクセス
  bool
  has_logic(
    SizeType pin_id ///< [in] 出力ピン番号 ( 0 <= pin_id < output_num2() )
  ) const;

  /// @brief 全ての出力が論理式を持っているときに true を返す．
  /// @exception std::logic_error is_valid() = false の場合
  bool
  has_logic() const;

  /// @brief 論理セルの場合に出力の論理式を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::out_of_range 範囲外のアクセス
  /// @sa Expr
  ///
  /// - 論理式中の変数番号は入力ピン番号に対応する．
  Expr
  logic_expr(
    SizeType pin_id ///< [in] 出力ピン番号 ( 0 <= pin_id < output_num2() )
  ) const;

  /// @brief 出力がトライステート条件を持っている時に true を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::out_of_range 範囲外のアクセス
  bool
  has_tristate(
    SizeType pin_id ///< [in] 出力ピン番号 ( 0 <= pin_id < output_num2() )
  ) const;

  /// @brief トライステートセルの場合にトライステート条件式を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::out_of_range 範囲外のアクセス
  /// @sa Expr
  ///
  /// - 論理式中の変数番号は入力ピン番号に対応する．
  /// - 通常の論理セルの場合には定数0を返す．
  Expr
  tristate_expr(
    SizeType pin_id ///< [in] 出力ピン番号 ( 0 <= pin_id < output_num2() )
  ) const;

  /// @brief 内部変数1の名前を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::logic_error 内部変数を持たない場合
  std::string
  qvar1() const;

  /// @brief 内部変数2の名前を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::logic_error 内部変数を持たない場合
  std::string
  qvar2() const;

  /// @brief 非同期 clear を持つ時 true を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::logic_error FF/ラッチセル以外の場合
  bool
  has_clear() const;

  /// @brief FFセル/ラッチセルの場合にクリア条件を表す論理式を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::logic_error has_clear() = false の場合
  /// @sa Expr
  Expr
  clear_expr() const;

  /// @brief 非同期 preset を持つ時 true を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::logic_error FF/ラッチセル以外の場合
  bool
  has_preset() const;

  /// @brief FFセル/ラッチセルの場合にプリセット条件を表す論理式を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::logic_error has_preset() = false の場合
  /// @sa Expr
  Expr
  preset_expr() const;

  /// @brief clear と preset が同時にアクティブになった時の値1
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::logic_error has_clear() == false || has_preset() == false
  /// @sa ClibCPV
  ClibCPV
  clear_preset_var1() const;

  /// @brief clear と preset が同時にアクティブになった時の値2
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::logic_error has_clear() == false || has_preset() == false
  /// @sa ClibCPV
  ClibCPV
  clear_preset_var2() const;

  /// @brief FFセルの場合にクロックのアクティブエッジを表す論理式を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::logic_error FFセル以外の場合
  /// @sa Expr
  Expr
  clock_expr() const;

  /// @brief FFセルの場合にスレーブクロックのアクティブエッジを表す論理式を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::logic_error FFセル以外の場合
  /// @sa Expr
  Expr
  clock2_expr() const;

  /// @brief FFセルの場合に次状態関数を表す論理式を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::logic_error FFセル以外の場合
  /// @sa Expr
  Expr
  next_state_expr() const;

  /// @brief ラッチセルの場合にイネーブル条件を表す論理式を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::logic_error ラッチセル以外の場合
  /// @sa Expr
  Expr
  enable_expr() const;

  /// @brief ラッチセルの場合に2つめのイネーブル条件を表す論理式を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::logic_error ラッチセル以外の場合
  /// @sa Expr
  Expr
  enable2_expr() const;

  /// @brief ラッチセルの場合にデータ入力関数を表す論理式を返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::logic_error ラッチセル以外の場合
  /// @sa Expr
  Expr
  data_in_expr() const;

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////

};

END_NAMESPACE_YM_CLIB

#endif // YM_CLIBCELL_H
