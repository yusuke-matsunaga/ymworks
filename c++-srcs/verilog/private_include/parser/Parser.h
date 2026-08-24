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
/// - Yacc/Bison のアクションに対応した処理を行う API の提供
///   本来は構文規則に対応したアクションで構文木を作るが，
///   Yacc/Bison の制約で YYSTYPE にコンストラクタ/デストラクタが
///   必要なデータ型を使用できない．
///   そこで Parser 内部に std::vector<> のリスト構造を持つ．
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

  /// @brief UDP定義の開始
  ///
  /// - port list の初期化
  /// - iohead list の初期化
  /// - declhead list の初期化
  /// - UDP entry list の初期化
  /// を行う．
  /// 'udp' キーワードに連動して呼ばれることを想定している．
  void
  init_udp();

  /// @brief UDP 定義の終了
  ///
  /// 'endudp' キーワードに連動して呼ばれることを想定している．
  void
  end_udp();

  /// @brief Verilog1995 タイプのUDP を生成する．
  void
  new_Udp1995(
    const FileRegion& file_region, ///< [in] ファイル上の位置
    const char* name,              ///< [in] 名前
    const char* init_name,         ///< [in] 初期値の名前
    const FileRegion& init_loc,    ///< [in] 初期値の位置
    const AstExpr* init_value,     ///< [in] 初期値のパース木
    const AstAttrInst* ai_top      ///< [in] 属性リスト
  );

  /// @brief Verilog2001 タイプのUDP を生成する．
  void
  new_Udp2001(
    const FileRegion& file_region, ///< [in] ファイル上の位置
    const char* name,		   ///< [in] 名前
    const char* init_name,	   ///< [in] 初期値の名前
    const FileRegion& init_loc,	   ///< [in] 初期値の位置
    const AstExpr* init_value,	   ///< [in] 初期値のパース木
    const AstAttrInst* ai_top      ///< [in] 属性リスト
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
    const AstAttrInst* ai_top,
    bool is_seq,
    const AstIOItem* out_item,
    const std::vector<PtPort*>& port_list,
    const std::vector<PtIOHead*>& iohead_list
  );


public:
  //////////////////////////////////////////////////////////////////////
  // UdpEntry の生成
  //
  // 最初に init_udpvalue_list() を呼ぶ．
  // add_udpvalue() で生成した UdpValue をリストに追加する．
  // 最後に udpvalue_list() でリストを取得する．
  //////////////////////////////////////////////////////////////////////

  /// @brief Udpentry をリストに追加する．
  void
  add_udpentry(
    PtUdpEntry* entry
  )
  {
    mUdpEntryList.push_back(entry);
  }

  /// @brief UdpValue のリストを初期化する．
  void
  init_udpvalue_list()
  {
    mUdpValueList.clear();
  }

  /// @brief UdpValue をリストに追加する．
  void
  add_udpvalue(
    PtUdpValue* val
  )
  {
    mUdpValueList.push_back(val);
  }

  /// @brief UdpValue のリストを取得する．
  const AstUdpValueVec&
  udpvalue_list() const
  {
    return mUdpValueList;
  }


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

  /// @brief モジュール定義の開始
  /// - port list の初期化
  /// - paramport list の初期化
  /// - iohead list の初期化
  /// - declhead list の初期化
  /// - item list の初期化
  /// を行う．
  ///
  /// 'module' キーワードに連動して呼ばれることを想定している．
  void
  init_module()
  {
    mPortList.clear();
    mParamPortHeadList.clear();
    mModuleIOHeadList.clear();
    push_declhead_list();
    push_item_list();

    mIOItemList.clear();
    mDeclItemList.clear();
  }

  /// @brief モジュール定義の終了
  ///
  /// 'endmodule' キーワードに連動して呼ばれることを想定している．
  void
  end_module()
  {
    mCurDeclList = pop_declhead_list();
    mCurItemList = pop_item_list();
  }

  /// @brief モジュール用のIOヘッダリストに追加する．
  void
  add_module_ioport_head(
    PtIOHead* head
  )
  {
    mModuleIOHeadList.push_back(head);
  }

  /// @brief モジュール用のIOヘッダリストにIO宣言ヘッダを追加する．
  ///
  /// こちらは完結しているのですぐに処理する．
  void
  add_module_iohead(
    PtIOHead* head
  )
  {
    add_module_ioport_head(head);
    flush_module_ioitem();
  }

  /// @brief モジュール用のIOヘッダリストの処理を行う．
  void
  flush_module_ioitem()
  {
    if ( mModuleIOHeadList.empty() ) {
      throw std::logic_error{"mModuleIOHeadList.empty()"};
    }
    auto last = mModuleIOHeadList.back();
    last->set_elem(PtIOItemArray(mAlloc, mIOItemList));
    mIOItemList.clear();
  }

  /// @brief Verilog1995 タイプのモジュール(のテンプレート)を生成する．
  void
  new_Module1995(
    const FileRegion& file_region,
    bool is_macro,
    const char* name,
    const AstAttrInst* ai_top
  );

  /// @brief Verilog2001 タイプのモジュール(のテンプレート)を生成する．
  void
  new_Module2001(
    const FileRegion& file_region,
    bool is_macro,
    const char* name,
    const AstAttrInst* ai_top
  );


