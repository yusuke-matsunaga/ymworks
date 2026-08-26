#ifndef PTFACTORY_H
#define PTFACTORY_H

/// @file PtFactory.h
/// @brief PtFactory のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/FileRegion.h"
#include "ym/vl/Ast.h"
#include "parser/PtFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

class Alloc;

//////////////////////////////////////////////////////////////////////
/// @class PtFactory PtFactory.h "PtFactory.h"
/// @brief Verilog-HDL のパース木を生成するファクトリクラス
//////////////////////////////////////////////////////////////////////
class PtFactory
{
public:

  /// @brief コンストラクタ
  PtFactory(
    Alloc& alloc ///< [in] メモリアロケータ
  ) : mAlloc{alloc}
  {
  }

  /// @brief デストラクタ
  ~PtFactory() { }


public:
  //////////////////////////////////////////////////////////////////////
  // UDP 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief combinational UDP の生成
  /// @return 生成された UDP
  PtUdp*
  new_CmbUdp(
    const FileRegion& fr, ///< [in] ファイル位置情報
    const char* name,     ///< [in] UDP名
    PtPort* port_top,     ///< [in] ポート名の配列
    PtIOHead* io_top,     ///< [in] IOの配列
    PtUdpEntry* entry_top ///< [in] テーブルエントリの先頭
  );

  /// @brief sequential UDP の生成
  /// @return 生成された UDP
  PtUdp*
  new_SeqUdp(
    const FileRegion& fr,      ///< [in] ファイル位置情報
    const char* name,	       ///< [in] UDP名
    PtPort* port_top,          ///< [in] ポート名の配列
    PtIOHead* io_top,          ///< [in] IOの配列
    const AstExpr* init_value, ///< [in] 初期値を表す式
    PtUdpEntry* entry_top      ///< [in] テーブルエントリの先頭
  );

  /// @brief combinational UDP 用のテーブルエントリの生成
  /// @return 生成されたテーブルエントリ
  PtUdpEntry*
  new_UdpEntry(
    const FileRegion& fr,     ///< [in] ファイル位置の情報
    PtUdpValue* input_top,    ///< [in] 入力値の配列
    const AstUdpValue* output ///< [in] 出力値
  );

  /// @brief sequential UDP 用のテーブルエントリの生成
  /// @return 生成されたテーブルエントリ
  PtUdpEntry*
  new_UdpEntry(
    const FileRegion& fr,       ///< [in] ファイル位置の情報
    PtUdpValue* input_top,      ///< [in] 入力値の配列
    const AstUdpValue* current,	///< [in] 現状態値
    const AstUdpValue* output   ///< [in] 出力値
  );

  /// @brief UDP のテーブルエントリの要素の値の生成
  /// @return 生成された値
  PtUdpValue*
  new_UdpValue(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    char symbol           ///< [in] シンボル
  );

  /// @brief UDP のテーブルエントリの要素の値の生成
  /// @return 生成された値
  PtUdpValue*
  new_UdpValue(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    char symbol1,         ///< [in] シンボル1
    char symbol2          ///< [in] シンボル2
  );


  //////////////////////////////////////////////////////////////////////
  // モジュール関係
  //////////////////////////////////////////////////////////////////////

  /// @brief モジュールの生成
  /// @return 生成されたモジュール
  PtModule*
  new_Module(
    const FileRegion& fr,       ///< [in] ファイル位置の情報
    const char* name,           ///< [in] モジュール名
    bool macro,                 ///< [in] macromodule の時 true となるフラグ
    bool is_cell,               ///< [in] `celldefine - `endcelldefine
                                ///<      で囲まれていたときに true となるフラグ
    bool is_protected,          ///< [in] 保護されているときに true となるフラグ．
                                ///<      verilog の構文にこんな情報はない．
    int time_unit,              ///< [in] 時刻の単位を表す数値．
    int time_precision,         ///< [in] 時刻の精度を表す数値．
                                ///<      有効な値は 2 - -15 で 2 の時に 100秒を表す．
                                ///<      以下，1減るごとに10分の1になる．
                                ///<      -16 で未定義を表す．
    VpiNetType net_type,        ///< [in] 未定義の識別子から暗黙のネットを生成する
                                ///<      時のネット型．
    VpiUnconnDrive unconn,      ///< [in] unconnected_drive の値．
    VpiDefDelayMode delay,      ///< [in] delay_mode の値．
    int decay,                  ///< [in] default_decay_time の値．
                                ///<      意味のある値は正の整数もしくは無限大を
                                ///<      あらわす -1
    bool explicit_name,         ///< [in] ポートがすべて名前付きのときに true
                                ///<      となるフラグ
    bool portfaults,            ///< [in] Verifault 用．
    bool suppress_faults,       ///< [in] Verifault 用
    const std::string& config,  ///< [in] config 情報
    const std::string& library, ///< [in] library 情報
    const std::string& cell,    ///< [in] cell 情報
    PtDeclHead* paramport_top , ///< [in] パラメータポートのリスト
    PtPort* port_top,           ///< [in] ポートのリスト
    PtIOHead* iodecl_top,       ///< [in] 入出力のリスト
    PtDeclHead* decl_top,       ///< [in] 宣言のリスト
    PtItem* item_top            ///< [in] 要素のリスト
  );


  //////////////////////////////////////////////////////////////////////
  // ポート関係
  //////////////////////////////////////////////////////////////////////

  /// @brief ポートの生成 (空のポート / 名前のみのポート)
  /// @return 生成されたポート
  PtPort*
  new_Port(
    const FileRegion& fr,          ///< [in] ファイル位置の情報
    const char* ext_name = nullptr ///< [in] ポート名
  );

  /// @brief ポートの生成 (ポート参照式があるポート)
  /// @return 生成されたポート
  PtPort*
  new_Port(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* ext_name, ///< [in] ポート名 (空文字列の場合もある)
    PtExpr* portref ///< [in] ポートに接続している式 (ポート参照式)
  );


  //////////////////////////////////////////////////////////////////////
  // IO 宣言関係
  //////////////////////////////////////////////////////////////////////

  /// @brief 1ビットの IO 宣言のヘッダの生成
  /// @return 生成された IO宣言ヘッダ
  PtIOHead*
  new_IOHead(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    VpiDir dir,		   ///< [in] IO の種類 (input, inout, output)
    PtIOItem* item_top     ///< [in] 要素の先頭
  );

  /// @brief 範囲付きの IO 宣言のヘッダの生成
  /// @return 生成された IO宣言ヘッダ
  PtIOHead*
  new_IOHead(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    VpiDir dir,		   ///< [in] IO の種類 (input, inout, output)
    bool sign,		   ///< [in] 符号付きのとき true となるフラグ
    const AstRange* range, ///< [in] パース木の範囲定義
    PtIOItem* item_top     ///< [in] 要素の先頭
  );

  /// @brief 1ビットの IO 宣言のヘッダの生成 (reg 型)
  /// @return 生成された IO宣言ヘッダ
  PtIOHead*
  new_RegIOHead(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    VpiDir dir,		   ///< [in] IO の種類 (input, inout, output)
    PtIOItem* item_top     ///< [in] 要素の先頭
  );

