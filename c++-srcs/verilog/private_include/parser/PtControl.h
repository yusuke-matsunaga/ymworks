#ifndef PARSER_PTCONTROL_H
#define PARSER_PTCONTROL_H

/// @file PtControl.h
/// @brief PtMisc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtBase.h"
#include "ym/vl/AstControl.h" // for AstControl::Type


BEGIN_NAMESPACE_YM_VERILOG

class PtExpr;

//////////////////////////////////////////////////////////////////////
/// @class PtControl PtControl.h "parser/PtControl.h"
/// @brief AstControl の実装クラス
//////////////////////////////////////////////////////////////////////
class PtControl :
  public PtBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtControl の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  /// @return 型
  virtual
  AstControl::Type
  type() const = 0;

  /// @brief 遅延式の取得
  /// @return 遅延を表す式
  ///
  /// - type() != Delay の時 std::logic_error 例外を送出する．
  virtual
  const PtExpr*
  delay() const = 0;

  /// @brief イベントリストの先頭の取得
  ///
  /// - type() == Delay の時 std::logic_error 例外を送出する．
  virtual
  const PtExpr*
  event_top() const = 0;

  /// @brief 繰り返し数の取得
  /// @return 繰り返し数を表す式
  ///
  /// - type() != Repeat の時 std::logic_error 例外を送出する．
  virtual
  const PtExpr*
  rep_expr() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // PARSER_PTCONTROL_H
