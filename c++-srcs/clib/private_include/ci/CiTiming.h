#ifndef CITIMING_H
#define CITIMING_H

/// @file　CiTiming.h
/// @brief ClibTiming の実装クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021, 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"
#include "ym/ClibTime.h"
#include "ym/ClibResistance.h"
#include "ym/Expr.h"
#include "ci/CiLut.h"
#include "ci/CiStLut.h"


BEGIN_NAMESPACE_YM_CLIB

class Serializer;
class Deserializer;

//////////////////////////////////////////////////////////////////////
/// @class CiTiming CiTiming.h "CiTiming.h"
/// @brief 共通の基底クラス
//////////////////////////////////////////////////////////////////////
class CiTiming
{
protected:

  /// @brief restore() 用のコンストラクタ
  CiTiming() = default;

  /// @brief コンストラクタ
  CiTiming(
    ClibTimingType type,      ///< [in] タイミング条件の型
    const Expr& cond          ///< [in] タイミング条件を表す式
  ) : mType{type},
      mCond{cond}
  {
  };


public:

  /// @brief CMOSジェネリックタイプのインスタンスを生成する．
  static
  std::unique_ptr<CiTiming>
  new_Generic(
    ClibTimingType timing_type,     ///< [in] タイミングの型
    const Expr& cond,               ///< [in] タイミング条件を表す式
    ClibTime intrinsic_rise,        ///< [in] 立ち上がり固有遅延
    ClibTime intrinsic_fall,        ///< [in] 立ち下がり固有遅延
    ClibTime slope_rise,            ///< [in] 立ち上がりスロープ遅延
    ClibTime slope_fall,            ///< [in] 立ち下がりスロープ遅延
    ClibResistance rise_resistance, ///< [in] 立ち上がり遷移遅延パラメータ
    ClibResistance fall_resistance  ///< [in] 立ち下がり遷移遅延パラメータ
  );

  /// @brief CMOS折れ線近似タイプのインスタンスを生成する．
  static
  std::unique_ptr<CiTiming>
  new_Piecewise(
    ClibTimingType timing_type,         ///< [in] タイミングの型
    const Expr& cond,                   ///< [in] タイミング条件を表す式
    ClibTime intrinsic_rise,            ///< [in] 立ち上がり固有遅延
    ClibTime intrinsic_fall,            ///< [in] 立ち下がり固有遅延
    ClibTime slope_rise,                ///< [in] 立ち上がりスロープ遅延
    ClibTime slope_fall,                ///< [in] 立ち下がりスロープ遅延
    const std::vector<ClibResistance>& rise_pin_resistance, ///< [in] 立ち上がりピン抵抗
    const std::vector<ClibResistance>& fall_pin_resistance, ///< [in] 立ち下がりピン抵抗
    const std::vector<ClibTime>& rise_delay_intercept,      ///< [in] 立ち上がりY切片
    const std::vector<ClibTime>& fall_delay_intercept       ///< [in] 立ち下がりY切片
  );

  /// @brief CMOS非線形タイプ(cell)のインスタンスを生成する．
  static
  std::unique_ptr<CiTiming>
  new_Lut_cell(
    ClibTimingType timing_type,               ///< [in] タイミングの型
    const Expr& cond,                         ///< [in] タイミング条件を表す式
    std::unique_ptr<CiLut>&& cell_rise,       ///< [in] 立ち上がりセル遅延テーブル
    std::unique_ptr<CiLut>&& cell_fall,       ///< [in] 立ち下がりセル遅延テーブル
    std::unique_ptr<CiLut>&& rise_transition, ///< [in] 立ち上がり遷移遅延テーブル
    std::unique_ptr<CiLut>&& fall_transition  ///< [in] 立ち下がり遷移遅延テーブル
  );

  /// @brief CMOS非線形タイプ(propagation)のインスタンスを生成する．
  static
  std::unique_ptr<CiTiming>
  new_Lut_prop(
    ClibTimingType timing_type,                ///< [in] タイミングの型
    const Expr& cond,                          ///< [in] タイミング条件を表す式
    std::unique_ptr<CiLut>&& rise_transition,  ///< [in] 立ち上がり遷移遅延テーブル
    std::unique_ptr<CiLut>&& fall_transition,  ///< [in] 立ち下がり遷移遅延テーブル
    std::unique_ptr<CiLut>&& rise_propagation, ///< [in] 立ち上がり伝搬遅延テーブル
    std::unique_ptr<CiLut>&& fall_propagation  ///< [in] 立ち下がり伝搬遅延テーブル
  );

  /// @brief CMOS非線形タイプ(cell)のインスタンスを生成する．
  static
  std::unique_ptr<CiTiming>
  new_Lut_cell(
    ClibTimingType timing_type,                 ///< [in] タイミングの型
    const Expr& cond,                           ///< [in] タイミング条件を表す式
    std::unique_ptr<CiStLut>&& cell_rise,       ///< [in] 立ち上がりセル遅延テーブル
    std::unique_ptr<CiStLut>&& cell_fall,       ///< [in] 立ち下がりセル遅延テーブル
    std::unique_ptr<CiStLut>&& rise_transition, ///< [in] 立ち上がり遷移遅延テーブル
    std::unique_ptr<CiStLut>&& fall_transition  ///< [in] 立ち下がり遷移遅延テーブル
  );

