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
  // PtPort の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部のポート結線の向きを設定する．
  virtual
  void
  set_portref_dir(
    SizeType index,
    VpiDir dir
  ) = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // PARSER_PTDECL_H
