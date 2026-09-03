#ifndef PARSER_PTATTRSPEC_H
#define PARSER_PTATTRSPEC_H

/// @file PtAttrSpec.h
/// @brief PtMisc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "parser/PtList.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtExpr;

//////////////////////////////////////////////////////////////////////
/// @class PtAttrSpec PtAttrSpec.h "parser/PtAttrSpec.h"
/// @brief AstAttrSpec の実装クラス
//////////////////////////////////////////////////////////////////////
class PtAttrSpec :
  public PtListElem<PtAttrSpec>
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtAttrSpec の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  /// @return 名前
  virtual
  const char*
  name() const = 0;

  /// @brief 式の取得
  /// @return 式
  virtual
  const PtExpr*
  expr() const = 0;

};

/// @brief PtAttrSepc のリスト
using PtAttrSpecList = PtList<PtAttrSpec>;

END_NAMESPACE_YM_VERILOG

#endif // PARSER_PTATTRSPEC_H
