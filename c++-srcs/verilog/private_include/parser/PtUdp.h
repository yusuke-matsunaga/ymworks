#ifndef PARSER_PTUDP_H
#define PARSER_PTUDP_H

/// @file PtUdp.h
/// @brief PtUdp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

//#include "ym/vl/AstUdp.h"
#include "parser/PtBase.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtPort;
class PtIOHead;
class PtExpr;
class PtUdpEntry;

//////////////////////////////////////////////////////////////////////
/// @class PtUdp PtUdp.h "parser/PtUdp.h"
/// @brief AstUdp の実装クラス
//////////////////////////////////////////////////////////////////////
class PtUdp :
  public PtBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtUdp の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  /// @return ファイル位置
  virtual
  FileRegion
  file_region() const = 0;

  /// @brief 名前の取得
  /// @return 名前
  virtual
  const char*
  name() const = 0;

  /// @brief primitive type を返す．
  virtual
  VpiPrimType
  prim_type() const = 0;

  /// @brief ポートのリストの先頭を取り出す．
  virtual
  const PtPort*
  port_top() const = 0;

  /// @brief 入出力宣言ヘッダのリストの先頭取得
  virtual
  const PtIOHead*
  iohead_top() const = 0;

  /// @brief 初期値を取出す．
  virtual
  const PtExpr*
  init_value() const = 0;

  /// @brief テーブルのリストの先頭を返す．
  virtual
  const PtUdpEntry*
  table_top() const = 0;

  /// @brief 次の要素を返す．
  PtUdp*
  link() const
  {
    return mLink;
  }

  /// @brief link を設定する．
  void
  set_link(
    PtUdp* link
  )
  {
    mLink = link;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素
  PtUdp* mLink{nullptr};

};

END_NAMESPACE_YM_VERILOG

#endif // PARSER_PTUDP_H
