#ifndef PARSER_PTDEFPARM_H
#define PARSER_PTDEFPARM_H

/// @file PtItem.h
/// @brief PtItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "parser/PtList.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtNameBranch;

//////////////////////////////////////////////////////////////////////
/// @class PtDefParam PtItem.h "parser/PtItem.h"
/// @brief AstDefParam の実装クラス
//////////////////////////////////////////////////////////////////////
class PtDefParam :
  public PtListElem<PtDefParam>
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtDefParam の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  /// @return 名前
  virtual
  const char*
  name() const = 0;

  /// @brief 階層ブランチのリストの先頭を返す．
  virtual
  const PtNameBranch*
  namebranch_top() const = 0;

  /// @brief 値の取得
  /// @return 値を表す式
  virtual
  const PtExpr*
  expr() const = 0;

};

/// @brief PtDefParam のリスト
using PtDefParamList = PtList<PtDefParam>;

END_NAMESPACE_YM_VERILOG

#endif // PARSER_PTDEFPARM_H
