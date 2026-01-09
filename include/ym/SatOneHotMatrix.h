#ifndef SATONEHOTMATRIX_H
#define SATONEHOTMATRIX_H

/// @file SatOneHotMatrix.h
/// @brief SatOneHotMatrix のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/sat.h"
#include "ym/SatLiteral.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
/// @class SatOneHotMatrix SatOneHotMatrix.h "SatOneHotMatrix.h"
/// @brief one-hot 符号化で多値の組み合わせを表すクラス
///
/// elem_size 個の要素のなかから sel_size 個を選ぶ組み合わせを表す．
/// 正確には順列を表す．
/// 内部では elem_size x sel_size の２次元配列(行列)を持つ．
/// 各行，各列の変数に対して one-hot 制約が課されている．
//////////////////////////////////////////////////////////////////////
class SatOneHotMatrix
{
  friend class SatSolver;

public:

  /// @brief 空のコンストラクタ
  ///
  /// 無効な値を表す．
  SatOneHotMatrix() = default;

  /// @brief デストラクタ
  ~SatOneHotMatrix() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素のサイズを返す．
  SizeType
  elem_size() const
  {
    return mElemSize;
  }

  /// @brief 選択数を返す．
  SizeType
  sel_size() const
  {
    return mSelSize;
  }

  /// @brief 値に対応する変数を返す．
  SatLiteral
  var(
    SizeType sel_val, ///< [in] 選択番号 ( 0 <= sel_val < sel_size() )
    SizeType elem_val ///< [in] 要素番号 ( 0 <= elem_val < elem_size() )
  ) const
  {
    _check_elem_range(elem_val, "elem_val");
    _check_sel_range(sel_val, "sel_val");
    return _var(sel_val, elem_val);
  }

  /// @brief 値に対応する変数のリストを返す．
  const std::vector<SatLiteral>&
  vars() const
  {
    return mVarArray;
  }

  /// @brief SATの解から値を得る．
  SizeType
  val(
    const SatModel& model, ///< [in] SATの解
    SizeType sel_val       ///< [in] 選択番号 ( 0 <= sel_val < sel_size() )
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  SatOneHotMatrix(
    SatSolver& solver,
    SizeType elem_size,
    SizeType sel_size
  );

  /// @brief 対象の変数を返す．
  SatLiteral
  _var(
    SizeType sel,
    SizeType elem
  ) const
  {
    // elem と sel の範囲チェックは行わない．
    return mVarArray[sel * mElemSize + elem];
  }

  /// @brief 値が要素数の範囲内か調べる．
  void
  _check_elem_range(
    SizeType val,
    const char* name
  ) const
  {
    if ( val >= elem_size() ) {
      std::ostringstream buf;
      buf << name << " is out of range";
      throw std::out_of_range{buf.str()};
    }
  }

  /// @brief 値が選択数の範囲内か調べる．
  void
  _check_sel_range(
    SizeType val,
    const char* name
  ) const
  {
    if ( val >= sel_size() ) {
      std::ostringstream buf;
      buf << name << " is out of range";
      throw std::out_of_range{buf.str()};
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  SizeType mElemSize{0};

  // 選択数
  SizeType mSelSize{0};

  // 変数の配列
  std::vector<SatLiteral> mVarArray;

};

END_NAMESPACE_YM_SAT

#endif // SATONEHOTMATRIX_H
