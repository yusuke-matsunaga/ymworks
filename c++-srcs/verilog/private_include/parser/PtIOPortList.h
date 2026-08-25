#ifndef PTIOPORTLIST_H
#define PTIOPORTLIST_H

/// @file PtIOPortList.h
/// @brief PtIOPortList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "parser/PtDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtIOPortList PtHeadList.h "parser/PtHeadList.h"
/// @brief IOポート宣言リストを表す構造体
///
/// 1つのリストの中に複数の PtIOHead が含まれるので，
/// PtIOHead と PtIOItem の二重のリスト構造を持つ．
///
/// YACC/BISON の YYSTYPE で用いるためコンストラクタ/デストラクタ
/// を持つことができない．
/// なので初期化や終了処理が必要のないポインタのみを持つ．
//////////////////////////////////////////////////////////////////////
struct PtIOPortList
{

  /// @brief IO宣言ヘッダのリスト
  PtIOHeadList head_list;

  /// @brief IO要素の末尾
  PtIOItem* item_tail;

  /// @brief 初期化する．
  void
  init(
    PtIOHead* head = nullptr
  )
  {
    head_list.init(head);
    item_tail = nullptr;
  }

  /// @brief ヘッダを追加する．
  void
  add_head(
    PtIOHead* head
  )
  {
    head_list.add(head);
    item_tail = head->_item_top();
  }

  /// @brief 要素を追加する．
  void
  add_item(
    PtIOItem* item
  )
  {
    item_tail->set_link(item);
    item_tail = item;
  }

};

END_NAMESPACE_YM_VERILOG

#endif // PTIOPORTLIST_H
