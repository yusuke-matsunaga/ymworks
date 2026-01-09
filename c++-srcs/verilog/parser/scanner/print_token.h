#ifndef PRINT_TOKEN_H
#define PRINT_TOKEN_H

/// @file print_token.h
/// @brief print_token() のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

/// @brief トークンの内容を出力する．
void
print_token(
  std::ostream& s,       ///< [in] 出力先のストリーム
  int id,                ///< [in] トークン番号
  const std::string& str ///< [in] トークン文字列
);

END_NAMESPACE_YM_VERILOG

#endif // PRINT_TOKEN_H
