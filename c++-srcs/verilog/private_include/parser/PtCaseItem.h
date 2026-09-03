#ifndef PARSER_PTCASEITEM_H
#define PARSER_PTCASEITEM_H

/// @file PtCaseItem.h
/// @brief PtStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtBase.h"
#include "parser/PtList.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtExpr;
class PtStmt;

//////////////////////////////////////////////////////////////////////
/// @class PtCaseItem PtCaseItem.h "parser/PtCaseItem.h"
/// @brief AstCaseItem の実装クラス
//////////////////////////////////////////////////////////////////////
class PtCaseItem :
  public PtListElem<PtCaseItem>
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtCaseItem の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ラベルリストの先頭の取得
  virtual
  const PtExpr*
  label_top() const = 0;

  /// @brief 本体のステートメントの取得
  /// @return 本体のステートメント
  virtual
  const PtStmt*
  body() const = 0;

};

/// @brief PtCaseItem のリスト
using PtCaseItemList = PtList<PtCaseItem>;

END_NAMESPACE_YM_VERILOG

#endif // PARSER_PTCASEITEM_H
