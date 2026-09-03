#ifndef PARSER_PTIOITEM_H
#define PARSER_PTIOITEM_H

/// @file PtIOItem.h
/// @brief PtDecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtList.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtExpr;

//////////////////////////////////////////////////////////////////////
/// @class PtIOItem PtIOItem.h "parser/PtIOItem.h"
/// @brief AstIOItem の実装クラス
//////////////////////////////////////////////////////////////////////
class PtIOItem :
  public PtListElem<PtIOItem>
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtIOItem の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  /// @return 名前
  virtual
  const char*
  name() const = 0;

  /// @brief 初期値の取得
  ///
  /// 初期値を持たないときは無効な値が返される
  virtual
  const PtExpr*
  init_value() const = 0;

};

/// @brief PtIOItem のリスト
using PtIOItemList = PtList<PtIOItem>;

END_NAMESPACE_YM_VERILOG

#endif // PTIOITEM_H
