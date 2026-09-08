#ifndef MODULEGEN_H
#define MODULEGEN_H

/// @file ModuleGen.h
/// @brief ModuleGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/vl/Ast.h"
#include "ElbProxy.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ModuleGen ModuleGen.h "ModuleGen.h"
/// @brief ElbModule を生成するクラス
//////////////////////////////////////////////////////////////////////
class ModuleGen :
  public ElbProxy
{
public:

  /// @brief コンストラクタ
  ModuleGen(
    Elaborator& elab ///< [in] 生成器
  );

  /// @brief デストラクタ
  ~ModuleGen();


public:
  //////////////////////////////////////////////////////////////////////
  // ModuleGen の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief top module のインスタンス化を行う．
  void
  phase1_topmodule(
    const VlScope* toplevel,    ///< [in] トップレベルのスコープ
    const AstModule& ast_module ///< [in] モジュール定義
  );

  /// @brief module の中身のうちスコープに関係するインスタンス化を行う．
  void
  phase1_module_item(
    ElbModule* module,                       ///< [in] モジュール
    const AstModule& ast_module,             ///< [in] モジュール定義
    const std::vector<ElbParamCon>& param_con_list ///< [in] パラメータ割り当ての情報
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief phase2 で処理する内容をキューに積む．
  void
  add_phase2stub(
    ElbModule* module,          ///< [in] モジュール
    const AstModule& ast_module ///< [in] モジュール定義
  )
  {
    auto stub = make_stub<ModuleGen,
			  ElbModule*,
			  const AstModule&>(this,
					    &ModuleGen::phase2_module_item,
					    module, ast_module);
    ElbProxy::add_phase2stub(stub);
  }

  /// @brief module の中身のインスタンス化を行う．
  void
  phase2_module_item(
    ElbModule* module,          ///< [in] モジュール
    const AstModule& ast_module ///< [in] モジュール定義
  );

  /// @brief port の生成を行う．
  void
  instantiate_ports(
    ElbModule* module,               ///< [in] 親のモジュール
    const AstPortList& ast_port_list ///< [in] ポート定義のリスト
  );

  /// @brief AstPortRef から expression を生成する．
  ///
  /// 同時に向きを求める．
  ElbExpr*
  instantiate_portref(
    ElbModule* module,          ///< [in] 親のモジュール
    const AstExpr& ast_portref, ///< [in] パース木の portref 定義
    VpiDir& dir                 ///< [out] 向き
  );

  /// @brief port の生成を行う(Verilog2001用)．
  void
  instantiate_ports(
    ElbModule* module,                   ///< [in] 親のモジュール
    const AstIOHeadList& ast_iohead_list ///< [in] IO宣言のリスト
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // インスタンス展開中のフラグ
  std::unordered_set<PtrIntType> mMark;

};

END_NAMESPACE_YM_VERILOG

#endif // MODULEGEN_H
