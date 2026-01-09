#ifndef YM_SATORDEREDSET_H
#define YM_SATORDEREDSET_H

/// @file ym/SatOrderedSet.h
/// @brief SatOrderedSet のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/sat.h"
#include "ym/SatLiteral.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
/// @class SatOrderedSet SatOrderedSet.h "SatOrderedSet.h"
/// @brief 順序符号化で多値を表すクラス
///
/// * 最小値と最大値の間の整数を値として取る．
/// * 最小値 <= 最大値であれば負数でも構わない
/// * 整数の大小関係に基づく順序関係を持つ．
//////////////////////////////////////////////////////////////////////
class SatOrderedSet
{
  friend class SatSolver;

public:

  /// @brief デストラクタ
  ~SatOrderedSet() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 最小値を返す．
  int
  min() const
  {
    return mMin;
  }

  /// @brief 最大値を返す．
  int
  max() const
  {
    return mMax;
  }

  /// @brief 値に対応する変数を返す．
  ///
  /// var(val) == True <==> 値が val
  SatLiteral
  var(
    int val ///< [in] 値 ( min <= val <= max )
  ) const
  {
    _check_range(val, "val");

    return mVarArray[val - mMin];
  }

  /// @brief 値に対応する変数のベクタを返す．
  const std::vector<SatLiteral>&
  vars() const
  {
    return mVarArray;
  }

  /// @brief 順序符号化された変数を返す．
  ///
  /// この変数が true == 値が val 以上
  /// 値が min 以上は常に成り立つので val > min となる．
  /// つまり var() よりも範囲が一つ短い．
  SatLiteral
  pri_var(
    int val ///< [in] 値 ( min < val <= max )
  ) const
  {
    _check_range(val, "val");

    return mPriVarArray[val - mMin - 1];
  }

  /// @brief 順序符号化された変数のベクタを返す．
  const std::vector<SatLiteral>&
  pri_vars() const
  {
    return mPriVarArray;
  }

  /// @brief SATの解から値を得る．
  int
  val(
    const SatModel& model ///< [in] SATの解
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値が範囲内か調べる．
  void
  _check_range(
    int val,
    const char* name
  ) const
  {
    if ( val < mMin || mMax < val ) {
      std::ostringstream buf;
      buf << name << " is out of range";
      throw std::out_of_range{buf.str()};
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 最小値
  int mMin;

  // 最大値
  int mMax;

  // 変数(実際にはリテラル)の配列
  std::vector<SatLiteral> mVarArray;

  // 順序符号化された裏の変数
  std::vector<SatLiteral> mPriVarArray;

};

END_NAMESPACE_YM_SAT

#endif // YM_SATORDEREDSET_H
