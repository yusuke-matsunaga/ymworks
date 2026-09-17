#ifndef ELBCONTASSIGN_H
#define ELBCONTASSIGN_H

/// @file ElbContAssign.h
/// @brief ElbContAssign のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/vl/VlContAssign.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbContAssign ElbContAssign.h "elaborator/ElbContAssign.h"
/// @brief VlContAssign の実装クラス
//////////////////////////////////////////////////////////////////////
class ElbContAssign :
  public VlContAssign
{
public:

  /// @brief コンストラクタ
  ElbContAssign() = default;

  /// @brief デストラクタ
  ~ElbContAssign() = default;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBCONTASSIGN_H
