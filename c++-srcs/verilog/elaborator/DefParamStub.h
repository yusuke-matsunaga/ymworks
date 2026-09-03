#ifndef DEFPARAMSTUB_H
#define DEFPARAMSTUB_H

/// @file DefParamStub.h
/// @brief DefParamStub のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/vl/AstItem.h"
#include "ym/vl/AstDefParam.h"


BEGIN_NAMESPACE_YM_VERILOG

/// @brief defparam 文の情報を表すクラス
struct DefParamStub
{

  /// @brief 対象のモジュール
  const VlModule* mModule;

  /// @brief パース木の DefParam ヘッダ
  AstItem mAstHeader;

  /// @brief パース木の DefParam 文
  AstDefParam mAstDefparam;
};

END_NAMESPACE_YM_VERILOG

#endif // DEFPARAMSTUB_H
