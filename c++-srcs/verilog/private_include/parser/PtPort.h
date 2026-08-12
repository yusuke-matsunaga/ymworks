#ifndef PARSER_PTPORT_H
#define PARSER_PTPORT_H

/// @file PtPort.h
/// @brief PtPort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstPort.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtPort PtDecle.h "parser/PtDecl.h"
/// @brief AstPort の実装クラス
///
/// 主な目的は設定用の仮想関数を定義すること．
//////////////////////////////////////////////////////////////////////
class PtPort :
  public AstPort
{
public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief portref の方向を設定する．
  virtual
  void
  set_portref_dir(
    SizeType pos, ///< [in] 位置番号 ( 0 <= pos < portref_num() )
    VpiDir dir    ///< [in] 方向
  ) = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // PARSER_PTDECL_H
