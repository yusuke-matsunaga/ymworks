#ifndef ELBDECLARRAY_H
#define ELBDECLARRAY_H

/// @file ElbDeclArray.h
/// @brief ElbDeclArray のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/vl/VlDeclArray.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbDeclArray ElbDeclArray.h "elaborator/ElbDeclArray.h"
/// @brief VlDeclArray の実装クラス
//////////////////////////////////////////////////////////////////////
class ElbDeclArray :
  public VlDeclArray
{
public:

  /// @brief コンストラクタ
  ElbDeclArray() = default;

  /// @brief デストラクタ
  ~ElbDeclArray() = default;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBDECLARRAY_H
