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
#include "parser/PtModule.h"
#include "parser/PtUdp.h"
#include "parser/PtPort.h"
#include "parser/PtDecl.h"
#include "parser/PtExpr.h"
#include "parser/PtItem.h"
#include "parser/PtStmt.h"
#include "parser/PtMisc.h"
#include "parser/PtHierName.h"
#include "parser/PtHeadList.h"
#include "parser/AstMgr.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

class VlLineWatcher;
class Lex;

union YYSTYPE;

//////////////////////////////////////////////////////////////////////
/// @class Parser Parser.h "parser/Parser.h"
/// @ingroup Parser
/// @brief Verilog-HDL のパーサークラス
///
/// 役割は2つある．
/// - Yacc/Bison を駆動する外部インターフェイス
/// - Yacc/Bison の文法規則では判断できない構文エラーのチェック
///   例えば Module のポートリスト中に現れる名前が入出力宣言にも
///   現れていなければならない，など
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

  /// @brief PtFactory を返す．
  PtFactory&
  factory()
  {
    return mFactory;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // UDP 関連の生成関数
  //
  // 最初に init_udp() を呼ぶ．
  // 宣言要素や UDP Entry などの生成を行う．
  // 最後に end_udp() を呼ぶ．
  // 直後に new_Udp() を呼ぶことで，直前の init_udp() から end_udp()
  // の間に生成された要素を持つ UDP を生成する．
  //////////////////////////////////////////////////////////////////////

  /// @brief Verilog1995 タイプのUDP を生成する．
  PtUdp*
  new_Udp1995(
    const FileRegion& file_region, ///< [in] ファイル上の位置
    const char* name,              ///< [in] 名前
    const char* init_name,         ///< [in] 初期値の名前
    const FileRegion& init_loc,    ///< [in] 初期値の位置
    PtExpr* init_value,     ///< [in] 初期値のパース木
    PtPort* port_top,   ///< [in] ポートの先頭
    PtIOHead* io_top,   ///< [in] IO宣言の先頭
    PtDeclHead* decl_top, ///< [in] Reg 宣言の先頭
    PtUdpEntry* entry_top ///< [in] テーブルエントリの先頭
  );

  /// @brief Verilog2001 タイプのUDP を生成する．
  PtUdp*
  new_Udp2001(
    const FileRegion& file_region, ///< [in] ファイル上の位置
    const char* name,		   ///< [in] 名前
    const char* init_name,	   ///< [in] 初期値の名前
    const FileRegion& init_loc,	   ///< [in] 初期値の位置
    PtExpr* init_value,	   ///< [in] 初期値のパース木
    PtIOHead* io_top,              ///< [in] IO宣言の先頭
    PtUdpEntry* entry_top          ///< [in] テーブルエントリの先頭
  );


private:

  /// @brief new_Udp の下請け関数
  PtUdp*
  new_Udp(
    const FileRegion& file_region,
    const char* udp_name,
    const char* init_name,
    const FileRegion& init_loc,
    const AstExpr* init_value,
    bool is_seq,
    const AstIOItem* out_item,
    PtPort* port_top,
    PtIOHead* iohead_top,
    PtUdpEntry* entry_top
  );


public:
  //////////////////////////////////////////////////////////////////////
  // モジュール関連の要素の生成関数
  //
  // 最初に init_module() を呼ぶ．
  // 要素の生成を行う．
  // 最後に end_module() を呼ぶ．
  // 直後に new_Module() を呼ぶことで直前の init_module() から
  // end_module() の間に生成された要素を持つモジュールを生成する．
  //////////////////////////////////////////////////////////////////////

  /// @brief Verilog1995 タイプのモジュール(のテンプレート)を生成する．
  PtModule*
  new_Module1995(
    const FileRegion& file_region,
    bool is_macro,
    const char* name,
    PtDeclHead* paramport_top,
    PtPort* port_top,
    PtIOHead* iohead_top,
    PtDeclHead* declhead_top,
    PtItem* item_top
  );

  /// @brief Verilog2001 タイプのモジュール(のテンプレート)を生成する．
  PtModule*
  new_Module2001(
    const FileRegion& file_region,
    bool is_macro,
    const char* name,
    PtDeclHead* paramport_top,
    PtIOHead* portdecl_top,
    PtDeclHead* declhead_top,
    PtItem* item_top
  );


public:
  //////////////////////////////////////////////////////////////////////
  // ポート関連の要素の生成関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力宣言中の重複チェックを行う．
  bool
  check_PortArray(
    const AstIOHeadList& iohead_list
  );

  /// @brief 入出力宣言からポートリストを作る．
  PtPortList
  new_PortArray(
    const AstIOHeadList& iohead_list
  );


public:
  //////////////////////////////////////////////////////////////////////
  // GenFor の生成
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  // 式の生成
  //////////////////////////////////////////////////////////////////////

  /// @brief 単項演算子の生成
  /// @return 生成された演算子
  PtExpr*
  new_Opr(
    const FileRegion& fr,     ///< [in] ファイル位置の情報
    VpiOpType type,           ///< [in] 演算の種類
    const AstExpr* opr,       ///< [in] オペランド
    const AstAttrInst* ai_top ///< [in] 属性リスト
  )
  {
    auto expr = mFactory.new_Opr(fr, type, opr);
    reg_attrinst(expr, ai_top);
    return expr;
  }

  /// @brief 二項演算子の生成
  /// @return 生成された演算子
  PtExpr*
  new_Opr(
    VpiOpType type,           ///< [in] 演算の種類
    const AstExpr* opr1,      ///< [in] オペランド1
    const AstExpr* opr2,      ///< [in] オペランド2
    const AstAttrInst* ai_top ///< [in] 属性リスト
  )
  {
    auto expr = mFactory.new_Opr(type, opr1, opr2);
    reg_attrinst(expr, ai_top);
    return expr;
  }

  /// @brief 三項演算子の生成
  /// @return 生成された演算子
  PtExpr*
  new_Opr(
    VpiOpType type,	      ///< [in] 演算の種類
    const AstExpr* opr1,      ///< [in] オペランド1
    const AstExpr* opr2,      ///< [in] オペランド2
    const AstExpr* opr3,      ///< [in] オペランド3
    const AstAttrInst* ai_top ///< [in] 属性リスト
  )
  {
    auto expr = mFactory.new_Opr(type, opr1, opr2, opr3);
    reg_attrinst(expr, ai_top);
    return expr;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // その他
  //////////////////////////////////////////////////////////////////////

  /// @brief 階層名の生成
  PtHierName
  new_HierName(
    const char* head_name,
    const char* tail_name
  )
  {
    auto nb = mFactory.new_NameBranch(head_name);
    return PtHierName{nb, nb, tail_name};
  }

  /// @brief 階層名の生成
  PtHierName
  new_HierName(
    const char* head_name,
    int index,
    const char* tail_name
  )
  {
    auto nb = mFactory.new_NameBranch(head_name, index);
    return PtHierName{nb, nb, tail_name};
  }

  /// @brief 階層名の追加
  ///
  /// hname の後ろに '.' name を追加する．
  void
  add_HierName(
    PtHierName& hname, ///< [in] 階層名
    const char* name   ///< [in] 追加する名前
  )
  {
    auto nb = mFactory.new_NameBranch(hname.tail_name);
    hname.nb_list.add(nb);
    hname.tail_name = name;
  }

  /// @brief 階層名の追加
  ///
  /// hname の後ろに '[' index ']' '.' name を追加する．
  void
  add_HierName(
    PtHierName& hname, ///< [in] 階層名
    int index,         ///< [in] インデックス
    const char* name   ///< [in] 追加する名前
  )
  {
    auto nb = mFactory.new_NameBranch(hname.tail_name, index);
    hname.nb_list.add(nb);
    hname.tail_name = name;
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

  /// @brief UDP定義を登録する．
  void
  reg_udp(
    const AstUdp* udp
  )
  {
    mAstMgr.reg_udp(udp);
  }

  /// @brief モジュール定義を登録する．
  void
  reg_module(
    const AstModule* module
  )
  {
    mAstMgr.reg_module(module);
  }

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
    const AstAttrInst* ai_top,
    bool def = false
  )
  {
    mAstMgr.reg_attrinst(obj, ai_top, def);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // チェックを行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ポート宣言とIO宣言の齟齬をチェックする．
  void
  check_IO(
    PtPort* port_top,        ///< [in] ポート宣言のリスト
    PtIOHead* iohead_top,           ///< [in] IO宣言のリスト
    std::unordered_map<std::string, VpiDir>& iodecl_dirs ///< [in] IO宣言名をキーとして向きを保持する辞書
  );

  /// @brief 関数内で使えるステートメントかどうかのチェック
  bool
  check_function_statement(
    const AstStmt* stmt
  );

  /// @briefdefault ラベルが2つ以上含まれていないかどうかのチェック
  bool
  check_default_label(
    const AstCaseItemList& caseitem_list
  );

  /// @brief GenFor 文のチェックを行う．
  ///
  /// 具体的には loop_var と next_var が同一かチェックする．
  bool
  check_GenFor(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const char* loop_var, ///< [in] ループ変数
    const char* next_var  ///< [in] 増加式の左辺の変数
  )
  {
    if ( strcmp(loop_var, next_var) == 0 ) {
      return true;
    }

    std::ostringstream buf;
    buf << "Lhs of the increment statement ("
	<< next_var
	<< ") does not match with Lhs of the initial statement ("
	<< loop_var
	<< ")";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    fr,
		    MsgType::Error,
		    "PARSER",
		    buf.str());
    return false;
  }


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

};

END_NAMESPACE_YM_VERILOG

#endif // PARSER_H
