#ifndef PTDECLPORTLIST_H
#define PTDECLPORTLIST_H

/// @file PtDeclPortList.h
/// @brief PtDeclPortList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "parser/PtDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtDeclPortList PtDeclPortList.h "parser/PtDeclPortList.h"
/// @brief 宣言ポートリストを表す構造体
///
/// 1つのリストの中に複数の PDeclOHead が含まれるので，
/// DeclIOHead と DeclIOItem の二重のリスト構造を持つ．
///
/// YACC/BISON の YYSTYPE で用いるためコンストラクタ/デストラクタ
/// を持つことができない．
/// なので初期化や終了処理が必要のないポインタのみを持つ．
//////////////////////////////////////////////////////////////////////
struct PtDeclPortList
{

  /// @brief IO宣言ヘッダのリスト
  PtDeclHeadList head_list;

  /// @brief IO要素の末尾
  PtDeclItem* item_tail;

  /// @brief 初期化する．
  void
  init(
    PtDeclHead* head = nullptr
  )
  {
    head_list.init(head);
    item_tail = nullptr;
  }

  /// @brief ヘッダを追加する．
  void
  add_head(
    PtDeclHead* head
  )
  {
    head_list.add(head);
    item_tail = head->_item_top();
  }

  /// @brief 要素を追加する．
  void
  add_item(
    PtDeclItem* item
  )
  {
    item_tail->set_link(item);
    item_tail = item;
  }

};

END_NAMESPACE_YM_VERILOG

#endif // PTDECLPORTLIST_H