  /// @brief 範囲付きの IO 宣言のヘッダの生成 (reg 型)
  /// @return 生成された IO宣言ヘッダ
  PtIOHead*
  new_RegIOHead(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    VpiDir dir,		   ///< [in] IO の種類 (input, inout, output)
    bool sign,		   ///< [in] 符号付きのとき true となるフラグ
    const AstRange* range, ///< [in] パース木の範囲定義
    PtIOItem* item_top     ///< [in] 要素の先頭
  );

  /// @brief 1ビットの IO 宣言のヘッダの生成 (ネット型)
  /// @return 生成された IO宣言ヘッダ
  PtIOHead*
  new_NetIOHead(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    VpiDir dir,		   ///< [in] IO の種類 (input, inout, output)
    VpiNetType net_type,   ///< [in] 補助的なネット型
    PtIOItem* item_top     ///< [in] 要素の先頭
  );

  /// @brief 範囲付きの IO 宣言のヘッダの生成 (ネット型)
  /// @return 生成された IO宣言ヘッダ
  PtIOHead*
  new_NetIOHead(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    VpiDir dir,		   ///< [in] IO の種類 (input, inout, output)
    VpiNetType net_type,   ///< [in] 補助的なネット型
    bool sign,             ///< [in] 符号付きのとき true となるフラグ
    const AstRange* range, ///< [in] パース木の範囲定義
    PtIOItem* item_top     ///< [in] 要素の先頭
  );

  /// @brief IO 宣言のヘッダの生成 (変数型)
  /// @return 生成された IO宣言ヘッダ
  PtIOHead*
  new_VarIOHead(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiDir dir,           ///< [in] IO の種類 (input, inout, output)
    VpiVarType var_type,  ///< [in] 補助的な変数型
    PtIOItem* item_top    ///< [in] 要素の先頭
  );

  /// @brief 初期値付き IO 宣言の要素の生成
  /// @return 生成された要素
  PtIOItem*
  new_IOItem(
    const FileRegion& fr,     ///< [in] ファイル位置の情報
    const char* name          ///< [in] 要素名
  );

  /// @brief 初期値付き IO 宣言の要素の生成
  /// @return 生成された要素
  PtIOItem*
  new_IOItem(
    const FileRegion& fr,     ///< [in] ファイル位置の情報
    const char* name,         ///< [in] 要素名
    const AstExpr* init_value ///< [in] 初期値を表す式
  );


  //////////////////////////////////////////////////////////////////////
  // その他の宣言関係
  //////////////////////////////////////////////////////////////////////

  /// @brief 1ビットのパラメータ宣言のヘッダの生成
  /// @return 生成されたパラメータ
  PtDeclHead*
  new_ParamH(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    PtDeclItem* item_top   ///< [in] 要素の先頭
  );

  /// @brief 範囲指定型パラメータ宣言のヘッダの生成
  /// @return 生成されたパラメータ
  PtDeclHead*
  new_ParamH(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    bool sign,             ///< [in] 符号付きのとき true となるフラグ
    const AstRange* range, ///< [in] パース木の範囲定義
    PtDeclItem* item_top   ///< [in] 要素の先頭
  );

  /// @brief 1ビットの型ローカルパラメータ宣言のヘッダの生成
  /// @return 生成されたパラメータ
  PtDeclHead*
  new_LocalParamH(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    PtDeclItem* item_top   ///< [in] 要素の先頭
  );

  /// @brief 範囲指定型ローカルパラメータ宣言のヘッダの生成
  /// @return 生成されたパラメータ
  PtDeclHead*
  new_LocalParamH(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    bool sign,             ///< [in] 符号付きのとき true となるフラグ
    const AstRange* range, ///< [in] パース木の範囲定義
    PtDeclItem* item_top   ///< [in] 要素の先頭
  );

