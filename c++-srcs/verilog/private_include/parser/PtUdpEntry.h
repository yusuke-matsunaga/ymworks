#ifndef PARSER_PTUDPENTRY_H
#define PARSER_PTUDPENTRY_H

/// @file PtUdpEntry.h
/// @brief PtUdp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtList.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtUdpValue;

//////////////////////////////////////////////////////////////////////
/// @class PtUdpEntry PtUdpEntry.h "parser/PtUdpEntry.h"
/// @brief AstUdpEntry の実装クラス
//////////////////////////////////////////////////////////////////////
class PtUdpEntry :
  public PtListElem<PtUdpEntry>
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtUdpEntry の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力値のリストの先頭を取り出す．
  virtual
  const PtUdpValue*
  input_top() const = 0;

  /// @brief 現状態の値を取り出す．
  virtual
  const PtUdpValue*
  current() const = 0;

  /// @brief 出力の値を取り出す．
  virtual
  const PtUdpValue*
  output() const = 0;

};

/// @brief PtUdpEntry のリスト
using PtUdpEntryList = PtList<PtUdpEntry>;

END_NAMESPACE_YM_VERILOG

#endif // PARSER_PTUDPENTRY_H
