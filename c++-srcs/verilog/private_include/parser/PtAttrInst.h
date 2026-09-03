#ifndef PARSER_PTATTRINST_H
#define PARSER_PTATTRINST_H

/// @file PtAttrInst.h
/// @brief PtMisc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtList.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtAttrSpec;

//////////////////////////////////////////////////////////////////////
/// @class PtAttrInst PtAttrInst.h "parser/PtAttrInst.h"
/// @brief AstAttrInst の実装クラス
//////////////////////////////////////////////////////////////////////
class PtAttrInst :
  public PtListElem<PtAttrInst>
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtAttrInst の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素のリストの先頭の取得
  virtual
  const PtAttrSpec*
  attrspec_top() const = 0;

};

/// @brief PtAttrInst のリスト
using PtAttrInstList = PtList<PtAttrInst>;

END_NAMESPACE_YM_VERILOG

#endif // PARSER_PTATTRINST_H