public:
  //////////////////////////////////////////////////////////////////////
  // ポート関連の要素の生成関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Port リストを初期化する．
  void
  init_port_list()
  {
    mPortList.clear();
  }

  /// @brief Port リストに追加する．
  void
  add_port(
    PtPort* port
  )
  {
    mPortList.push_back(port);
  }

  /// @brief 入出力宣言中の重複チェックを行う．
  bool
  check_PortArray(
    const std::vector<PtIOHead*>& iohead_list
  );

  /// @brief 入出力宣言からポートを作る．
  std::vector<PtPort*>
  new_PortArray(
    const std::vector<PtIOHead*>& iohead_list
  );


public:
  //////////////////////////////////////////////////////////////////////
  // IO 宣言関係のリストの操作関数
  //////////////////////////////////////////////////////////////////////

  /// @brief IOItem をリストに追加する．
  ///
  /// mIOItemList は init_module(), init_tf() で初期化され，
  /// flush_module_ioitem(), flush_tf_ioitem() で最後のヘッダにセットされる．
  void
  add_ioitem(
    PtIOItem* item
  )
  {
    mIOItemList.push_back(item);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 宣言関連の要素の生成
  //////////////////////////////////////////////////////////////////////

  /// @brief DeclItem をリストに追加する．
  void
  add_declitem(
    PtDeclItem* item
  )
  {
    mDeclItemList.push_back(item);
  }

  /// @brief DeclItem リストを取り出す．
  const AstDeclItemVec&
  declitem_list() const
  {
    return mDeclItemList;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // DefParam リスト関連の関数
  //
  // 最初に init_defparam_list() を呼ぶ．
  // その後 add_defparam() で生成した DefParam をリストに追加する．
  // 最後に defparam_list() でリストの内容を取り出す．
  //////////////////////////////////////////////////////////////////////

  /// @brief DefParam のリストを初期化する．
  void
  init_defparam_list()
  {
    mDefParamList.clear();
  }

  /// @brief DefParam をリストに追加する．
  void
  add_defparam(
    PtDefParam* defparam
  )
  {
    mDefParamList.push_back(defparam);
  }

  /// @brief DefParam のリストを返す．
  const AstDefParamVec&
  defparam_list() const
  {
    return mDefParamList;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // ContAssign リスト関連の関数
  //
  // 最初に init_contassign_list() を呼ぶ．
  // その後に add_contassign() で生成した ContAssign をリストに追加する．
  // 最後に contassign_list() でリストの内容を取り出す．
  //////////////////////////////////////////////////////////////////////

  /// @brief ContAssign のリストを初期化する．
  void
  init_contassign_list()
  {
    mContAssignList.clear();
  }

  /// @brief ContAssign をリストに追加する．
  void
  add_contassign(
    PtContAssign* contassign
  )
  {
    mContAssignList.push_back(contassign);
  }

  /// @brief ContAssign のリストを取り出す．
  const AstContAssignVec&
  contassign_list() const
  {
    return mContAssignList;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // Task/Func の生成
  //
  // 最初に init_tf() を呼ぶ．
  // 宣言要素などの生成を行う．
  // 最後に new_Task()/new_Function() を呼ぶ．
  //////////////////////////////////////////////////////////////////////

  /// @brief task/function 定義の開始
  /// - iohead list の初期化
  /// - paramhead list の初期化
  /// - localparamhead list の初期化
  /// - declhead list の初期化
  /// を行う．
  void
  init_tf()
  {
    mTfIOHeadList.clear();
    push_declhead_list();

    mIOItemList.clear();
    mDeclItemList.clear();
  }

  /// @brief task/function 定義の終了
  void
  end_tf()
  {
    mCurDeclList = pop_declhead_list();
  }

  /// @brief タスク/関数用のIOヘッダリストに追加する．
  void
  add_tf_ioport_head(
    PtIOHead* head
  )
  {
    mTfIOHeadList.push_back(head);
  }

  /// @brief タスク/関数用のIOヘッダリストにIO宣言ヘッダを追加する．
  ///
  /// こちらは完結しているのですぐに処理する．
  void
  add_tf_iohead(
    PtIOHead* head
  )
  {
    add_tf_ioport_head(head);
    flush_tf_ioitem();
  }

  /// @brief タスク/関数用のIOヘッダリストの処理を行う．
  void
  flush_tf_ioitem()
  {
    if ( mTfIOHeadList.empty() ) {
      throw std::logic_error{"mTfIOHeadList.empty()"};
    }
    auto last = mTfIOHeadList.back();
    last->set_elem(PtIOItemArray(mAlloc, mIOItemList));
    mIOItemList.clear();
  }

  /// @brief タスク/関数用のIOヘッダリストを取り出す．
  const std::vector<PtIOHead*>&
  tf_iohead_list() const
  {
    return mTfIOHeadList;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // GateInst/MuInst リスト関係の関数
  //
  // 最初に init_inst_list() を呼ぶ．
  // add_isnt() で生成した Inst をリストに追加する．
  // 最後に inst_list() で内容を取り出す．
  //////////////////////////////////////////////////////////////////////

  /// @brief Inst のリストを初期化する．
  void
  init_inst_list()
  {
    mInstList.clear();
  }

  /// @brief Inst をリストに追加する．
  void
  add_inst(
    PtInst* inst
  )
  {
    mInstList.push_back(inst);
  }

  /// @brief Inst のリストを返す．
  const AstInstVec&
  inst_list() const
  {
    return mInstList;
  }

  /// @brief ParamAssign のリストを初期化する．
  void
  init_paramassign_list()
  {
    mParamAssignList.clear();
  }

  /// @brief ParamAssign のリストに追加する．
  void
  add_paramassign(
    PtConnection* item
  )
  {
    mParamAssignList.push_back(item);
  }

  /// @brief ParamAssign のリストに追加する．
  void
  add_paramassign(
    PtExpr* expr
  )
  {
    auto con = mFactory.new_OrderedCon(expr);
    add_paramassign(con);
  }

  /// @brief ParamAssign のリストを返す．
  const AstConnectionVec&
  paramassign_list() const
  {
    return mParamAssignList;
  }

  /// @brief Connection のリストを初期化する．
  void
  init_connection_list()
  {
    mConnectionList.clear();
  }

  /// @brief Connection のリストに追加する．
  void
  add_connection(
    PtConnection* item
  )
  {
    mConnectionList.push_back(item);
  }

  /// @brief Connection のリストに追加する．
  void
  add_connection(
    PtExpr* expr
  )
  {
    auto con = mFactory.new_OrderedCon(expr);
    add_connection(con);
  }

  /// @brief Conneciton のリストを返す．
  const AstConnectionVec&
  connection_list() const
  {
    return mConnectionList;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // Generate 関係の生成
  //
  // 最初に init_generate() を呼ぶ．
  // 宣言要素とItemの生成を行う．
  // 最後に end_generate() を呼ぶ．
  // 直後に new_Generate() や new_GenBlock() を呼ぶことで直前の
  // init_generate() から end_generate() の間に生成された要素を
  // 持つ Generate ブロックを生成する．
  //////////////////////////////////////////////////////////////////////

  /// @brief generate block の開始
  void
  init_generate()
  {
    push_declhead_list();
    push_item_list();
  }

  /// @brief generate block の終了
  void
  end_generate()
  {
    mCurDeclList = pop_declhead_list();
    mCurItemList = pop_item_list();
  }


public:
  //////////////////////////////////////////////////////////////////////
  // GenIf の生成
  //
  // 条件が成り立った時に生成する要素を
  // init_genif() から end_genif() の間で生成する．
  // 条件が成り立たなかった時に生成する要素を
  // init_genelse() からの end_genelse() の間に生成する．
  // 最後に new_GenIf() か new_GenIfElse() を呼ぶ．
  //////////////////////////////////////////////////////////////////////

  /// @brief generate-if の開始
  void
  init_genif()
  {
    push_declhead_list();
    push_item_list();
  }

  /// @brief generate-if の終了
  void
  end_genif()
  {
    mGenThenDeclList = pop_declhead_list();
    mGenThenItemList = pop_item_list();
  }

  /// @brief generate-if の else 節の開始
  void
  init_genelse()
  {
    push_declhead_list();
    push_item_list();
  }

  /// @brief generate-if の else 節の終了
  void
  end_genelse()
  {
    mGenElseDeclList = pop_declhead_list();
    mGenElseItemList = pop_item_list();
  }

  /// @brief GenThenDeclList の取得
  const std::vector<PtDeclHead*>&
  genthendecl_list() const
  {
    return mGenThenDeclList;
  }

  /// @brief GenThenItemList の取得
  const AstItemVec&
  genthenitem_list() const
  {
    return mGenThenItemList;
  }

  /// @brief GenElseDeclList の取得
  const std::vector<PtDeclHead*>&
  genelsedecl_list() const
  {
    return mGenElseDeclList;
  }

  /// @brief GenElseItemList の取得
  const AstItemVec&
  genelseitem_list() const
  {
    return mGenElseItemList;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // GenFor の生成
  //////////////////////////////////////////////////////////////////////

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


public:
  //////////////////////////////////////////////////////////////////////
  // ステートメント関連の要素の生成
  //////////////////////////////////////////////////////////////////////

  /// @brief case の開始
  void
  init_case()
  {
    push_caseitem_list();
  }

  void
  add_caseitem(
    PtCaseItem* item
  )
  {
    mCaseItemListStack.back().push_back(item);
  }

  /// @brief case の終了
  void
  end_case()
  {
    pop_caseitem_list();
  }

  /// @brief CaseItem リストの取得
  const AstCaseItemVec&
  caseitem_list() const
  {
    return mCurCaseItemList;
  }

  /// @brief StmtList の初期化
  void
  init_stmt_list()
  {
    push_stmt_list();
  }

  /// @brief stmt を追加する．
  void
  add_stmt(
    PtStmt* stmt
  )
  {
    mStmtListStack.back().push_back(stmt);
  }

  /// @brief StmtList の終了
  ///
  /// 結果は mCurStmtList が保持している．
  void
  end_stmt_list()
  {
    pop_stmt_list();
  }

  /// @brief StmtList の取得
  const AstStmtVec&
  stmt_list() const
  {
    return mCurStmtList;
  }

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
    mCurDeclList = pop_declhead_list();
  }

  /// @brief CurDeclList の取得
  const std::vector<PtDeclHead*>&
  declhead_list() const
  {
    return mCurDeclList;
  }

  /// @brief CurItemList の取得
  const AstItemVec&
  item_list() const
  {
    return mCurItemList;
  }


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
    const FileRegion& fr,     ///< [in] ファイル位置の情報
    VpiOpType type,           ///< [in] 演算の種類
    const AstExpr* opr1,      ///< [in] オペランド1
    const AstExpr* opr2,      ///< [in] オペランド2
    const AstAttrInst* ai_top ///< [in] 属性リスト
  )
  {
    auto expr = mFactory.new_Opr(fr, type, opr1, opr2);
    reg_attrinst(expr, ai_top);
    return expr;
  }

  /// @brief 三項演算子の生成
  /// @return 生成された演算子
  PtExpr*
  new_Opr(
    const FileRegion& fr,     ///< [in] ファイル位置の情報
    VpiOpType type,	      ///< [in] 演算の種類
    const AstExpr* opr1,      ///< [in] オペランド1
    const AstExpr* opr2,      ///< [in] オペランド2
    const AstExpr* opr3,      ///< [in] オペランド3
    const AstAttrInst* ai_top ///< [in] 属性リスト
  )
  {
    auto expr = mFactory.new_Opr(fr, type, opr1, opr2, opr3);
    reg_attrinst(expr, ai_top);
    return expr;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // ExprList の生成関数
  //////////////////////////////////////////////////////////////////////

  /// @brief AstExpr の内部リストを初期化する
  void
  init_expr_list()
  {
    mExprListStack.push_back(AstExprVec());
  }

  /// @brief AstExpr の内部リストに追加する．
  void
  add_expr(
    const AstExpr* expr
  )
  {
    mExprListStack.back().push_back(expr);
  }

  /// @brief AstExpr の内部リストを取り出す．
  PtExprList*
  end_expr_list()
  {
    auto expr_list = mFactory.new_ExprList(mExprListStack.back());
    mExprListStack.pop_back();
    return expr_list;
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
    return PtHierName{nb, tail_name};
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
    return PtHierName{nb, tail_name};
  }

  /// @brief 階層名の追加
  ///
  /// hname の後ろに '.' name を追加する．
  void
  add_HierName(
    PtHierName& hname, ///< [in] 階層名の上位部分
    const char* name   ///< [in] 追加する名前
  )
  {
    auto nb = mFactory.new_NameBranch(hname.tail_name);
    nb->set_link(hname.nb_top);
    hname.nb_top = nb;
    hname.tail_name = name;
  }

  /// @brief 階層名の追加
  ///
  /// hname の後ろに '[' index ']' '.' name を追加する．
  void
  add_HierName(
    PtHierName& hname, ///< [in] 階層名の上位部分
    int index,         ///< [in] インデックス
    const char* name   ///< [in] 追加する名前
  )
  {
    auto nb = mFactory.new_NameBranch(hname.tail_name, index);
    nb->set_link(hname.nb_top);
    hname.nb_top = nb;
    hname.tail_name = name;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // Ver2.0 リスト関係
  //////////////////////////////////////////////////////////////////////

  /// @brief parameter port 宣言ヘッダを追加する．
  void
  add_paramport_head(
    PtDeclHead* head
  )
  {
    mParamPortHeadList.push_back(head);
  }

  /// @brief parameter port 宣言の終わり
  void
  flush_paramport()
  {
    if ( mDeclItemList.empty() ) {
      // リストが空なら無視する．
      return;
    }
    if ( mParamPortHeadList.empty() ) {
      throw std::logic_error{"mParamPortHeadList.empty()"};
    }
    auto last = mParamPortHeadList.back();
    last->set_elem(PtDeclItemArray(mAlloc, mDeclItemList));
    mDeclItemList.clear();
  }

  /// @brief 宣言リストに宣言ヘッダを追加する．
  void
  add_declhead(
    PtDeclHead* head
  )
  {
    cur_declhead_list().push_back(head);
    head->set_elem(PtDeclItemArray(mAlloc, mDeclItemList));
    mDeclItemList.clear();
  }

  /// @brief item リストに要素を追加する．
  void
  add_item(
    PtItem* item
  )
  {
    cur_item_list().push_back(item);
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
  check_default_label();

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
  // デバッグ/テスト用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ポートリストを得る．
  const std::vector<PtPort*>&
  _port_list() const
  {
    return mPortList;
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
  const AstIOItemVec&
  _ioitem_list() const
  {
    return mIOItemList;
  }

  /// @brief 宣言要素リストを得る．
  const AstDeclItemVec&
  _declitem_list() const
  {
    return mDeclItemList;
  }

  /// @brief UDPエントリのリストを得る．
  const AstUdpEntryVec&
  _udp_entry_list() const
  {
    return mUdpEntryList;
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
    mItemListStack.push_back(AstItemVec());
  }

  /// @brief スタックのトップを取り出す．
  AstItemVec
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
  AstItemVec&
  cur_item_list()
  {
    return mItemListStack.back();
  }

  /// @brief StmtList のスタックに空のリストを追加する．
  void
  push_stmt_list()
  {
    mStmtListStack.push_back(AstStmtVec());
  }

  /// @brief StmtList のスタックのトップを取り出す．
  void
  pop_stmt_list()
  {
    mCurStmtList = mStmtListStack.back();
    mStmtListStack.pop_back();
  }

  /// @brief CaseItemList のスタックに空のリストを追加する．
  void
  push_caseitem_list()
  {
    mCaseItemListStack.push_back(AstCaseItemVec());
  }

  /// @brief CaseItemList のスタックのトップを取り出す．
  void
  pop_caseitem_list()
  {
    mCurCaseItemList = mCaseItemListStack.back();
    mCaseItemListStack.pop_back();
  }


public:
  //////////////////////////////////////////////////////////////////////
  // チェックを行う関数
  //////////////////////////////////////////////////////////////////////

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

  // parameter port 宣言ヘッダリスト
  std::vector<PtDeclHead*> mParamPortHeadList;

  // モジュール用 IO宣言ヘッダリスト
  std::vector<PtIOHead*> mModuleIOHeadList;

  // task/function 用 IO宣言ヘッダリスト
  std::vector<PtIOHead*> mTfIOHeadList;

  // IO宣言要素リスト
  AstIOItemVec mIOItemList;

  // 宣言要素リスト
  AstDeclItemVec mDeclItemList;

  // UDP エントリのリスト
  AstUdpEntryVec mUdpEntryList;

  // UDP のテーブルの値のリスト
  AstUdpValueVec mUdpValueList;

  // defparam 要素のリスト
  AstDefParamVec mDefParamList;

  // contassign リスト
  AstContAssignVec mContAssignList;

  // instance リスト
  AstInstVec mInstList;

  // ParamAssign のリスト
  AstConnectionVec mParamAssignList;

  // Connection のリスト
  AstConnectionVec mConnectionList;


private:
  //////////////////////////////////////////////////////////////////////
  // 現在，登録対象となっているリストを指すポインタ
  //////////////////////////////////////////////////////////////////////

  // 現在の IO宣言ヘッダリスト
  // 実際には mModuleIOHeadList か mTfIOHeadList を指す．
  std::vector<PtIOHead*>* mCurIOHeadList;

  // 現在の宣言ヘッダの配列
  // スタックから取り出された最終結果
  std::vector<PtDeclHead*> mCurDeclList;

  // 現在の item の配列
  // スタックから取り出された最終結果
  AstItemVec mCurItemList;

  // 現在の stmt の配列
  // スタックから取り出された最終結果
  AstStmtVec mCurStmtList;

  // 現在の caseitem の配列
  // スタックから取り出された最終結果
  AstCaseItemVec mCurCaseItemList;

  // generate-if の then 節の宣言ヘッダリスト
  std::vector<PtDeclHead*> mGenThenDeclList;

  // generate-if の then 節の item リスト
  AstItemVec mGenThenItemList;

  // generate-if の else 節の宣言ヘッダリスト
  std::vector<PtDeclHead*> mGenElseDeclList;

  // generate-if の else 節の item リスト
  AstItemVec mGenElseItemList;


public:
  //////////////////////////////////////////////////////////////////////
  // mCurXXXList のスタック
  //////////////////////////////////////////////////////////////////////

  // 宣言ヘッダリストのスタック
  std::vector<std::vector<PtDeclHead*>> mDeclHeadListStack;

  // item リストのスタック
  std::vector<AstItemVec> mItemListStack;

  // StmtList のスタック
  std::vector<AstStmtVec> mStmtListStack;

  // CaseItemList のスタック
  std::vector<AstCaseItemVec> mCaseItemListStack;

  // ExprList のスタック
  std::vector<AstExprVec> mExprListStack;

};

END_NAMESPACE_YM_VERILOG

#endif // PARSER_H
