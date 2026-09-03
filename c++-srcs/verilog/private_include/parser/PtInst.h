#ifndef PARSER_PTINST_H
#define PARSER_PTINST_H

/// @file PtInst.h
/// @brief PtInst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtList.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtConnection;
class PtRange;

//////////////////////////////////////////////////////////////////////
/// @class PtInst PtInst.h "parser/PtInst.h"
/// @brief AstInst の実装クラス
//////////////////////////////////////////////////////////////////////
class PtInst :
  public PtListElem<PtInst>
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtInst の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  /// @return 名前
  virtual
  const char*
  name() const = 0;

  /// @brief 範囲の取得
  /// @return 範囲
  virtual
  const PtRange*
  range() const = 0;

  /// @brief ポートリストの先頭の取得
  virtual
  const PtConnection*
  port_top() const = 0;

};

/// @brief PtInst のリスト
using PtInstList = PtList<PtInst>;

END_NAMESPACE_YM_VERILOG

#endif // PARSER_PTINST_H
