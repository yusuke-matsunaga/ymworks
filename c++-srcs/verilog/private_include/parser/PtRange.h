#ifndef PARSER_PTRANGE_H
#define PARSER_PTRANGE_H

/// @file PtRange.h
/// @brief PtRange のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtList.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtExpr;

//////////////////////////////////////////////////////////////////////
/// @class PtRange PtRange.h "parser/PtRange.h"
/// @brief AstRange の実装クラス
//////////////////////////////////////////////////////////////////////
class PtRange :
  public PtListElem<PtRange>
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtRange の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲の MSB を取り出す．
  virtual
  const PtExpr*
  left() const = 0;

  /// @brief 範囲の LSB を取り出す．
  virtual
  const PtExpr*
  right() const = 0;

};

/// @brief PtRange のリスト
using PtRangeList = PtList<PtRange>;

END_NAMESPACE_YM_VERILOG

#endif // PTRANGE_H
