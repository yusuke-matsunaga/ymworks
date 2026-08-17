
/// @file AstHierNamedBase.cc
/// @brief AstHierNamedBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"
#include "ym/vl/AstMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstHierNamedBase
//////////////////////////////////////////////////////////////////////

// @brief 階層名を展開する．
std::string
AstHierNamedBase::decompile_name() const
{
  std::ostringstream buf;
  const char* period = "";
  for ( auto nb: namebranch_list() ) {
    buf << period << nb->decompile();
    period = ".";
  }
  if ( name() != nullptr ) {
    buf << period << name();
  }
  return buf.str();
}

END_NAMESPACE_YM_VERILOG
