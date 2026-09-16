#ifndef UDPGEN_H
#define UDPGEN_H

/// @file UdpGen.h
/// @brief UdpGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/vl/Ast.h"
#include "ElbProxy.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class UdpGen UdpGen.h "UdpGen.h"
/// @brief ElbUdpDefn を生成するクラス
//////////////////////////////////////////////////////////////////////
class UdpGen :
  public ElbProxy
{
public:

  /// @brief コンストラクタ
  UdpGen(
    Elaborator& elab ///< [in] 生成器
  );

  /// @brief デストラクタ
  ~UdpGen();


public:
  //////////////////////////////////////////////////////////////////////
  // UdpGen の関数
  //////////////////////////////////////////////////////////////////////
  /// @brief UDP定義を生成する．
  void
  instantiate_udp(
    const AstUdp& ast_udp ///< [in] パース木の UDP 定義
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Verilog1995 スタイルのIOポート生成を行う．
  /// @return 出力の宣言ヘッダを返す．
  AstIOHead
  gen_io1995(
    ElbUdpDefn* udp,                 ///< [in] 対象のUDP
    const AstPortList& port_list,    ///< [in] ポートリスト
    const AstIOHeadList& iohead_list ///< [in] IOヘッダリスト
  );

  /// @brief Verilog2001 スタイルのIOポート生成を行う．
  /// @return 出力の宣言ヘッダを返す．
  AstIOHead
  gen_io2001(
    ElbUdpDefn* udp,                 ///< [in] 対象のUDP
    const AstIOHeadList& iohead_list ///< [in] IOヘッダリスト
  );

  /// @brief 組み合わせ回路用のテーブルを生成する．
  void
  gen_comb_table(
    ElbUdpDefn* udp,
    SizeType io_size,
    const AstUdpEntryList& table_list
  );

  /// @brief 順序回路用のテーブルを生成する．
  void
  gen_seq_table(
    ElbUdpDefn* udp,
    SizeType io_size,
    const AstUdpEntryList& table_list
  );

};

END_NAMESPACE_YM_VERILOG

#endif // ELBUDPGEN_H
