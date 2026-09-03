#ifndef PARSER_PTPATHDELAY_H
#define PARSER_PTPATHDELAY_H

/// @file PtItem.h
/// @brief PtItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtPathDelay Ptitem.h "parser/PtItem.h"
/// @brief AstPathDelay の実装クラス
//////////////////////////////////////////////////////////////////////
class PtPathDelay :
  public PtBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtPathDelay の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を取り出す．
  /// 0の場合もある．
  virtual
  const PtExpr*
  value(
    SizeType pos ///< [in] 位置 ( 0 <= pos < 3 )
  ) const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // PARSER_PTPATHDELAY_H
