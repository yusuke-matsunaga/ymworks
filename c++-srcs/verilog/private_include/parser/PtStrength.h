#ifndef PARSER_PTSTRENGTH_H
#define PARSER_PTSTRENGTH_H

/// @file PtStrength.h
/// @brief PtStrength のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtStrength PtStrength.h "parser/PtStrength.h"
/// @brief AstStrength の実装クラス
//////////////////////////////////////////////////////////////////////
class PtStrength :
  public PtBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtStrength の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief drive strength0 の取得
  /// @return 0 の強度
  virtual
  VpiStrength
  drive0() const = 0;

  /// @brief drive strength1 の取得
  /// @return 1 の強度
  virtual
  VpiStrength
  drive1() const = 0;

  /// @brief charge strength の取得
  /// @return 電荷の強度
  virtual
  VpiStrength
  charge() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // PARSER_PTSTRENGTH_H
