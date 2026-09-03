#ifndef PARSER_PTDECLITEM_H
#define PARSER_PTDECLITEM_H

/// @file PtDeclItem.h
/// @brief PtDeclItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/FileRegion.h"
#include "parser/PtList.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtRange;
class PtExpr;

//////////////////////////////////////////////////////////////////////
/// @class PtDeclItem PtDeclItem.h "parser/PtDeclItem.h"
/// @brief AstDeclItem の実装クラス
//////////////////////////////////////////////////////////////////////
class PtDeclItem :
  public PtListElem<PtDeclItem>
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtDeclItem の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  /// @return 名前
  virtual
  const char*
  name() const = 0;

  /// @brief 範囲のリストの先頭を返す．
  virtual
  const PtRange*
  range_top() const = 0;

  /// @brief 初期値の取得
  ///
  /// 設定がない場合は無効な値が返される．
  virtual
  const PtExpr*
  init_value() const = 0;

};

/// @brief PtDeclItem のリスト
using PtDeclItemList = PtList<PtDeclItem>;

END_NAMESPACE_YM_VERILOG

#endif // PTDECLITEM_H
