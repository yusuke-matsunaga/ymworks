#ifndef CITIMING_SUB_H
#define CITIMING_SUB_H

/// @file　CiTiming_sub.h
/// @brief ClibTiming の実装クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ci/CiTiming.h"
#include "CiLut_sub.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
/// @class CiTiming CiTiming.h "CiTiming.h"
/// @brief CMOSジェネリック/折れ線近似タイプ共通の ClibTiming の実装クラス
//////////////////////////////////////////////////////////////////////
class CiTimingGP :
  public CiTiming
{
protected:

  /// @brief restore() 用のコンストラクタ
  CiTimingGP() = default;

  /// @brief コンストラクタ
  CiTimingGP(
    ClibTimingType timing_type, ///< [in] タイミングの型
    const Expr& cond,           ///< [in] タイミング条件を表す式
    ClibTime intrinsic_rise,    ///< [in] 立ち上がり固有遅延
    ClibTime intrinsic_fall,    ///< [in] 立ち下がり固有遅延
    ClibTime slope_rise,        ///< [in] 立ち上がりスロープ遅延
    ClibTime slope_fall         ///< [in] 立ち下がりスロープ遅延
  ) : CiTiming{timing_type, cond},
      mIntrinsicRise{intrinsic_rise},
      mIntrinsicFall{intrinsic_fall},
      mSlopeRise{slope_rise},
      mSlopeFall{slope_fall}
  {
  }

  /// @brief デストラクタ
  ~CiTimingGP() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // CMOSジェネリック/CMOS折れ線近似遅延モデルの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり固有遅延の取得
  ClibTime
  intrinsic_rise() const override;

  /// @brief 立ち下がり固有遅延の取得
  ClibTime
  intrinsic_fall() const override;

  /// @brief 立ち上がりスロープ遅延の取得
  ClibTime
  slope_rise() const override;

  /// @brief 立ち下がりスロープ遅延の取得
  ClibTime
  slope_fall() const override;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  void
  dump_GP(
    Serializer& s ///< [in] シリアライザ
  ) const;

  /// @brief 内容を復元する．
  void
  restore_GP(
    Deserializer& s ///< [in] デシリアライザ
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 立ち上がり固有遅延
  ClibTime mIntrinsicRise;

  // 立ち下がり固有遅延
  ClibTime mIntrinsicFall;

  // 立ち上がりスロープ遅延
  ClibTime mSlopeRise;

  // 立ち下がりスロープ遅延
  ClibTime mSlopeFall;

};


//////////////////////////////////////////////////////////////////////
/// @class CiTimingGeneric CiTiming.h "CiTiming.h"
/// @brief CMOSジェネリックタイプの ClibTiming の実装クラス
//////////////////////////////////////////////////////////////////////
class CiTimingGeneric :
  public CiTimingGP
{
public:

  /// @brief restore() 用のコンストラクタ
  CiTimingGeneric() = default;

  /// @brief コンストラクタ
  CiTimingGeneric(
    ClibTimingType timing_type,     ///< [in] タイミングの型
    const Expr& cond,               ///< [in] タイミング条件を表す式
    ClibTime intrinsic_rise,        ///< [in] 立ち上がり固有遅延
    ClibTime intrinsic_fall,        ///< [in] 立ち下がり固有遅延
    ClibTime slope_rise,            ///< [in] 立ち上がりスロープ遅延
    ClibTime slope_fall,            ///< [in] 立ち下がりスロープ遅延
    ClibResistance rise_resistance, ///< [in] 立ち上がり抵抗
    ClibResistance fall_resistance  ///< [in] 立ち下がり抵抗
  ) : CiTimingGP{timing_type, cond,
		 intrinsic_rise, intrinsic_fall,
		 slope_rise, slope_fall},
      mRiseResistance(rise_resistance),
      mFallResistance(fall_resistance)
  {
  }

  /// @brief デストラクタ
  ~CiTimingGeneric() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 共通の属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり遅延時間を計算する．
  ClibTime
  calc_rise_delay(
    ClibTime input_transition,         ///< [in] 入力信号の遷移時間
    ClibCapacitance output_capacitance ///< [in] 出力の負荷容量
  ) const override;

  /// @brief 立ち下がり遅延時間を計算する．
  ClibTime
  calc_fall_delay(
    ClibTime input_transition,         ///< [in] 入力信号の遷移時間
    ClibCapacitance output_capacitance ///< [in] 出力の負荷容量
  ) const override;

  /// @brief 立ち上がり遷移時間を計算する．
  ///
  /// 立ち上がり遷移時間は出力信号がしきい値1(通常20%)を超えてから
  /// しきい値2(通常80%)を超えるまでの時間
  ClibTime
  calc_rise_transition(
    ClibTime input_transition,         ///< [in] 入力信号の遷移時間
    ClibCapacitance output_capacitance ///< [in] 出力の負荷容量
  ) const override;

  /// @brief 立ち下がり遷移時間を計算する．
  ///
  /// 立ち上がり遷移時間は出力信号がしきい値1(通常80%)を下回ってから
  /// しきい値2(通常20%)を下回るまでの時間
  ClibTime
  calc_fall_transition(
    ClibTime input_transition,         ///< [in] 入力信号の遷移時間
    ClibCapacitance output_capacitance ///< [in] 出力の負荷容量
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // CMOSジェネリック遅延モデルの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり抵抗の取得
  ClibResistance
  rise_resistance() const override;

  /// @brief 立ち下がり抵抗の取得
  ClibResistance
  fall_resistance() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // dump/restore 関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  void
  dump(
    Serializer& s ///< [in] シリアライザ
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を復元する．
  void
  _restore(
    Deserializer& s  ///< [in] デシリアライザ
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 立ち上がり遷移遅延
  ClibResistance mRiseResistance;

  // 立ち下がり遷移遅延
  ClibResistance mFallResistance;

};


//////////////////////////////////////////////////////////////////////
/// @class CiTimingPiecewise CiTiming.h "CiTiming.h"
/// @brief CMOS折れ線近似タイプの ClibTiming の実装クラス
//////////////////////////////////////////////////////////////////////
class CiTimingPiecewise :
  public CiTimingGP
{
public:

  /// @brief restore() 用のコンストラクタ
  CiTimingPiecewise() = default;

  /// @brief コンストラクタ
  CiTimingPiecewise(
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
  ) : CiTimingGP{timing_type, cond,
		 intrinsic_rise, intrinsic_fall,
		 slope_rise, slope_fall},
      mRisePinResistance{rise_pin_resistance},
      mFallPinResistance{fall_pin_resistance},
      mRiseDelayIntercept{rise_delay_intercept},
      mFallDelayIntercept{fall_delay_intercept}
  {
  }

  /// @brief デストラクタ
  ~CiTimingPiecewise() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 共通の属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり遅延時間を計算する．
  ClibTime
  calc_rise_delay(
    ClibTime input_transition,         ///< [in] 入力信号の遷移時間
    ClibCapacitance output_capacitance ///< [in] 出力の負荷容量
  ) const override;

  /// @brief 立ち下がり遅延時間を計算する．
  ClibTime
  calc_fall_delay(
    ClibTime input_transition,         ///< [in] 入力信号の遷移時間
    ClibCapacitance output_capacitance ///< [in] 出力の負荷容量
  ) const override;

  /// @brief 立ち上がり遷移時間を計算する．
  ///
  /// 立ち上がり遷移時間は出力信号がしきい値1(通常20%)を超えてから
  /// しきい値2(通常80%)を超えるまでの時間
  ClibTime
  calc_rise_transition(
    ClibTime input_transition,         ///< [in] 入力信号の遷移時間
    ClibCapacitance output_capacitance ///< [in] 出力の負荷容量
  ) const override;

  /// @brief 立ち下がり遷移時間を計算する．
  ///
  /// 立ち上がり遷移時間は出力信号がしきい値1(通常80%)を下回ってから
  /// しきい値2(通常20%)を下回るまでの時間
  ClibTime
  calc_fall_transition(
    ClibTime input_transition,         ///< [in] 入力信号の遷移時間
    ClibCapacitance output_capacitance ///< [in] 出力の負荷容量
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // CMOS折れ線近似遅延モデルの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がりピン抵抗の取得
  ClibResistance
  rise_pin_resistance(
    SizeType piece_id ///< [in] 区間番号
  ) const override;

  /// @brief 立ち下がりピン抵抗の取得
  ClibResistance
  fall_pin_resistance(
    SizeType piece_id ///< [in] 区間番号
  ) const override;

  /// @brief 立ち上がりY切片の取得
  ClibTime
  rise_delay_intercept(
    SizeType piece_id ///< [in] 区間番号
  ) const override;

  /// @brief 立ち下がりY切片の取得
  ClibTime
  fall_delay_intercept(
    SizeType piece_id ///< [in] 区間番号
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // dump/restore 関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  void
  dump(
    Serializer& s ///< [in] シリアライザ
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を復元する．
  void
  _restore(
    Deserializer& s  ///< [in] デシリアライザ
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 立ち上がりピン抵抗のリスト
  std::vector<ClibResistance> mRisePinResistance;

  // 立ち下がりピン抵抗のリスト
  std::vector<ClibResistance> mFallPinResistance;

  // 立ち上がりY切片のリスト
  std::vector<ClibTime> mRiseDelayIntercept;

  // 立ち下がりY切片のリスト
  std::vector<ClibTime> mFallDelayIntercept;

};


//////////////////////////////////////////////////////////////////////
/// @class CiTimingLut CiTiming.h "CiTiming.h"
/// @brief CMOS非線形タイプの ClibTiming の共通の基底クラス
//////////////////////////////////////////////////////////////////////
class CiTimingLut :
  public CiTiming
{
public:

  /// @brief restore() 用のコンストラクタ
  CiTimingLut() = default;

  /// @brief コンストラクタ
  CiTimingLut(
    ClibTimingType timing_type,          ///< [in] タイミングの型
    const Expr& cond,                    ///< [in] タイミング条件を表す式
    std::unique_ptr<CiLut>&& rise_transition, ///< [in] 立ち上がり遷移遅延テーブル
    std::unique_ptr<CiLut>&& fall_transition  ///< [in] 立ち下がり遷移遅延テーブル
  ) : CiTiming{timing_type, cond},
      mRiseTransition{std::move(rise_transition)},
      mFallTransition{std::move(fall_transition)}
  {
  }

  /// @brief デストラクタ
  ~CiTimingLut() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 共通の属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり遷移時間を計算する．
  ///
  /// 立ち上がり遷移時間は出力信号がしきい値1(通常20%)を超えてから
  /// しきい値2(通常80%)を超えるまでの時間
  ClibTime
  calc_rise_transition(
    ClibTime input_transition,         ///< [in] 入力信号の遷移時間
    ClibCapacitance output_capacitance ///< [in] 出力の負荷容量
  ) const override;

  /// @brief 立ち下がり遷移時間を計算する．
  ///
  /// 立ち上がり遷移時間は出力信号がしきい値1(通常80%)を下回ってから
  /// しきい値2(通常20%)を下回るまでの時間
  ClibTime
  calc_fall_transition(
    ClibTime input_transition,         ///< [in] 入力信号の遷移時間
    ClibCapacitance output_capacitance ///< [in] 出力の負荷容量
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // CMOS非線形遅延モデルの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり遷移遅延テーブルの取得
  const CiLut*
  rise_transition() const override;

  /// @brief 立ち下がり遷移遅延テーブルの取得
  const CiLut*
  fall_transition() const override;

  /// @brief 立ち上がり伝搬遅延テーブルの取得
  const CiLut*
  rise_propagation() const override;

  /// @brief 立ち下がり伝搬遅延テーブルの取得
  const CiLut*
  fall_propagation() const override;

  /// @brief 立ち上がりセル遅延テーブルの取得
  const CiLut*
  cell_rise() const override;

  /// @brief 立ち下がりセル遅延テーブルの取得
  const CiLut*
  cell_fall() const override;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  void
  dump_LUT(
    Serializer& s ///< [in] シリアライザ
  ) const;

  /// @brief 内容を復元する．
  void
  restore_LUT(
    Deserializer& s ///< [in] デシリアライザ
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 立ち上がり遷移遅延テーブル
  std::unique_ptr<CiLut> mRiseTransition;

  // 立ち下がり遷移遅延テーブル
  std::unique_ptr<CiLut> mFallTransition;

};


//////////////////////////////////////////////////////////////////////
/// @class CiTimingLut_cell CiTiming.h "CiTiming.h"
/// @brief CMOS非線形タイプ1の ClibTiming の実装クラス
//////////////////////////////////////////////////////////////////////
class CiTimingLut_cell :
  public CiTimingLut
{
public:

  /// @brief restore() 用のコンストラクタ
  CiTimingLut_cell() = default;

  /// @brief コンストラクタ
  CiTimingLut_cell(
    ClibTimingType timing_type,          ///< [in] タイミングの型
    const Expr& cond,                    ///< [in] タイミング条件を表す式
    std::unique_ptr<CiLut>&& cell_rise,       ///< [in] 立ち上がりセル遅延テーブル
    std::unique_ptr<CiLut>&& cell_fall,       ///< [in] 立ち下がりセル遅延テーブル
    std::unique_ptr<CiLut>&& rise_transition, ///< [in] 立ち上がり遷移遅延テーブル
    std::unique_ptr<CiLut>&& fall_transition  ///< [in] 立ち下がり遷移遅延テーブル
  ) : CiTimingLut{timing_type, cond,
		  std::move(rise_transition),
		  std::move(fall_transition)},
      mCellRise{std::move(cell_rise)},
      mCellFall{std::move(cell_fall)}
  {
  }


  /// @brief デストラクタ
  ~CiTimingLut_cell() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 共通の属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり遅延時間を計算する．
  ClibTime
  calc_rise_delay(
    ClibTime input_transition,         ///< [in] 入力信号の遷移時間
    ClibCapacitance output_capacitance ///< [in] 出力の負荷容量
  ) const override;

  /// @brief 立ち下がり遅延時間を計算する．
  ClibTime
  calc_fall_delay(
    ClibTime input_transition,         ///< [in] 入力信号の遷移時間
    ClibCapacitance output_capacitance ///< [in] 出力の負荷容量
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // CMOS非線形遅延モデルの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がりセル遅延テーブルの取得
  const CiLut*
  cell_rise() const override;

  /// @brief 立ち下がりセル遅延テーブルの取得
  const CiLut*
  cell_fall() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // dump/restore 関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  void
  dump(
    Serializer& s ///< [in] シリアライザ
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を復元する．
  void
  _restore(
    Deserializer& s  ///< [in] デシリアライザ
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 立ち上がりセル遅延テーブル
  std::unique_ptr<CiLut> mCellRise;

  // 立ち下がりセル遅延テーブル
  std::unique_ptr<CiLut> mCellFall;

};


//////////////////////////////////////////////////////////////////////
/// @class CiTimingLut_prop CiTiming.h "CiTiming.h"
/// @brief CMOS非線形タイプ2の ClibTiming の実装クラス
//////////////////////////////////////////////////////////////////////
class CiTimingLut_prop :
  public CiTimingLut
{
public:

  /// @brief restore() 用のコンストラクタ
  CiTimingLut_prop() = default;

  /// @brief コンストラクタ
  CiTimingLut_prop(
    ClibTimingType timing_type,           ///< [in] タイミングの型
    const Expr& cond,                     ///< [in] タイミング条件を表す式
    std::unique_ptr<CiLut>&& rise_transition,  ///< [in] 立ち上がり遷移遅延テーブル
    std::unique_ptr<CiLut>&& fall_transition,  ///< [in] 立ち下がり遷移遅延テーブル
    std::unique_ptr<CiLut>&& rise_propagation, ///< [in] 立ち上がり伝搬遅延テーブル
    std::unique_ptr<CiLut>&& fall_propagation  ///< [in] 立ち下がり伝搬遅延テーブル
  ) : CiTimingLut{timing_type, cond,
		  std::move(rise_transition),
		  std::move(fall_transition)},
      mRisePropagation{std::move(rise_propagation)},
      mFallPropagation{std::move(fall_propagation)}
  {
  }

  /// @brief デストラクタ
  ~CiTimingLut_prop() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 共通の属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり遅延時間を計算する．
  ClibTime
  calc_rise_delay(
    ClibTime input_transition,         ///< [in] 入力信号の遷移時間
    ClibCapacitance output_capacitance ///< [in] 出力の負荷容量
  ) const override;

  /// @brief 立ち下がり遅延時間を計算する．
  ClibTime
  calc_fall_delay(
    ClibTime input_transition,         ///< [in] 入力信号の遷移時間
    ClibCapacitance output_capacitance ///< [in] 出力の負荷容量
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // CMOS非線形遅延モデルの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり伝搬遅延テーブルの取得
  const CiLut*
  rise_propagation() const override;

  /// @brief 立ち下がり伝搬遅延テーブルの取得
  const CiLut*
  fall_propagation() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // dump/restore 関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  void
  dump(
    Serializer& s ///< [in] シリアライザ
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を復元する．
  void
  _restore(
    Deserializer& s  ///< [in] デシリアライザ
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 立ち上がり伝搬遅延テーブル
  std::unique_ptr<CiLut> mRisePropagation;

  // 立ち下がり伝搬遅延テーブル
  std::unique_ptr<CiLut> mFallPropagation;

};


//////////////////////////////////////////////////////////////////////
/// @class CiTimingStLut CiTiming.h "CiTiming.h"
/// @brief CMOS非線形タイプの ClibTiming の共通の基底クラス
///
/// 第1変数が total_output_net_capacitance
/// 第2変数が input_net_transition
/// の2次元LUTのタイプ
//////////////////////////////////////////////////////////////////////
class CiTimingStLut :
  public CiTiming
{
public:

  /// @brief restore() 用のコンストラクタ
  CiTimingStLut() = default;

  /// @brief コンストラクタ
  CiTimingStLut(
    ClibTimingType timing_type,            ///< [in] タイミングの型
    const Expr& cond,                      ///< [in] タイミング条件を表す式
    std::unique_ptr<CiStLut>&& rise_transition, ///< [in] 立ち上がり遷移遅延テーブル
    std::unique_ptr<CiStLut>&& fall_transition  ///< [in] 立ち下がり遷移遅延テーブル
  ) : CiTiming{timing_type, cond},
      mRiseTransition{std::move(rise_transition)},
      mFallTransition{std::move(fall_transition)}
  {
  }

  /// @brief デストラクタ
  ~CiTimingStLut() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 共通の属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり遷移時間を計算する．
  ///
  /// 立ち上がり遷移時間は出力信号がしきい値1(通常20%)を超えてから
  /// しきい値2(通常80%)を超えるまでの時間
  ClibTime
  calc_rise_transition(
    ClibTime input_transition,         ///< [in] 入力信号の遷移時間
    ClibCapacitance output_capacitance ///< [in] 出力の負荷容量
  ) const override;

  /// @brief 立ち下がり遷移時間を計算する．
  ///
  /// 立ち上がり遷移時間は出力信号がしきい値1(通常80%)を下回ってから
  /// しきい値2(通常20%)を下回るまでの時間
  ClibTime
  calc_fall_transition(
    ClibTime input_transition,         ///< [in] 入力信号の遷移時間
    ClibCapacitance output_capacitance ///< [in] 出力の負荷容量
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // CMOS非線形遅延モデルの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり遷移遅延テーブルの取得
  const CiLut*
  rise_transition() const override;

  /// @brief 立ち下がり遷移遅延テーブルの取得
  const CiLut*
  fall_transition() const override;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  void
  dump_LUT(
    Serializer& s ///< [in] シリアライザ
  ) const;

  /// @brief 内容を復元する．
  void
  restore_LUT(
    Deserializer& s ///< [in] デシリアライザ
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 立ち上がり遷移遅延テーブル
  std::unique_ptr<CiStLut> mRiseTransition;

  // 立ち下がり遷移遅延テーブル
  std::unique_ptr<CiStLut> mFallTransition;

};


//////////////////////////////////////////////////////////////////////
/// @class CiTimingStLut_cell CiTiming.h "CiTiming.h"
/// @brief CMOS非線形タイプ(cell)の ClibTiming の実装クラス
//////////////////////////////////////////////////////////////////////
class CiTimingStLut_cell :
  public CiTimingStLut
{
public:

  /// @brief restore() 用のコンストラクタ
  CiTimingStLut_cell() = default;

  /// @brief コンストラクタ
  CiTimingStLut_cell(
    ClibTimingType timing_type,          ///< [in] タイミングの型
    const Expr& cond,                    ///< [in] タイミング条件を表す式
    std::unique_ptr<CiStLut>&& cell_rise,       ///< [in] 立ち上がりセル遅延テーブル
    std::unique_ptr<CiStLut>&& cell_fall,       ///< [in] 立ち下がりセル遅延テーブル
    std::unique_ptr<CiStLut>&& rise_transition, ///< [in] 立ち上がり遷移遅延テーブル
    std::unique_ptr<CiStLut>&& fall_transition  ///< [in] 立ち下がり遷移遅延テーブル
  ) : CiTimingStLut{timing_type, cond,
		    std::move(rise_transition),
		    std::move(fall_transition)},
      mCellRise{std::move(cell_rise)},
      mCellFall{std::move(cell_fall)}
  {
  }


  /// @brief デストラクタ
  ~CiTimingStLut_cell() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 共通の属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり遅延時間を計算する．
  ClibTime
  calc_rise_delay(
    ClibTime input_transition,         ///< [in] 入力信号の遷移時間
    ClibCapacitance output_capacitance ///< [in] 出力の負荷容量
  ) const override;

  /// @brief 立ち下がり遅延時間を計算する．
  ClibTime
  calc_fall_delay(
    ClibTime input_transition,         ///< [in] 入力信号の遷移時間
    ClibCapacitance output_capacitance ///< [in] 出力の負荷容量
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // CMOS非線形遅延モデルの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がりセル遅延テーブルの取得
  const CiLut*
  cell_rise() const override;

  /// @brief 立ち下がりセル遅延テーブルの取得
  const CiLut*
  cell_fall() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // dump/restore 関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  void
  dump(
    Serializer& s ///< [in] シリアライザ
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を復元する．
  void
  _restore(
    Deserializer& s  ///< [in] デシリアライザ
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 立ち上がりセル遅延テーブル
  std::unique_ptr<CiStLut> mCellRise;

  // 立ち下がりセル遅延テーブル
  std::unique_ptr<CiStLut> mCellFall;

};


//////////////////////////////////////////////////////////////////////
/// @class CiTimingStLut_prop CiTiming.h "CiTiming.h"
/// @brief CMOS非線形タイプ(propgation)の ClibTiming の実装クラス
///
/// 第1変数が total_output_net_capacitance
/// 第2変数が input_net_transition
/// の2次元LUTのタイプ
//////////////////////////////////////////////////////////////////////
class CiTimingStLut_prop :
  public CiTimingStLut
{
public:

  /// @brief restore() 用のコンストラクタ
  CiTimingStLut_prop() = default;

  /// @brief コンストラクタ
  CiTimingStLut_prop(
    ClibTimingType timing_type,           ///< [in] タイミングの型
    const Expr& cond,                     ///< [in] タイミング条件を表す式
    std::unique_ptr<CiStLut>&& rise_transition,  ///< [in] 立ち上がり遷移遅延テーブル
    std::unique_ptr<CiStLut>&& fall_transition,  ///< [in] 立ち下がり遷移遅延テーブル
    std::unique_ptr<CiStLut>&& rise_propagation, ///< [in] 立ち上がり伝搬遅延テーブル
    std::unique_ptr<CiStLut>&& fall_propagation  ///< [in] 立ち下がり伝搬遅延テーブル
  ) : CiTimingStLut{timing_type, cond,
		    std::move(rise_transition),
		    std::move(fall_transition)},
      mRisePropagation{std::move(rise_propagation)},
      mFallPropagation{std::move(fall_propagation)}
  {
  }

  /// @brief デストラクタ
  ~CiTimingStLut_prop() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 共通の属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり遅延時間を計算する．
  ClibTime
  calc_rise_delay(
    ClibTime input_transition,         ///< [in] 入力信号の遷移時間
    ClibCapacitance output_capacitance ///< [in] 出力の負荷容量
  ) const override;

  /// @brief 立ち下がり遅延時間を計算する．
  ClibTime
  calc_fall_delay(
    ClibTime input_transition,         ///< [in] 入力信号の遷移時間
    ClibCapacitance output_capacitance ///< [in] 出力の負荷容量
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // CMOS非線形遅延モデルの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり伝搬遅延テーブルの取得
  const CiLut*
  rise_propagation() const override;

  /// @brief 立ち下がり伝搬遅延テーブルの取得
  const CiLut*
  fall_propagation() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // dump/restore 関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  void
  dump(
    Serializer& s ///< [in] シリアライザ
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を復元する．
  void
  _restore(
    Deserializer& s  ///< [in] デシリアライザ
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 立ち上がり伝搬遅延テーブル
  std::unique_ptr<CiStLut> mRisePropagation;

  // 立ち下がり伝搬遅延テーブル
  std::unique_ptr<CiStLut> mFallPropagation;

};

END_NAMESPACE_YM_CLIB

#endif // CITIMING_SUB_H
