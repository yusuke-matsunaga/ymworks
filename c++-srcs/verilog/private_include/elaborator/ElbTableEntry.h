#ifndef ELBTABLEENTRY_H
#define ELBTABLEENTRY_H

/// @file ElbTableEntry.h
/// @brief ElbTableEntry のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/VlTableEntry.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbTableEntry ElbUdp.h "ElbUdp.h"
/// @brief UDP の table entry を表すクラス
//////////////////////////////////////////////////////////////////////
class ElbTableEntry :
  public VlTableEntry
{
public:

  /// @brief コンストラクタ
  ElbTableEntry() = default;

  /// @brief デストラクタ
  ~ElbTableEntry() = default;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBTABLEENTRY_H
