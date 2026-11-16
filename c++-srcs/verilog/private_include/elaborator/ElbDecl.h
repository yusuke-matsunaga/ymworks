#ifndef ELBDECL_H
#define ELBDECL_H

/// @file ElbDecl.h
/// @brief ElbDecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/VlDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbDecl ElbDecl.h "ElbDecl.h"
/// @brief 名前付きの宣言要素を表すクラス
/// IEEE Std 1364-2001 26.6.6 Nets and net arrays
/// IEEE Std 1364-2001 26.6.7 Regs and reg arrays
/// IEEE Std 1364-2001 26.6.8 Variables
/// IEEE Std 1364-2001 26.6.11 Named event
/// IEEE Std 1364-2001 26.6.12 Parameter, specparam
//////////////////////////////////////////////////////////////////////
class ElbDecl :
  public VlDecl
{
public:

  /// @brief コンストラクタ
  ElbDecl() = default;

  /// @brief デストラクタ
  ~ElbDecl() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号付きに補正する．
  virtual
  void
  set_signed() = 0;

  /// @brief 初期値の設定
  virtual
  void
  set_init(
    const VlExpr* expr ///< [in] 初期値
  ) = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBDECL_H
