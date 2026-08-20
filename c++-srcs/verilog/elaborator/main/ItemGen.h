#ifndef ITEMGEN_H
#define ITEMGEN_H

/// @file ItemGen.h
/// @brief ItemGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/vl/Ast.h"
#include "ym/clib.h"
#include "ElbProxy.h"
#include "elaborator/ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

struct DefParamStub;

//////////////////////////////////////////////////////////////////////
/// @class ItemGen ItemGen.h "ItemGen.h"
/// @brief 構成要素を生成するクラス
//////////////////////////////////////////////////////////////////////
class ItemGen :
  public ElbProxy
{
public:

  /// @brief コンストラクタ
  ItemGen(
    Elaborator& elab, ///< [in] 生成器
    ElbMgr& elb_mgr   ///< [in] Elbオブジェクトを管理するクラス
  );

  /// @brief デストラクタ
  ~ItemGen();


public:
  //////////////////////////////////////////////////////////////////////
  // ItemGen の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief スコープに関係する要素を実体化する．
  void
  phase1_items(
    const VlScope* parent,           ///< [in] 親のスコープ
    const std::vector<const AstItem*>& ast_item_list ///< [in] 要素定義のリスト
  );

  /// @brief defparam 文によるパラメータ割り当てを行う．
  /// @return 該当するパラメータが見つかったら true を返す．
  bool
  defparam_override(
    const DefParamStub& stub, ///< [in] defparam 文の情報
    const VlScope* ulimit     ///< [in] スコープの上限
  );

  /// @brief constant function の生成を行う．
  const VlTaskFunc*
  instantiate_constant_function(
    const VlScope* parent,    ///< [in] 親のスコープ
    const AstItem* ast_function ///< [in] 関数定義
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief スコープに関係する要素を実体化する．
  void
  phase1_item(
    const VlScope* parent, ///< [in] 親のスコープ
    const AstItem* ast_item  ///< [in] 要素定義
  );

  /// @brief continous assignment に関連した式の名前解決を行う．
  void
  instantiate_cont_assign(
    const VlScope* parent, ///< [in] 親のスコープ
    const AstItem* ast_head  ///< [in] ヘッダ
  );

  /// @brief process 文の生成を行う．
  void
  instantiate_process(
    const VlScope* parent, ///< [in] 親のスコープ
    const AstItem* ast_item  ///< [in] パース木の定義
  );

  /// @brief task/function の生成を行う．
  void
  phase1_tf(
    const VlScope* parent, ///< [in] 親のスコープ
    const AstItem* ast_tf    ///< [in] タスク/関数定義
  );

  /// @param[in] task/function 内の宣言要素の生成を行う．
  void
  phase2_tf(
    ElbTaskFunc* taskfunc, ///< [in] タスク/関数本体
    const AstItem* ast_item  ///< [in] パース木の関数定義
  );

  /// @param[in] task/function 内のステートメントの生成を行う．
  void
  phase3_tf(
    ElbTaskFunc* task,    ///< [in] タスク/関数本体
    const AstItem* ast_item ///< [in] パース木の関数定義
  );

  /// @brief module/Udp instance の生成を行う
  void
  phase1_muheader(
    const VlScope* parent, ///< [in] 親のスコープ
    const AstItem* ast_head  ///< [in] ヘッダ
  );

  /// @brief module instance の生成を行う．
  void
  phase1_module(
    const VlScope* parent,    ///< [in] 親のスコープ
    const AstItem* ast_head,    ///< [in] ヘッダ
    const AstModule* ast_module ///< [in] モジュールの構文木要素
  );

  /// @brief UDP instance の生成を行う．
  void
  phase1_udp(
    const VlScope* parent,   ///< [in] 親のスコープ
    const AstItem* ast_head,   ///< [in] ヘッダ
    const VlUdpDefn* udpdefn ///< [in] UDP
  );

  /// @brief cell instance の生成を行う．
  void
  phase1_cell(
    const VlScope* parent, ///< [in] 親のスコープ
    const AstItem* ast_head, ///< [in] ヘッダ
    ClibCell cell          ///< [in] セル番号
  );

  /// @brief module array のインスタンス化を行う．
  void
  phase1_module_array(
    const VlScope* parent,     ///< [in] 親のスコープ
    const AstModule* ast_module, ///< [in] モジュール定義
    const AstItem* ast_head,     ///< [in] ヘッダ
    const AstInst* ast_inst      ///< [in] インスタンス定義
  );

  /// @brief module array instance の入出力端子の接続を行う．
  void
  link_module_array(
    ElbModuleArray* module_array, ///< [in] モジュール配列
    const AstModule* ast_module,    ///< [in] モジュール定義
    const AstInst* ast_inst         ///< [in] インスタンス定義
  );

  /// @brief module instance の入出力端子の接続を行う．
  void
  link_module(
    ElbModule* module,         ///< [in] モジュール
    const AstModule* ast_module, ///< [in] モジュール定義
    const AstInst* ast_inst      ///< [in] インスタンス定義
  );

  /// @brief gate instance の生成を行う
  void
  instantiate_gateheader(
    const VlScope* parent, ///< [in] 親のスコープ
    const AstItem* ast_head  ///< [in] ヘッダ
  );

  /// @brief UDP instance の生成を行う
  void
  instantiate_udpheader(
    const VlScope* parent,   ///< [in] 親のスコープ
    const AstItem* ast_head,   ///< [in] ヘッダ
    const VlUdpDefn* udpdefn ///< [in] UDP
  );

  /// @brief セル instance の生成を行う
  void
  instantiate_cell(
    const VlScope* parent, ///< [in] 親のスコープ
    const AstItem* ast_head, ///< [in] ヘッダ
    ClibCell cell          ///< [in] セル
  );

  /// @brief gate delay の生成を行う
  void
  link_gate_delay(
    ElbPrimHead* prim_head, ///< [in] ゲートプリミティブのヘッダ
    const AstDelay* ast_delay ///< [in] パース木の遅延式
  );

  /// @brief UDP instance に付随する遅延式の生成を行う．
  void
  link_udp_delay(
    ElbPrimHead* prim_head, ///< [in] ゲートプリミティブのヘッダ
    const AstItem* ast_head   ///< [in] パース木のインスタンス定義ヘッダ
  );

  /// @brief gate array instance で使われている式の名前解決を行う．
  void
  link_prim_array(
    ElbPrimArray* prim_array, ///< [in] プリミティブ配列
    const AstInst* ast_inst     ///< [in] インスタンス定義
  );

  /// @brief gate instance で使われている式の名前解決を行う．
  void
  link_primitive(
    ElbPrimitive* primitive, ///< [in] プリミティブ配列
    const AstInst* ast_inst    ///< [in] インスタンス定義
  );

  /// @brief cell array instance で使われている式の名前解決を行う．
  void
  link_cell_array(
    ElbPrimArray* prim_array, ///< [in] プリミティブ配列
    const AstInst* ast_inst     ///< [in]インスタンス定義
  );

  /// @brief cell instance で使われている式の名前解決を行う．
  void
  link_cell(
    ElbPrimitive* primitive, ///< [in] プリミティブ配列
    const AstInst* ast_inst    ///< [in] インスタンス定義
  );

  /// @brief generate block を実際にインスタンス化を行う．
  void
  phase1_generate(
    const VlScope* parent,    ///< [in] 親のスコープ
    const AstItem* ast_genblock ///< [in] generate block 定義
  );

  /// @brief AstGenBlock に対応するインスタンスの生成を行う
  void
  phase1_genblock(
    const VlScope* parent,    ///< [in] 親のスコープ
    const AstItem* ast_genblock ///< [in] generate block 定義
  );

  /// @brief generate if に対応するインスタンスの生成を行う
  void
  phase1_genif(
    const VlScope* parent, ///< [in] 親のスコープ
    const AstItem* ast_genif ///< [in] generate if 定義
  );

  /// @brief generate case に対応するインスタンスの生成を行う
  void
  phase1_gencase(
    const VlScope* parent,   ///< [in] 親のスコープ
    const AstItem* ast_gencase ///< [in] generate case 定義
  );

  /// @brief generate for に対応するインスタンスの生成を行う
  void
  phase1_genfor(
    const VlScope* parent,  ///< [in] 親のスコープ
    const AstItem* ast_genfor ///< [in] generate for 定義
  );

  /// @brief generate block の要素でスコープに関連するものの生成を行う．
  ///
  /// と同時の残りの処理をキューに積む．
  void
  phase1_genitem(
    const VlScope* parent,                ///< [in] 親のスコープ
    const std::vector<const AstDeclHead*>& ast_decl_list, ///< [in] パース木の宣言の配列
    const std::vector<const AstItem*>& ast_item_list      ///< [in] パース木の要素の配列
  );

  /// @brief パラメータ割り当て情報を作る．
  std::vector<ElbParamCon>
  gen_param_con_list(
    const VlScope* parent, ///< [in] 親のスコープ
    const AstItem* ast_head  ///< [in] 構文木のヘッダ要素
  );

};

END_NAMESPACE_YM_VERILOG

#endif // ITEMGEN_H
