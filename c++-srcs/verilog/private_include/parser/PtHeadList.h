#ifndef PTHEADLIST_H
#define PTHEADLIST_H

/// @file PtHeadList.h
/// @brief PtHeadList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "parser/PtDecl.h"
#include "parser/PtItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtHeadList PtHeadList.h "parser/PtHeadList.h"
/// @brief PtIOHead, PtDeclHead と PtItem のリストを表すクラス
///
/// YACC/BISON の YYSTYPE で用いるためコンストラクタ/デストラクタ
/// を持つことができない．
/// なので初期化や終了処理が必要のないポインタのみを持つ．
//////////////////////////////////////////////////////////////////////
struct PtHeadList
{
  /// @brief IO宣言ヘッダのリスト
  PtIOHeadList iohead_list;

  /// @brief 宣言ヘッダのリスト
  PtDeclHeadList declhead_list;

  /// @brief 要素のリスト
  PtItemList item_list;

  /// @brief 初期化する．
  void
  init()
  {
    iohead_list.init();
    declhead_list.init();
    item_list.init();
  }

};

END_NAMESPACE_YM_VERILOG

#endif // PTHEADLIST_H
