#ifndef PARSER_PTPORT_H
#define PARSER_PTPORT_H

/// @file PtPort.h
/// @brief PtPort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtList.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtExpr;

//////////////////////////////////////////////////////////////////////
/// @class PtPort PtDecle.h "parser/PtDecl.h"
/// @brief AstPort の実装クラス
///
/// 主な目的は設定用の仮想関数を定義すること．
//////////////////////////////////////////////////////////////////////
class PtPort :
  public PtListElem<PtPort>
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtPort の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 外向の名前の取得
  /// @return 外向の名前(本当のポート名)
  ///
  /// 無い場合は nullptr を返す
  virtual
  const char*
  ext_name() const = 0;

  /// @brief 内部の結線を表す式の取得
  ///
  /// - 無効な値の場合もある．
  /// - 複数の結線からなる場合には Concat 演算となる．
  virtual
  const PtExpr*
  expr() const = 0;

  /// @brief 内部のポート結線のリストの先頭の取得
  ///
  /// portef_size() <= 1 の時は nullptr を返す．
  virtual
  const PtExpr*
  portref_top() const = 0;

  /// @brief 内部のポート結線の向きの取得
  virtual
  VpiDir
  portref_dir(
    SizeType index ///< [in] インデックス ( 0 <= index < portref_size() )
  ) const = 0;

  /// @brief 内部のポート結線の向きを設定する．
  virtual
  void
  set_portref_dir(
    SizeType index,
    VpiDir dir
  ) = 0;

};

/// @brief PtPort のリスト
using PtPortList = PtList<PtPort>;

END_NAMESPACE_YM_VERILOG

#endif // PARSER_PTDECL_H