  /// @brief CMOS非線形タイプ(propagation)のインスタンスを生成する．
  static
  std::unique_ptr<CiTiming>
  new_Lut_prop(
    ClibTimingType timing_type,                  ///< [in] タイミングの型
    const Expr& cond,                            ///< [in] タイミング条件を表す式
    std::unique_ptr<CiStLut>&& rise_transition,  ///< [in] 立ち上がり遷移遅延テーブル
    std::unique_ptr<CiStLut>&& fall_transition,  ///< [in] 立ち下がり遷移遅延テーブル
    std::unique_ptr<CiStLut>&& rise_propagation, ///< [in] 立ち上がり伝搬遅延テーブル
    std::unique_ptr<CiStLut>&& fall_propagation  ///< [in] 立ち下がり伝搬遅延テーブル
  );

  /// @brief デストラクタ
  virtual
  ~CiTiming() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 共通の属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  ClibTimingType
  type() const
  {
    return mType;
  }

  /// @brief タイミング条件式の取得
  ///
  /// ない場合には不適正な式が返される．
  Expr
  timing_cond() const
  {
    return mCond;
  }

  /// @brief 立ち上がり遅延時間を計算する．
  virtual
  ClibTime
  calc_rise_delay(
    ClibTime input_transition,         ///< [in] 入力信号の遷移時間
    ClibCapacitance output_capacitance ///< [in] 出力の負荷容量
  ) const = 0;

  /// @brief 立ち下がり遅延時間を計算する．
  virtual
  ClibTime
  calc_fall_delay(
    ClibTime input_transition,         ///< [in] 入力信号の遷移時間
    ClibCapacitance output_capacitance ///< [in] 出力の負荷容量
  ) const = 0;

  /// @brief 立ち上がり遷移時間を計算する．
  ///
  /// 立ち上がり遷移時間は出力信号がしきい値1(通常20%)を超えてから
  /// しきい値2(通常80%)を超えるまでの時間
  virtual
  ClibTime
  calc_rise_transition(
    ClibTime input_transition,         ///< [in] 入力信号の遷移時間
    ClibCapacitance output_capacitance ///< [in] 出力の負荷容量
  ) const = 0;

  /// @brief 立ち下がり遷移時間を計算する．
  ///
  /// 立ち上がり遷移時間は出力信号がしきい値1(通常80%)を下回ってから
  /// しきい値2(通常20%)を下回るまでの時間
  virtual
  ClibTime
  calc_fall_transition(
    ClibTime input_transition,         ///< [in] 入力信号の遷移時間
    ClibCapacitance output_capacitance ///< [in] 出力の負荷容量
  ) const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // CMOSジェネリック/CMOS折れ線近似遅延モデルの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり固有遅延の取得
  virtual
  ClibTime
  intrinsic_rise() const;

  /// @brief 立ち下がり固有遅延の取得
  virtual
  ClibTime
  intrinsic_fall() const;

  /// @brief 立ち上がりスロープ遅延の取得
  virtual
  ClibTime
  slope_rise() const;

  /// @brief 立ち下がりスロープ遅延の取得
  virtual
  ClibTime
  slope_fall() const;


public:
  //////////////////////////////////////////////////////////////////////
  // CMOSジェネリック遅延モデルの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり遷移遅延の取得
  virtual
  ClibResistance
  rise_resistance() const;

  /// @brief 立ち下がり遷移遅延の取得
  virtual
  ClibResistance
  fall_resistance() const;


public:
  //////////////////////////////////////////////////////////////////////
  // CMOS折れ線近似遅延モデルの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がりピン抵抗の取得
  virtual
  ClibResistance
  rise_pin_resistance(
    SizeType piece_id ///< [in] 区間番号
  ) const;

  /// @brief 立ち下がりピン抵抗の取得
  virtual
  ClibResistance
  fall_pin_resistance(
    SizeType piece_id ///< [in] 区間番号
  ) const;

  /// @brief 立ち上がりY切片の取得
  virtual
  ClibTime
  rise_delay_intercept(
    SizeType piece_id ///< [in] 区間番号
  ) const;

  /// @brief 立ち下がりY切片の取得
  virtual
  ClibTime
  fall_delay_intercept(
    SizeType piece_id ///< [in] 区間番号
  ) const;


public:
  //////////////////////////////////////////////////////////////////////
  // CMOS非線形遅延モデルの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり遷移遅延テーブルの取得
  virtual
  const CiLut*
  rise_transition() const;

  /// @brief 立ち下がり遷移遅延テーブルの取得
  virtual
  const CiLut*
  fall_transition() const;

  /// @brief 立ち上がり伝搬遅延テーブルの取得
  virtual
  const CiLut*
  rise_propagation() const;

  /// @brief 立ち下がり伝搬遅延テーブルの取得
  virtual
  const CiLut*
  fall_propagation() const;

  /// @brief 立ち上がりセル遅延テーブルの取得
  virtual
  const CiLut*
  cell_rise() const;

  /// @brief 立ち下がりセル遅延テーブルの取得
  virtual
  const CiLut*
  cell_fall() const;


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
  std::unique_ptr<CiTiming>
  restore(
    Deserializer& s ///< [in] デシリアライザ
  );


protected:
  //////////////////////////////////////////////////////////////////////
  // dump/restore 用の下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 共通な情報をダンプする．
  void
  dump_common(
    Serializer& s, ///< [in] シリアライザ
    int type_id    ///< [in] ClibTimingType を表すシグネチャ
  ) const;

  /// @brief 内容を復元する．
  virtual
  void
  _restore(
    Deserializer& s  ///< [in] デシリアライザ
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 型
  ClibTimingType mType{ClibTimingType::none};

  // タイミング条件
  Expr mCond;

};

END_NAMESPACE_YM_CLIB

#endif // CITIMING_H
