#ifndef CICELL_H
#define CICELL_H

/// @file CiCell.h
/// @brief CiCell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"
#include "ym/ClibArea.h"
#include "ym/logic.h"
#include "ym/ShString.h"
#include "ci/CiPin.h"
#include "ci/CiBus.h"
#include "ci/CiBundle.h"
#include "ci/CiTiming.h"
#include "ci/conv_list.h"


BEGIN_NAMESPACE_YM_CLIB

class CiCellLibrary;
class CiCellGroup;
class CiBusType;
class CiLut;
class CiStLut;
class CgSignature;
class Serializer;
class Deserializer;

//////////////////////////////////////////////////////////////////////
/// @class CiCell CiCell.h "CiCell.h"
/// @brief ClibCell の実装クラス
///
/// このクラスはデフォルトの仮想関数を実装している．
//////////////////////////////////////////////////////////////////////
class CiCell
{
protected:

  /// @brief 空のコンストラクタ
  CiCell() = default;

  /// @brief コンストラクタ
  CiCell(
    const ShString& name, ///< [in] 名前
    ClibArea area         ///< [in] 面積
  ) : mName{name},
      mArea{area}
  {
  }


public:

  /// @brief 論理セルを生成するクラスメソッド
  static
  std::unique_ptr<CiCell>
  new_Logic(
    const ShString& name, ///< [in] 名前
    ClibArea area         ///< [in] 面積
  );

  /// @brief FFセルを生成するクラスメソッド
  ///
  /// clocked_on_also が空の場合には single-stage 型になる．
  static
  std::unique_ptr<CiCell>
  new_FF(
    const ShString& name,        ///< [in] 名前
    ClibArea area,               ///< [in] 面積
    const ShString& var1,        ///< [in] 内部変数1の名前
    const ShString& var2,        ///< [in] 内部変数2の名前
    const Expr& clocked_on,      ///< [in] "clocked_on" 関数の式
    const Expr& clocked_on_also, ///< [in] "clocked_on_also" 関数の式
    const Expr& next_state,      ///< [in] "next_state" 関数の式
    const Expr& clear,           ///< [in] "clear" 関数の式
    const Expr& preset,          ///< [in] "preset" 関数の式
    ClibSeqAttr seq_attr         ///< [in] 順序セルの属性
  );

  /// @brief single-stage 型のラッチセルを生成するクラスメソッド
  static
  std::unique_ptr<CiCell>
  new_Latch(
    const ShString& name,        ///< [in] 名前
    ClibArea area,               ///< [in] 面積
    const ShString& var1,        ///< [in] 内部変数1の名前
    const ShString& var2,        ///< [in] 内部変数2の名前
    const Expr& enable,          ///< [in] "enable" 関数の式
    const Expr& enable_also,	 ///< [in] "enable_also" 関数の式
    const Expr& data_in,         ///< [in] "data_in" 関数の式
    const Expr& clear,           ///< [in] "clear" 関数の式
    const Expr& preset,          ///< [in] "preset" 関数の式
    ClibSeqAttr seq_attr         ///< [in] 順序セルの属性
  );

  /// @brief FSM型の順序セルを生成するクラスメソッド
  static
  std::unique_ptr<CiCell>
  new_FSM(
    const ShString& name,     ///< [in] 名前
    ClibArea area             ///< [in] 面積
  );

  /// @brief デストラクタ
  virtual
  ~CiCell() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 基本情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のライブラリを返す．
  const CiCellLibrary*
  library() const;

  /// @brief 親のグループの取得
  const CiCellGroup*
  group() const
  {
    return mGroup;
  }

  /// @brief 名前の取得
  std::string
  name() const
  {
    return mName;
  }

  /// @brief 名前の取得
  ShString
  _name() const
  {
    return mName;
  }

  /// @brief 面積の取得
  ClibArea
  area() const
  {
    return mArea;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 参照回数の管理
  //////////////////////////////////////////////////////////////////////

  /// @brief 参照回数を増やす．
  void
  inc_ref() const;

  /// @brief 参照回数を減らす．
  void
  dec_ref() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ピン情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief ピン数の取得
  SizeType
  pin_num() const
  {
    return mPinList.size();
  }

  /// @brief ピンの取得
  const CiPin*
  pin(
    SizeType pos ///< [in] ピン番号 ( 0 <= pos < pin_num() )
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < pin_num() );
    return mPinList[pos].get();
  }

