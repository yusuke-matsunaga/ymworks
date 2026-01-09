#ifndef CIPIN_SUB_H
#define CIPIN_SUB_H

/// @file CiPin_sub.h
/// @brief CiPin 継承クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ci/CiPin.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
/// @class CiInputPin CiPin.h "CiPin.h"
/// @brief セルの入力ピンを表すクラス
//////////////////////////////////////////////////////////////////////
class CiInputPin :
  public CiPin
{
public:

  /// @brief restore() 用のコンストラクタ
  CiInputPin() = default;

  /// @brief コンストラクタ
  CiInputPin(
    SizeType pin_id,                  ///< [in] ピン番号
    SizeType input_id,                ///< [in] 入力番号
    const ShString& name,             ///< [in] ピン名
    ClibCapacitance capacitance,      ///< [in] 負荷容量
    ClibCapacitance rise_capacitance, ///< [in] 立ち上がり時の負荷容量
    ClibCapacitance fall_capacitance  ///< [in] 立ち下がり時の負荷容量
  ) : CiPin{pin_id, name},
      mInputId{input_id},
      mCapacitance{capacitance},
      mRiseCapacitance{rise_capacitance},
      mFallCapacitance{fall_capacitance}
  {
  }

  /// @brief デストラクタ
  ~CiInputPin() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 共通属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 方向を返す．
  ClibDirection
  direction() const override;

  /// @brief 入力ピンの時に true を返す．
  bool
  is_input() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // 入力ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ピン番号を返す．
  ///
  /// 入力ピンもしくは入出力ピンの時のみ意味を持つ．
  SizeType
  input_id() const override;

  /// @brief 負荷容量を返す．
  ClibCapacitance
  capacitance() const override;

  /// @brief 立ち上がり時の負荷容量を返す．
  ClibCapacitance
  rise_capacitance() const override;

  /// @brief 立ち下がり時の負荷容量を返す．
  ClibCapacitance
  fall_capacitance() const override;


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
  // dump/restore の下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief restore() の下請け関数
  void
  _restore(
    Deserializer& s ///< [in] デシリアライザ
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ピン番号
  SizeType mInputId;

  // 負荷
  ClibCapacitance mCapacitance;

  // rise 負荷
  ClibCapacitance mRiseCapacitance;

  // fall 負荷
  ClibCapacitance mFallCapacitance;

};


//////////////////////////////////////////////////////////////////////
/// @class CiOutputPin CiPin.h "CiPin.h"
/// @brief セルの出力ピンと入出力ピンの基底クラス
//////////////////////////////////////////////////////////////////////
class CiOutputPinBase :
  public CiPin
{
public:

  /// @brief restore() 用のコンストラクタ
  CiOutputPinBase() = default;

  /// @brief コンストラクタ
  CiOutputPinBase(
    SizeType pin_id,                 ///< [in] ピン番号
    SizeType output_id,              ///< [in] 出力番号
    const ShString& name,            ///< [in] ピン名
    ClibCapacitance max_fanout,      ///< [in] 最大ファンアウト容量
    ClibCapacitance min_fanout,      ///< [in] 最小ファンアウト容量
    ClibCapacitance max_capacitance, ///< [in] 最大負荷容量
    ClibCapacitance min_capacitance, ///< [in] 最小負荷容量
    ClibTime max_transition,         ///< [in] 最大遷移時間
    ClibTime min_transition,         ///< [in] 最小遷移時間
    const Expr& function,            ///< [in] 出力の論理式
    const Expr& tristate             ///< [in] tristate 条件
  ) : CiPin{pin_id, name},
      mOutputId{output_id},
      mMaxFanout{max_fanout},
      mMinFanout{min_fanout},
      mMaxCapacitance{max_capacitance},
      mMinCapacitance{min_capacitance},
      mMaxTransition{max_transition},
      mMinTransition{min_transition},
      mFunction{function},
      mTristate{tristate}
  {
  }

  /// @brief デストラクタ
  ~CiOutputPinBase() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 出力ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力ピン番号を返す．
  ///
  /// 出力ピンもしくは入出力ピンの時のみ意味を持つ．
  SizeType
  output_id() const override;

  /// @brief 最大ファンアウト容量を返す．
  ClibCapacitance
  max_fanout() const override;

  /// @brief 最小ファンアウト容量を返す．
  ClibCapacitance
  min_fanout() const override;

  /// @brief 最大負荷容量を返す．
  ClibCapacitance
  max_capacitance() const override;

  /// @brief 最小負荷容量を返す．
  ClibCapacitance
  min_capacitance() const override;

  /// @brief 最大遷移時間を返す．
  ClibTime
  max_transition() const override;

  /// @brief 最小遷移時間を返す．
  ClibTime
  min_transition() const override;

  /// @brief 論理式を返す．
  Expr
  function() const override;

  /// @brief tristate 条件式を返す．
  Expr
  tristate() const override;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  void
  dump_base(
    Serializer& s, ///< [in] シリアライザ
    int sig        ///< [in] シグネチャ
  ) const;

  /// @brief 内容を読み込む．
  void
  restore_base(
    Deserializer& s ///< [in] デシリアライザ
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力ピン番号
  SizeType mOutputId;

  // ファンアウトの負荷
  ClibCapacitance mFanoutLoad;

  // 最大ファンアウト容量
  ClibCapacitance mMaxFanout;

  // 最小ファンアウト容量
  ClibCapacitance mMinFanout;

  // 最大負荷容量
  ClibCapacitance mMaxCapacitance;

  // 最小負荷容量
  ClibCapacitance mMinCapacitance;

  // 最大遷移時間
  ClibTime mMaxTransition;

  // 最小遷移時間
  ClibTime mMinTransition;

  // 論理式
  Expr mFunction;

  // tristate 条件
  Expr mTristate;

};


//////////////////////////////////////////////////////////////////////
/// @class CiOutputPin CiPin.h "CiPin.h"
/// @brief セルの出力ピンを表すクラス
//////////////////////////////////////////////////////////////////////
class CiOutputPin :
  public CiOutputPinBase
{
public:

  /// @brief restore() 用のコンストラクタ
  CiOutputPin() = default;

  /// @brief コンストラクタ
  CiOutputPin(
    SizeType pin_id,                 ///< [in] ピン番号
    SizeType output_id,              ///< [in] 出力番号
    const ShString& name,            ///< [in] ピン名
    ClibCapacitance max_fanout,      ///< [in] 最大ファンアウト容量
    ClibCapacitance min_fanout,      ///< [in] 最大ファンアウト容量
    ClibCapacitance max_capacitance, ///< [in] 最大負荷容量
    ClibCapacitance min_capacitance, ///< [in] 最大負荷容量
    ClibTime max_transition,         ///< [in] 最大遷移時間
    ClibTime min_transition,         ///< [in] 最大遷移時間
    const Expr& function,            ///< [in] 出力の論理式
    const Expr& tristate             ///< [in] tristate 条件
  ) : CiOutputPinBase{pin_id, output_id, name,
		      max_fanout, min_fanout,
		      max_capacitance, min_capacitance,
		      max_transition, min_transition,
		      function, tristate}
  {
  }

  /// @brief デストラクタ
  ~CiOutputPin() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 共通属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 方向を返す．
  ClibDirection
  direction() const override;

  /// @brief 出力ピンの時に true を返す．
  bool
  is_output() const override;


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
  // dump/restore の下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief restore() の下請け関数
  void
  _restore(
    Deserializer& s ///< [in] デシリアライザ
  ) override;

};


//////////////////////////////////////////////////////////////////////
/// @class CiInoutPin CiPin.h "CiPin.h"
/// @brief セルの入出力ピンを表すクラス
///
/// 多重継承はオーバーヘッドがかかるので愚直な実装を用いている．
//////////////////////////////////////////////////////////////////////
class CiInoutPin :
  public CiOutputPinBase
{
public:

  /// @brief restore() 用のコンストラクタ
  CiInoutPin() = default;

  /// @brief コンストラクタ
  CiInoutPin(
    SizeType pin_id,                  ///< [in] ピン番号
    SizeType input_id,                ///< [in] 入力番号
    SizeType output_id,               ///< [in] 出力番号
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
  ) : CiOutputPinBase{pin_id, output_id, name,
		      max_fanout, min_fanout,
		      max_capacitance, min_capacitance,
		      max_transition, min_transition,
		      function, tristate},
      mInputId{input_id},
      mCapacitance{capacitance},
      mRiseCapacitance{rise_capacitance},
      mFallCapacitance{fall_capacitance}
  {
  }

  /// @brief デストラクタ
  ~CiInoutPin() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 共通属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 方向を返す．
  ClibDirection
  direction() const override;

  /// @brief 入出力ピンの時に true を返す．
  bool
  is_inout() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // 入力ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ピン番号を返す．
  ///
  /// 入力ピンもしくは入出力ピンの時のみ意味を持つ．
  SizeType
  input_id() const override;

  /// @brief 負荷容量を返す．
  ClibCapacitance
  capacitance() const override;

  /// @brief 立ち上がり時の負荷容量を返す．
  ClibCapacitance
  rise_capacitance() const override;

  /// @brief 立ち下がり時の負荷容量を返す．
  ClibCapacitance
  fall_capacitance() const override;


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
  // dump/restore の下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief restore() の下請け関数
  void
  _restore(
    Deserializer& s ///< [in] デシリアライザ
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ピン番号
  SizeType mInputId;

  // 負荷
  ClibCapacitance mCapacitance;

  // rise 負荷
  ClibCapacitance mRiseCapacitance;

  // fall 負荷
  ClibCapacitance mFallCapacitance;

};


//////////////////////////////////////////////////////////////////////
/// @class CiInternalPin CiPin.h "CiPin.h"
/// @brief セルの内部ピンを表すクラス
//////////////////////////////////////////////////////////////////////
class CiInternalPin :
  public CiPin
{
public:

  /// @brief restore() 用のコンストラクタ
  CiInternalPin() = default;

  /// @brief コンストラクタ
  CiInternalPin(
    SizeType internal_id,     ///< [in] 内部ピン番号
    const ShString& name      ///< [in] ピン名
  ) : CiPin{CLIB_NULLID, name},
      mInternalId{internal_id}
  {
  }

  /// @brief デストラクタ
  ~CiInternalPin() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 共通属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 方向を返す．
  ClibDirection
  direction() const override;

  /// @brief 内部ピンの時に true を返す．
  bool
  is_internal() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // 内部ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部ピン番号を返す．
  ///
  /// 内部ピンの時のみ意味を持つ．
  SizeType
  internal_id() const override;


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
  // dump/restore の下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief restore() の下請け関数
  void
  _restore(
    Deserializer& s ///< [in] デシリアライザ
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 内部ピン番号
  SizeType mInternalId;

};

END_NAMESPACE_YM_CLIB

#endif // CIPIN_SUB_H
