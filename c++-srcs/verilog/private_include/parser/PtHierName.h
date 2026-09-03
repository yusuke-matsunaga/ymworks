#ifndef PARSER_PTHIERNAME_H
#define PARSER_PTHIERNAME_H

/// @file parser/PtHierName.h
/// @brief PtHierName のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "parser/PtNameBranch.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtHierName PtHierName.h "parser/PtHierName.h"
/// @ingroup VlParser
/// @brief 階層つき名を表すクラス
///
/// 中身は階層ブランチの先頭と末尾さらに本体の名前の文字列から成る．
/// 階層を追加する場合には現在の本体の名前が最下位の階層名となる．
///
/// YACC/BISON の YYSTYPE で用いるためコンストラクタ/デストラクタ
/// を持つことができない．
/// なので初期化や終了処理が必要のないポインタのみを持つ．
/// ポインタの初期化は外部の責任で行う必要がある．
//////////////////////////////////////////////////////////////////////
struct PtHierName
{

  /// @brief 階層ブランチのリスト
  PtNameBranchList nb_list;

  /// @brief 末尾の名前
  const char* tail_name;

};

END_NAMESPACE_YM_VERILOG

#endif // PARSER_PTHIERNAME_H
