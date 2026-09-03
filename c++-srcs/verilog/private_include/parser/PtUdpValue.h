#ifndef PARSER_PTUDPVALUE_H
#define PARSER_PTUDPVALUE_H

/// @file PtUdpValue.h
/// @brief PtUdp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtList.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtUdpValue PtUdpValue.h "parser/PtUdpValue.h"
/// @brief AstUdpValue の実装クラス
//////////////////////////////////////////////////////////////////////
class PtUdpValue :
  public PtListElem<PtUdpValue>
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtUdpValue の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を取り出す．
  virtual
  VlUdpVal
  symbol() const = 0;

};

/// @brief PtUdpValue のリスト
using PtUdpValueList = PtList<PtUdpValue>;

END_NAMESPACE_YM_VERILOG

#endif // PARSER_PTUDPVALUE_H
