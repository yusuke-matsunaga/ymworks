#ifndef ELBFACTORY_H
#define ELBFACTORY_H

/// @file ElbFactory.h
/// @brief ElbFactory のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/vl/Ast.h"
#include "ym/clib.h"
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

class RangeVal;

//////////////////////////////////////////////////////////////////////
/// @class ElbFactory ElbFactory "ElbFactory.h"
/// @brief ElbObj の派生クラスを生成するファクトリクラス
//////////////////////////////////////////////////////////////////////
class ElbFactory
{
public:

  /// @brief オブジェクトを生成する関数
  static
  ElbFactory*
  new_obj();

  /// @brief デストラクタ
  virtual
  ~ElbFactory() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 管理用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内訳を表示する．
  virtual
  void
  dump_prof(
    std::ostream& s ///< [in] 出力ストリーム
  ) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // インスタンス生成関係のメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief トップレベルのスコープを生成する
  virtual
  const VlScope*
  new_Toplevel() = 0;

  /// @brief ステートメントブロックのスコープを生成する．
  virtual
  const VlScope*
  new_StmtBlockScope(
    const VlScope* parent,  ///< [in] 親のスコープ環境
    const AstStmt& ast_stmt ///< [in] 対応するパース木の要素
  ) = 0;

  /// @brief generate block を生成する．
  virtual
  const VlScope*
  new_GenBlock(
    const VlScope* parent,  ///< [in] 親のスコープ環境
    const AstItem& ast_item ///< [in] 対応するパース木の要素
  ) = 0;

  /// @brief GfBlock 検索用の親の名前付きオブジェクトを作る．
  virtual
  ElbGfRoot*
  new_GfRoot(
    const VlScope* parent,  ///< [in] 親のスコープ環境
    const AstItem& ast_item ///< [in] 対応するパース木の要素
  ) = 0;

  /// @brief generate for block を生成する．
  virtual
  const VlScope*
  new_GfBlock(
    const VlScope* parent,   ///< [in] 親のスコープ環境
    const AstItem& ast_item, ///< [in] 対応するパース木の要素
    int gvi                  ///< [in] 対応する genvar の値
  ) = 0;

  /// @brief UDP定義を生成する．
  virtual
  ElbUdpDefn*
  new_UdpDefn(
    const AstUdp& ast_udp, ///< [in] パース木の UDP 定義
    bool is_protected      ///< [in] プロテクト属性
  ) = 0;

  /// @brief module を生成する．
  virtual
  ElbModule*
  new_Module(
    const VlScope* parent,       ///< [in] 親のスコープ
    const AstModule& ast_module, ///< [in] モジュールテンプレート
    const AstItem& ast_head,     ///< [in] パース木のヘッダ定義
    const AstInst& ast_inst      ///< [in] パース木のインスタンス定義
  ) = 0;

  /// @brief module array を生成する．
  virtual
  ElbModuleArray*
  new_ModuleArray(
    const VlScope* parent,       ///< [in] 親のスコープ
    const AstModule& ast_module, ///< [in] モジュールテンプレート
    const AstItem& ast_head,     ///< [in] パース木のヘッダ定義
    const AstInst& ast_inst,     ///< [in] パース木のインスタンス定義
    const AstRange& ast_range,   ///< [in] パース木の範囲定義
    const RangeVal& range        ///< [in] 範囲の値
  ) = 0;

  /// @brief module IO ヘッダを生成する．
  virtual
  ElbIOHead*
  new_IOHead(
    const VlModule* module,     ///< [in] 親のモジュール
    const AstIOHead& ast_header ///< [in] パース木のIO宣言ヘッダ
  ) = 0;

  /// @brief タスク/関数 IO ヘッダを生成する．
  virtual
  ElbIOHead*
  new_IOHead(
    const VlTaskFunc* module,   ///< [in] 親のタスク/関数
    const AstIOHead& ast_header ///< [in] パース木のIO宣言ヘッダ
  ) = 0;

