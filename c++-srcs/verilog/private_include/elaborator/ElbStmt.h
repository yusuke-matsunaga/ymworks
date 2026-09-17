#ifndef ELBSTMT_H
#define ELBSTMT_H

/// @file ElbStmt.h
/// @brief ElbStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/vl/VlStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbStmt ElbStmt.h "elaborator/ElbStmt.h"
/// @brief VlStmt の実装クラス
//////////////////////////////////////////////////////////////////////
class ElbStmt :
  public VlStmt
{
public:

  /// @brief コンストラクタ
  ElbStmt() = default;

  /// @brief デストラクタ
  ~ElbStmt() = default;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBSTMT_H
