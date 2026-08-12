
/// @file Pt.cc
/// @brief PtXXX の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtHierName.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// PtNameBranch 用の関数
// 適当な場所がないのでここに置いている．
//////////////////////////////////////////////////////////////////////

// @brief 階層名を展開する．
std::string
PtHierName::fullname() const
{
  std::ostringstream buf;
  const char* period = "";
  for ( auto nb: namebranch_list() ) {
    buf << period << nb->expand_name();
    period = ".";
  }
  if ( name() != nullptr ) {
    buf << period << name();
  }
  return buf.str();
}

END_NAMESPACE_YM_VERILOG
