#ifndef PARSER_PTPART_H
#define PARSER_PTPART_H

/// @file PtPart.h
/// @brief PtPart のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtBase.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtExpr;

//////////////////////////////////////////////////////////////////////
/// @class PtPart PtPart.h "parser/PtPart.h"
/// @brief AstPart の実装クラス
//////////////////////////////////////////////////////////////////////
class PtPart :
  public PtBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲指定のモードを返す．
  virtual
  VpiRangeMode
  mode() const = 0;

  /// @brief 1番目の式を取り出す．
  virtual
  const PtExpr*
  left() const = 0;

  /// @brief 2番めの式を取り出す．
  virtual
  const PtExpr*
  right() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // PARSER_PTPART_H