  /// @brief 宣言要素のヘッダを生成する．
  virtual
  ElbDeclHead*
  new_DeclHead(
    const VlScope* parent,       ///< [in] 親のスコープ
    const AstDeclHead& ast_head, ///< [in] パース木の宣言ヘッダ
    bool has_delay = false       ///< [in] 遅延値を持つとき true
  ) = 0;

  /// @brief 宣言要素のヘッダを生成する．
  virtual
  ElbDeclHead*
  new_DeclHead(
    const VlScope* parent,       ///< [in] 親のスコープ
    const AstDeclHead& ast_head, ///< [in] パース木の宣言ヘッダ
    const AstRange& ast_range,   ///< [in] パース木の範囲定義
    const RangeVal& range,       ///< [in] 範囲の値
    bool has_delay = false       ///< [in] 遅延値を持つとき true
  ) = 0;

  /// @brief 宣言要素のヘッダを生成する．(IODecl 中の宣言用)
  virtual
  ElbDeclHead*
  new_DeclHead(
    const VlScope* parent,     ///< [in] 親のスコープ
    const AstIOHead& ast_head, ///< [in] パース木のIO宣言ヘッダ
    VpiAuxType aux_type        ///< [in] 補助的なデータ型
  ) = 0;

  /// @brief 宣言要素のヘッダを生成する．(IODecl 中の宣言用)
  virtual
  ElbDeclHead*
  new_DeclHead(
    const VlScope* parent,     ///< [in] 親のスコープ
    const AstIOHead& ast_head, ///< [in] パース木のIO宣言ヘッダ
    VpiAuxType aux_type,       ///< [in] 補助的なデータ型
    const AstRange& ast_range, ///< [in] パース木の範囲定義
    const RangeVal& range      ///< [in] 範囲の値
  ) = 0;

  /// @brief 宣言要素のヘッダを生成する．(function の暗黙宣言用)
  virtual
  ElbDeclHead*
  new_DeclHead(
    const VlScope* parent,  ///< [in] 親のスコープ
    const AstItem& ast_item ///< [in] パース木の関数定義
  ) = 0;

  /// @brief 宣言要素のヘッダを生成する．(function の暗黙宣言用)
  virtual
  ElbDeclHead*
  new_DeclHead(
    const VlScope* parent,     ///< [in] 親のスコープ
    const AstItem& ast_item,   ///< [in] パース木の関数定義
    const AstRange& ast_range, ///< [in] パース木の範囲定義
    const RangeVal& range      ///< [in] 範囲の値
  ) = 0;

  /// @brief 宣言要素を生成する．
  virtual
  ElbDecl*
  new_Decl(
    ElbDeclHead* head,            ///< [in] ヘッダ
    const AstNamedBase& ast_item, ///< [in] パース木の宣言要素
    const VlExpr* init = nullptr  ///< [in] 初期値
  ) = 0;

  /// @brief 暗黙のネットを生成する．
  virtual
  ElbDecl*
  new_ImpNet(
    const VlScope* parent,   ///< [in] 親のスコープ
    const AstExpr& ast_expr, ///< [in] パース木のプライマリ式
    VpiNetType net_type      ///< [in] ネットの型
  ) = 0;

  /// @brief 宣言要素の配列を生成する．
  virtual
  const VlDeclArray*
  new_DeclArray(
    ElbDeclHead* head,                        ///< [in] ヘッダ
    const AstNamedBase& ast_item,             ///< [in] パース木の宣言要素
    const std::vector<ElbRangeSrc>& range_src ///< [in] 範囲の配列
  ) = 0;

  /// @brief parameter 宣言のヘッダを生成する(範囲指定なし)．
  virtual
  ElbParamHead*
  new_ParamHead(
    const VlScope* parent,      ///< [in] 親のスコープ
    const AstDeclHead& ast_head ///< [in] パース木の宣言ヘッダ
  ) = 0;

  /// @brief parameter 宣言のヘッダを生成する(範囲指定あり)．
  virtual
  ElbParamHead*
  new_ParamHead(
    const VlScope* parent,       ///< [in] 親のスコープ
    const AstDeclHead& ast_head, ///< [in] パース木の宣言ヘッダ
    const AstRange& ast_range,   ///< [in] パース木の範囲定義
   const RangeVal& range         ///< [in] 範囲の値
  ) = 0;

