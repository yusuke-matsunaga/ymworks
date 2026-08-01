#ifndef ELBUSERSYSTF_H
#define ELBUSERSYSTF_H

/// @file ElbUserSystf.h
/// @brief ElbUserSystf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/VlUserSystf.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbUserSystf ElbUserSystf.h "ElbUserSystf.h"
/// @brief エラボレーション中の VlUserSystf を表すクラス
//////////////////////////////////////////////////////////////////////
class ElbUserSystf :
  public VlUserSystf
{
public:
  //////////////////////////////////////////////////////////////////////
  // ElbUserSystf の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 引数の個数をチェックする．
  virtual
  bool
  check_n_of_args(
    SizeType arg_num ///< [in] 引数の個数
  ) const = 0;

  /// @brief 引数の型をチェックする．
  virtual
  bool
  check_argument(
    SizeType pos,           ///< [in] 位置番号
    const ElbExpr* argument ///< [in] 引数
  ) const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBUSERSYSTF_H
