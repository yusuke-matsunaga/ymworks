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
  /// 不正値となる．
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
  ClibCellLibrary
  library() const;

  /// @brief 親のセルグループの取得
  ClibCellGroup
  group() const;

  /// @brief 名前の取得
  std::string
  name() const;

  /// @brief 面積の取得
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
  SizeType
  pin_num() const;

  /// @brief ピンの取得
  /// @return ピン情報を返す．
  ClibPin
  pin(
    SizeType pos ///< [in] ピン番号 ( 0 <= pos < pin_num() )
  ) const;

  /// @brief 名前からピン番号の取得
  /// @return name という名前のピンを返す．
  ///
  /// なければ不正値を返す．
  ClibPin
  pin(
    const std::string& name ///< [in] ピン名
  ) const;

  /// @brief ピンのリストを返す．
  ClibPinList
  pin_list() const;

  /// @brief 入力ピン数の取得
  SizeType
  input_num() const;

  /// @brief 入力ピンの取得
  ClibPin
  input(
    SizeType pos ///< [in] 番号 ( 0 <= pos < input_num() )
  ) const;

  /// @brief 入力ピンのリストの取得
  ClibPinList
  input_list() const;

  /// @brief 出力ピン数の取得
  SizeType
  output_num() const;

  /// @brief 出力ピンの取得
  ClibPin
  output(
    SizeType pos ///< [in] 番号 ( 0 <= pos < output_num() )
  ) const;

  /// @brief 出力ピンのリストの取得
  ClibPinList
  output_list() const;

  /// @brief 入出力ピン数の取得
  SizeType
  inout_num() const;

  /// @brief 入出力ピンの取得
  ClibPin
  inout(
    SizeType pos ///< [in] 番号 ( 0 <= pos < inout_num() )
  ) const;

  /// @brief 入出力ピンのリストの取得
  ClibPinList
  inout_list() const;

  /// @brief 入力ピン+入出力ピン数の取得
  ///
  /// input_num() + inout_num() に等しい．
  SizeType
  input2_num() const;

  /// @brief 入力ピンの取得
  ///
  /// id >= input_num() の場合には入出力ピンが返される．
  ClibPin
  input2(
    SizeType pos ///< [in] 番号 ( 0 <= pos < input_num2() )
  ) const;

  /// @brief 出力ピン+入出力ピン数の取得
  ///
  /// output_num() + inout_num() に等しい．
  SizeType
  output2_num() const;

  /// @brief 出力ピンの取得
  ///
  /// id >= output_num() の場合には入出力ピンが返される．
  ClibPin
  output2(
    SizeType pos ///< [in] 番号 ( 0 <= pos < output_num2() )
  ) const;

  /// @brief 内部ピン数の取得
  SizeType
  internal_num() const;

  /// @brief 内部ピンの取得
  ClibPin
  internal(
    SizeType pos ///< [in] 内部ピン番号 ( 0 <= pos < internal_num() )
  ) const;

  /// @brief 内部ピンのリストの取得
  ClibPinList
  internal_list() const;

  /// @brief バス数の取得
  SizeType
  bus_num() const;

  /// @brief バスの取得
  ClibBus
  bus(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < bus_num() )
  ) const;

  /// @brief 名前からバスの取得
  /// @return name という名前のバスを返す．
  ///
  /// なければ不正値を返す．
  ClibBus
  bus(
    const std::string& name ///< [in] バス名
  ) const;

  /// @brief バスのリストの取得
  ClibBusList
  bus_list() const;

  /// @brief バンドル数の取得
  SizeType
  bundle_num() const;

  /// @brief バンドルの取得
  ClibBundle
  bundle(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < bundle_num() )
  ) const;

  /// @brief 名前からバンドルの取得
  /// @return name という名前のバンドルを返す．
  ///
  /// なければ不正値を返す．
  ClibBundle
  bundle(
    const std::string& name ///< [in] バンドル名
  ) const;

  /// @brief バンドルのリストの取得
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
  ClibCellType
  type() const;

  /// @brief 組み合わせ論理タイプの時 true を返す．
  bool
  is_logic() const;

  /// @brief FFタイプの時 true を返す．
  bool
  is_ff() const;

  /// @brief ラッチタイプの時 true を返す．
  bool
  is_latch() const;

  /// @brief 出力の論理式を持っている時に true を返す．
  bool
  has_logic(
    SizeType pin_id ///< [in] 出力ピン番号 ( 0 <= pin_id < output_num2() )
  ) const;

  /// @brief 全ての出力が論理式を持っているときに true を返す．
  bool
  has_logic() const;

  /// @brief 論理セルの場合に出力の論理式を返す．
  ///
  /// 論理式中の変数番号は入力ピン番号に対応する．
  Expr
  logic_expr(
    SizeType pin_id ///< [in] 出力ピン番号 ( 0 <= pin_id < output_num2() )
  ) const;

  /// @brief 出力がトライステート条件を持っている時に true を返す．
  bool
  has_tristate(
    SizeType pin_id ///< [in] 出力ピン番号 ( 0 <= pin_id < output_num2() )
  ) const;

  /// @brief トライステートセルの場合にトライステート条件式を返す．
  ///
  /// - 論理式中の変数番号は入力ピン番号に対応する．
  /// - 通常の論理セルの場合には定数0を返す．
  Expr
  tristate_expr(
    SizeType pin_id ///< [in] 出力ピン番号 ( 0 <= pin_id < output_num2() )
  ) const;

  /// @brief 内部変数1の名前を返す．
  std::string
  qvar1() const;

  /// @brief 内部変数1の名前を返す．
  std::string
  qvar2() const;

  /// @brief 非同期 clear を持つ時 true を返す．
  ///
  /// FF/ラッチセル以外の場合には返り値は不定
  bool
  has_clear() const;

  /// @brief FFセル/ラッチセルの場合にクリア条件を表す論理式を返す．
  ///
  /// クリア端子がない場合の返り値は不定
  Expr
  clear_expr() const;

  /// @brief 非同期 preset を持つ時 true を返す．
  ///
  /// FF/ラッチセル以外の場合には返り値は不定
  bool
  has_preset() const;

  /// @brief FFセル/ラッチセルの場合にプリセット条件を表す論理式を返す．
  ///
  /// プリセット端子がない場合の返り値は不定
  Expr
  preset_expr() const;

  /// @brief clear と preset が同時にアクティブになった時の値1
  ///
  /// has_clear() == true && has_preset() == true の時のみ意味を持つ．
  /// FF/ラッチセル以外の場合には返り値は不定
  ClibCPV
  clear_preset_var1() const;

  /// @brief clear と preset が同時にアクティブになった時の値1
  ///
  /// has_clear() == true && has_preset() == true の時のみ意味を持つ．
  /// FF/ラッチセル以外の場合には返り値は不定
  ClibCPV
  clear_preset_var2() const;

  /// @brief FFセルの場合にクロックのアクティブエッジを表す論理式を返す．
  ///
  /// それ以外の型の場合の返り値は不定
  Expr
  clock_expr() const;

  /// @brief FFセルの場合にスレーブクロックのアクティブエッジを表す論理式を返す．
  ///
  /// それ以外の型の場合の返り値は不定
  Expr
  clock2_expr() const;

  /// @brief FFセルの場合に次状態関数を表す論理式を返す．
  ///
  /// それ以外の型の場合の返り値は不定
  Expr
  next_state_expr() const;

  /// @brief ラッチセルの場合にイネーブル条件を表す論理式を返す．
  ///
  /// それ以外の型の場合の返り値は不定
  Expr
  enable_expr() const;

  /// @brief ラッチセルの場合に2つめのイネーブル条件を表す論理式を返す．
  ///
  /// それ以外の型の場合の返り値は不定
  Expr
  enable2_expr() const;

  /// @brief ラッチセルの場合にデータ入力関数を表す論理式を返す．
  ///
  /// それ以外の型の場合の返り値は不定
  Expr
  data_in_expr() const;

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////

};

END_NAMESPACE_YM_CLIB

#endif // YM_CLIBCELL_H