  /// @brief parameter 宣言を生成する．
  virtual
  ElbParameter*
  new_Parameter(
    ElbParamHead* head,           ///< [in] ヘッダ
    const AstNamedBase& ast_item, ///< [in] パース木の宣言要素
    bool is_local = false         ///< [in] localparam の時 true
  ) = 0;

  /// @brief genvar を生成する．
  virtual
  ElbGenvar*
  new_Genvar(
    const VlScope* parent,       ///< [in] 親のスコープ環境
    const AstDeclItem& ast_item, ///< [in] 対応するパース木の要素
    int val                      ///< [in] 初期値
  ) = 0;

  /// @brief continuous assignment のヘッダを生成する．
  ///
  /// 遅延を持たないときは nullptr を与える．
  virtual
  ElbCaHead*
  new_CaHead(
    const VlModule* module,        ///< [in] 親のモジュール
    const AstItem& ast_head,       ///< [in] パース木のヘッダ定義
    const VlDelay* delay = nullptr ///< [in] 遅延値
  ) = 0;

  /// @brief continuous assignment を生成する．
  virtual
  const VlContAssign*
  new_ContAssign(
    ElbCaHead* head,        ///< [in] ヘッダ
    const AstBase& ast_obj, ///< [in] 対応するパース木中の定義要素
    const VlExpr* lhs,      ///< [in] 左辺式
    const VlExpr* rhs       ///< [in] 右辺式
  ) = 0;

  /// @brief net 宣言中の continuous assignment を生成する．
  virtual
  const VlContAssign*
  new_ContAssign(
    const VlModule* module, ///< [in] 親のモジュール
    const AstBase& ast_obj, ///< [in] パース木の定義要素
    const VlExpr* lhs,      ///< [in] 左辺式
    const VlExpr* rhs       ///< [in] 右辺式
  ) = 0;

  /// @brief パラメータ割り当て文を生成する．
  virtual
  const VlParamAssign*
  new_ParamAssign(
    const VlModule* module,  ///< [in] 親のモジュール
    const AstBase& ast_obj,  ///< [in] パース木の定義要素
    ElbParameter* param,     ///< [in] 対象の parameter
    const AstExpr& rhs_expr, ///< [in] 割り当て式の右辺
    const VlValue& rhs_value ///< [in] 右辺の値
  ) = 0;

  /// @brief 名前によるパラメータ割り当て文を生成する．
  virtual
  const VlParamAssign*
  new_NamedParamAssign(
    const VlModule* module,  ///< [in] 親のモジュール
    const AstBase& ast_obj,  ///< [in] パース木の定義要素
    ElbParameter* param,     ///< [in] 対象の parameter
    const AstExpr& rhs_expr, ///< [in] 割り当て式の右辺
    const VlValue& rhs_value ///< [in] 右辺の値
  ) = 0;

  /// @brief defparam 文を生成する．
  virtual
  const VlDefParam*
  new_DefParam(
    const VlModule* module,          ///< [in] 親のモジュール
    const AstItem& ast_header,       ///< [in] パース木の defparam ヘッダ
    const AstDefParam& ast_defparam, ///< [in] パース木の defparam 定義
    ElbParameter* param,             ///< [in] 対象の parameter
    const AstExpr& rhs_expr,         ///< [in] 割り当て式の右辺式
    const VlValue& rhs_value         ///< [in] 右辺の値
  ) = 0;

  /// @brief ゲートプリミティブのヘッダを生成する．
  virtual
  ElbPrimHead*
  new_PrimHead(
    const VlScope* parent,     ///< [in] 親のスコープ
    const AstItem& ast_header, ///< [in] パース木の定義
    bool has_delay             ///< [in] 遅延値を持つとき true
  ) = 0;

  /// @brief UDPプリミティブのヘッダを生成する．
  virtual
  ElbPrimHead*
  new_UdpHead(
    const VlScope* parent,     ///< [in] 親のスコープ
    const AstItem& ast_header, ///< [in] パース木の定義
    const VlUdpDefn* udp,      ///< [in] 定義
    bool has_delay             ///< [in] 遅延値を持つとき true
  ) = 0;

