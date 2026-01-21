#ifndef SATONEHOTVAR_H
#define SATONEHOTVAR_H

/// @file SatOneHotVar.h
/// @brief SatOneHotVar のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/sat.h"
#include "ym/SatLiteral.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
/// @class SatOneHotVar SatOneHotVar.h "SatOneHotVar.h"
/// @brief one-hot 符号化で多値を表すクラス
///
/// 値は 0 〜 (size() - 1) の多値だが値の間に関係はない．
//////////////////////////////////////////////////////////////////////
class SatOneHotVar
{
  friend class SatSolver;

public:

  /// @brief デストラクタ
  ~SatOneHotVar() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief サイズを返す．
  SizeType
  size() const
  {
    return mVarArray.size();
  }

  /// @brief 値に対応する変数を返す．
  /// @exception std::out_of_range 範囲外のアクセス
  /// @sa SatLiteral
  SatLiteral
  var(
    SizeType val ///< [in] 値 ( 0 <= val < size() )
  ) const
  {
    _check_range(val, "val");

    return mVarArray[val];
  }

  /// @brief 値に対応する変数のリストを返す．
  /// @sa SatLiteral
  const std::vector<SatLiteral>&
  vars() const
  {
    return mVarArray;
  }

  /// @brief SATの解から値を得る．
  /// @sa SatModel
  SizeType
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
    SizeType val,
    const char* name
  ) const
  {
    if ( mVarArray.size() <= val ) {
      std::ostringstream buf;
      buf << name << " is out of range";
      throw std::out_of_range{buf.str()};
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数の配列
  std::vector<SatLiteral> mVarArray;

};

END_NAMESPACE_YM_SAT

#endif // SATONEHOTVAR_H