  /// @brief 組み込み型パラメータ宣言のヘッダの生成
  /// @return 生成されたパラメータ
  PtDeclHead*
  new_ParamH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiVarType var_type,  ///< [in] データ型
    PtDeclItem* item_top  ///< [in] 要素の先頭
  );

  /// @brief 組み込み型ローカルパラメータ宣言のヘッダの生成
  /// @return 生成されたパラメータ
  PtDeclHead*
  new_LocalParamH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiVarType var_type,  ///< [in] データ型
    PtDeclItem* item_top   ///< [in] 要素の先頭
  );

  /// @brief 1ビットの specparam 宣言のヘッダの生成
  /// @return 生成された specparam
  PtDeclHead*
  new_SpecParamH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PtDeclItem* item_top  ///< [in] 要素の先頭
  );

  /// @brief 範囲指定型 specparam 宣言のヘッダの生成
  /// @return 生成された specparam
  PtDeclHead*
  new_SpecParamH(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    const AstRange* range, ///< [in] パース木の範囲定義
    PtDeclItem* item_top   ///< [in] 要素の先頭
  );

  /// @brief イベント宣言のヘッダの生成
  /// @return 生成されたイベント
  PtDeclHead*
  new_EventH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PtDeclItem* item_top  ///< [in] 要素の先頭
  );

  /// @brief genvar 宣言のヘッダの生成
  /// @return 生成された genvar
  PtDeclHead*
  new_GenvarH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PtDeclItem* item_top  ///< [in] 要素の先頭
  );

  /// @brief 変数宣言のヘッダの生成
  /// @return 生成された変数
  PtDeclHead*
  new_VarH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiVarType var_type,  ///< [in] データ型
    PtDeclItem* item_top  ///< [in] 要素の先頭
  );

  /// @brief 1ビットの reg 宣言のヘッダの生成
  /// @return 生成された reg
  PtDeclHead*
  new_RegH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PtDeclItem* item_top  ///< [in] 要素の先頭
  );

  /// @brief 範囲指定型 reg 宣言のヘッダの生成
  /// @return 生成された reg
  PtDeclHead*
  new_RegH(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    bool sign,             ///< [in] 符号付きの時 true となるフラグ
    const AstRange* range, ///< [in] パース木の範囲定義
    PtDeclItem* item_top   ///< [in] 要素の先頭
  );

  /// @brief 1ビット net 宣言のヘッダの生成 (strengthなし, 遅延なし)
  /// @return 生成されたネット
  PtDeclHead*
  new_NetH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiNetType type,      ///< [in] net の型
    PtDeclItem* item_top  ///< [in] 要素の先頭
  );

  /// @brief 範囲指定型 net 宣言のヘッダの生成 (strengthなし, 遅延なし)
  /// @return 生成されたネット
  PtDeclHead*
  new_NetH(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    VpiNetType type,	   ///< [in] net の型
    VpiVsType vstype,	   ///< [in] vector/scalar 指定
    bool sign,		   ///< [in] 符号の有無を表すフラグ
    const AstRange* range, ///< [in] パース木の範囲定義
    PtDeclItem* item_top   ///< [in] 要素の先頭
  );

  /// @brief 1ビット net 宣言のヘッダの生成 (strengthなし, 遅延あり)
  /// @return 生成されたネット
  PtDeclHead*
  new_NetH(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    VpiNetType type,	   ///< [in] net の型
    const AstDelay* delay, ///< [in] 遅延
    PtDeclItem* item_top   ///< [in] 要素の先頭
  );

  /// @brief 範囲指定型 net 宣言のヘッダの生成 (strengthなし, 遅延あり)
  /// @return 生成されたネット
  PtDeclHead*
  new_NetH(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    VpiNetType type,	   ///< [in] net の型
    VpiVsType vstype,	   ///< [in] vector/scalar 指定
    bool sign,		   ///< [in] 符号の有無を表すフラグ
    const AstRange* range, ///< [in] パース木の範囲定義
    const AstDelay* delay, ///< [in] 遅延
    PtDeclItem* item_top   ///< [in] 要素の先頭
  );

  /// @brief 1ビット net 宣言のヘッダの生成 (strengthあり，遅延なし)
  /// @return 生成されたネット
  PtDeclHead*
  new_NetH(
    const FileRegion& fr,        ///< [in] ファイル位置の情報
    VpiNetType type,  	         ///< [in] net の型
    const AstStrength* strength, ///< [in] 信号強度
    PtDeclItem* item_top         ///< [in] 要素の先頭
  );

  /// @brief 範囲指定型 net 宣言のヘッダの生成 (strengthあり，遅延なし)
  /// @return 生成されたネット
  PtDeclHead*
  new_NetH(
    const FileRegion& fr,        ///< [in] ファイル位置の情報
    VpiNetType type,  	         ///< [in] net の型
    const AstStrength* strength, ///< [in] 信号強度
    VpiVsType vstype,	         ///< [in] vector/scalar 指定
    bool sign,		         ///< [in] 符号の有無を表すフラグ
    const AstRange* range,       ///< [in] パース木の範囲定義
    PtDeclItem* item_top         ///< [in] 要素の先頭
  );

  /// @brief 1ビット net 宣言のヘッダの生成 (strengthあり，遅延あり)
  /// @return 生成されたネット
  PtDeclHead*
  new_NetH(
    const FileRegion& fr,        ///< [in] ファイル位置の情報
    VpiNetType type,  	         ///< [in] net の型
    const AstStrength* strength, ///< [in] 信号強度
    const AstDelay* delay,       ///< [in] 遅延
    PtDeclItem* item_top         ///< [in] 要素の先頭
  );

  /// @brief 範囲指定型 net 宣言のヘッダの生成 (strengthあり，遅延あり)
  /// @return 生成されたネット
  PtDeclHead*
  new_NetH(
    const FileRegion& fr,        ///< [in] ファイル位置の情報
    VpiNetType type,  	         ///< [in] net の型
    const AstStrength* strength, ///< [in] 信号強度
    VpiVsType vstype,	         ///< [in] vector/scalar 指定
    bool sign,		         ///< [in] 符号の有無を表すフラグ
    const AstRange* range,       ///< [in] パース木の範囲定義
    const AstDelay* delay,       ///< [in] 遅延
    PtDeclItem* item_top         ///< [in] 要素の先頭
  );

  /// @brief 宣言要素の生成
  /// @return 生成された要素
  PtDeclItem*
  new_DeclItem(
    const FileRegion& fr,     ///< [in] ファイル位置の情報
    const char* name          ///< [in] 名前
  );

  /// @brief 初期値付き宣言要素の生成
  /// @return 生成された要素
  PtDeclItem*
  new_DeclItem(
    const FileRegion& fr,     ///< [in] ファイル位置の情報
    const char* name,         ///< [in] 名前
    const AstExpr* init_value ///< [in] 初期値を表す式
  );

  /// @brief 配列型宣言要素の生成
  /// @return 生成された要素
  PtDeclItem*
  new_DeclItem(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    PtRange* range_tail   ///< [in] 配列の範囲の末尾
  );

  /// @brief 範囲の生成
  PtRange*
  new_Range(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* msb,   ///< [in] MSB を表す式
    const AstExpr* lsb    ///< [in] LSB を表す式
  );


  //////////////////////////////////////////////////////////////////////
  // item 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief defparam 文のヘッダの生成
  /// @return 生成された defparam ヘッダ
  PtItem*
  new_DefParamH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PtDefParam* elem_top  ///< [in] 要素のリスト
  );

  /// @brief defparam 文の要素の生成
  /// @return 生成された要素
  PtDefParam*
  new_DefParam(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    const AstExpr* value  ///< [in] 値を表す式
  );

  /// @brief defparam 文の要素の生成 (階層つき識別子)
  /// @return 生成された要素
  PtDefParam*
  new_DefParam(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const PtHierName& hname, ///< [in] 階層名
    const AstExpr* value     ///< [in] 値を表す式
  );

  /// @brief continuous assign 文のヘッダの生成 (strengthなし, 遅延なし)
  /// @return 生成された continuous assign 文のヘッダ
  PtItem*
  new_ContAssignH(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    PtContAssign* elem_top ///< [in] 要素の先頭
  );

  /// @brief continuous assign 文のヘッダの生成 (strengthなし, 遅延付き)
  /// @return 生成された continuous assign 文のヘッダ
  PtItem*
  new_ContAssignH(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    const AstDelay* delay, ///< [in] 遅延値
    PtContAssign* elem_top ///< [in] 要素の先頭
  );

  /// @brief continuous assign 文のヘッダの生成 (strengthあり, 遅延なし)
  /// @return 生成された continuous assign 文のヘッダ
  PtItem*
  new_ContAssignH(
    const FileRegion& fr,        ///< [in] ファイル位置の情報
    const AstStrength* strength, ///< [in] 信号強度
    PtContAssign* elem_top       ///< [in] 要素の先頭
  );

  /// @brief continuous assign 文のヘッダの生成 (strength, 遅延付き)
  /// @return 生成された continuous assign 文のヘッダ
  PtItem*
  new_ContAssignH(
    const FileRegion& fr,        ///< [in] ファイル位置の情報
    const AstStrength* strength, ///< [in] 信号強度
    const AstDelay* delay,       ///< [in] 遅延値
    PtContAssign* elem_top       ///< [in] 要素の先頭
  );

  /// @brief continuous assign 文の生成
  /// @return 生成された continuous assign 文
  PtContAssign*
  new_ContAssign(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* lhs,   ///< [in] 左辺式
    const AstExpr* rhs    ///< [in] 右辺式
  );

  /// @brief initial 文の生成
  /// @return 生成された initial 文
  PtItem*
  new_Initial(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstStmt* body   ///< [in] 本体のステートメント
  );

  /// @brief always 文の生成
  /// @return 生成された always 文
  PtItem*
  new_Always(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstStmt* body   ///< [in] 本体のステートメント
  );

  /// @brief task 文の生成
  /// @return 生成された task
  PtItem*
  new_Task(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] task 名
    bool automatic,       ///< [in] automatic task の時に true となるフラグ
    PtIOHead* iodecl_top, ///< [in] IO宣言のリスト
    PtDeclHead* decl_top, ///< [in] 宣言のリスト
    const AstStmt* stmt   ///< [in] 本体のステートメント
  );

  /// @brief 1ビット型 function 文の生成
  /// @return 生成された function
  PtItem*
  new_Function(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,	  ///< [in] task 名
    bool automatic,	  ///< [in] automatic task の時に true となるフラグ
    bool sign,		  ///< [in] signed 属性がついていたら true となるフラグ
    PtIOHead* iodecl_top, ///< [in] IO宣言のリスト
    PtDeclHead* decl_top, ///< [in] 宣言のリスト
    const AstStmt* stmt	  ///< [in] 本体のステートメント
  );

  /// @brief 範囲指定型 function 文の生成
  /// @return 生成された function
  PtItem*
  new_SizedFunc(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    const char* name,	   ///< [in] task 名
    bool automatic,	   ///< [in] automatic task の時に true となるフラグ
    bool sign,		   ///< [in] signed 属性がついていたら true となるフラグ
    const AstRange* range, ///< [in] パース木の範囲定義
    PtIOHead* iodecl_top,  ///< [in] IO宣言のリスト
    PtDeclHead* decl_top,  ///< [in] 宣言のリスト
    const AstStmt* stmt	   ///< [in] 本体のステートメント
  );

  /// @brief 組み込み型 function 文の生成
  /// @return 生成された task 文
  PtItem*
  new_TypedFunc(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,	  ///< [in] task 名
    bool automatic,	  ///< [in] automatic task の時に true となるフラグ
    bool sign,		  ///< [in] signed 属性がついていたら true となるフラグ
    VpiVarType func_type, ///< [in] 関数の戻値の型
    PtIOHead* iodecl_top, ///< [in] IO宣言のリスト
    PtDeclHead* decl_top, ///< [in] 宣言のリスト
    const AstStmt* stmt	  ///< [in] 本体のステートメント
  );

  /// @brief gate instance 文のヘッダの生成 (strengthなし, 遅延なし)
  /// @return 生成された gate instance 文のヘッダ
  PtItem*
  new_GateH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiPrimType type,	  ///< [in] primitive の型
    PtInst* elem_top      ///< [in] 要素の配列
  );

  /// @brief gate instance 文のヘッダの生成 (strengthなし, 遅延付き)
  /// @return 生成された gate instance 文のヘッダ
  PtItem*
  new_GateH(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    VpiPrimType type,	   ///< [in] primitive の型
    const AstDelay* delay, ///< [in] 遅延値
    PtInst* elem_top       ///< [in] 要素の配列
  );

  /// @brief gate instance 文のヘッダの生成 (strengthあり, 遅延なし)
  /// @return 生成された gate instance 文のヘッダ
  PtItem*
  new_GateH(
    const FileRegion& fr,        ///< [in] ファイル位置の情報
    VpiPrimType type,	         ///< [in] primitive の型
    const AstStrength* strength, ///< [in] 信号強度
    PtInst* elem_top             ///< [in] 要素の配列
  );

  /// @brief gate instance 文のヘッダの生成 (strength, 遅延付き)
  /// @return 生成された gate instance 文のヘッダ
  PtItem*
  new_GateH(
    const FileRegion& fr,        ///< [in] ファイル位置の情報
    VpiPrimType type,	         ///< [in] primitive の型
    const AstStrength* strength, ///< [in] 信号強度
    const AstDelay* delay,	 ///< [in] 遅延値
    PtInst* elem_top             ///< [in] 要素の配列
  );

  /// @brief module instance/UDP instance 文のヘッダの生成 (strengthなし, 遅延なし)
  /// @return 生成された module instance/UDP instance 文のヘッダ
  PtItem*
  new_MuH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* def_name, ///< [in] 定義名
    PtInst* elem_top      ///< [in] 要素の配列
  );

  /// @brief module instance/UDP instance 文のヘッダの生成 (strengthなし, 遅延なし)
  /// @return 生成された module instance/UDP instance 文のヘッダ
  PtItem*
  new_MuH(
    const FileRegion& fr,          ///< [in] ファイル位置の情報
    const char* def_name,	   ///< [in] 定義名
    PtConnection* paramassign_top, ///< [in] パラメータ割り当てリスト
    PtInst* elem_top               ///< [in] 要素の配列
  );

  /// @brief module instance/UDP instance 文のヘッダの生成 (strengthなし, 遅延付き)
  /// @return 生成された module instance/UDP instance 文のヘッダ
  PtItem*
  new_MuH(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    const char* def_name,  ///< [in] 定義名
    const AstDelay* delay, ///< [in] 遅延値
    PtInst* elem_top       ///< [in] 要素の配列
  );

  /// @brief module instance/UDP instance 文のヘッダの生成 (strengthあり, 遅延なし)
  /// @return 生成された module instance/UDP instance 文のヘッダ
  PtItem*
  new_MuH(
    const FileRegion& fr,        ///< [in] ファイル位置の情報
    const char* def_name,	 ///< [in] 定義名
    const AstStrength* strength, ///< [in] 信号強度
    PtInst* elem_top             ///< [in] 要素の配列
  );

  /// @brief module instance/UDP instance 文のヘッダの生成 (strength, 遅延付き)
  /// @return 生成された module instance/UDP instance 文のヘッダ
  PtItem*
  new_MuH(
    const FileRegion& fr,        ///< [in] ファイル位置の情報
    const char* def_name,	 ///< [in] 定義名
    const AstStrength* strength, ///< [in] 信号強度
    const AstDelay* delay,       ///< [in] 遅延値
    PtInst* elem_top             ///< [in] 要素の配列
  );

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @return 生成された module instance/UDP instance の要素
  PtInst*
  new_Inst(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    const char* name,	   ///< [in] 名前
    const AstRange* range, ///< [in] パース木の範囲定義
    PtConnection* con_top  ///< [in] ポート割り当ての配列
  );

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @return 生成された module instance/UDP instance の要素
  PtInst*
  new_Inst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PtConnection* con_top ///< [in] ポート割り当ての配列
  );

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @return 生成された module instance/UDP instance の要素
  PtInst*
  new_Inst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,	  ///< [in] 名前
    PtConnection* con_top ///< [in] ポート割り当ての配列
  );

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @return 生成された module instance/UDP instance の要素
  PtInst*
  new_Inst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* expr1  ///< [in] 接続1
  );

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @return 生成された module instance/UDP instance の要素
  PtInst*
  new_Inst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,	  ///< [in] 名前
    const AstExpr* expr1  ///< [in] 接続1
  );

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @return 生成された module instance/UDP instance の要素
  PtInst*
  new_Inst(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    const char* name,	   ///< [in] 名前
    const AstRange* range, ///< [in] パース木の範囲定義
    const AstExpr* expr1   ///< [in] 接続1
  );

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @return 生成された module instance/UDP instance の要素
  PtInst*
  new_Inst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* expr1, ///< [in] 接続1
    const AstExpr* expr2  ///< [in] 接続2
  );

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @return 生成された module instance/UDP instance の要素
  PtInst*
  new_Inst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,	  ///< [in] 名前
    const AstExpr* expr1, ///< [in] 接続1
    const AstExpr* expr2  ///< [in] 接続2
  );

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @return 生成された module instance/UDP instance の要素
  PtInst*
  new_Inst(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    const char* name,	   ///< [in] 名前
    const AstRange* range, ///< [in] パース木の範囲定義
    const AstExpr* expr1,  ///< [in] 接続1
    const AstExpr* expr2   ///< [in] 接続2
  );

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @return 生成された module instance/UDP instance の要素
  PtInst*
  new_Inst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* expr1, ///< [in] 接続1
    const AstExpr* expr2, ///< [in] 接続2
    const AstExpr* expr3  ///< [in] 接続3
  );

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @return 生成された module instance/UDP instance の要素
  PtInst*
  new_Inst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,	  ///< [in] 名前
    const AstExpr* expr1, ///< [in] 接続1
    const AstExpr* expr2, ///< [in] 接続2
    const AstExpr* expr3  ///< [in] 接続3
  );

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @return 生成された module instance/UDP instance の要素
  PtInst*
  new_Inst(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    const char* name,	   ///< [in] 名前
    const AstRange* range, ///< [in] パース木の範囲定義
    const AstExpr* expr1,  ///< [in] 接続1
    const AstExpr* expr2,  ///< [in] 接続2
    const AstExpr* expr3   ///< [in] 接続3
  );

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @return 生成された module instance/UDP instance の要素
  PtInst*
  new_Inst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* expr1, ///< [in] 接続1
    const AstExpr* expr2, ///< [in] 接続2
    const AstExpr* expr3, ///< [in] 接続3
    const AstExpr* expr4  ///< [in] 接続4
  );

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @return 生成された module instance/UDP instance の要素
  PtInst*
  new_Inst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,	  ///< [in] 名前
    const AstExpr* expr1, ///< [in] 接続1
    const AstExpr* expr2, ///< [in] 接続2
    const AstExpr* expr3, ///< [in] 接続3
    const AstExpr* expr4  ///< [in] 接続4
  );

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  /// @return 生成された module instance/UDP instance の要素
  PtInst*
  new_Inst(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    const char* name,	   ///< [in] 名前
    const AstRange* range, ///< [in] パース木の範囲定義
    const AstExpr* expr1,  ///< [in] 接続1
    const AstExpr* expr2,  ///< [in] 接続2
    const AstExpr* expr3,  ///< [in] 接続3
    const AstExpr* expr4   ///< [in] 接続4
  );

  /// @brief generate 文の生成
  /// @return 生成された generate 文
  PtItem*
  new_Generate(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PtDeclHead* decl_top, ///< [in] 宣言の先頭
    PtItem* item_top      ///< [in] 要素の先頭
  );

  /// @brief generate block 文の生成
  /// @return 生成された generate block 文
  PtItem*
  new_GenBlock(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PtDeclHead* decl_top, ///< [in] 宣言の先頭
    PtItem* item_top      ///< [in] 要素の先頭
  );

  /// @brief 名前付き generate block 文の生成
  /// @return 生成された generate block 文
  PtItem*
  new_GenBlock(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    PtDeclHead* decl_top, ///< [in] 宣言の先頭
    PtItem* item_top      ///< [in] 要素の先頭
  );

  /// @brief generate if 文の生成
  /// @return 生成された generate if 文
  PtItem*
  new_GenIf(
    const FileRegion& fr,      ///< [in] ファイル位置の情報
    const AstExpr* cond,       ///< [in] 条件を表す式
    PtDeclHead* then_decl_top, ///< [in] then 節の宣言の先頭
    PtItem* then_item_top      ///< [in] then 節の要素の先頭
  );

  /// @brief generate if 文の生成
  /// @return 生成された generate if 文
  PtItem*
  new_GenIfElse(
    const FileRegion& fr,      ///< [in] ファイル位置の情報
    const AstExpr* cond,       ///< [in] 条件を表す式
    PtDeclHead* then_decl_top, ///< [in] then 節の宣言の先頭
    PtItem* then_item_top,     ///< [in] then 節の要素の先頭
    PtDeclHead* else_decl_top, ///< [in] then 節の宣言の先頭
    PtItem* else_item_top      ///< [in] then 節の要素の先頭
  );

  /// @brief generate case 文の生成
  /// @return 生成された generate case 文
  PtItem*
  new_GenCase(
    const FileRegion& fr,   ///< [in] ファイル位置の情報
    const AstExpr* expr,    ///< [in] 選択式
    PtGenCaseItem* item_top ///< [in] generate case item の先頭
  );

  /// @brief generate case の要素の生成
  /// @return 生成された generate case item
  PtGenCaseItem*
  new_GenCaseItem(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PtExpr* label_top,    ///< [in] 比較式の先頭
    PtDeclHead* decl_top, ///< [in] 宣言の先頭
    PtItem* item_top      ///< [in] 要素の先頭
  );

  /// @brief generate for 文の生成
  /// @return 生成された generate for 文
  PtItem*
  new_GenFor(
    const FileRegion& fr,     ///< [in] ファイル位置の情報
    const char* loop_var,     ///< [in] ループ変数
    const AstExpr* init_expr, ///< [in] 初期化式
    const AstExpr* cond,      ///< [in] ループ条件式
    const AstExpr* next_expr, ///< [in] 増加式
    const char* block_name,   ///< [in] ブロック名
    PtDeclHead* decl_top,     ///< [in] 宣言の先頭
    PtItem* item_top          ///< [in] 要素の先頭
  );

  /// @brief specify block item の生成
  /// @return 生成された specify block item
  PtItem*
  new_SpecItem(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiSpecItemType id,   ///< [in] specify block item の種類
    PtExpr* terminal_top  ///< [in] 端子のリスト
  );

  /// @brief path 仕様を生成する．
  /// @return 生成された spec path
  PtItem*
  new_SpecPath(
    const FileRegion& fr,        ///< [in] ファイル位置の情報
    VpiSpecPathType id,          ///< [in] spec path の種類
    const AstExpr* expr,         ///< [in] 条件式
    const AstPathDecl* path_decl ///< [in] パス記述
  );

  /// @brief パス記述の生成
  /// @return 生成されたパス記述
  PtPathDecl*
  new_PathDecl(
    const FileRegion& fr,         ///< [in] ファイル位置の情報
    int edge,
    PtExpr* input_top,
    int input_pol,
    VpiPathType op,
    PtExpr* output_top,
    int output_pol,
    const AstExpr* expr,
    const AstPathDelay* path_delay
  );

  /// @brief パス記述の生成
  /// @return 生成されたパス記述
  PtPathDecl*
  new_PathDecl(
    const FileRegion& fr,         ///< [in] ファイル位置の情報
    int edge,
    PtExpr* input_top,
    int input_pol,
    VpiPathType op,
    const AstExpr* output,
    int output_pol,
    const AstExpr* expr,
    const AstPathDelay* path_delay
  );

  /// @brief path delay value の生成 (値が1個)
  /// @return 生成された path delay value
  PtPathDelay*
  new_PathDelay(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* value  ///< [in] 値
  );

  /// @brief path delay value の生成 (値が2個)
  /// @return 生成された path delay value
  PtPathDelay*
  new_PathDelay(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    const AstExpr* value1, ///< [in] 値1
    const AstExpr* value2  ///< [in] 値2
  );

  /// @brief path delay value の生成 (値が3個)
  /// @return 生成された path delay value
  PtPathDelay*
  new_PathDelay(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    const AstExpr* value1, ///< [in] 値1
    const AstExpr* value2, ///< [in] 値2
    const AstExpr* value3  ///< [in] 値3
  );

  /// @brief path delay value の生成 (値が6個)
  /// @return 生成された path delay value
  PtPathDelay*
  new_PathDelay(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    const AstExpr* value1, ///< [in] 値1
    const AstExpr* value2, ///< [in] 値2
    const AstExpr* value3, ///< [in] 値3
    const AstExpr* value4, ///< [in] 値4
    const AstExpr* value5, ///< [in] 値5
    const AstExpr* value6  ///< [in] 値6
  );

  /// @brief path delay value の生成 (値が12個)
  /// @return 生成された path delay value
  PtPathDelay*
  new_PathDelay(
    const FileRegion& fr,   ///< [in] ファイル位置の情報
    const AstExpr* value1,  ///< [in] 値1
    const AstExpr* value2,  ///< [in] 値2
    const AstExpr* value3,  ///< [in] 値3
    const AstExpr* value4,  ///< [in] 値4
    const AstExpr* value5,  ///< [in] 値5
    const AstExpr* value6,  ///< [in] 値6
    const AstExpr* value7,  ///< [in] 値7
    const AstExpr* value8,  ///< [in] 値8
    const AstExpr* value9,  ///< [in] 値9
    const AstExpr* value10, ///< [in] 値10
    const AstExpr* value11, ///< [in] 値11
    const AstExpr* value12  ///< [in] 値12
  );


  //////////////////////////////////////////////////////////////////////
  // statement 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief disable 文の生成
  /// @return 生成された disable 文
  PtStmt*
  new_Disable(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name      ///< [in] 停止するスコープ名
  );

  /// @brief disable 文の生成 (階層付き識別子)
  /// @return 生成された disable 文
  PtStmt*
  new_Disable(
    const FileRegion& fr,   ///< [in] ファイル位置の情報
    const PtHierName& hname ///< [in] 階層名
  );

  /// @brief enable 文の生成
  /// @return 生成された enable 文
  PtStmt*
  new_Enable(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 起動するタスク名
    PtExpr* arg_top       ///< [in] 引数のリスト
  );

  /// @brief enable 文の生成 (階層付き識別子)
  /// @return 生成された enable 文
  PtStmt*
  new_Enable(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const PtHierName& hname, ///< [in] 階層名
    PtExpr* arg_top          ///< [in] 引数のリスト
  );

  /// @brief system task enable 文の生成
  /// @return 生成された system task enable 文
  PtStmt*
  new_SysEnable(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,	  ///< [in] 起動するシステムタスク名
    PtExpr* arg_top       ///< [in] 引数のリスト
  );

  /// @brief delay control 文の生成
  /// @return 生成された delay control 文
  PtStmt*
  new_DcStmt(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const AstControl* delay, ///< [in] delay control (delay control 文とは別物)
    const AstStmt* body      ///< [in] 本体のステートメント
  );

  /// @brief event control 文の生成
  /// @return 生成された event control 文
  PtStmt*
  new_EcStmt(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const AstControl* event, ///< [in] event control (event control 文とは別物)
    const AstStmt* body      ///< [in] 本体のステートメント
  );

  /// @brief wait 文の生成
  /// @return 生成された wait 文
  PtStmt*
  new_Wait(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* cond,  ///< [in] 条件式
    const AstStmt* body   ///< [in] 本体のステートメント
  );

  /// @brief assign 文の生成
  /// @return 生成された assign 文
  PtStmt*
  new_Assign(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* lhs,   ///< [in] 左辺式
    const AstExpr* rhs    ///< [in] 右辺式
  );

  /// @brief control 付き assign 文の生成
  /// @return 生成された assign 文
  PtStmt*
  new_Assign(
    const FileRegion& fr,     ///< [in] ファイル位置の情報
    const AstExpr* lhs,       ///< [in] 左辺式
    const AstExpr* rhs,       ///< [in] 右辺式
    const AstControl* control ///< [in] 制御式
  );

  /// @brief nonblocking assign 文の生成
  /// @return 生成された nonblocking assign 文
  PtStmt*
  new_NbAssign(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* lhs,	  ///< [in] 左辺式
    const AstExpr* rhs	  ///< [in] 右辺式
  );

  /// @brief control 付き nonblocking assign 文の生成
  /// @return 生成された nonblocking assign 文
  PtStmt*
  new_NbAssign(
    const FileRegion& fr,     ///< [in] ファイル位置の情報
    const AstExpr* lhs,	      ///< [in] 左辺式
    const AstExpr* rhs,	      ///< [in] 右辺式
    const AstControl* control ///< [in] 制御式
  );

  /// @brief event 文の生成
  /// @return 生成された event 文
  PtStmt*
  new_EventStmt(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* event  ///< [in] イベント
  );

  /// @brief null 文の生成
  /// @return 生成された null 文
  PtStmt*
  new_NullStmt(
    const FileRegion& fr ///< [in] ファイル位置の情報
  );

  /// @brief if 文の生成
  /// @return 生成された if 文
  PtStmt*
  new_If(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const AstExpr* expr,     ///< [in] 条件式
    const AstStmt* then_body ///< [in] 成り立ったときに実行されるステートメント
  );

  /// @brief if-else 文の生成
  /// @return 生成された if 文
  PtStmt*
  new_IfElse(
    const FileRegion& fr,     ///< [in] ファイル位置の情報
    const AstExpr* expr,      ///< [in] 条件式
    const AstStmt* then_body, ///< [in] 成り立ったときに実行されるステートメント
    const AstStmt* else_body  ///< [in] 成り立たなかったときに実行されるステートメント
  );

  /// @brief case 文の生成
  /// @return 生成された case 文
  PtStmt*
  new_Case(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const AstExpr* expr,     ///< [in] 条件式
    PtCaseItem* caseitem_top ///< [in] case item の先頭
  );

  /// @brief casex 文の生成
  /// @return 生成された case 文
  PtStmt*
  new_CaseX(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const AstExpr* expr,     ///< [in] 条件式
    PtCaseItem* caseitem_top ///< [in] case item の先頭
  );

  /// @brief casez 文の生成
  /// @return 生成された case 文
  PtStmt*
  new_CaseZ(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const AstExpr* expr,     ///< [in] 条件式
    PtCaseItem* caseitem_top ///< [in] case item の先頭
  );

  /// @brief case item の生成
  /// @return 生成された case item
  PtCaseItem*
  new_CaseItem(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PtExpr* label_top,    ///< [in] ラベルの先頭
    const AstStmt* body   ///< [in] 本体のステートメント
  );

  /// @brief forever 文の生成
  /// @return 生成された forever 文
  PtStmt*
  new_Forever(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstStmt* body   ///< [in] 本体のステートメント
  );

  /// @brief repeat 文の生成
  /// @return 生成された repeat 文
  PtStmt*
  new_Repeat(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* expr,  ///< [in] 繰り返し数を表す式
    const AstStmt* body   ///< [in] 本体のステートメント
  );

  /// @brief while 文の生成
  /// @return 生成された while 文
  PtStmt*
  new_While(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* cond,  ///< [in] 繰り返し条件式
    const AstStmt* body   ///< [in] 本体のステートメント
  );

  /// @brief for 文の生成
  /// @return 生成された for 文
  PtStmt*
  new_For(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstStmt* init,  ///< [in] 初期化文
    const AstExpr* cond,  ///< [in] 繰り返し条件式
    const AstStmt* next,  ///< [in] 増加文
    const AstStmt* body   ///< [in] 本体のステートメント
  );

  /// @brief procedural assign 文の生成
  /// @return 生成された procedural assign 文
  PtStmt*
  new_PcAssign(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* lhs,   ///< [in] 左辺式
    const AstExpr* rhs    ///< [in] 右辺式
  );

  /// @brief deassign 文の生成
  /// @return 生成された deassign 文
  PtStmt*
  new_Deassign(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* lhs    ///< [in] 左辺式
  );

  /// @brief force 文の生成
  /// @return 生成された force 文
  PtStmt*
  new_Force(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* lhs,   ///< [in] 左辺式
    const AstExpr* rhs    ///< [in] 右辺式
  );

  /// @brief release 文の生成
  /// @return 生成された release 文
  PtStmt*
  new_Release(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* lhs    ///< [in] 左辺式
  );

  /// @brief parallel block の生成
  /// @return 生成された parallel block
  PtStmt*
  new_ParBlock(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PtStmt* stmt_top      ///< [in] 本体のステートメントのリスト
  );

  /// @brief 名前付き parallel block の生成
  /// @return 生成された parallel block
  PtStmt*
  new_NamedParBlock(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] ブロック名
    PtDeclHead* decl_top, ///< [in] 宣言のリスト
    PtStmt* stmt_top      ///< [in] 本体のステートメントのリスト
  );

  /// @brief sequential block の生成
  /// @return 生成された sequential block
  PtStmt*
  new_SeqBlock(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PtStmt* stmt_top      ///< [in] 本体のステートメントのリスト
  );

  /// @brief 名前付き sequential block の生成
  /// @return 生成された sequential block
  PtStmt*
  new_NamedSeqBlock(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] ブロック名
    PtDeclHead* decl_top, ///< [in] 宣言のリスト
    PtStmt* stmt_top      ///< [in] 本体のステートメントのリスト
  );


  //////////////////////////////////////////////////////////////////////
  // expression 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief 単項演算子の生成
  /// @return 生成された演算子
  PtExpr*
  new_Opr(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiOpType type,       ///< [in] 演算の種類
    const AstExpr* opr    ///< [in] オペランド
  );

  /// @brief 二項演算子の生成
  /// @return 生成された演算子
  PtExpr*
  new_Opr(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiOpType type,	  ///< [in] 演算の種類
    const AstExpr* opr1,  ///< [in] オペランド1
    const AstExpr* opr2	  ///< [in] オペランド2
  );

  /// @brief 三項演算子の生成
  /// @return 生成された演算子
  PtExpr*
  new_Opr(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiOpType type,	  ///< [in] 演算の種類
    const AstExpr* opr1,  ///< [in] オペランド1
    const AstExpr* opr2,  ///< [in] オペランド2
    const AstExpr* opr3   ///< [in] オペランド3
  );

  /// @brief concatination 演算子の生成
  /// @return 生成された concatination 演算子
  PtExpr*
  new_Concat(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PtExpr* expr_top      ///< [in] オペランドの先頭
  );

  /// @brief multi-concatination 演算子の生成
  /// @return 生成された multi-concatination 演算子
  PtExpr*
  new_MultiConcat(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* rep,   ///< [in] 繰り返し数
    PtExpr* expr_top      ///< [in] オペランドの先頭
  );

  /// @brief min/typ/max delay 演算子の生成
  /// @return 生成された min/typ/max 演算子
  PtExpr*
  new_MinTypMax(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* val0,  ///< [in] minimum 値
    const AstExpr* val1,  ///< [in] typical 値
    const AstExpr* val2   ///< [in] maximum 値
  );

  /// @brief primary の生成
  /// @return 生成された primary
  PtExpr*
  new_Primary(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name      ///< [in] 識別子名
  );

  /// @brief インデックス付き primary の生成
  /// @return 生成された primary
  PtExpr*
  new_Primary(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 識別子名
    PtExpr* index_top     ///< [in] インデックスの先頭
  );

  /// @brief 範囲指定付き primary の生成
  /// @return 生成された primary
  PtExpr*
  new_Primary(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 識別子名
    const AstPart* part   ///< [in] 範囲指定
  );

  /// @brief インデックスと範囲指定付き primary の生成
  /// @return 生成された primary
  PtExpr*
  new_Primary(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 識別子名
    PtExpr* index_top,    ///< [in] インデックスの先頭
    const AstPart* part   ///< [in] 範囲指定
  );

  /// @brief primary の生成 (階層付き)
  /// @return 生成された primary
  PtExpr*
  new_Primary(
    const FileRegion& fr,   ///< [in] ファイル位置の情報
    const PtHierName& hname ///< [in] 階層名
  );

  /// @brief インデックス付き primary の生成 (階層付き)
  /// @return 生成された primary
  PtExpr*
  new_Primary(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const PtHierName& hname, ///< [in] 階層名
    PtExpr* index_top        ///< [in] インデックスの先頭
  );

  /// @brief 範囲指定付き primary の生成 (階層付き)
  /// @return 生成された primary
  PtExpr*
  new_Primary(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const PtHierName& hname, ///< [in] 階層名
    const AstPart* part      ///< [in] 範囲指定
  );

  /// @brief インデックスと範囲指定付き primary の生成 (階層付き)
  /// @return 生成された primary
  PtExpr*
  new_Primary(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const PtHierName& hname, ///< [in] 階層名
    PtExpr* index_top,       ///< [in] インデックスの先頭
    const AstPart* part      ///< [in] 範囲指定
  );

  /// @brief constant primary の生成
  /// @return 生成された const primary
  PtExpr*
  new_CPrimary(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 識別子名
    PtExpr* index_top     ///< [in] インデックスの先頭
  );

  /// @brief 範囲指定付き constant primary の生成
  /// @return 生成された constant primary
  PtExpr*
  new_CPrimary(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,	  ///< [in] 識別子名
    const AstPart* part   ///< [in] 範囲指定
  );

  /// @brief インデックス付き constant primary の生成 (階層付き)
  /// @return 生成された constant primary
  PtExpr*
  new_CPrimary(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const PtHierName& hname, ///< [in] 階層名
    PtExpr* index_top        ///< [in] インデックスの先頭
  );

  /// @brief function call の生成
  /// @return 生成された function call
  PtExpr*
  new_FuncCall(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 関数名
    PtExpr* arg_top       ///< [in] 引数の先頭
  );

  /// @brief function call の生成 (階層付き)
  /// @return 生成された function call
  PtExpr*
  new_FuncCall(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const PtHierName& hname, ///< [in] 階層名
    PtExpr* arg_top          ///< [in] 引数の先頭
  );

  /// @brief system function call の生成
  /// @return 生成された function call
  PtExpr*
  new_SysFuncCall(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name      ///< [in] 関数名
  );

  /// @brief system function call の生成
  /// @return 生成された function call
  PtExpr*
  new_SysFuncCall(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const char* name,        ///< [in] 関数名
    PtExpr* arg_top          ///< [in] 引数の先頭
  );

  /// @brief 整数型の定数の生成
  /// @return 生成された定数
  PtExpr*
  new_IntConst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    SizeType value        ///< [in] 定数を表す整数値
  );

  /// @brief 整数型の定数の生成
  /// @return 生成された定数
  PtExpr*
  new_IntConst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* value     ///< [in] 定数の文字列表現
  );

  /// @brief 基底付き整数型の定数の生成
  /// @return 生成された定数
  PtExpr*
  new_IntConst(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    VpiConstType const_type, ///< [in] 定数の種類
    const char* value        ///< [in] 定数の文字列表現
  );

  /// @brief サイズと基底付き定数の生成
  /// @return 生成された定数
  PtExpr*
  new_IntConst(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    SizeType size,           ///< [in] サイズ
    VpiConstType const_type, ///< [in] 定数の種類
    const char* value        ///< [in] 定数の文字列表現
  );

  /// @brief 実数型の定数の生成
  /// @return 生成された定数
  PtExpr*
  new_RealConst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    double value          ///< [in] 定数の文字列表現
  );

  /// @brief 文字列型の定数の生成
  /// @return 生成された定数
  PtExpr*
  new_StringConst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* value     ///< [in] 文字列
  );


  //////////////////////////////////////////////////////////////////////
  // その他
  //////////////////////////////////////////////////////////////////////

  /// @brief ディレイコントロールの生成
  /// @return 生成されたディレイコントロール
  PtControl*
  new_DelayControl(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* value  ///< [in] 遅延を表す式
  );

  /// @brief イベントコントロールの生成
  /// @return 生成されたイベントコントロール
  PtControl*
  new_EventControl(
    const FileRegion& fr  ///< [in] ファイル位置の情報
  );

  /// @brief イベントコントロールの生成
  /// @return 生成されたイベントコントロール
  PtControl*
  new_EventControl(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PtExpr* event_top     ///< [in] イベントの先頭
  );

  /// @brief リピートコントロールの生成
  /// @return 生成されたリピートコントロール
  PtControl*
  new_RepeatControl(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* rep    ///< [in] 繰り返し数を表す式
  );

  /// @brief リピートコントロールの生成
  /// @return 生成されたリピートコントロール
  PtControl*
  new_RepeatControl(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* rep,   ///< [in] 繰り返し数を表す式
    PtExpr* event_top     ///< [in] 繰り返しの単位となるイベントの先頭
  );

  /// @brief 順序つき結合子の生成
  /// @return 生成された結合子
  PtConnection*
  new_OrderedCon();

  /// @brief 順序つき結合子の生成
  /// @return 生成された結合子
  PtConnection*
  new_OrderedCon(
    const AstExpr* expr ///< [in] 結合する式
  );

  /// @brief 順序つき結合子の生成
  /// @return 生成された結合子
  PtConnection*
  new_OrderedCon(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* expr   ///< [in] 結合する式
  );

  /// @brief 名前付き結合子の生成
  /// @return 生成された結合子
  PtConnection*
  new_NamedCon(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name      ///< [in] 名前
  );

  /// @brief 名前付き結合子の生成
  /// @return 生成された結合子
  PtConnection*
  new_NamedCon(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    const AstExpr* expr   ///< [in] 結合する式
  );

  /// @brief strength の生成
  /// @return 生成された strength
  PtStrength*
  new_Strength(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiStrength value0,   ///< [in] '0' の強度
    VpiStrength value1    ///< [in] '1' の強度
  );

  /// @brief charge strength の生成
  /// @return 生成された strength
  PtStrength*
  new_Strength(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiStrength value     ///< [in] 強度
  );

  /// @brief 遅延値の生成 (1つの値)
  /// @return 生成された遅延値
  PtDelay*
  new_Delay(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* value1 ///< [in] 値1
  );

  /// @brief 遅延値の生成 (2つの値)
  /// @return 生成された遅延値
  PtDelay*
  new_Delay(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    const AstExpr* value1, ///< [in] 値1
    const AstExpr* value2  ///< [in] 値2
  );

  /// @brief 遅延値の生成 (3つの値)
  /// @return 生成された遅延値
  PtDelay*
  new_Delay(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    const AstExpr* value1, ///< [in] 値1
    const AstExpr* value2, ///< [in] 値2
    const AstExpr* value3  ///< [in] 値3
  );

  /// @brief 階層名の生成
  /// @return 生成された階層名
  PtNameBranch*
  new_NameBranch(
    const char* name ///< [in] 名前
  );

  /// @brief インデックス付きの階層名の生成
  /// @return 生成された階層名
  PtNameBranch*
  new_NameBranch(
    const char* name, ///< [in] 名前
    int index         ///< [in] インデックス
  );

  /// @brief 範囲指定の生成
  PtPart*
  new_PartConst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* expr1, ///< [in] 1番目の式
    const AstExpr* expr2  ///< [in] 2番目の式
  );

  /// @brief 範囲指定の生成
  PtPart*
  new_PartPlus(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* expr1, ///< [in] 1番目の式
    const AstExpr* expr2  ///< [in] 2番目の式
  );

  /// @brief 範囲指定の生成
  PtPart*
  new_PartMinus(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* expr1, ///< [in] 1番目の式
    const AstExpr* expr2  ///< [in] 2番目の式
  );


  //////////////////////////////////////////////////////////////////////
  // attribute instance 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief attribute instance の生成
  /// @return 生成された attribute instance
  PtAttrInst*
  new_AttrInst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PtAttrSpec* as_top    ///< [in] attribute spec の先頭
  );

  /// @brief attribute spec の生成
  /// @return 生成された attribute spec
  PtAttrSpec*
  new_AttrSpec(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name      ///< [in] 名前
  );

  /// @brief attribute spec の生成
  /// @return 生成された attribute spec
  PtAttrSpec*
  new_AttrSpec(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    const AstExpr* expr   ///< [in] 値
  );


public:
  //////////////////////////////////////////////////////////////////////
  // おもにデバッグ用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 確保したオブジェクトに関する情報を出力する．
  void
  dump_profile(
    std::ostream& s ///< [in] 出力先のストリーム
  ) const;


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  Alloc& mAlloc;

};

END_NAMESPACE_YM_VERILOG

#endif // PTFACTORY_H
