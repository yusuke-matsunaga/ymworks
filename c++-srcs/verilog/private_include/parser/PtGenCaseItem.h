#ifndef PARSER_PTGENCASEITEM_H
#define PARSER_PTGENCASEITEM_H

/// @file PtGenCaseItem.h
/// @brief PtGenCaseItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtList.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtGenCaseItem PtGenCaseItem.h "parser/PtGenCaseItem.h"
/// @brief AstGenCaseItem の実装クラス
//////////////////////////////////////////////////////////////////////
class PtGenCaseItem :
  public PtListElem<PtGenCaseItem>
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtGenCaseItem の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ラベルリストの先頭の取得
  virtual
  const PtExpr*
  label_top() const = 0;

  /// @brief 宣言リストの先頭の取得
  virtual
  const PtDeclHead*
  declhead_top() const = 0;

  /// @brief item リストの先頭取得
  virtual
  const PtItem*
  item_top() const = 0;

};

/// @brief PtGenCaseItem のリスト
using PtGenCaseItemList = PtList<PtGenCaseItem>;

END_NAMESPACE_YM_VERILOG

#endif // PARSER_PTGENCASEITEM_H
