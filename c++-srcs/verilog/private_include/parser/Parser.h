#ifndef PARSER_PARSER_H
#define PARSER_PARSER_H

/// @file Parser.h
/// @brief Parser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/Ast.h"
#include "ym/PathList.h"
#include "parser/PtFactory.h"
#include "parser/PtDecl.h"
#include "parser/PtExpr.h"
#include "parser/PtItem.h"
#include "parser/PtStmt.h"
#include "parser/PtMisc.h"
#include "parser/PtList.h"
#include "parser/PtHierName.h"
#include "parser/AstMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

class VlLineWatcher;
class Lex;

union YYSTYPE;

//////////////////////////////////////////////////////////////////////
/// @class Parser Parser.h "parser/Parser.h"
/// @ingroup Parser
/// @brief Verilog-HDL のパーサークラス
///
/// 役割は3つある．
/// - Yacc/Bison を駆動する外部インターフェイス
/// - Yacc/Bison のアクションに対応した処理を行う API の提供
/// - 構文木用の生成を行う PtFactory のプロキシ
//////////////////////////////////////////////////////////////////////
class Parser
{
public:

  /// @brief コンストラクタ
  Parser(
    AstMgr& astmgr ///< [in] 読んだ結果のパース木を登録するマネージャ
  );

  /// @brief デストラクタ
  ~Parser();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルを読み込む．
  /// @retval true 正常に終了した．
  /// @retval false エラーが起こった．
  bool
  read_file(
    const std::string& filename,                    ///< [in] 読み込むファイル名
    const PathList& searchpath,                     ///< [in] サーチパス
    const std::vector<VlLineWatcher*>& watcher_list ///< [in] 行番号ウオッチャーのリスト
  );


public:
  //////////////////////////////////////////////////////////////////////
  // UDP 関連の生成関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Verilog1995 タイプのUDP を生成する．
  void
  new_Udp1995(
    const FileRegion& file_region, ///< [in] ファイル上の位置
    const char* name,              ///< [in] 名前
    const char* init_name,         ///< [in] 初期値の名前
    const FileRegion& init_loc,    ///< [in] 初期値の位置
    const AstExpr* init_value,      ///< [in] 初期値のパース木
    PtAttrInstList* ai_list        ///< [in] 属性リスト
  );

  /// @brief Verilog2001 タイプのUDP を生成する．
  void
  new_Udp2001(
    const FileRegion& file_region, ///< [in] ファイル上の位置
    const char* name,		   ///< [in] 名前
    const char* init_name,	   ///< [in] 初期値の名前
    const FileRegion& init_loc,	   ///< [in] 初期値の位置
    const AstExpr* init_value,	   ///< [in] 初期値のパース木
    PtAttrInstList* ai_list        ///< [in] 属性リスト
  );

  /// @brief combinational UDP 用のテーブルエントリの生成
  ///
  /// - 現在の mUdpValueList を用いる．
  /// - 結果は mUdpEntryList に追加される．
  void
  new_UdpEntry(
    const FileRegion& fr,         ///< [in] ファイル位置の情報
    const FileRegion& output_loc, ///< [in] 出力記号の位置
    char output_symbol            ///< [in] 出力記号
  );

  /// @brief sequential UDP 用のテーブルエントリの生成
  ///
  /// - 現在の mUdpValueList を用いる．
  /// - 結果は mUdpEntryList に追加される．
  void
  new_UdpEntry(
    const FileRegion& fr,          ///< [in] ファイル位置の情報
    const FileRegion& current_loc, ///< [in] 現状態記号の位置
    char current_symbol,           ///< [in] 現状態記号
    const FileRegion& output_loc,  ///< [in] 出力記号の位置
    char output_symbol             ///< [in] 出力記号
  );

  /// @brief UdpValue のリストを初期化する．
  void
  init_udp_value_list()
  {
    mUdpValueList.clear();
  }

  /// @brief UDP のテーブルエントリの要素の値の生成
  ///
  /// 結果は mUdpValueList に追加される．
  void
  new_UdpValue(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    char symbol           ///< [in] シンボル
  );

  /// @brief UDP のテーブルエントリの要素の値の生成
  ///
  /// 結果は mUdpValueList に追加される．
  void
  new_UdpValue(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    char symbol1,         ///< [in] 1番目のシンボル
    char symbol2          ///< [in] 2番目のシンボル
  );


private:

  /// @brief new_Udp の下請け関数
  void
  new_Udp(
    const FileRegion& file_region,
    const char* udp_name,
    const char* init_name,
    const FileRegion& init_loc,
    const AstExpr* init_value,
    PtAttrInstList* ai_list,
    bool is_seq,
    const AstIOItem* out_item,
    const std::vector<PtPort*>& port_list,
    const std::vector<PtIOHead*>& iohead_list
  );


public:
  //////////////////////////////////////////////////////////////////////
  // モジュール関連の要素の生成関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Verilog1995 タイプのモジュール(のテンプレート)を生成する．
  void
  new_Module1995(
    const FileRegion& file_region,
    bool is_macro,
    const char* name,
    PtAttrInstList* ai_list
  );

  /// @brief Verilog2001 タイプのモジュール(のテンプレート)を生成する．
  void
  new_Module2001(
    const FileRegion& file_region,
    bool is_macro,
    const char* name,
    PtAttrInstList* ai_list
  );


public:
  //////////////////////////////////////////////////////////////////////
  // ポート関連の要素の生成関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力宣言からポートを作る．
  std::vector<PtPort*>
  new_PortArray(
    const std::vector<PtIOHead*>& iohead_list
  );

  /// @brief 空のポートの生成
  ///
  /// 結果は mPortList に追加される．
  void
  new_Port();

  /// @brief ポートの生成 (内側の式のみ指定するタイプ)
  ///
  /// - 内側の式は mPortRefList を用いる．
  /// - 結果は mPortList に追加される．
  void
  new_Port1(
    const FileRegion& file_region ///< [in] file_region ファイル位置
  );

  /// @brief ポートの生成 (外側の名前のみ指定するタイプ)
  ///
  /// 結果は mPortList に追加される．
  void
  new_Port2(
    const FileRegion& file_region, ///< [in] ファイル位置
    const char* name               ///< [in] 外側の名前
  );

  /// @brief ポートの生成 (外側の名前と内側の式を指定するタイプ)
  ///
  /// - 内側の式は mPortRefList を用いる．
  /// - 結果は mPortList に追加される．
  void
  new_Port3(
    const FileRegion& file_region, ///< [in] ファイル位置
    const char* name               ///< [in] 外側の名前
  );

  /// @brief ポート参照リストを初期化する．
  void
  init_portref_list();

  /// @brief ポート参照式の生成
  ///
  /// 結果は mPortRefList に追加される．
  void
  new_PortRef(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name      ///< [in] ポートに接続している内部の識別子名
  );

  /// @brief ビット指定つきポート参照式の生成
  ///
  /// 結果は mPortRefList に追加される．
  void
  new_PortRef(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] ポートに接続している内部の識別子名
    const AstExpr* index   ///< [in] ビット指定用の式
  );

  /// @brief 範囲指定付きポート参照式の生成
  ///
  /// 結果は mPortRefList に追加される．
  void
  new_PortRef(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] ポートに接続している内部の識別子名
    const AstPart* part    ///< [in] 範囲指定
  );