  /// @brief セルプリミティブのヘッダを生成する．
  virtual
  ElbPrimHead*
  new_CellHead(
    const VlScope* parent,     ///< [in] 親のスコープ
    const AstItem& ast_header, ///< [in] パース木の定義
    const ClibCell& cell       ///< [in] セル
  ) = 0;

  /// @brief プリミティブインスタンスを生成する．
  virtual
  ElbPrimitive*
  new_Primitive(
    ElbPrimHead* head,      ///< [in] ヘッダ
    const AstInst& ast_inst ///< [in] インスタンス定義
  ) = 0;

  /// @brief プリミティブ配列インスタンスを生成する．
  virtual
  ElbPrimArray*
  new_PrimitiveArray(
    ElbPrimHead* head,         ///< [in] インスタンス定義
    const AstInst& ast_inst,   ///< [in] パース木の定義
    const AstRange& ast_range, ///< [in] パース木の範囲定義
    const RangeVal& range      ///< [in] 範囲の値
  ) = 0;

  /// @brief セルプリミティブインスタンスを生成する．
  virtual
  ElbPrimitive*
  new_CellPrimitive(
    ElbPrimHead* head,      ///< [in] ヘッダ
    const ClibCell& cell,   ///< [in] セル
    const AstInst& ast_inst ///< [in] インスタンス定義
  ) = 0;

  /// @brief セルプリミティブ配列インスタンスを生成する．
  virtual
  ElbPrimArray*
  new_CellPrimitiveArray(
    ElbPrimHead* head,         ///< [in] ヘッダ
    const ClibCell& cell,      ///< [in] セル
    const AstInst& ast_inst,   ///< [in] インスタンス定義
    const AstRange& ast_range, ///< [in] パース木の範囲定義
    const RangeVal& range      ///< [in] 範囲の値
  ) = 0;

  /// @brief function を生成する．
  virtual
  ElbTaskFunc*
  new_Function(
    const VlScope* parent,   ///< [in] 親のスコープ
    const AstItem& ast_item, ///< [in] パース木の定義
    bool const_func          ///< [in] 定数関数フラグ
  ) = 0;

  /// @brief function を生成する．
  virtual
  ElbTaskFunc*
  new_Function(
    const VlScope* parent,     ///< [in] 親のスコープ
    const AstItem& ast_item,   ///< [in] パース木の定義
    const AstRange& ast_range, ///< [in] パース木の範囲定義
    const RangeVal& range,     ///< [in] 範囲の値
    bool const_func            ///< [in] 定数関数フラグ
  ) = 0;

  /// @brief task を生成する．
  virtual
  ElbTaskFunc*
  new_Task(
    const VlScope* parent,  ///< [in] 親のスコープ
    const AstItem& ast_item ///< [in] パース木の定義
  ) = 0;

  /// @brief プロセス文を生成する．
  virtual
  ElbProcess*
  new_Process(
    const VlScope* parent,  ///< [in] 親のスコープ
    const AstItem& ast_item ///< [in] パース木の要素定義
  ) = 0;

  /// @brief 代入文を生成する．
  virtual
  const VlStmt*
  new_Assignment(
    const VlScope* parent,             ///< [in] 親のスコープ
    const VlProcess* process,          ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,           ///< [in] パース木のステートメント定義
    const VlExpr* lhs,                 ///< [in] 左辺の式
    const VlExpr* rhs,                 ///< [in] 右辺の式
    bool block,                        ///< [in] ブロッキング代入の時 true
    const VlControl* control = nullptr ///< [in] コントロール
  ) = 0;

