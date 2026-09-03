#ifndef ELBRANGE_H
#define ELBRANGE_H

/// @file ElbRange.h
/// @brief ElbRange のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/VlRange.h"
#include "ym/vl/Ast.h"
#include "ym/vl/AstRange.h"
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbRangeSrc ElbRange.h "ElbRange.h"
/// @brief ElbRange を作るための情報を持つデータ構造
///
/// いわゆるビルダクラス
//////////////////////////////////////////////////////////////////////
class ElbRangeSrc
{
public:

  /// @brief コンストラクタ
  ElbRangeSrc(
    const AstRange& ast_range,
    const AstExpr& left,
    const AstExpr& right,
    int left_val,
    int right_val
  ) : mAstRange{ast_range},
      mLeftRange{left},
      mRightRange{right},
      mLeftVal{left_val},
      mRightVal{right_val}
  {
  }

  /// @brief デストラクタ
  ~ElbRangeSrc() = default;


public:

  /// @brief パース木の範囲定義を返す．
  AstRange
  ast_range() const { return mAstRange; }

  /// @brief 範囲の MSB の式を返す．
  AstExpr
  left_range() const { return mLeftRange; }

  /// @brief 範囲の LSB の式を返す．
  AstExpr
  right_range() const { return mRightRange; }

  /// @brief 範囲の MSB の値を返す．
  int
  left_range_val() const { return mLeftVal; }

  /// @brief 範囲の LSB の値を返す．
  int
  right_range_val() const { return mRightVal; }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の範囲定義
  AstRange mAstRange;

  // 範囲の MSB の式
  AstExpr mLeftRange;

  // 範囲の LSB の式
  AstExpr mRightRange;

  // 範囲の MSB
  int mLeftVal;

  // 範囲の LSB
  int mRightVal;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbRange ElbRange.h "ElbRange.h"
/// @brief 範囲を表すクラス
/// IEEE Std 1364-2001 26.6.10 Object range
//////////////////////////////////////////////////////////////////////
class ElbRange :
  public VlRange
{
public:
  //////////////////////////////////////////////////////////////////////
  // ElbRange の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を設定する．
  virtual
  void
  set(
    const ElbRangeSrc& src ///< [in] 元となる情報
  ) = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBRANGE_H
