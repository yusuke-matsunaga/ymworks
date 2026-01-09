#ifndef CIPIN_H
#define CIPIN_H

/// @file CiPin.h
/// @brief CiPin のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"
#include "ym/ClibTime.h"
#include "ym/ClibCapacitance.h"
#include "ym/Expr.h"
#include "ym/ShString.h"


BEGIN_NAMESPACE_YM_CLIB

class Serializer;
class Deserializer;

//////////////////////////////////////////////////////////////////////
/// @class ClibCellPinBase CiPin.h "CiPin.h"
/// @brief ピンの基底クラス
//////////////////////////////////////////////////////////////////////
class CiPin
{
protected:

  /// @brief restore() 用のコンストラクタ
  CiPin() = default;

  /// @brief コンストラクタ
  CiPin(
    SizeType pin_id,          ///< [in] ピン番号
    const ShString& name      ///< [in] ピン名
  ) : mPinId{pin_id},
      mName{name}
  {
  }


public:

  /// @brief 入力ピンを生成するクラスメソッド
  static
  std::unique_ptr<CiPin>
  new_Input(
    SizeType pin_id,                  ///< [in] ピン番号
    SizeType input_id,                ///< [in] 入力番号
    const ShString& name,             ///< [in] ピン名
    ClibCapacitance capacitance,      ///< [in] 負荷容量
    ClibCapacitance rise_capacitance, ///< [in] 立ち上がり時の負荷容量
    ClibCapacitance fall_capacitance  ///< [in] 立ち下がり時の負荷容量
  );

  /// @brief 出力ピンを生成するクラスメソッド
  static
  std::unique_ptr<CiPin>
  new_Output(
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
  );

  /// @brief 入出力ピンを生成するクラスメソッド
  static
  std::unique_ptr<CiPin>
  new_Inout(
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
  );

  /// @brief 内部ピンを生成するクラスメソッド
  static
  std::unique_ptr<CiPin>
  new_Internal(
    SizeType internal_id,     ///< [in] 内部ピン番号
    const ShString& name      ///< [in] ピン名
  );

  /// @brief デストラクタ
  virtual
  ~CiPin() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 共通属性
  //////////////////////////////////////////////////////////////////////

  /// @brief ピン名を返す．
  std::string
  name() const
  {
    return mName;
  }

  /// @brief ピン名を返す．
  ShString
  _name() const
  {
    return mName;
  }

  /// @brief 方向を返す．
  virtual
  ClibDirection
  direction() const = 0;

  /// @brief 入力ピンの時に true を返す．
  virtual
  bool
  is_input() const;

  /// @brief 出力ピンの時に true を返す．
  virtual
  bool
  is_output() const;

  /// @brief 入出力ピンの時に true を返す．
  virtual
  bool
  is_inout() const;

  /// @brief 内部ピンの時に true を返す．
  virtual
  bool
  is_internal() const;

  /// @brief ピン番号を返す．
  SizeType
  pin_id() const
  {
    return mPinId;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 入力ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ピン番号を返す．
  ///
  /// 入力ピンもしくは入出力ピンの時のみ意味を持つ．
  virtual
  SizeType
  input_id() const;

  /// @brief 負荷容量を返す．
  virtual
  ClibCapacitance
  capacitance() const;

  /// @brief 立ち上がり時の負荷容量を返す．
  virtual
  ClibCapacitance
  rise_capacitance() const;

  /// @brief 立ち下がり時の負荷容量を返す．
  virtual
  ClibCapacitance
  fall_capacitance() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 出力ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力ピン番号を返す．
  ///
  /// 出力ピンもしくは入出力ピンの時のみ意味を持つ．
  virtual
  SizeType
  output_id() const;

  /// @brief 最大ファンアウト容量を返す．
  virtual
  ClibCapacitance
  max_fanout() const;

  /// @brief 最小ファンアウト容量を返す．
  virtual
  ClibCapacitance
  min_fanout() const;

  /// @brief 最大負荷容量を返す．
  virtual
  ClibCapacitance
  max_capacitance() const;

  /// @brief 最小負荷容量を返す．
  virtual
  ClibCapacitance
  min_capacitance() const;

  /// @brief 最大遷移時間を返す．
  virtual
  ClibTime
  max_transition() const;

  /// @brief 最小遷移時間を返す．
  virtual
  ClibTime
  min_transition() const;

  /// @brief 論理式を返す．
  virtual
  Expr
  function() const;

  /// @brief tristate 条件式を返す．
  virtual
  Expr
  tristate() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内部ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部ピン番号を返す．
  ///
  /// 内部ピンの時のみ意味を持つ．
  virtual
  SizeType
  internal_id() const;


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

  /// @brief 内容を復元する．
  static
  std::unique_ptr<CiPin>
  restore(
    Deserializer& s ///< [in] デシリアライザ
  );


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief dump 用の共通情報を出力する．
  void
  dump_common(
    Serializer& s, ///< [in] シリアライザ
    int sig        ///< [in] シグネチャ(0, 1, 2, 3)
  ) const;

  /// @brief restore() の下請け関数
  virtual
  void
  _restore(
    Deserializer& s ///< [in] デシリアライザ
  ) = 0;

  /// @brief 内容を読み込む．
  void
  restore_common(
    Deserializer& s ///< [in] デシリアライザ
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // ピン番号
  SizeType mPinId{CLIB_NULLID};

};

END_NAMESPACE_YM_CLIB

#endif // CIPIN_H