  /// @brief assign ステートメントを生成する．
  virtual
  const VlStmt*
  new_AssignStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,  ///< [in] パース木のステートメント定義
    const VlExpr* lhs,        ///< [in] 左辺の式
    const VlExpr* rhs         ///< [in] 右辺の式
  ) = 0;

  /// @brief deassign ステートメントを生成する．
  virtual
  const VlStmt*
  new_DeassignStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,  ///< [in] パース木のステートメント定義
    const VlExpr* lhs         ///< [in] 左辺の式
  ) = 0;

  /// @brief force ステートメントを生成する．
  virtual
  const VlStmt*
  new_ForceStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,  ///< [in] パース木のステートメント定義
    const VlExpr* lhs,        ///< [in] 左辺の式
    const VlExpr* rhs         ///< [in] 右辺の式
  ) = 0;

  /// @brief release ステートメントを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス (or nullptr)
  /// @param[in] ast_stmt パース木のステートメント定義
  /// @param[in] lhs 左辺の式
  virtual
  const VlStmt*
  new_ReleaseStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,  ///< [in] パース木のステートメント定義
    const VlExpr* lhs	      ///< [in] 左辺の式
  ) = 0;

  /// @brief begin ブロックを生成する．
  virtual
  const VlStmt*
  new_Begin(
    const VlScope* parent,                 ///< [in] 親のスコープ
    const VlProcess* process,              ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,               ///< [in] パース木のステートメント定義
    const std::vector<const VlStmt*>& stmt_list ///< [in] 子のステートメントリスト
  ) = 0;

  /// @brief fork ブロックを生成する．
  virtual
  const VlStmt*
  new_Fork(
    const VlScope* parent,                 ///< [in] 親のスコープ
    const VlProcess* process,		   ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,		   ///< [in] パース木のステートメント定義
    const std::vector<const VlStmt*>& stmt_list ///< [in] 子のステートメントリスト
  ) = 0;

  /// @brief 名前付き begin ブロックを生成する．
  virtual
  const VlStmt*
  new_NamedBegin(
    const VlScope* block,                  ///< [in] 親のスコープ
    const VlProcess* process,		   ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,		   ///< [in] パース木のステートメント定義
    const std::vector<const VlStmt*>& stmt_list ///< [in] 子のステートメントリスト
  ) = 0;


  /// @brief 名前付き fork ブロックを生成する．
  virtual
  const VlStmt*
  new_NamedFork(
    const VlScope* block,                  ///< [in] 親のスコープ
    const VlProcess* process,		   ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,		   ///< [in] パース木のステートメント定義
    const std::vector<const VlStmt*>& stmt_list ///< [in] 子のステートメントリスト
  ) = 0;

  /// @brief while 文を生成する．
  virtual
  const VlStmt*
  new_WhileStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,  ///< [in] パース木のステートメント定義
    const VlExpr* cond,       ///< [in] 条件式
    const VlStmt* stmt        ///< [in] 本体のステートメント
  ) = 0;

  /// @brief repeat 文を生成する．
  virtual
  const VlStmt*
  new_RepeatStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,  ///< [in] パース木のステートメント定義
    const VlExpr* cond,	      ///< [in] 条件式
    const VlStmt* stmt	      ///< [in] 本体のステートメント
  ) = 0;

  /// @brief wait 文を生成する．
  virtual
  const VlStmt*
  new_WaitStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,  ///< [in] パース木のステートメント定義
    const VlExpr* cond,	      ///< [in] 条件式
    const VlStmt* stmt	      ///< [in] 本体のステートメント
  ) = 0;

  /// @brief for 文を生成する．
  virtual
  const VlStmt*
  new_ForStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,  ///< [in] パース木のステートメント定義
    const VlExpr* cond,	      ///< [in] 条件式
    const VlStmt* init_stmt,  ///< [in] 初期化式
    const VlStmt* inc_stmt,   ///< [in] 増加式
    const VlStmt* stmt        ///< [in] 本体のステートメント
  ) = 0;

  /// @brief forever 文を生成する．
  virtual
  const VlStmt*
  new_ForeverStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,  ///< [in] パース木のステートメント定義
    const VlStmt* stmt        ///< [in] 本体のステートメント
  ) = 0;

  /// @brief if 文を生成する．
  ///
  /// else_stmt は nullptr もありうる．
  virtual
  const VlStmt*
  new_IfStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,  ///< [in] パース木のステートメント定義
    const VlExpr* cond,       ///< [in] 条件式
    const VlStmt* stmt,       ///< [in] 本体のステートメント
    const VlStmt* else_stmt   ///< [in] else節のステートメント
  ) = 0;

  /// @brief case 文を生成する．
  virtual
  const VlStmt*
  new_CaseStmt(
    const VlScope* parent,        ///< [in] 親のスコープ
    const VlProcess* process,     ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,      ///< [in] パース木のステートメント定義
    const VlExpr* expr,           ///< [in] 条件式
    const std::vector<const VlCaseItem*>& caseitem_list ///< [in] caseitem のリスト
  ) = 0;

  /// @brief caseitem を生成する．
  virtual
  const VlCaseItem*
  new_CaseItem(
    const AstCaseItem& ast_item,        ///< [in] パース木の caseitem 要素
    const std::vector<ElbExpr*>& label_list, ///< [in] ラベルを表す式のリスト
    const VlStmt* body                  ///< [in] 本体のステートメント
  ) = 0;

  /// @brief イベント文を生成する．
  virtual
  const VlStmt*
  new_EventStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,  ///< [in] パース木のステートメント定義
    ElbExpr* named_event      ///< [in] 対象のイベント
  ) = 0;

  /// @brief null ステートメントを生成する．
  virtual
  const VlStmt*
  new_NullStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt   ///< [in] パース木のステートメント定義
  ) = 0;

  /// @brief タスクコール文を生成する．
  virtual
  const VlStmt*
  new_TaskCall(
    const VlScope* parent,            ///< [in]親のスコープ
    const VlProcess* process,         ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,          ///< [in] パース木のステートメント定義
    const VlTaskFunc* task,           ///< [in] 対象のタスク
    const std::vector<ElbExpr*>& arg_array ///< [in] 引数の配列
  ) = 0;

  /// @brief システムタスクコール文を生成する．
  virtual
  const VlStmt*
  new_SysTaskCall(
    const VlScope* parent,            ///< [in] 親のスコープ
    const VlProcess* process,         ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,          ///< [in] パース木のステートメント定義
    const VlUserSystf* user_systf,    ///< [in] システムタスク
    const std::vector<ElbExpr*>& arg_array ///< [in] 引数の配列
  ) = 0;

  /// @brief disable 文を生成する．
  virtual
  const VlStmt*
  new_DisableStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,  ///< [in] パース木のステートメント定義
    const VlScope* target     ///< [in] 対象のスコープ
  ) = 0;

  /// @brief コントロール文を生成する．
  virtual
  const VlStmt*
  new_CtrlStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,  ///< [in] パース木のステートメント定義
    const VlControl* control, ///< [in] コントロール
    const VlStmt* stmt        ///< [in] 本体のステートメント
  ) = 0;

  /// @brief 遅延コントロールを生成する．
  virtual
  const VlControl*
  new_DelayControl(
    const AstControl& ast_control, ///< [in] パース木の定義要素
    ElbExpr* delay                 ///< [in] 遅延式
  ) = 0;

  /// @brief イベントコントロールを生成する．
  virtual
  const VlControl*
  new_EventControl(
    const AstControl& ast_control,          ///< [in] パース木の定義要素
    const std::vector<ElbExpr*>& event_list ///< [in] イベントリストを表す配列
  ) = 0;

  /// @brief リピートコントロールを生成する．
  virtual
  const VlControl*
  new_RepeatControl(
    const AstControl& ast_control,     ///< [in] パース木の定義要素
    ElbExpr* rep,                      ///< [in] 繰り返し式
    const std::vector<ElbExpr*>& event_list ///< [in] イベントリストを表す配列
  ) = 0;

  /// @brief 単項演算子を生成する．
  virtual
  ElbExpr*
  new_UnaryOp(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    VpiOpType op_type,       ///< [in] 演算子のタイプ
    ElbExpr* opr1            ///< [in] オペランド
  ) = 0;

  /// @brief 2項演算子を生成する．
  virtual
  ElbExpr*
  new_BinaryOp(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    VpiOpType op_type,       ///< [in] 演算子のタイプ
    ElbExpr* opr1,           ///< [in] 第一オペランド
    ElbExpr* opr2            ///< [in] 第二オペランド
  ) = 0;

  /// @brief 3項演算子を生成する．
  virtual
  ElbExpr*
  new_TernaryOp(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    VpiOpType op_type,	     ///< [in] 演算子のタイプ
    ElbExpr* opr1,	     ///< [in] 第一オペランド
    ElbExpr* opr2,	     ///< [in] 第二オペランド
    ElbExpr* opr3            ///< [in] 第三オペランド
  ) = 0;

  /// @brief 連結演算子を生成する．
  virtual
  ElbExpr*
  new_ConcatOp(
    const AstExpr& ast_expr,              ///< [in] パース木の定義要素
    const std::vector<ElbExpr*>& opr_list ///< [in] オペランドのリスト
  ) = 0;

  /// @brief 反復連結演算子を生成する．
  virtual
  ElbExpr*
  new_MultiConcatOp(
    const AstExpr& ast_expr,         ///< [in] パース木の定義要素
    SizeType rep_num,                ///< [in] 繰り返し数を表す式
    ElbExpr* rep_expr,               ///< [in] 繰り返し数
    const std::vector<ElbExpr*>& opr_list ///< [in] オペランドのリスト
  ) = 0;

  /// @brief プライマリ式を生成する．
  virtual
  ElbExpr*
  new_Primary(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDecl* obj        ///< [in] 本体のオブジェクト
  ) = 0;

  /// @brief プライマリ式を生成する(net decl の初期値用)．
  virtual
  ElbExpr*
  new_Primary(
    const AstDeclItem& ast_item, ///< [in] パース木の定義要素
    const VlDecl* obj            ///< [in] 本体のオブジェクト
  ) = 0;

  /// @brief プライマリ式を生成する．
  virtual
  ElbExpr*
  new_Primary(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    ElbParameter* obj        ///< [in] 本体のオブジェクト
  ) = 0;

  /// @brief プライマリ式を生成する(配列要素版)．
  virtual
  ElbExpr*
  new_Primary(
    const AstExpr& ast_expr,           ///< [in] パース木の定義要素
    const VlDeclArray* obj,            ///< [in] 本体のオブジェクト
    const std::vector<ElbExpr*>& index_list ///< [in] インデックスのリスト
  ) = 0;

  /// @brief プライマリ式を生成する(固定インデックスの配列要素版)．
  virtual
  ElbExpr*
  new_Primary(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDeclArray* obj,  ///< [in] 本体のオブジェクト
    SizeType offset          ///< [in] オフセット
  ) = 0;

  /// @brief 固定ビット選択式を生成する．
  virtual
  ElbExpr*
  new_BitSelect(
    const AstExpr& ast_expr,  ///< [in] パース木の定義要素
    ElbExpr* expr,            ///< [in] 本体のオブジェクト
    const AstExpr& bit_index, ///< [in] ビット選択式
    int bit_index_val         ///< [in] ビット選択式の値
  ) = 0;

  /// @brief 固定ビット選択式を生成する．
  virtual
  ElbExpr*
  new_BitSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    ElbExpr* expr,           ///< [in] 本体の式
    int bit_index_val        ///< [in] ビット選択式の値
  ) = 0;

  /// @brief 可変ビット選択式を生成する．
  virtual
  ElbExpr*
  new_BitSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    ElbExpr* expr,           ///< [in] 本体のオブジェクト
    ElbExpr* bit_index       ///< [in] ビット選択式
  ) = 0;

  /// @brief 固定部分選択式を生成する．
  virtual
  ElbExpr*
  new_PartSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    ElbExpr* obj,            ///< [in] 本体のオブジェクト
    const AstExpr& index1,   ///< [in] パート選択式のMSB
    const AstExpr& index2,   ///< [in] パート選択式のLSB
    int index1_val,          ///< [in] パート選択式のMSBの値
    int index2_val           ///< [in] パート選択式のLSBの値
  ) = 0;

  /// @brief 固定部分選択式を生成する．
  virtual
  ElbExpr*
  new_PartSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    ElbExpr* expr,           ///< [in] 本体の式
    int index1,              ///< [in] パート選択式のMSB
    int index2               ///< [in] パート選択式のLSB
  ) = 0;

  /// @brief 可変部分選択式を生成する．
  virtual
  ElbExpr*
  new_PlusPartSelect(
    const AstExpr& ast_expr,   ///< [in] パース木の定義要素
    ElbExpr* obj,              ///< [in] 本体のオブジェクト
    ElbExpr* base,             ///< [in] 範囲のベースアドレスを表す式
    const AstExpr& range_expr, ///< [in] 範囲を表す式
    SizeType range_val         ///< [in] 範囲の値
  ) = 0;

  /// @brief 可変部分選択式を生成する．
  virtual
  ElbExpr*
  new_MinusPartSelect(
    const AstExpr& ast_expr,   ///< [in] パース木の定義要素
    ElbExpr* obj,              ///< [in] 本体のオブジェクト
    ElbExpr* base,             ///< [in] 範囲のベースアドレスを表す式
    const AstExpr& range_expr, ///< [in] 範囲を表す式
    SizeType range_val         ///< [in] 範囲の値
  ) = 0;

  /// @brief 定数式を生成する．
  virtual
  ElbExpr*
  new_Constant(
    const AstExpr& ast_expr ///< [in] パース木の定義要素
  ) = 0;

  /// @brief genvar 起因の定数式を生成する．
  virtual
  ElbExpr*
  new_GenvarConstant(
    const AstExpr& ast_primary, ///< [in] パース木の定義要素
    int val                     ///< [in] 値
  ) = 0;

  /// @brief 関数呼び出し式を生成する．
  virtual
  ElbExpr*
  new_FuncCall(
    const AstExpr& ast_expr,         ///< [in] パース木の定義要素
    const VlTaskFunc* func,          ///< [in] 関数
    const std::vector<ElbExpr*>& arg_list ///< [in] 引数のリスト
  ) = 0;

  /// @brief システム関数呼び出し式を生成する．
  virtual
  ElbExpr*
  new_SysFuncCall(
    const AstExpr& ast_expr,         ///< [in] パース木の定義要素
    const VlUserSystf* user_systf,   ///< [in] システム関数
    const std::vector<ElbExpr*>& arg_list ///< [in] 引数のリスト
  ) = 0;

  /// @brief システム関数/システムタスクの引数を生成する．
  virtual
  ElbExpr*
  new_ArgHandle(
    const AstExpr& ast_expr, ///< [in] パース木中で参照している要素
    const VlScope* arg       ///< [in] 引数本体(スコープ)
  ) = 0;

  /// @brief システム関数/システムタスクの引数を生成する．
  virtual
  ElbExpr*
  new_ArgHandle(
    const AstExpr& ast_expr, ///< [in] パース木中で参照している要素
    const VlPrimitive* arg   ///< [in] 引数本体(プリミティブ)
  ) = 0;

  /// @brief システム関数/システムタスクの引数を生成する．
  virtual
  ElbExpr*
  new_ArgHandle(
    const AstExpr& ast_expr, ///< [in] パース木中で参照している要素
    const VlDeclArray* arg   ///< [in] 引数本体(配列宣言)
  ) = 0;

  /// @brief 連結演算子の左辺式を生成する．
  virtual
  ElbExpr*
  new_Lhs(
    const AstExpr& ast_expr,                    ///< [in] パース木の定義要素
    const std::vector<ElbExpr*>& opr_array,     ///< [in] オペランドを格納する配列
    const std::vector<ElbExpr*>& lhs_elem_array ///< [in] 左辺の要素の配列
  ) = 0;

  /// @brief 遅延値を生成する．
  virtual
  const VlDelay*
  new_Delay(
    const AstBase& ast_obj,                ///< [in] パース木の定義要素
    const std::vector<ElbExpr*>& expr_list ///< [in] 式の配列
  ) = 0;

  /// @brief attribute instance のリストを生成する．
  virtual
  const VlAttribute*
  new_Attribute(
    const AstAttrSpec& ast_attr, ///< [in] パース木の定義要素
    const VlExpr* expr,          ///< [in] 値
    bool def                     ///< [in] 定義側の属性の時 true
  ) = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBFACTORY_H
