#ifndef DECLGEN_H
#define DECLGEN_H

/// @file DeclGen.h
/// @brief DeclGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/vl/Ast.h"
#include "ElbProxy.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class DeclGen DeclGen.h "DeclGen.h"
/// @brief ElbDecl を生成するクラス
//////////////////////////////////////////////////////////////////////
class DeclGen :
  public ElbProxy
{
public:

  /// @brief コンストラクタ
  DeclGen(
    Elaborator& elab, ///< [in] 生成器
    ElbMgr& elb_mgr   ///< [in] Elbオブジェクトを管理するクラス
  );

  /// @brief デストラクタ
  ~DeclGen();


public:
  //////////////////////////////////////////////////////////////////////
  // DeclGen の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief parameter と genvar を実体化する．
  void
  phase1_decl(
    const VlScope* parent,  ///< [in] 親のスコープ
    const AstDeclHeadList& ast_head_list, ///< [in] 宣言ヘッダのリスト
    bool force_to_local  ///< [in] true なら parameter を localparam にする．
  );

  /// @brief IO宣言要素を実体化する．
  ///
  /// module と taskfunc はどちらか一方が nullptr
  void
  instantiate_iodecl(
    ElbModule* module,      ///< [in] 親のモジュール
    ElbTaskFunc* taskfunc,  ///< [in] 親のタスク/関数
    const AstIOHeadList& ast_head_array ///< [in] IO宣言ヘッダのリスト
  );

  /// @brief 宣言要素のリストをインスタンス化する．
  void
  instantiate_decl(
    const VlScope* parent,  ///< [in] 親のスコープ
    const AstDeclHeadList& ast_head_array ///< [in] 宣言ヘッダのリスト
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パラメータ用の instantiate 関数
  void
  instantiate_param_head(
    const VlScope* parent,       ///< [in] 親のスコープ
    const AstDeclHead& ast_head, ///< [in] 宣言ヘッダ
    bool is_local                ///< [in] local_param にする時 true
  );

  /// @brief net をインスタンス化する．
  void
  instantiate_net_head(
    const VlScope* parent,      ///< [in] 親のスコープ
    const AstDeclHead& ast_head ///< [in] 宣言のヘッダ
  );

  /// @brief link_net_delay() を起動するスタブを作る．
  ElbStub*
  make_net_delay_stub(
    ElbDeclHead* net_head,    ///< [in] ネットのヘッダ
    const AstDelay& ast_delay ///< [in] パース木の遅延式定義
  );

  /// @brief net の遅延値を生成する．
  void
  link_net_delay(
    ElbDeclHead* net_head,    ///< [in] ネットのヘッダ
    const AstDelay& ast_delay ///< [in] パース木の遅延式定義
  );

  /// @brief link_net_delay() を起動するスタブを作る．
  ElbStub*
  make_net_assign_stub(
    ElbDecl* net,               ///< [in] ネットのヘッダ
    const AstDeclItem& ast_item ///< [in] パース木の遅延式定義
  );

  /// @brief net の初期値を生成する．
  void
  link_net_assign(
    ElbDecl* net,               ///< [in] ネット
    const AstDeclItem& ast_item ///< [in] パース木のネット定義要素
  );

  /// @brief reg をインスタンス化する．
  void
  instantiate_reg_head(
    const VlScope* parent,      ///< [in] 親のスコープ
    const AstDeclHead& ast_head ///< [in] 宣言のヘッダ
  );

  /// @brief variable をインスタンス化する．
  void
  instantiate_var_head(
    const VlScope* parent,      ///< [in] 親のスコープ
    const AstDeclHead& ast_head ///< [in] 宣言のヘッダ
  );

  /// @brief named_event をインスタンス化する．
  void
  instantiate_event_head(
    const VlScope* parent,      ///< [in] 親のスコープ
    const AstDeclHead& ast_head ///< [in] 宣言のヘッダ
  );

  /// @brief genvar をインスタンス化する．
  void
  instantiate_genvar_head(
    const VlScope* parent,      ///< [in] 親のスコープ
    const AstDeclHead& ast_head ///< [in] 宣言のヘッダ
  );

  /// @brief 配列の次元リストを生成する．
  bool
  instantiate_dimension_list(
    const VlScope* parent,              ///< [in] 親のスコープ
    const AstDeclItem& ast_item,        ///< [in] 要素定義
    std::vector<ElbRangeSrc>& range_src ///< [in] 範囲の情報を設定する配列
  );

};

END_NAMESPACE_YM_VERILOG

#endif // DECLGEN_H
