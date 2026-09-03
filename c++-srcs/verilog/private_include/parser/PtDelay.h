#ifndef PARSER_PTDELAY_H
#define PARSER_PTDELAY_H

/// @file PtDelay.h
/// @brief PtMisc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtBase.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtExpr;

//////////////////////////////////////////////////////////////////////
/// @class PtDelay PtDelay.h "parser/PtDelay.h"
/// @brief AstDelay の実装クラス
//////////////////////////////////////////////////////////////////////
class PtDelay :
  public PtBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtDelay の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値の取得
  ///
  /// 該当する要素がなければ nullptr を返す．
  virtual
  const PtExpr*
  value0() const = 0;

  /// @brief 値の取得
  ///
  /// 該当する要素がなければ nullptr を返す．
  virtual
  const PtExpr*
  value1() const = 0;

  /// @brief 値の取得
  ///
  /// 該当する要素がなければ nullptr を返す．
  virtual
  const PtExpr*
  value2() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // PARSER_PTDELAY_H