  /// @brief 名前からピンの取得
  ///
  /// なければ nullptr を返す．
  const CiPin*
  find_pin(
    const ShString& name ///< [in] ピン名
  ) const;

  /// @brief ピンのリストの取得
  std::vector<const CiPin*>
  pin_list() const
  {
    return conv_list(mPinList);
  }

  /// @brief ピンのリストの取得
  const std::vector<std::unique_ptr<CiPin>>&
  _pin_list() const
  {
    return mPinList;
  }

  /// @brief 入力ピン数の取得
  SizeType
  input_num() const
  {
    return mInputNum;
  }

  /// @brief 入力ピンの取得
  const CiPin*
  input(
    SizeType pos ///< [in] 入力番号 ( 0 <= pos < input2_num() )
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < input2_num() );
    return mInputList[pos];
  }

  /// @brief 入力ピンのリストの取得
  const std::vector<const CiPin*>&
  input_list() const
  {
    return mInputList;
  }

  /// @brief 出力ピン数の取得
  SizeType
  output_num() const
  {
    return mOutputNum;
  }

  /// @brief 出力ピンの取得
  ///
  /// pos >= output_num() の場合には入出力ピンが返される．
  const CiPin*
  output(
    SizeType pos ///< [in] 出力番号 ( 0 <= id < output2_num() )
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < output2_num() );
    return mOutputList[pos];
  }

  /// @brief 出力ピンのリスト
  const std::vector<const CiPin*>&
  output_list() const
  {
    return mOutputList;
  }

  /// @brief 入出力ピン数の取得
  SizeType
  inout_num() const
  {
    return mInoutNum;
  }

  /// @brief 入出力ピンの取得
  const CiPin*
  inout(
    SizeType id ///< [in] 番号 ( 0 <= id < inout_num() )
  ) const
  {
    ASSERT_COND( 0 <= id && id < inout_num() );
    return mInputList[id + input_num()];
  }

  /// @brief 入出力ピン番号のリストの先頭
  std::vector<const CiPin*>::const_iterator
  inout_begin() const
  {
    return mInputList.begin() + input_num();
  }

  /// @brief 入力ピン+入出力ピン数の取得
  ///
  /// input_num() + inout_num() に等しい．
  SizeType
  input2_num() const
  {
    return mInputList.size();
  }

  /// @brief 出力ピン+入出力ピン数の取得
  ///
  /// output_num() + inout_num() に等しい．
  SizeType
  output2_num() const
  {
    return mOutputList.size();
  }

  /// @brief 内部ピン数の取得
  SizeType
  internal_num() const
  {
    return mInternalList.size();
  }

  /// @brief 内部ピンの取得
  const CiPin*
  internal(
    SizeType pos ///< [in] 内部ピン番号 ( 0 <= pos < internal_num() )
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < mInternalList.size() );
    return mInternalList[pos].get();
  }

  /// @brief 内部ピンのリスト
  std::vector<const CiPin*>
  internal_list() const
  {
    return conv_list(mInternalList);
  }

  /// @brief バス数の取得
  SizeType
  bus_num() const
  {
    return mBusList.size();
  }

  /// @brief バスの取得
  const CiBus*
  bus(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < bus_num() )
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < bus_num() );
    return mBusList[pos].get();
  }

  /// @brief 名前からバスの取得
  const CiBus*
  find_bus(
    const ShString& name ///< [in] 名前
  ) const;

  /// @brief バスのリスト
  std::vector<const CiBus*>
  bus_list() const
  {
    return conv_list(mBusList);
  }

  /// @brief バスのリスト
  const std::vector<std::unique_ptr<CiBus>>&
  _bus_list() const
  {
    return mBusList;
  }

  /// @brief バンドル数の取得
  SizeType
  bundle_num() const
  {
    return mBundleList.size();
  }

  /// @brief バンドルの取得
  const CiBundle*
  bundle(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < bundle_num() )
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < bundle_num() );
    return mBundleList[pos].get();
  }

  /// @brief 名前からバスの取得
  const CiBundle*
  find_bundle(
    const ShString& name ///< [in] 名前
  ) const;

  /// @brief バンドルのリスト
  std::vector<const CiBundle*>
  bundle_list() const
  {
    return conv_list(mBundleList);
  }

  /// @brief バンドルのリスト
  const std::vector<std::unique_ptr<CiBundle>>&
  _bundle_list() const
  {
    return mBundleList;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // タイミング情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 条件に合致するタイミング情報のインデックスのリストを返す．
  const std::vector<const CiTiming*>&
  timing_list(
    SizeType ipos,        ///< [in] 開始ピン番号 ( 0 <= ipos < input_num2() )
    SizeType opos,        ///< [in] 終了ピン番号 ( 0 <= opos < output_num2() )
    ClibTimingSense sense ///< [in] タイミング情報の摘要条件
  ) const
  {
    SizeType base = (opos * input2_num() + ipos) * 2;
    switch ( sense ) {
    case ClibTimingSense::positive_unate: base += 0; break;
    case ClibTimingSense::negative_unate: base += 1; break;
    default:
      ASSERT_NOT_REACHED;
    }
    return mTimingMap[base];
  }


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 機能情報の取得
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief セルの種類を返す．
  virtual
  ClibCellType
  type() const;

  /// @brief 組み合わせ論理タイプの時 true を返す．
  virtual
  bool
  is_logic() const;

  /// @brief FFタイプの時 true を返す．
  virtual
  bool
  is_ff() const;

  /// @brief ラッチタイプの時 true を返す．
  virtual
  bool
  is_latch() const;

  /// @brief FSMタイプの時 true を返す．
  virtual
  bool
  is_fsm() const;

  /// @brief 内部変数1の名前を返す．
  virtual
  std::string
  qvar1() const;

  /// @brief 内部変数1の名前を返す．
  virtual
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
  virtual
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
  virtual
  Expr
  preset_expr() const;

  /// @brief 順序セルの属性を返す．
  virtual
  ClibSeqAttr
  seq_attr() const;

  /// @brief FFセルの場合にクロックのアクティブエッジを表す論理式を返す．
  ///
  /// それ以外の型の場合の返り値は不定
  virtual
  Expr
  clock_expr() const;

  /// @brief FFセルの場合にスレーブクロックのアクティブエッジを表す論理式を返す．
  ///
  /// それ以外の型の場合の返り値は不定
  virtual
  Expr
  clock2_expr() const;

  /// @brief FFセルの場合に次状態関数を表す論理式を返す．
  ///
  /// それ以外の型の場合の返り値は不定
  virtual
  Expr
  next_state_expr() const;

  /// @brief ラッチセルの場合にイネーブル条件を表す論理式を返す．
  ///
  /// それ以外の型の場合の返り値は不定
  virtual
  Expr
  enable_expr() const;

  /// @brief ラッチセルの場合に2つめのイネーブル条件を表す論理式を返す．
  ///
  /// それ以外の型の場合の返り値は不定
  virtual
  Expr
  enable2_expr() const;

  /// @brief ラッチセルの場合にデータ入力関数を表す論理式を返す．
  ///
  /// それ以外の型の場合の返り値は不定
  virtual
  Expr
  data_in_expr() const;

  /// @brief シグネチャを返す．
  virtual
  CgSignature
  make_signature() const;

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のグループを設定する．
  void
  set_group(
    const CiCellGroup* group ///< [in] 親のグループ
  )
  {
    mGroup = group;
  }

  /// @brief 入力ピンを追加する．
  CiPin*
  add_input(
    const ShString& name,             ///< [in] ピン名
    ClibCapacitance capacitance,      ///< [in] 負荷容量
    ClibCapacitance rise_capacitance, ///< [in] 立ち上がり時の負荷容量
    ClibCapacitance fall_capacitance  ///< [in] 立ち下がり時の負荷容量
  );

  /// @brief 出力ピンを追加する．
  CiPin*
  add_output(
    const ShString& name,            ///< [in] ピン名
    ClibCapacitance max_fanout,      ///< [in] 最大ファンアウト容量
    ClibCapacitance min_fanout,      ///< [in] 最大ファンアウト容量
    ClibCapacitance max_capacitance, ///< [in] 最大負荷容量
    ClibCapacitance min_capacitance, ///< [in] 最大負荷容量
    ClibTime max_transition,         ///< [in] 最大遷移時間
    ClibTime min_transition,         ///< [in] 最大遷移時間
    const Expr& function,            ///< [in] 出力の論理式
    const Expr& tristate             ///< [in] tristate 条件
  );

  /// @brief 入出力ピンを追加する．
  CiPin*
  add_inout(
    const ShString& name,             ///< [in] ピン名
    ClibCapacitance capacitance,      ///< [in] 負荷容量
    ClibCapacitance rise_capacitance, ///< [in] 立ち上がり時の負荷容量
    ClibCapacitance fall_capacitance, ///< [in] 立ち上がり時の負荷容量
    ClibCapacitance max_fanout,	      ///< [in] 最大ファンアウト容量
    ClibCapacitance min_fanout,	      ///< [in] 最大ファンアウト容量
    ClibCapacitance max_capacitance,  ///< [in] 最大負荷容量
    ClibCapacitance min_capacitance,  ///< [in] 最大負荷容量
    ClibTime max_transition,	      ///< [in] 最大遷移時間
    ClibTime min_transition,          ///< [in] 最大遷移時間
    const Expr& function,             ///< [in] 出力の論理式
    const Expr& tristate              ///< [in] tristate 条件
  );

  /// @brief 内部ピンを追加する．
  CiPin*
  add_internal(
    const ShString& name ///< [in] 名前
  );

  /// @brief バスを追加する．
  CiBus*
  add_bus(
    const ShString& name,                     ///< [in] 名前
    const CiBusType* bus_type,                ///< [in] バスタイプ
    const std::vector<const CiPin*>& pin_list ///< [in] ピンリスト
  );

  /// @brief バンドルを追加する．
  CiBundle*
  add_bundle(
    const ShString& name,                     ///< [in] 名前
    const std::vector<const CiPin*>& pin_list ///< [in] ピンリスト
  );

  /// @brief タイミング情報を作る(ジェネリック遅延モデル)．
  /// @return 生成されたタイミングを返す．
  CiTiming*
  add_timing_generic(
    ClibTimingType type,            ///< [in] タイミングの種類
    const Expr& cond,               ///< [in] 条件式
    ClibTime intrinsic_rise,        ///< [in] 立ち上がり固有遅延
    ClibTime intrinsic_fall,        ///< [in] 立ち下がり固有遅延
    ClibTime slope_rise,            ///< [in] 立ち上がり負荷依存遅延
    ClibTime slope_fall,            ///< [in] 立ち下がり負荷依存遅延
    ClibResistance rise_resistance, ///< [in] 立ち上がり抵抗
    ClibResistance fall_resistance  ///< [in] 立ち下がり抵抗
  );

  /// @brief タイミング情報を作る(折れ線近似)．
  /// @return 生成されたタイミングを返す．
  CiTiming*
  add_timing_piecewise(
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
  );

  /// @brief タイミング情報を作る(非線形タイプ1)．
  /// @return 生成されたタイミングを返す．
  CiTiming*
  add_timing_lut_cell(
    ClibTimingType timing_type,
    const Expr& cond,
    std::unique_ptr<CiLut>&& cell_rise,
    std::unique_ptr<CiLut>&& cell_fall,
    std::unique_ptr<CiLut>&& rise_transition,
    std::unique_ptr<CiLut>&& fall_transition
  );

  /// @brief タイミング情報を作る(非線形タイプ2)．
  /// @return 生成されたタイミングを返す．
  CiTiming*
  add_timing_lut_prop(
    ClibTimingType timing_type,
    const Expr& cond,
    std::unique_ptr<CiLut>&& rise_transition,
    std::unique_ptr<CiLut>&& fall_transition,
    std::unique_ptr<CiLut>&& rise_propagation,
    std::unique_ptr<CiLut>&& fall_propagation
  );

  /// @brief タイミング情報を作る(非線形タイプ1)．
  /// @return 生成されたタイミングを返す．
  CiTiming*
  add_timing_lut_cell(
    ClibTimingType timing_type,
    const Expr& cond,
    std::unique_ptr<CiStLut>&& cell_rise,
    std::unique_ptr<CiStLut>&& cell_fall,
    std::unique_ptr<CiStLut>&& rise_transition,
    std::unique_ptr<CiStLut>&& fall_transition
  );

  /// @brief タイミング情報を作る(非線形タイプ2)．
  /// @return 生成されたタイミングを返す．
  CiTiming*
  add_timing_lut_prop(
    ClibTimingType timing_type,
    const Expr& cond,
    std::unique_ptr<CiStLut>&& rise_transition,
    std::unique_ptr<CiStLut>&& fall_transition,
    std::unique_ptr<CiStLut>&& rise_propagation,
    std::unique_ptr<CiStLut>&& fall_propagation
  );

  /// @brief タイミング情報用のデータ構造を初期化する．
  void
  init_timing_map();

  /// @brief タイミング情報をセットする(単独のタイミング情報)．
  ///
  /// この関数を呼ぶ前に init_timing_map() を呼んでおくこと．
  void
  set_timing(
    SizeType ipin_id,                   ///< [in] 入力ピン番号
    SizeType opin_id,                   ///< [in] 出力ピン番号
    ClibTimingSense timing_sense,       ///< [in] タイミング条件
    const CiTiming* timing              ///< [in] 設定するタイミング
  )
  {
    set_timing(ipin_id, opin_id, timing_sense,
	       std::vector<const CiTiming*>{timing});
  }

  /// @brief タイミング情報をセットする(複数のタイミング情報)．
  ///
  /// この関数を呼ぶ前に init_timing_map() を呼んでおくこと．
  void
  set_timing(
    SizeType ipin_id,                               ///< [in] 入力ピン番号
    SizeType opin_id,                               ///< [in] 出力ピン番号
    ClibTimingSense timing_sense,                   ///< [in] タイミング条件
    const std::vector<const CiTiming*>& timing_list ///< [in] 設定するタイミング番号のリスト
  );


public:
  //////////////////////////////////////////////////////////////////////
  // dump/restore 関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をシリアライズする．
  void
  serialize(
    Serializer& s ///< [in] シリアライザ
  ) const;

  /// @brief 内容をバイナリダンプする．
  virtual
  void
  dump(
    Serializer& s ///< [in] シリアライザ
  ) const = 0;

  /// @brief 内容を読み込む．
  static
  std::unique_ptr<CiCell>
  restore(
    Deserializer& s ///< [in] デシリアライザ
  );


protected:
  //////////////////////////////////////////////////////////////////////
  // dump/restore の下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 共通部分のダンプ
  void
  dump_common(
    Serializer& s ///< [in] シリアライザ
  ) const;

  /// @brief restore() の下請け関数
  virtual
  void
  _restore(
    Deserializer& s ///< [in] デシリアライザ
  ) = 0;

  /// @brief restore() の共通部分
  void
  restore_common(
    Deserializer& s ///< [in] デシリアライザ
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のグループ
  const CiCellGroup* mGroup{nullptr};

  // 名前
  ShString mName;

  // 面積
  ClibArea mArea{0};

  // 入力ピン数
  SizeType mInputNum{0};

  // 出力ピン数
  SizeType mOutputNum{0};

  // 入出力ピン数
  SizeType mInoutNum{0};

  // ピンのリスト
  std::vector<std::unique_ptr<CiPin>> mPinList;

  // 入力ピン+入出力ピンのリスト
  // サイズ mInputNum + mInoutNum
  std::vector<const CiPin*> mInputList;

  // 出力ピン+入出力ピンのリスト
  // サイズ mOutputNum + mInoutNum
  std::vector<const CiPin*> mOutputList;

  // 内部ピンのリスト
  std::vector<std::unique_ptr<CiPin>> mInternalList;

  // バスのリスト
  std::vector<std::unique_ptr<CiBus>> mBusList;

  // バンドルのリスト
  std::vector<std::unique_ptr<CiBundle>> mBundleList;

  // 全体のタイミング情報のリスト
  std::vector<std::unique_ptr<CiTiming>> mTimingList;

  // 条件ごとのタイミングのリストの配列
  // サイズは(入力数＋入出力数) x (出力数+入出力数)  x 2
  std::vector<std::vector<const CiTiming*>> mTimingMap;

};

END_NAMESPACE_YM_CLIB

#endif // CICELL_H
