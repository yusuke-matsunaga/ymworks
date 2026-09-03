#ifndef PARSER_PTIOHEAD_H
#define PARSER_PTIOHEAD_H

/// @file PtIOHead.h
/// @brief PtDecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtList.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtIOItem;
class PtRange;

//////////////////////////////////////////////////////////////////////
/// @class PtIOHead PtIOHead.h "parser/PtIOHead.h"
/// @brief AstIOHead の実装クラス
//////////////////////////////////////////////////////////////////////
class PtIOHead :
  public PtListElem<PtIOHead>
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtIOHead の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 方向の取得
  /// @return 方向(Input, Output, Inout)
  virtual
  VpiDir
  direction() const = 0;

  /// @brief 補助的な型の取得
  /// @return 補助的な型
  virtual
  VpiAuxType
  aux_type() const = 0;

  /// @brief 補助的なネット型の取得
  /// @return ネット型
  virtual
  VpiNetType
  net_type() const = 0;

  /// @brief 補助的な変数型の取得
  /// @return 変数型
  virtual
  VpiVarType
  var_type() const = 0;

  /// @brief 符号の取得
  /// @retval true 符号付き
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const = 0;

  /// @brief 範囲の取得
  /// @retval 範囲
  /// @retval nullptr 範囲を持たないとき
  virtual
  const PtRange*
  range() const = 0;

  /// @brief 先頭の要素を返す．
  virtual
  const PtIOItem*
  item_top() const = 0;

};

/// @brief PtIOHead のリスト
using PtIOHeadList = PtList<PtIOHead>;

/// @brief PtIOHead と PtIOItem の二重構造のリスト
using PtIODList = PtDList<PtIOHead, PtIOItem>;

/// @brief PtIOHead と PtIOItem のペア
struct PtIOHeadItem {
  PtIOHead* head;
  PtIOItem* item;
};

END_NAMESPACE_YM_VERILOG

#endif // PTIOHEAD_H
