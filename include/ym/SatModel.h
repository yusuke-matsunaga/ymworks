#ifndef SATMODEL_H
#define SATMODEL_H

/// @file SatModel.h
/// @brief SatModel のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/sat.h"
#include "ym/SatBool3.h"
#include "ym/SatLiteral.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
/// @class SatModel SatModel.h "SatModel.h"
/// @brief SATの解を表すクラス
//////////////////////////////////////////////////////////////////////
class SatModel
{
public:

  /// @brief コンストラクタ
  SatModel() = default;

  /// @brief コピーコンストラクタ
  SatModel(const SatModel& src) = default;

  /// @brief ムーブコンストラクタ
  SatModel(SatModel&& src) = default;

  /// @brief コピー代入演算子
  SatModel&
  operator=(const SatModel& src) = default;

  /// @brief ムーブ代入演算子
  SatModel&
  operator=(SatModel&& src) = default;

  /// @brief デストラクタ
  ~SatModel() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief サイズを返す．
  SizeType
  size() const
  {
    return mValArray.size();
  }

  /// @brief 対象の値を返す．
  SatBool3
  operator[](
    SatLiteral lit ///< [in] リテラル
  ) const
  {
    if ( !lit.is_valid() ) {
      throw std::invalid_argument{"lit is invalid"};
    }
    auto vid = lit.varid();
    if ( vid >= mValArray.size() ) {
      throw std::out_of_range{"lit.varid() is out of range"};
    }
    auto val = mValArray[vid];
    if ( lit.is_negative() ) {
      val = ~val;
    }
    return val;
  }

  /// @brief 対象の値を返す(operator[] の別名)．
  SatBool3
  get(
    SatLiteral lit ///< [in] リテラル
  ) const
  {
    return operator[](lit);
  }

  /// @brief 配列のサイズを設定する．
  void
  resize(
    SizeType size ///< [in] サイズ
  )
  {
    mValArray.clear();
    mValArray.resize(size, SatBool3::X);
  }

  /// @brief 値をセットする．
  void
  set(
    SizeType pos, ///< [in] 位置
    SatBool3 val  ///< [in] 値
  )
  {
    if ( pos >= mValArray.size() ) {
      throw std::out_of_range{"pos is out of range"};
    }
    mValArray[pos] = val;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 各変数の値の配列
  std::vector<SatBool3> mValArray;

};

END_NAMESPACE_YM_SAT

#endif // SATMODEL_H
