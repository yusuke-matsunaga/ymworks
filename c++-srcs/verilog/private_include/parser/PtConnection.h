#ifndef PARSER_PTCONNECTION_H
#define PARSER_PTCONNECTION_H

/// @file PtConnection.h
/// @brief PtConnection のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtList.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtExpr;

//////////////////////////////////////////////////////////////////////
/// @class PtConnection PtConnection.h "parser/PtConnection.h"
/// @brief AstConnection の実装クラス
//////////////////////////////////////////////////////////////////////
class PtConnection :
  public PtListElem<PtConnection>
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtConnection の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  /// @retval 名前 named connection の場合
  /// @retval "" ordered connection の場合
  virtual
  const char*
  name() const = 0;

  /// @brief 式の取得
  /// @return 式
  virtual
  const PtExpr*
  expr() const = 0;

};

/// @brief PtConnection のリスト
using PtConnectionList = PtList<PtConnection>;

END_NAMESPACE_YM_VERILOG

#endif // PARSER_PTCONNECTION_H