public:
  //////////////////////////////////////////////////////////////////////
  // IO 宣言関係の要素の生成関数
  //////////////////////////////////////////////////////////////////////

  /// @brief IO 宣言のヘッダの生成
  PtIOHead*
  new_IOHead(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiDir dir,           ///< [in] IO の種類(方向)
    bool sign             ///< [in] 符号付きのとき true となるフラグ
  )
  {
    return mFactory.new_IOHead(fr, dir, sign, nullptr);
  }

  /// @brief 範囲付きの IO 宣言のヘッダの生成
  PtIOHead*
  new_IOHead(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiDir dir,           ///< [in] IO の種類(方向)
    bool sign,            ///< [in] 符号付きのとき true となるフラグ
    const AstRange* range  ///< [in] 範囲
  )
  {
    return mFactory.new_IOHead(fr, dir, sign, range);
  }

  /// @brief IO 宣言のヘッダの生成 (reg 型)
  PtIOHead*
  new_RegIOHead(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiDir dir,           ///< [in] IO の種類(方向)
    bool sign             ///< [in] 符号付きのとき true となるフラグ
  )
  {
    return mFactory.new_RegIOHead(fr, dir, sign, nullptr);
  }

  /// @brief 範囲付きの IO 宣言のヘッダの生成 (reg 型)
  PtIOHead*
  new_RegIOHead(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiDir dir,           ///< [in] IO の種類(方向)
    bool sign,            ///< [in] 符号付きのとき true となるフラグ
    const AstRange* range  ///< [in] 範囲
  )
  {
    return mFactory.new_RegIOHead(fr, dir, sign, range);
  }

  /// @brief IO 宣言のヘッダの生成 (ネット型)
  PtIOHead*
  new_NetIOHead(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiDir dir,           ///< [in] IO の種類(方向)
    VpiNetType net_type,  ///< [in] 補助的なネット型
    bool sign
  )
  {
    return mFactory.new_NetIOHead(fr, dir, net_type, sign, nullptr);
  }

  /// @brief 範囲付きの IO 宣言のヘッダの生成 (ネット型)
  PtIOHead*
  new_NetIOHead(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiDir dir,           ///< [in] IO の種類(方向)
    VpiNetType net_type,  ///< [in] 補助的なネット型
    bool sign,            ///< [in] 符号付きのとき true となるフラグ
    const AstRange* range  ///< [in] 範囲
  )
  {
    return mFactory.new_NetIOHead(fr, dir, net_type, sign, range);
  }

  /// @brief IO 宣言のヘッダの生成 (変数型)
  PtIOHead*
  new_VarIOHead(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiDir dir,           ///< [in] IO の種類(方向)
    VpiVarType var_type   ///< [in] 補助的な変数型
  )
  {
    return mFactory.new_VarIOHead(fr, dir, var_type);
  }

  /// @brief IO 宣言の要素の生成
  ///
  /// 結果は mIOItemList に追加される．
  void
  new_IOItem(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name      ///< [in] 要素名
  )
  {
    auto item = mFactory.new_IOItem(fr, name, nullptr);
    mIOItemList.push_back(item);
  }


  /// @brief 初期値付き IO 宣言の要素の生成
  ///
  /// 結果は mIOItemList に追加される．
  void
  new_IOItem(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const char* name,        ///< [in] 要素名
    const AstExpr* init_value ///< [in] 初期値を表す式
  )
  {
    auto item = mFactory.new_IOItem(fr, name, init_value);
    mIOItemList.push_back(item);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 宣言関連の要素の生成
  //////////////////////////////////////////////////////////////////////

  /// @brief パラメータ宣言のヘッダの生成 (型指定なし)
  PtDeclHead*
  new_ParamH(
    const FileRegion& fr ///< [in] ファイル位置の情報
  )
  {
    return mFactory.new_ParamH(fr, false, nullptr);
  }

  /// @brief 範囲指定型パラメータ宣言のヘッダの生成
  PtDeclHead*
  new_ParamH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    bool sign,            ///< [in] 符号付きのとき true となるフラグ
    const AstRange* range  ///< [in] 範囲
  )
  {
    return mFactory.new_ParamH(fr, sign, range);
  }

  /// @brief 組み込み型パラメータ宣言のヘッダの生成
  PtDeclHead*
  new_ParamH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiVarType var_type   ///< [in] データ型
  )
  {
    return mFactory.new_ParamH(fr, var_type);
  }

  /// @brief local param 宣言のヘッダの生成 (型指定なし)
  PtDeclHead*
  new_LocalParamH(
    const FileRegion& fr ///< [in] ファイル位置の情報
  )
  {
    return mFactory.new_ParamH(fr, false, nullptr, true);
  }

  /// @brief 範囲指定型 local param 宣言のヘッダの生成
  PtDeclHead*
  new_LocalParamH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    bool sign,            ///< [in] 符号付きのとき true となるフラグ
    const AstRange* range  ///< [in] 範囲
  )
  {
    return mFactory.new_ParamH(fr, sign, range, true);
  }

  /// @brief 組み込み型パラメータ宣言のヘッダの生成
  PtDeclHead*
  new_LocalParamH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiVarType var_type   ///< [in] データ型
  )
  {
    return mFactory.new_ParamH(fr, var_type, true);
  }

  /// @brief specparam 宣言のヘッダの生成
  /// @return 生成された specparam
  PtDeclHead*
  new_SpecParamH(
    const FileRegion& fr ///< [in] ファイル位置の情報
  )
  {
    return mFactory.new_SpecParamH(fr, nullptr);
  }

  /// @brief 範囲指定型 specparam 宣言のヘッダの生成
  PtDeclHead*
  new_SpecParamH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstRange* range  ///< [in] 範囲
  )
  {
    return mFactory.new_SpecParamH(fr, range);
  }

  /// @brief イベント宣言のヘッダの生成
  /// @return 生成されたイベント
  PtDeclHead*
  new_EventH(
    const FileRegion& fr ///< [in] ファイル位置の情報
  )
  {
    return mFactory.new_EventH(fr);
  }

  /// @brief genvar 宣言のヘッダの生成
  PtDeclHead*
  new_GenvarH(
    const FileRegion& fr ///< [in] ファイル位置の情報
  )
  {
    return mFactory.new_GenvarH(fr);
  }

  /// @brief 変数宣言のヘッダの生成
  PtDeclHead*
  new_VarH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiVarType var_type   ///< [in] データ型
  )
  {
    return mFactory.new_VarH(fr, var_type);
  }

  /// @brief 1ビット型 reg 宣言のヘッダの生成
  PtDeclHead*
  new_RegH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    bool sign             ///< [in] 符号付きの時 true となるフラグ
  )
  {
    return mFactory.new_RegH(fr, sign, nullptr);
  }

  /// @brief 範囲指定型 reg 宣言のヘッダの生成
  PtDeclHead*
  new_RegH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    bool sign,            ///< [in] 符号付きの時 true となるフラグ
    const AstRange* range  ///< [in] 範囲
  )
  {
    return mFactory.new_RegH(fr, sign, range);
  }

  /// @brief 1ビット型 net 宣言のヘッダの生成
  PtDeclHead*
  new_NetH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiNetType type,      ///< [in] net の型
    bool sign             ///< [in] 符号の有無を表すフラグ
  )
  {
    return mFactory.new_NetH(fr, type, VpiVsType::None,
			     sign, nullptr,
			     nullptr, nullptr);
  }

  /// @brief 1ビット型 net 宣言のヘッダの生成 (strength あり)
  PtDeclHead*
  new_NetH(
    const FileRegion& fr,      ///< [in] ファイル位置の情報
    VpiNetType type,           ///< [in] net の型
    bool sign,                 ///< [in] 符号の有無を表すフラグ
    const AstStrength* strength ///< [in] 信号強度
  )
  {
    return mFactory.new_NetH(fr, type, VpiVsType::None,
			     sign, nullptr,
			     strength, nullptr);
  }

  /// @brief 1ビット型 net 宣言のヘッダの生成 (遅延あり)
  PtDeclHead*
  new_NetH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiNetType type,      ///< [in] net の型
    bool sign,            ///< [in] 符号の有無を表すフラグ
    const AstDelay* delay  ///< [in] 遅延
  )
  {
    return mFactory.new_NetH(fr, type, VpiVsType::None,
			     sign, nullptr,
			     nullptr, delay);
  }

  /// @brief 1ビット型 net 宣言のヘッダの生成 (strength, 遅延あり)
  PtDeclHead*
  new_NetH(
    const FileRegion& fr,       ///< [in] ファイル位置の情報
    VpiNetType type,            ///< [in] net の型
    bool sign,                  ///< [in] 符号の有無を表すフラグ
    const AstStrength* strength, ///< [in] 信号強度
    const AstDelay* delay        ///< [in] 遅延
  )
  {
    return mFactory.new_NetH(fr, type, VpiVsType::None,
			     sign, nullptr,
			     strength, delay);
  }

  /// @brief 範囲指定型 net 宣言のヘッダの生成
  PtDeclHead*
  new_NetH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiNetType type,      ///< [in] net の型
    VpiVsType vstype,     ///< [in] vector/scalar 指定
    bool sign,            ///< [in] 符号の有無を表すフラグ
    const AstRange* range  ///< [in] 範囲
  )
  {
    return mFactory.new_NetH(fr, type, vstype,
			     sign, range,
			     nullptr, nullptr);
  }

  /// @brief 範囲指定型 net 宣言のヘッダの生成 (strengthあり)
  PtDeclHead*
  new_NetH(
    const FileRegion& fr,       ///< [in] ファイル位置の情報
    VpiNetType type,            ///< [in] net の型
    VpiVsType vstype,           ///< [in] vector/scalar 指定
    bool sign,                  ///< [in] 符号の有無を表すフラグ
    const AstRange* range,       ///< [in] 範囲
    const AstStrength* strength  ///< [in] 信号強度
  )
  {
    return mFactory.new_NetH(fr, type, vstype,
			     sign, range,
			     strength, nullptr);
  }

  /// @brief 範囲指定型 net 宣言のヘッダの生成 (遅延あり)
  PtDeclHead*
  new_NetH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiNetType type,	  ///< [in] net の型
    VpiVsType vstype,	  ///< [in] vector/scalar 指定
    bool sign,		  ///< [in] 符号の有無を表すフラグ
    const AstRange* range, ///< [in] 範囲
    const AstDelay* delay  ///< [in] 遅延
  )
  {
    return mFactory.new_NetH(fr, type, vstype,
			     sign, range,
			     nullptr, delay);
  }

  /// @brief 範囲指定型 net 宣言のヘッダの生成 (strength, 遅延あり)
  PtDeclHead*
  new_NetH(
    const FileRegion& fr,       ///< [in] ファイル位置の情報
    VpiNetType type,	        ///< [in] net の型
    VpiVsType vstype,	        ///< [in] vector/scalar 指定
    bool sign,		        ///< [in] 符号の有無を表すフラグ
    const AstRange* range,       ///< [in] 範囲
    const AstStrength* strength, ///< [in] 信号強度
    const AstDelay* delay        ///< [in] 遅延
  )
  {
    return mFactory.new_NetH(fr, type, vstype,
			     sign, range,
			     strength, delay);
  }

  /// @brief 宣言要素の生成
  ///
  /// 結果は mDeclItemList に追加される．
  void
  new_DeclItem(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name      ///< [in] 名前
  )
  {
    auto item = mFactory.new_DeclItem(fr, name);
    mDeclItemList.push_back(item);
  }

  /// @brief 初期値付き宣言要素の生成
  ///
  /// 結果は mDeclItemList に追加される．
  void
  new_DeclItem(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const char* name,        ///< [in] 名前
    const AstExpr* init_value ///< [in] 初期値を表す式
  )
  {
    auto item = mFactory.new_DeclItem(fr, name, init_value);
    mDeclItemList.push_back(item);
  }

  /// @brief 配列型宣言要素の生成
  ///
  /// 結果は mDeclItemList に追加される．
  void
  new_DeclItem(
    const FileRegion& fr,   ///< [in] ファイル位置の情報
    const char* name,       ///< [in] 名前
    PtRangeList* range_list ///< [in] 配列の各次元の範囲のリスト
  )
  {
    auto item = mFactory.new_DeclItem(fr, name,
				      range_list->to_array(mAlloc));
    mDeclItemList.push_back(item);
  }

  /// @brief 範囲の生成
  PtRange*
  new_Range(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* msb,    ///< [in] MSB を表す式
    const AstExpr* lsb     ///< [in] LSB を表す式
  )
  {
    return mFactory.new_Range(fr, msb, lsb);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // item 関連の要素の生成
  //////////////////////////////////////////////////////////////////////

  /// @brief defparam 文のヘッダの生成
  ///
  /// 現在の mDefParamList を要素とする．
  PtItem*
  new_DefParamH(
    const FileRegion& fr ///< [in] ファイル位置の情報
  )
  {
    return mFactory.new_DefParamH(fr,
				  PtDefParamArray(mAlloc, mDefParamList));
  }

  /// @brief mDefParamList を初期化する．
  void
  init_defparam_list()
  {
    mDefParamList.clear();
  }

  /// @brief defparam 文の要素の生成
  ///
  /// 結果は mDefparamList に追加される．
  void
  new_DefParam(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    const AstExpr* value   ///< [in] 値を表す式
  )
  {
    auto defparam = mFactory.new_DefParam(fr, name, value);
    mDefParamList.push_back(defparam);
  }

  /// @brief defparam 文の要素の生成 (階層つき識別子)
  ///
  /// 結果は mDefparamList に追加される．
  void
  new_DefParam(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PtHierName* hname,    ///< [in] 階層名
    const AstExpr* value   ///< [in] 値を表す式
  )
  {
    auto defparam = mFactory.new_DefParam(fr, hname, value);
    mDefParamList.push_back(defparam);
  }

  /// @brief continuous assign 文のヘッダの生成
  ///
  /// 現在の mContAssignList を要素とする．
  PtItem*
  new_ContAssignH(
    const FileRegion& fr ///< [in] ファイル位置の情報
  )
  {
    return mFactory.new_ContAssignH(fr, nullptr, nullptr,
				    PtContAssignArray(mAlloc, mContAssignList));
  }

  /// @brief continuous assign 文のヘッダの生成 (strengthつき)
  ///
  /// 現在の mContAssignList を要素とする．
  PtItem*
  new_ContAssignH(
    const FileRegion& fr,      ///< [in] ファイル位置の情報
    const AstStrength* strength ///< [in] 信号強度
  )
  {
    return mFactory.new_ContAssignH(fr, strength, nullptr,
				    PtContAssignArray(mAlloc, mContAssignList));
  }

  /// @brief continuous assign 文のヘッダの生成 (遅延付き)
  ///
  /// 現在の mContAssignList を要素とする．
  PtItem*
  new_ContAssignH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstDelay* delay  ///< [in] 遅延値
  )
  {
    return mFactory.new_ContAssignH(fr, nullptr, delay,
				    PtContAssignArray(mAlloc, mContAssignList));
  }

  /// @brief continuous assign 文のヘッダの生成 (strength, 遅延付き)
  ///
  /// 現在の mContAssignList を要素とする．
  PtItem*
  new_ContAssignH(
    const FileRegion& fr,       ///< [in] ファイル位置の情報
    const AstStrength* strength, ///< [in] 信号強度
    const AstDelay* delay        ///< [in] 遅延値
  )
  {
    return mFactory.new_ContAssignH(fr, strength, delay,
				    PtContAssignArray(mAlloc, mContAssignList));
  }

  /// @brief mContAssignList を初期化する．
  void
  init_contassign_list()
  {
    mContAssignList.clear();
  }

  /// @brief continuous assign 文の生成
  ///
  /// 結果は mContAssignList に追加される．
  void
  new_ContAssign(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* lhs,    ///< [in] 左辺式
    const AstExpr* rhs     ///< [in] 右辺式
  )
  {
    auto ca = mFactory.new_ContAssign(fr, lhs, rhs);
    mContAssignList.push_back(ca);
  }

  /// @brief initial 文の生成
  PtItem*
  new_Initial(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstStmt* body    ///< [in] 本体のステートメント
  )
  {
    return mFactory.new_Initial(fr, body);
  }

  /// @brief always 文の生成
  PtItem*
  new_Always(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstStmt* body    ///< [in] 本体のステートメント
  )
  {
    return mFactory.new_Always(fr, body);
  }

  /// @brief task/function 定義の開始
  /// - iohead list の初期化
  /// - paramhead list の初期化
  /// - localparamhead list の初期化
  /// - declhead list の初期化
  /// を行う．
  void
  init_tf()
  {
    mCurIOHeadList = &mTfIOHeadList;
    push_declhead_list();

    mCurIOHeadList->clear();
    mIOItemList.clear();
    cur_declhead_list().clear();
    mDeclItemList.clear();
  }

  /// @brief task/function 定義の終了
  void
  end_tf()
  {
    mCurIOHeadList = &mModuleIOHeadList;
    mCurDeclArray = pop_declhead_list();
  }

  /// @brief task 文の生成
  PtItem*
  new_Task(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] task 名
    bool automatic,       ///< [in] automatic task の時に true となるフラグ
    const AstStmt* stmt    ///< [in] 本体のステートメント
  )
  {
    return mFactory.new_Task(fr, name, automatic,
			     PtIOHeadArray(mAlloc, mTfIOHeadList, true),
			     PtDeclHeadArray(mAlloc, mCurDeclArray, true),
			     stmt);
  }

  /// @brief 1ビット型 function 文の生成
  PtItem*
  new_Function(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] function 名
    bool automatic,       ///< [in] automatic task の時に true となるフラグ
    bool sign,            ///< [in] signed 属性がついていたら true となるフラグ
    const AstStmt* stmt    ///< [in] 本体のステートメント
  )
  {
    return mFactory.new_Function(fr, name, automatic,
				 sign,
				 PtIOHeadArray(mAlloc, mTfIOHeadList, true),
				 PtDeclHeadArray(mAlloc, mCurDeclArray, true),
				 stmt);
  }

  /// @brief 範囲指定型 function 文の生成
  PtItem*
  new_SizedFunc(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] function 名
    bool automatic,       ///< [in] automatic task の時に true となるフラグ
    bool sign,		  ///< [in] signed 属性がついていたら true となるフラグ
    const AstRange* range, ///< [in] 範囲
    const AstStmt* stmt    ///< [in] 本体のステートメント
  )
  {
    return mFactory.new_SizedFunc(fr, name, automatic,
				  sign, range,
				  PtIOHeadArray(mAlloc, mTfIOHeadList, true),
				PtDeclHeadArray(mAlloc, mCurDeclArray, true),
				  stmt);
  }

  /// @brief 組み込み型 function 文の生成
  PtItem*
  new_TypedFunc(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,	  ///< [in] function 名
    bool automatic,	  ///< [in] automatic task の時に true となるフラグ
    bool sign,		  ///< [in] signed 属性がついていたら true となるフラグ
    VpiVarType func_type, ///< [in] 関数の戻値の型
    const AstStmt* stmt    ///< [in] 本体のステートメント
  )
  {
    return mFactory.new_TypedFunc(fr, name, automatic,
				  sign, func_type,
				  PtIOHeadArray(mAlloc, mTfIOHeadList, true),
				  PtDeclHeadArray(mAlloc, mCurDeclArray, true),
				  stmt);
  }

  /// @brief gate instance 文のヘッダの生成
  ///
  /// 現在の mInstList を要素とする．
  PtItem*
  new_GateH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiPrimType type      ///< [in] primitive の型
  );

  /// @brief gate instance 文のヘッダの生成 (strength付き)
  ///
  /// 現在の mInstList を要素とする．
  PtItem*
  new_GateH(
    const FileRegion& fr,      ///< [in] ファイル位置の情報
    VpiPrimType type,          ///< [in] primitive の型
    const AstStrength* strength ///< [in] 信号強度
  );

  /// @brief gate instance 文のヘッダの生成 (遅延付き)
  ///
  /// 現在の mInstList を要素とする．
  PtItem*
  new_GateH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiPrimType type,	  ///< [in] primitive の型
    const AstDelay* delay ///< [in] 遅延値
  );

  /// @brief gate instance 文のヘッダの生成 (strength, 遅延付き)
  ///
  /// 現在の mInstList を要素とする．
  PtItem*
  new_GateH(
    const FileRegion& fr,       ///< [in] ファイル位置の情報
    VpiPrimType type,	        ///< [in] primitive の型
    const AstStrength* strength, ///< [in] 信号強度
    const AstDelay* delay        ///< [in] 遅延値
  );

  /// @brief module instance/UDP instance 文のヘッダの生成
  ///
  /// 現在の mInstList を要素とする．
  PtItem*
  new_MuH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* def_name  ///< [in] 定義名
  );

  /// @brief module instance/UDP instance 文のヘッダの生成 (strength付き)
  ///
  /// 現在の mInstList を要素とする．
  PtItem*
  new_MuH(
    const FileRegion& fr,      ///< [in] ファイル位置の情報
    const char* def_name,      ///< [in] 定義名
    const AstStrength* strength ///< [in] 信号強度
  );

  /// @brief module instance/UDP instance 文のヘッダの生成 (遅延付き)
  ///
  /// 現在の mInstList を要素とする．
  PtItem*
  new_MuH(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* def_name, ///< [in] 定義名
    const AstDelay* delay  ///< [in] 遅延値
  );

  /// @brief module instance/UDP instance 文のヘッダの生成 (strength, 遅延付き)
  ///
  /// 現在の mInstList を要素とする．
  PtItem*
  new_MuH(
    const FileRegion& fr,       ///< [in] ファイル位置の情報
    const char* def_name,       ///< [in] 定義名
    const AstStrength* strength, ///< [in] 信号強度
    const AstDelay* delay        ///< [in] 遅延値
  );

  /// @brief module instance/UDP instance 文のヘッダの生成
  ///
  /// 現在の mInstList を要素とする．
  PtItem*
  new_MuH(
    const FileRegion& fr,      ///< [in] ファイル位置の情報
    const char* def_name,      ///< [in] 定義名
    PtConnectionList* con_list ///< [in] ポート割り当てリスト
  );

  /// @brief mInstList を初期化する．
  void
  init_inst_list()
  {
    mInstList.clear();
  }

  /// @brief module instance/UDP/gate instance の要素の生成
  ///
  /// 結果は mInstList に追加される．
  void
  new_Inst(
    const FileRegion& fr,      ///< [in] ファイル位置の情報
    PtConnectionList* con_list ///< [in] ポート割り当ての配列
  );

  /// @brief module instance/UDP/gate instance の要素の生成
  ///
  /// 結果は mInstList に追加される．
  void
  new_Inst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* expr1   ///< [in] ポート割り当て
  );

  /// @brief module instance/UDP/gate instance の要素の生成
  ///
  /// 結果は mInstList に追加される．
  void
  new_Inst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* expr1,  ///< [in] ポート割り当て式1
    const AstExpr* expr2   ///< [in] ポート割り当て式2
  );

  /// @brief module instance/UDP/gate instance の要素の生成
  ///
  /// 結果は mInstList に追加される．
  void
  new_Inst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* expr1, ///< [in] ポート割り当て式1
    const AstExpr* expr2, ///< [in] ポート割り当て式2
    const AstExpr* expr3  ///< [in] ポート割り当て式3
  );

  /// @brief module instance/UDP/gate instance の要素の生成
  ///
  /// 結果は mInstList に追加される．
  void
  new_Inst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* expr1,  ///< [in] ポート割り当て式1
    const AstExpr* expr2,  ///< [in] ポート割り当て式2
    const AstExpr* expr3,  ///< [in] ポート割り当て式3
    const AstExpr* expr4   ///< [in] ポート割り当て式4
  );

  /// @brief 名前付き module instance/UDP/gate instance の要素の生成
  ///
  /// 結果は mInstList に追加される．
  void
  new_InstN(
    const FileRegion& fr,      ///< [in] ファイル位置の情報
    const char* name,          ///< [in] 名前
    PtConnectionList* con_list ///< [in] ポート割り当ての配列
  );

  /// @brief 名前付き module instance/UDP/gate instance の要素の生成
  ///
  /// 結果は mInstList に追加される．
  void
  new_InstN(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    const AstExpr* expr1   ///< [in] ポート割り当て式1
  );

  /// @brief 名前付き module instance/UDP/gate instance の要素の生成
  ///
  /// 結果は mInstList に追加される．
  void
  new_InstN(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    const AstExpr* expr1,  ///< [in] ポート割り当て式1
    const AstExpr* expr2   ///< [in] ポート割り当て式2
  );

  /// @brief 名前付き module instance/UDP/gate instance の要素の生成
  ///
  /// 結果は mInstList に追加される．
  void
  new_InstN(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    const AstExpr* expr1,  ///< [in] ポート割り当て式1
    const AstExpr* expr2,  ///< [in] ポート割り当て式2
    const AstExpr* expr3   ///< [in] ポート割り当て式3
  );

  /// @brief 名前付き module instance/UDP/gate instance の要素の生成
  ///
  /// 結果は mInstList に追加される．
  void
  new_InstN(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    const AstExpr* expr1,  ///< [in] ポート割り当て式1
    const AstExpr* expr2,  ///< [in] ポート割り当て式2
    const AstExpr* expr3,  ///< [in] ポート割り当て式3
    const AstExpr* expr4   ///< [in] ポート割り当て式4
  );

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  ///
  /// 結果は mInstList に追加される．
  void
  new_InstV(
    const FileRegion& fr,      ///< [in] ファイル位置の情報
    const char* name,          ///< [in] 名前
    const AstRange* range,      ///< [in] 範囲
    PtConnectionList* con_list ///< [in] ポート割り当ての配列
  );

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  ///
  /// 結果は mInstList に追加される．
  void
  new_InstV(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    const AstRange* range, ///< [in] 範囲
    const AstExpr* expr1   ///< [in] ポート割り当て式1
  );

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  ///
  /// 結果は mInstList に追加される．
  void
  new_InstV(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    const AstRange* range, ///< [in] 範囲
    const AstExpr* expr1,  ///< [in] ポート割り当て式1
    const AstExpr* expr2   ///< [in] ポート割り当て式2
  );

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  ///
  /// 結果は mInstList に追加される．
  void
  new_InstV(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    const AstRange* range, ///< [in] 範囲
    const AstExpr* expr1,  ///< [in] ポート割り当て式1
    const AstExpr* expr2,  ///< [in] ポート割り当て式2
    const AstExpr* expr3   ///< [in] ポート割り当て式3
  );

  /// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
  ///
  /// 結果は mInstList に追加される．
  void
  new_InstV(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    const AstRange* range, ///< [in] 範囲
    const AstExpr* expr1,  ///< [in] ポート割り当て式1
    const AstExpr* expr2,  ///< [in] ポート割り当て式2
    const AstExpr* expr3,  ///< [in] ポート割り当て式3
    const AstExpr* expr4   ///< [in] ポート割り当て式4
  );

  /// @brief generate 文の生成
  PtItem*
  new_Generate(
    const FileRegion& fr ///< [in] ファイル位置の情報
  );

  /// @brief generate block 文の生成
  ///
  /// 結果は cur_item_list() に追加される．
  void
  new_GenBlock(
    const FileRegion& fr ///< [in] ファイル位置の情報
  );

  /// @brief 名前付き generate block 文の生成
  ///
  /// 結果は cur_item_list() に追加される．
  void
  new_GenBlock(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name      ///< [in] 名前
  );

  /// @brief generate if 文の生成
  ///
  /// 結果は cur_item_list() に追加される．
  void
  new_GenIf(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* cond    ///< [in] 条件を表す式
  );

  /// @brief generate if 文の生成
  ///
  /// 結果は cur_item_list() に追加される．
  void
  new_GenIfElse(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* cond    ///< [in] 条件を表す式
  );

  /// @brief generate case 文の生成
  ///
  /// 結果は cur_item_list() に追加される．
  void
  new_GenCase(
    const FileRegion& fr,        ///< [in] ファイル位置の情報
    const AstExpr* expr,          ///< [in] 選択式
    PtGenCaseItemList* item_list ///< [in] generate case item のリスト
  );

  /// @brief generate case の要素の生成
  /// @return 生成された generate case item
  PtGenCaseItem*
  new_GenCaseItem(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    PtExprList* label_list ///< [in] 比較式のリスト
  );

  /// @brief generate for 文の生成
  ///
  /// - 結果は cur_item_list() に追加される．
  /// - loop_var と next_var が等しくなければ
  ///   エラーメッセージを出力する．
  void
  new_GenFor(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const char* loop_var,    ///< [in] ループ変数
    const AstExpr* init_expr, ///< [in] 初期化式
    const AstExpr* cond,      ///< [in] ループ条件式
    const char* next_var,    ///< [in] 増加式の左辺の変数
    const AstExpr* next_expr, ///< [in] 増加式
    const char* block_name   ///< [in] ブロック名
  );

  /// @brief specify block item の生成
  ///
  /// 結果は cur_item_list() に追加される．
  void
  new_SpecItem(
    const FileRegion& fr,     ///< [in] ファイル位置の情報
    VpiSpecItemType id,       ///< [in] specify block item の種類
    PtExprList* terminal_list ///< [in] 端子のリスト
  )
  {
    auto item = mFactory.new_SpecItem(fr, id,
				      terminal_list->to_array(mAlloc));
    add_item(item);
  }

  /// @brief path 仕様を生成する．
  ///
  /// 結果は cur_item_list() に追加される．
  void
  new_SpecPath(
    const FileRegion& fr,       ///< [in] ファイル位置の情報
    VpiSpecPathType id,         ///< [in] spec path の種類
    const AstExpr* expr,         ///< [in] 条件式
    const AstPathDecl* path_decl ///< [in] パス記述
  )
  {
    auto item = mFactory.new_SpecPath(fr, id, expr, path_decl);
    add_item(item);
  }

  /// @brief パス記述の生成
  PtPathDecl*
  new_PathDecl(
    const FileRegion& fr,         ///< [in] ファイル位置の情報
    int edge,                     ///< [in] エッジ
    PtExprList* input_list,       ///< [in] 入力リスト
    int input_pol,                ///< [in] 入力の極性
    VpiPathType op,               ///< [in] パスタイプ
    PtExprList* output_list,      ///< [in] 出力リスト
    int output_pol,               ///< [in] 出力の極性
    const AstExpr* expr,           ///< [in] 条件式
    const AstPathDelay* path_delay ///< [in] パス遅延
  )
  {
    return mFactory.new_PathDecl(fr, edge,
				 input_list->to_array(mAlloc), input_pol,
				 op,
				 output_list->to_array(mAlloc), output_pol,
				 expr, path_delay);
  }

  /// @brief パス記述の生成
  PtPathDecl*
  new_PathDecl(
    const FileRegion& fr,         ///< [in] ファイル位置の情報
    int edge,			  ///< [in] エッジ
    PtExprList* input_list,       ///< [in] 入力リスト
    int input_pol,		  ///< [in] 入力の極性
    VpiPathType op,		  ///< [in] パスタイプ
    const AstExpr* output,	  ///< [in] 出力
    int output_pol,		  ///< [in] 出力の極性
    const AstExpr* expr,		  ///< [in] 条件式
    const AstPathDelay* path_delay ///< [in] パス遅延
  )
  {
    return mFactory.new_PathDecl(fr, edge,
				 input_list->to_array(mAlloc), input_pol,
				 op,
				 PtExprArray(mAlloc, output), output_pol,
				 expr, path_delay);
  }

  /// @brief path delay value の生成 (値が1個)
  /// @return 生成された path delay value
  PtPathDelay*
  new_PathDelay(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* value   ///< [in] 値
  )
  {
    return mFactory.new_PathDelay(fr, value);
  }

  /// @brief path delay value の生成 (値が2個)
  /// @return 生成された path delay value
  PtPathDelay*
  new_PathDelay(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* value1, ///< [in] 値1
    const AstExpr* value2  ///< [in] 値2
  )
  {
    return mFactory.new_PathDelay(fr, value1, value2);
  }

  /// @brief path delay value の生成 (値が3個)
  /// @return 生成された path delay value
  PtPathDelay*
  new_PathDelay(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* value1, ///< [in] 値1
    const AstExpr* value2, ///< [in] 値2
    const AstExpr* value3  ///< [in] 値3
  )
  {
    return mFactory.new_PathDelay(fr, value1, value2, value3);
  }

  /// @brief path delay value の生成 (値が6個)
  /// @return 生成された path delay value
  PtPathDelay*
  new_PathDelay(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* value1, ///< [in] 値1
    const AstExpr* value2, ///< [in] 値2
    const AstExpr* value3, ///< [in] 値3
    const AstExpr* value4, ///< [in] 値4
    const AstExpr* value5, ///< [in] 値5
    const AstExpr* value6  ///< [in] 値6
  )
  {
    return mFactory.new_PathDelay(fr,
				  value1, value2, value3,
				  value4, value5, value6);
  }

  /// @brief path delay value の生成 (値が12個)
  /// @return 生成された path delay value
  PtPathDelay*
  new_PathDelay(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
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
  )
  {
    return mFactory.new_PathDelay(fr,
				  value1, value2, value3,
				  value4, value5, value6,
				  value7, value8, value9,
				  value10, value11, value12);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // ステートメント関連の要素の生成
  //////////////////////////////////////////////////////////////////////

  /// @brief disable 文の生成
  /// @return 生成された disable 文
  PtStmt*
  new_Disable(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name      ///< [in] 名前
  );

  /// @brief disable 文の生成 (階層付き識別子)
  /// @return 生成された disable 文
  PtStmt*
  new_Disable(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PtHierName* hname     ///< [in] 階層名
  );

  /// @brief enable 文の生成
  /// @return 生成された enable 文
  PtStmt*
  new_Enable(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    PtExprList* arg_list  ///< [in] 引数のリスト
  );

  /// @brief enable 文の生成 (階層付き識別子)
  /// @return 生成された enable 文
  PtStmt*
  new_Enable(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PtHierName* hname,    ///< [in] 階層名
    PtExprList* arg_list  ///< [in] 引数のリスト
  );

  /// @brief system task enable 文の生成
  PtStmt*
  new_SysEnable(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    PtExprList* arg_list  ///< [in] 引数のリスト
  );

  /// @brief delay control 文の生成
  /// @return 生成された delay control 文
  PtStmt*
  new_DcStmt(
    const FileRegion& fr,   ///< [in] ファイル位置の情報
    const AstControl* delay, ///< [in] 遅延コントロール
    const AstStmt* body      ///< [in] 本体のステートメント
  );

  /// @brief event control 文の生成
  /// @return 生成された event control 文
  PtStmt*
  new_EcStmt(
    const FileRegion& fr,   ///< [in] ファイル位置の情報
    const AstControl* event, ///< [in] イベントコントロール
    const AstStmt* body      ///< [in] 本体のステートメント
  );

  /// @brief wait 文の生成
  /// @return 生成された wait 文
  PtStmt*
  new_Wait(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* cond,   ///< [in] 条件式
    const AstStmt* body    ///< [in] 本体のステートメント
  );

  /// @brief assign 文の生成
  /// @return 生成された assign 文
  PtStmt*
  new_Assign(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* lhs,    ///< [in] 左辺式
    const AstExpr* rhs     ///< [in] 右辺式
  );

  /// @brief control 付き assign 文の生成
  /// @return 生成された assign 文
  PtStmt*
  new_Assign(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const AstExpr* lhs,       ///< [in] 左辺式
    const AstExpr* rhs,       ///< [in] 右辺式
    const AstControl* control ///< [in] 制御式
  );

  /// @brief nonblocking assign 文の生成
  /// @return 生成された nonblocking assign 文
  PtStmt*
  new_NbAssign(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* lhs,    ///< [in] 左辺式
    const AstExpr* rhs     ///< [in] 右辺式
  );

  /// @brief control 付き nonblocking assign 文の生成
  /// @return 生成された nonblocking assign 文
  PtStmt*
  new_NbAssign(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const AstExpr* lhs,       ///< [in] 左辺式
    const AstExpr* rhs,       ///< [in] 右辺式
    const AstControl* control ///< [in] 制御式
  );

  /// @brief event 文の生成
  /// @return 生成された event 文
  PtStmt*
  new_EventStmt(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* event   ///< [in] イベント名
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
    const FileRegion& fr,   ///< [in] ファイル位置の情報
    const AstExpr* expr,     ///< [in] 条件を表す式
    const AstStmt* then_body ///< [in] 成り立ったときに実行されるステートメント
  );

  /// @brief if 文の生成
  /// @return 生成された if 文
  PtStmt*
  new_If(
    const FileRegion& fr,    ///< [in] ファイル位置の情報
    const AstExpr* expr,      ///< [in] 条件を表す式
    const AstStmt* then_body, ///< [in] 成り立ったときに実行されるステートメント
    const AstStmt* else_body  ///< [in] 成り立たなかったときに実行されるステートメント
  );

  /// @brief case 文の生成
  /// @return 生成された case 文
  PtStmt*
  new_Case(
    const FileRegion& fr,         ///< [in] ファイル位置の情報
    const AstExpr* expr,           ///< [in] 条件を表す式
    PtCaseItemList* caseitem_list ///< [in] case item のリスト
  );

  /// @brief casex 文の生成
  /// @return 生成された case 文
  PtStmt*
  new_CaseX(
    const FileRegion& fr,         ///< [in] ファイル位置の情報
    const AstExpr* expr,		  ///< [in] 条件を表す式
    PtCaseItemList* caseitem_list ///< [in] case item のリスト
  );

  /// @brief casez 文の生成
  /// @return 生成された case 文
  PtStmt*
  new_CaseZ(
    const FileRegion& fr,         ///< [in] ファイル位置の情報
    const AstExpr* expr,		  ///< [in] 条件を表す式
    PtCaseItemList* caseitem_list ///< [in] case item のリスト
  );

  /// @brief case item の生成
  /// @return 生成された case item
  PtCaseItem*
  new_CaseItem(
    const FileRegion& fr,   ///< [in] ファイル位置の情報
    PtExprList* label_list, ///< [in] ラベルのリスト
    const AstStmt* body      ///< [in] 本体のステートメント
  );

  /// @brief forever 文の生成
  /// @return 生成された forever 文
  PtStmt*
  new_Forever(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstStmt* body    ///< [in] 本体のステートメント
  );

  /// @brief repeat 文の生成
  /// @return 生成された repeat 文
  PtStmt*
  new_Repeat(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* expr,   ///< [in] 繰り返し数を表す式
    const AstStmt* body    ///< [in] 本体のステートメント
  );

  /// @brief while 文の生成
  /// @return 生成された while 文
  PtStmt*
  new_While(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* cond,   ///< [in] 繰り返し条件を表す式
    const AstStmt* body    ///< [in] 本体のステートメント
  );

  /// @brief for 文の生成
  /// @return 生成された for 文
  PtStmt*
  new_For(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstStmt* init,   ///< [in] 初期化文
    const AstExpr* cond,   ///< [in] 繰り返し条件を表す式
    const AstStmt* next,   ///< [in] 増加文
    const AstStmt* body    ///< [in] 本体のステートメント
  );

  /// @brief procedural assign 文の生成
  /// @return 生成された procedural assign 文
  PtStmt*
  new_PcAssign(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* lhs,    ///< [in] 左辺式
    const AstExpr* rhs     ///< [in] 右辺式
  );

  /// @brief deassign 文の生成
  /// @return 生成された deassign 文
  PtStmt*
  new_Deassign(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* lhs     ///< [in] 左辺式
  );

  /// @brief force 文の生成
  /// @return 生成された force 文
  PtStmt*
  new_Force(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* lhs,    ///< [in] 左辺式
    const AstExpr* rhs     ///< [in] 右辺式
  );

  /// @brief release 文の生成
  /// @return 生成された release 文
  PtStmt*
  new_Release(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* lhs     ///< [in] 左辺式
  );

  /// @brief parallel block の生成
  /// @return 生成された parallel block
  PtStmt*
  new_ParBlock(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PtStmtList* stmt_list ///< [in] 本体のステートメントのリスト
  );

  /// @brief 名前付き parallel block の生成
  /// @return 生成された parallel block
  PtStmt*
  new_NamedParBlock(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    PtStmtList* stmt_list ///< [in] 本体のステートメントのリスト
  );

  /// @brief sequential block の生成
  /// @return 生成された sequential block
  PtStmt*
  new_SeqBlock(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PtStmtList* stmt_list ///< [in] 本体のステートメントのリスト
  );

  /// @brief 名前付き sequential block の生成
  /// @return 生成された sequential block
  PtStmt*
  new_NamedSeqBlock(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    PtStmtList* stmt_list ///< [in] 本体のステートメントのリスト
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 式の生成
  //////////////////////////////////////////////////////////////////////

  /// @brief 単項演算子の生成
  /// @return 生成された演算子
  PtExpr*
  new_Opr(
    const FileRegion& fr,   ///< [in] ファイル位置の情報
    VpiOpType type,         ///< [in] 演算の種類
    const AstExpr* opr,      ///< [in] オペランド
    PtAttrInstList* ai_list ///< [in] 属性リスト
  );

  /// @brief 二項演算子の生成
  /// @return 生成された演算子
  PtExpr*
  new_Opr(
    const FileRegion& fr,   ///< [in] ファイル位置の情報
    VpiOpType type,         ///< [in] 演算の種類
    const AstExpr* opr1,     ///< [in] オペランド1
    const AstExpr* opr2,     ///< [in] オペランド2
    PtAttrInstList* ai_list ///< [in] 属性リスト
  );

  /// @brief 三項演算子の生成
  /// @return 生成された演算子
  PtExpr*
  new_Opr(
    const FileRegion& fr,   ///< [in] ファイル位置の情報
    VpiOpType type,	    ///< [in] 演算の種類
    const AstExpr* opr1,    ///< [in] オペランド1
    const AstExpr* opr2,    ///< [in] オペランド2
    const AstExpr* opr3,    ///< [in] オペランド3
    PtAttrInstList* ai_list ///< [in] 属性リスト
  );

  /// @brief concatination 演算子の生成
  /// @return 生成された concatination 演算子
  PtExpr*
  new_Concat(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PtExprList* expr_list ///< [in] オペランドのリスト
  );

  /// @brief multi-concatination 演算子の生成
  /// @return 生成された multi-concatination 演算子
  PtExpr*
  new_MultiConcat(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* rep,   ///< [in] 繰り返し数
    PtExprList* expr_list ///< [in] オペランドのリスト
  );

  /// @brief min/typ/max delay 演算子の生成
  /// @param[in] fr ファイル位置の情報
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
    const FileRegion& fr,   ///< [in] ファイル位置の情報
    const char* name,       ///< [in] 名前
    PtExprList* index_array ///< [in] インデックスのリスト
  );

  /// @brief 範囲指定付き primary の生成
  /// @return 生成された primary
  PtExpr*
  new_Primary(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    const AstPart* part   ///< [in] 範囲指定
  );

  /// @brief インデックスと範囲指定付き primary の生成
  /// @return 生成された primary
  PtExpr*
  new_Primary(
    const FileRegion& fr,   ///< [in] ファイル位置の情報
    const char* name,       ///< [in] 名前
    PtExprList* index_list, ///< [in] インデックスのリスト
    const AstPart* part     ///< [in] 範囲指定
  );

  /// @brief primary の生成 (階層付き)
  /// @return 生成された primary
  PtExpr*
  new_Primary(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PtHierName* hname     ///< [in] 階層名
  );

  /// @brief インデックス付き primary の生成 (階層付き)
  /// @return 生成された primary
  PtExpr*
  new_Primary(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    PtHierName* hname,     ///< [in] 階層名
    PtExprList* index_list ///< [in] インデックスのリスト
  );

  /// @brief 範囲指定付き primary の生成 (階層付き)
  /// @return 生成された primary
  PtExpr*
  new_Primary(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    PtHierName* hname,    ///< [in] 階層名
    const AstPart* part   ///< [in] 範囲指定
  );

  /// @brief インデックスと範囲指定付き primary の生成 (階層付き)
  /// @return 生成された primary
  PtExpr*
  new_Primary(
    const FileRegion& fr,   ///< [in] ファイル位置の情報
    PtHierName* hname,      ///< [in] 階層名
    PtExprList* index_list, ///< [in] インデックスのリスト
    const AstPart* part     ///< [in] 範囲指定
  );

  /// @brief constant primary の生成
  /// @return 生成された const primary
  PtExpr*
  new_CPrimary(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    const AstExpr* index  ///< [in] インデックス
  );

  /// @brief constant primary の生成
  /// @return 生成された const primary
  PtExpr*
  new_CPrimary(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    const char* name,      ///< [in] 名前
    PtExprList* index_list ///< [in] インデックスのリスト
  );

  /// @brief 範囲指定付き constant primary の生成
  /// @return 生成された constant primary
  PtExpr*
  new_CPrimary(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    const AstPart* part   ///< [in] 範囲指定
  );

  /// @brief インデックス付き constant primary の生成 (階層付き)
  /// @return 生成された constant primary
  PtExpr*
  new_CPrimary(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    PtHierName* hname,     ///< [in] 階層名
    PtExprList* index_list ///< [in] インデックスのリスト
  );

  /// @brief function call の生成
  PtExpr*
  new_FuncCall(
    const FileRegion& fr,   ///< [in] ファイル位置の情報
    const char* name,       ///< [in] 名前
    PtExprList* arg_list,   ///< [in] 引数のリスト
    PtAttrInstList* ai_list ///< [in] 属性リスト
  );

  /// @brief function call の生成 (階層付き)
  /// @return 生成された function call
  PtExpr*
  new_FuncCall(
    const FileRegion& fr,   ///< [in] ファイル位置の情報
    PtHierName* hname,      ///< [in] 階層名
    PtExprList* arg_list,   ///< [in] 引数のリスト
    PtAttrInstList* ai_list ///< [in] 属性リスト
  );

  /// @brief system function call の生成
  /// @return 生成された function call
  PtExpr*
  new_SysFuncCall(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    PtExprList* arg_list  ///< [in] 引数のリスト
  );

  /// @brief 整数型の定数の生成
  /// @return 生成された定数
  ///
  /// 仕様上符号なし整数となる．
  PtExpr*
  new_IntConst(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    SizeType value        ///< [in] 定数を表す整数値
  );

  /// @brief 整数型の定数の生成
  /// @return 生成された定数
  ///
  /// 通常の整数より大きな整数を表す文字列の場合
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


public:
  //////////////////////////////////////////////////////////////////////
  // その他
  //////////////////////////////////////////////////////////////////////

  /// @brief ディレイコントロールの生成
  /// @return 生成されたディレイコントロール
  PtControl*
  new_DelayControl(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* value   ///< [in] 遅延を表す式
  );

  /// @brief イベントコントロールの生成 (any-event)
  /// @return 生成されたイベントコントロール
  PtControl*
  new_EventControl(
    const FileRegion& fr ///< [in] ファイル位置の情報
  );

  /// @brief イベントコントロールの生成
  /// @return 生成されたイベントコントロール
  PtControl*
  new_EventControl(
    const FileRegion& fr,      ///< [in] ファイル位置の情報
    const char* event_name,    ///< [in] イベントを表す名前
    const FileRegion& name_loc ///< [in] event_name の位置
  );

  /// @brief イベントコントロールの生成
  /// @return 生成されたイベントコントロール
  PtControl*
  new_EventControl(
    const FileRegion& fr,      ///< [in] ファイル位置の情報
    PtHierName* event_name,    ///< [in] イベントを表す名前
    const FileRegion& name_loc ///< [in] event_name の位置
  );

  /// @brief イベントコントロールの生成
  /// @return 生成されたイベントコントロール
  PtControl*
  new_EventControl(
    const FileRegion& fr,   ///< [in] ファイル位置の情報
    PtExprList* event_array ///< [in] イベントのリスト
  );

  /// @brief リピートコントロールの生成 (any-event)
  /// @return 生成されたリピートコントロール
  PtControl*
  new_RepeatControl(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const AstExpr* rep     ///< [in] 繰り返し数を表す式
  );

  /// @brief リピートコントロールの生成
  /// @return 生成されたリピートコントロール
  PtControl*
  new_RepeatControl(
    const FileRegion& fr,      ///< [in] ファイル位置の情報
    const AstExpr* rep,         ///< [in] 繰り返し数を表す式
    const char* event_name,    ///< [in] 繰り返しの単位となるイベント
    const FileRegion& name_loc ///< [in] event_name の位置
  );

  /// @brief リピートコントロールの生成
  /// @return 生成されたリピートコントロール
  PtControl*
  new_RepeatControl(
    const FileRegion& fr,      ///< [in] ファイル位置の情報
    const AstExpr* rep,	       ///< [in] 繰り返し数を表す式
    PtHierName* event_name,    ///< [in] 繰り返しの単位となるイベント
    const FileRegion& name_loc ///< [in] event_name の位置
  );

  /// @brief リピートコントロールの生成
  /// @return 生成されたリピートコントロール
  PtControl*
  new_RepeatControl(
    const FileRegion& fr,  ///< [in] ファイル位置の情報
    const AstExpr* rep,     ///< [in] 繰り返し数を表す式
    PtExprList* event_list ///< [in] 繰り返しの単位となるイベントのリスト
  );

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
    const FileRegion& fr,   ///< [in] ファイル位置の情報
    const AstExpr* expr,     ///< [in] 結合する式
    PtAttrInstList* ai_list ///< [in] 属性リスト
  );

  /// @brief 名前付き結合子の生成
  /// @return 生成された結合子
  PtConnection*
  new_NamedCon(
    const FileRegion& fr,             ///< [in] ファイル位置の情報
    const char* name,                 ///< [in] 名前
    const AstExpr* expr = nullptr,     ///< [in] 結合する式
    PtAttrInstList* ai_list = nullptr ///< [in] 属性リスト
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
  ///
  /// head_name '.' name という階層名を作る．
  PtHierName*
  new_HierName(
    const char* head_name, ///< [in] 階層の上位部分
    const char* name       ///< [in] 階層の最下位部分
  )
  {
    auto nb = mFactory.new_NameBranch(head_name);
    return new_HierName(nb, name);
  }

  /// @brief 階層名の生成
  ///
  /// head_name '[' index ']' '.' name という階層名を作る．
  PtHierName*
  new_HierName(
    const char* head_name, ///< [in] 階層の上位部分
    int index,             ///< [in] インデックス
    const char* name       ///< [in] 階層の最下位部分
  )
  {
    auto nb = mFactory.new_NameBranch(head_name, index);
    return new_HierName(nb, name);
  }

  /// @brief 階層名の生成
  PtHierName*
  new_HierName(
    const AstNameBranch* nb, ///< [in] 上位の名前のリスト
    const char* name        ///< [in] 階層の最下位部分
  )
  {
    return mFactory.new_HierName(nb, name);
  }

  /// @brief 階層名の追加
  ///
  /// hname の後ろに '.' name を追加する．
  void
  add_HierName(
    PtHierName* hname, ///< [in] 階層名の上位部分
    const char* name   ///< [in] 追加する名前
  )
  {
    auto nb = mFactory.new_NameBranch(hname->tail_name());
    hname->add(mAlloc, nb, name);
  }

  /// @brief 階層名の追加
  ///
  /// hname の後ろに '[' index ']' '.' name を追加する．
  void
  add_HierName(
    PtHierName* hname, ///< [in] 階層名の上位部分
    int index,         ///< [in] インデックス
    const char* name   ///< [in] 追加する名前
  )
  {
    auto nb = mFactory.new_NameBranch(hname->tail_name(), index);
    hname->add(mAlloc, nb, name);
  }

  /// @brief 範囲指定の生成
  PtPart*
  new_Part(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    VpiRangeMode mode,    ///< [in] 範囲指定のモード
    const AstExpr* expr1,  ///< [in] 1番目の式
    const AstExpr* expr2   ///< [in] 2番目の式
  );


  //////////////////////////////////////////////////////////////////////
  // attribute instance 関係
  //////////////////////////////////////////////////////////////////////

  /// @brief attribute instance の生成
  /// @return 生成された attribute instance
  PtAttrInst*
  new_AttrInst(
    const FileRegion& fr,   ///< [in] ファイル位置の情報
    PtAttrSpecList* as_list ///< [in] attribute spec のリスト
  );

  /// @brief attribute spec の生成
  /// @return 生成された attribute spec
  PtAttrSpec*
  new_AttrSpec(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* name,     ///< [in] 名前
    const AstExpr* expr    ///< [in] 値
  );


public:
  //////////////////////////////////////////////////////////////////////
  // Ver2.0 リスト関係
  //////////////////////////////////////////////////////////////////////

  /// @brief モジュール定義の開始
  /// - port list の初期化
  /// - paramport list の初期化
  /// - iohead list の初期化
  /// - paramhead list の初期化
  /// - localparamhead list の初期化
  /// - declhead list の初期化
  /// - item list の初期化
  /// を行う．
  void
  init_module();

  /// @brief モジュール定義の終了
  void
  end_module();

  /// @brief UDP定義の開始
  /// - port list の初期化
  /// - iohead list の初期化
  /// - declhead list の初期化
  /// - UDP entry list の初期化
  /// を行う．
  void
  init_udp();

  /// @brief UDP 定義の終了
  void
  end_udp();

  /// @brief generate block の開始
  void
  init_generate();

  /// @brief generate block の終了
  void
  end_generate();

  /// @brief generate-if の開始
  void
  init_genif();

  /// @brief generate-if の終了
  void
  end_genif();

  /// @brief generate-if の else 節の開始
  void
  init_genelse();

  /// @brief generate-if の else 節の終了
  void
  end_genelse();

  /// @brief block-statment の開始
  void
  init_block()
  {
    push_declhead_list();
  }

  /// @brief block-statement の終了
  void
  end_block()
  {
    mCurDeclArray = pop_declhead_list();
  }

  /// @brief parameter port 宣言ヘッダを追加する．
  void
  add_paramport_head(
    PtDeclHead* head,
    PtAttrInstList* attr_list
  )
  {
    if ( head ) {
      reg_attrinst(head, attr_list);
      mParamPortHeadList.push_back(head);
    }
  }

  /// @brief parameter port 宣言の終わり
  void
  flush_paramport()
  {
    if ( !mDeclItemList.empty() ) {
      ASSERT_COND( !mParamPortHeadList.empty() );
      auto last = mParamPortHeadList.back();
      last->set_elem(PtDeclItemArray(mAlloc, mDeclItemList));
      mDeclItemList.clear();
    }
  }

  /// @brief IOポート宣言リストにIO宣言ヘッダを追加する．
  void
  add_ioport_head(
    PtIOHead* head,
    PtAttrInstList* attr_list
  )
  {
    if ( head ) {
      reg_attrinst(head, attr_list);
      mCurIOHeadList->push_back(head);
    }
  }

  /// @brief IO宣言の終わり
  void
  flush_io()
  {
    if ( !mIOItemList.empty() ) {
      ASSERT_COND( !mCurIOHeadList->empty() );
      auto last = mCurIOHeadList->back();
      last->set_elem(PtIOItemArray(mAlloc, mIOItemList));
      mIOItemList.clear();
    }
  }

  /// @brief IO宣言リストにIO宣言ヘッダを追加する．
  void
  add_io_head(
    PtIOHead* head,
    PtAttrInstList* attr_list
  )
  {
    add_ioport_head(head, attr_list);
    flush_io();
  }

  /// @brief 宣言リストに宣言ヘッダを追加する．
  void
  add_decl_head(
    PtDeclHead* head,
    PtAttrInstList* attr_list = nullptr
  )
  {
    if ( head ) {
      reg_attrinst(head, attr_list);
      cur_declhead_list().push_back(head);
      if ( !mDeclItemList.empty() ) {
	head->set_elem(PtDeclItemArray(mAlloc, mDeclItemList));
      }
    }
    mDeclItemList.clear();
  }

  /// @brief item リストに要素を追加する．
  void
  add_item(
    PtItem* item,
    PtAttrInstList* attr_list = nullptr
  )
  {
    if ( item ) {
      reg_attrinst(item, attr_list);
      cur_item_list().push_back(item);
    }
  }


public:
  //////////////////////////////////////////////////////////////////////
  // リスト関係
  //////////////////////////////////////////////////////////////////////

  /// @brief AttrInst のリストを作る．
  PtAttrInstList*
  new_attrinst_list()
  {
    void* p = mAlloc.get_memory(sizeof(PtAttrInstList));
    return new (p) PtAttrInstList;
  }

  /// @brief AttrInst のリストの末尾に要素を追加する．
  void
  push_back(
    PtAttrInstList* list,
    PtAttrInst* elem
  )
  {
    list->push_back(mAlloc, elem);
  }

  /// @brief AttrSpec のリストを作る．
  PtAttrSpecList*
  new_attrspec_list()
  {
    void* p = mAlloc.get_memory(sizeof(PtAttrSpecList));
    return new (p) PtAttrSpecList;
  }

  /// @brief AttrSpec のリストの末尾に要素を追加する．
  void
  push_back(
    PtAttrSpecList* list,
    PtAttrSpec* elem
  )
  {
    list->push_back(mAlloc, elem);
  }

  /// @brief CaseItem のリストを作る．
  PtCaseItemList*
  new_caseitem_list()
  {
    void* p = mAlloc.get_memory(sizeof(PtCaseItemList));
    return new (p) PtCaseItemList;
  }

  /// @brief CaseItem のリストの末尾に要素を追加する．
  void
  push_back(
    PtCaseItemList* list,
    PtCaseItem* elem
  )
  {
    list->push_back(mAlloc, elem);
  }

  /// @brief Connection のリストを作る．
  PtConnectionList*
  new_connection_list()
  {
    void* p = mAlloc.get_memory(sizeof(PtConnectionList));
    return new (p) PtConnectionList;
  }

  /// @brief Connection のリストの末尾に要素を追加する．
  void
  push_back(
    PtConnectionList* list,
    PtConnection* elem
  )
  {
    list->push_back(mAlloc, elem);
  }

  /// @brief Expr のリストを作る．
  PtExprList*
  new_expr_list()
  {
    void* p = mAlloc.get_memory(sizeof(PtExprList));
    return new (p) PtExprList;
  }

  /// @brief Expr のリストの末尾に要素を追加する．
  void
  push_back(
    PtExprList* list,
    PtExpr* elem
  )
  {
    list->push_back(mAlloc, elem);
  }

  /// @brief GenCaseItem のリストを作る．
  PtGenCaseItemList*
  new_gencaseitem_list()
  {
    void* p = mAlloc.get_memory(sizeof(PtGenCaseItemList));
    return new (p) PtGenCaseItemList;
  }

  /// @brief GenCaseItem のリストの末尾に要素を追加する．
  void
  push_back(
    PtGenCaseItemList* list,
    PtGenCaseItem* elem
  )
  {
    list->push_back(mAlloc, elem);
  }

  /// @brief Range のリストを作る．
  PtRangeList*
  new_range_list()
  {
    void* p = mAlloc.get_memory(sizeof(PtRangeList));
    return new (p) PtRangeList;
  }

  /// @brief Range のリストの末尾の要素を追加する．
  void
  push_back(
    PtRangeList* list,
    PtRange* elem
  )
  {
    list->push_back(mAlloc, elem);
  }

  /// @brief Stmt のリストを作る．
  PtStmtList*
  new_stmt_list()
  {
    void* p = mAlloc.get_memory(sizeof(PtStmtList));
    return new (p) PtStmtList;
  }

  /// @brief Stmt のリストの末尾に要素を追加する．
  void
  push_back(
    PtStmtList* list,
    PtStmt* elem
  )
  {
    list->push_back(mAlloc, elem);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // Lex 用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief lex オブジェクトの取得
  /// @return lex オブジェクトを返す．
  Lex&
  lex()
  {
    return *mLex;
  }

  /// @brief yylex とのインターフェイス
  /// @return 読み込んだトークンの id を返す．
  int
  yylex(
    YYSTYPE& lval,   ///< [out] 値を格納する変数
    FileRegion& lloc ///< [out] 位置情報を格納する変数
  );


public:
  //////////////////////////////////////////////////////////////////////
  // その他の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 関数内で使えるステートメントかどうかのチェック
  bool
  check_function_statement(
    const AstStmt* stmt
  );

  /// @brief default ラベルが2つ以上含まれていないかどうかのチェック
  bool
  check_default_label(
    const PtCaseItemList* ci_list
  );

  /// @brief 使用されているモジュール名を登録する．
  /// @param[in] name 登録する名前
  void
  reg_defname(
    const char* name
  )
  {
    mAstMgr.reg_defname(name);
  }

  /// @brief attribute instance を登録する．
  void
  reg_attrinst(
    const AstBase* obj,
    PtAttrInstList* attr_list,
    bool def = false
  )
  {
    mAstMgr.reg_attrinst(obj, attr_list, def);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // デバッグ/テスト用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ポートリストを得る．
  const std::vector<PtPort*>&
  _port_list() const
  {
    return mPortList;
  }

  /// @brief ポート参照リストを得る．
  const std::vector<const AstExpr*>&
  _poretref_list() const
  {
    return mPortRefList;
  }

  /// @brief パラメータポート宣言リストを得る．
  const std::vector<PtDeclHead*>&
  _paramport_list() const
  {
    return mParamPortHeadList;
  }

  /// @brief モジュール用IO宣言リストを得る．
  const std::vector<PtIOHead*>&
  _module_io_list() const
  {
    return mModuleIOHeadList;
  }

  /// @brief タスク/関数用IO宣言リストを得る．
  const std::vector<PtIOHead*>&
  _tf_io_list() const
  {
    return mTfIOHeadList;
  }

  /// @brief IO宣言要素リストを得る．
  const std::vector<const AstIOItem*>&
  _ioitem_list() const
  {
    return mIOItemList;
  }

  /// @brief 宣言要素リストを得る．
  const std::vector<const AstDeclItem*>&
  _declitem_list() const
  {
    return mDeclItemList;
  }

  /// @brief UDPエントリのリストを得る．
  const std::vector<const AstUdpEntry*>&
  _udp_entry_list() const
  {
    return mUdpEntryList;
  }

  /// @brief UDPテーブルの値のリストを得る．
  const std::vector<const AstUdpValue*>&
  _udp_value_list() const
  {
    return mUdpValueList;
  }

  /// @brief defparam 要素のリストを得る．
  const std::vector<const AstDefParam*>&
  _defparam_list() const
  {
    return mDefParamList;
  }

  /// @broef contassign のリストを得る．
  const std::vector<const AstContAssign*>&
  _contassign_list() const
  {
    return mContAssignList;
  }

  /// @brief インスタンスのリストを得る．
  const std::vector<const AstInst*>&
  _inst_list() const
  {
    return mInstList;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 現在の declhead リストをスタックに積む．
  void
  push_declhead_list()
  {
    mDeclHeadListStack.push_back(std::vector<PtDeclHead*>());
  }

  /// @brief スタックのトップを取り出す．
  std::vector<PtDeclHead*>
  pop_declhead_list()
  {
    auto vec = cur_declhead_list();
    mDeclHeadListStack.pop_back();
    return vec;
  }

  /// @brief 現在の item リストをスタックに積む．
  void
  push_item_list()
  {
    mItemListStack.push_back(std::vector<PtItem*>());
  }

  /// @brief スタックのトップを取り出す．
  std::vector<PtItem*>
  pop_item_list()
  {
    auto vec = cur_item_list();
    mItemListStack.pop_back();
    return vec;
  }

  /// @brief 現在の宣言ヘッダのリストを返す．
  std::vector<PtDeclHead*>&
  cur_declhead_list()
  {
    return mDeclHeadListStack.back();
  }

  /// @brief 現在の item リストを返す．
  std::vector<PtItem*>&
  cur_item_list()
  {
    return mItemListStack.back();
  }

  /// @brief 入出力宣言中の重複チェックを行う．
  bool
  check_PortArray(
    const std::vector<PtIOHead*>& iohead_list
  );

  /// @brief ポート宣言とIO宣言の齟齬をチェックする．
  void
  check_IO(
    const std::vector<PtPort*>& port_list,        ///< [in] ポート宣言のリスト
    const std::vector<PtIOHead*>& iohead_list,           ///< [in] IO宣言のリスト
    std::unordered_map<std::string, VpiDir>& iodecl_dirs ///< [in] IO宣言名をキーとして向きを保持する辞書
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  Alloc& mAlloc;

  // パース木を保持するクラス
  AstMgr& mAstMgr;

  // パース木の要素の生成を行うクラス
  PtFactory mFactory;

  // 字句解析を行うオブジェクト
  std::unique_ptr<Lex> mLex;


public:
  //////////////////////////////////////////////////////////////////////
  // 生成した構文木要素を一時的に保持しておくためのリスト
  //////////////////////////////////////////////////////////////////////

  // ポートリスト
  std::vector<PtPort*> mPortList;

  // ポート参照リスト
  std::vector<const AstExpr*> mPortRefList;

  // parameter port 宣言ヘッダリスト
  std::vector<PtDeclHead*> mParamPortHeadList;

  // モジュール用 IO宣言ヘッダリスト
  std::vector<PtIOHead*> mModuleIOHeadList;

  // task/function 用 IO宣言ヘッダリスト
  std::vector<PtIOHead*> mTfIOHeadList;

  // IO宣言要素リスト
  std::vector<const AstIOItem*> mIOItemList;

  // 宣言要素リスト
  std::vector<const AstDeclItem*> mDeclItemList;

  // UDP エントリのリスト
  std::vector<const AstUdpEntry*> mUdpEntryList;

  // UDP のテーブルの値のリスト
  std::vector<const AstUdpValue*> mUdpValueList;

  // defparam 要素のリスト
  std::vector<const AstDefParam*> mDefParamList;

  // contassign リスト
  std::vector<const AstContAssign*> mContAssignList;

  // instance リスト
  std::vector<const AstInst*> mInstList;


private:
  //////////////////////////////////////////////////////////////////////
  // 現在，登録対象となっているリストを指すポインタ
  //////////////////////////////////////////////////////////////////////

  // 現在の IO宣言ヘッダリスト
  // 実際には mModuleIOHeadList か mTfIOHeadList を指す．
  std::vector<PtIOHead*>* mCurIOHeadList;

  // 現在の宣言ヘッダの配列
  // スタックから取り出された最終結果
  std::vector<PtDeclHead*> mCurDeclArray;

  // 現在の item の配列
  // スタックから取り出された最終結果
  std::vector<PtItem*> mCurItemArray;

  // generate-if の then 節の宣言ヘッダリスト
  std::vector<PtDeclHead*> mGenThenDeclArray;

  // generate-if の then 節の item リスト
  std::vector<PtItem*> mGenThenItemArray;

  // generate-if の else 節の宣言ヘッダリスト
  std::vector<PtDeclHead*> mGenElseDeclArray;

  // generate-if の else 節の item リスト
  std::vector<PtItem*> mGenElseItemArray;


public:
  //////////////////////////////////////////////////////////////////////
  // mCurXXXList のスタック
  //////////////////////////////////////////////////////////////////////

  // 宣言ヘッダリストのスタック
  std::vector<std::vector<PtDeclHead*>> mDeclHeadListStack;

  // item リストのスタック
  std::vector<std::vector<PtItem*>> mItemListStack;

};

END_NAMESPACE_YM_VERILOG

#endif // PARSER_H
