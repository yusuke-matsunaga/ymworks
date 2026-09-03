#ifndef PARSER_PTPATHDECL_H
#define PARSER_PTPATHDECL_H

/// @file PtItem.h
/// @brief PtItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtBase.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtPathDelay;

//////////////////////////////////////////////////////////////////////
/// @class PtPathDecl Ptitem.h "parser/PtItem.h"
/// @brief AstPathDecl の実装クラス
//////////////////////////////////////////////////////////////////////
class PtPathDecl :
  public PtBase
{
public:

  /// @brief edge_descriptor の取得
  /// @return edge_descriptor
  ///
  /// 0の場合もある．
  virtual
  int
  edge() const = 0;

  /// @brief 入力のリストの先頭の取得
  virtual
  const PtExpr*
  input_top() const = 0;

  /// @brief 入力の極性の取得
  /// @return 入力の極性\n
  /// 0の場合もありうる．
  virtual
  int
  input_pol() const = 0;

  /// @brief パス記述子(?)の取得
  /// @return vpiParallel か vpiFull
  virtual
  VpiPathType
  op() const = 0;

  /// @brief 出力リストの先頭の取得
  virtual
  const PtExpr*
  output_top() const = 0;

  /// @brief 出力の極性の取得
  /// @return 出力の極性\n
  /// 0の場合もありうる．
  virtual
  int
  output_pol() const = 0;

  /// @brief 式の取得
  /// @return 式
  virtual
  const PtExpr*
  expr() const = 0;

  /// @brief path_delay_value の取得
  /// @return path_delay_value
  virtual
  const PtPathDelay*
  path_delay() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // PARSER_PTPATHDECL_H
