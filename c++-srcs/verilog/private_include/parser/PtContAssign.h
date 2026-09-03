#ifndef PARSER_PTCONTASSIGN_H
#define PARSER_PTCONTASSIGN_H

/// @file PtContAssign.h
/// @brief PtContAssign のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "parser/PtList.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtContAssign PtContAssign.h "parser/PtContAssign.h"
/// @brief AstContAssign の実装クラス
//////////////////////////////////////////////////////////////////////
class PtContAssign :
  public PtListElem<PtContAssign>
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtContAssign の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 左辺式の取得
  /// @return 左辺式
  virtual
  const PtExpr*
  lhs() const = 0;

  /// @brief 右辺式の取得
  /// @return 右辺式
  virtual
  const PtExpr*
  rhs() const = 0;

};

/// @brief PtContAssign のリスト
using PtContAssignList = PtList<PtContAssign>;

END_NAMESPACE_YM_VERILOG

#endif // PARSER_PTCONTASSIGN_H
