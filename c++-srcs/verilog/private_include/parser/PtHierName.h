#ifndef PARSER_PTHIERNAME_H
#define PARSER_PTHIERNAME_H

/// @file parser/PtHierName.h
/// @brief PtHierName のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtHierName PtHierName.h "parser/PtHierName.h"
/// @ingroup VlParser
/// @brief 階層つき名を表すクラス
///
/// 中身は階層ブランチの先頭と文字列から成る．
/// 実際にはパーズ中は階層ブランチは逆順のリンクトリストになっている．
/// 最後に逆転してから構文木に組み込む．
///
/// YACC/BISON の YYSTYPE で用いるためコンストラクタ/デストラクタ
/// を持つことができない．
/// なので初期化や終了処理が必要のないポインタのみを持つ．
//////////////////////////////////////////////////////////////////////
struct PtHierName
{

  // 先頭の階層ブランチ
  PtNameBranch* nb_top;

  // 最下層の名前
  const char* tail_name;

};

END_NAMESPACE_YM_VERILOG

#endif // PARSER_PTHIERNAME_H
