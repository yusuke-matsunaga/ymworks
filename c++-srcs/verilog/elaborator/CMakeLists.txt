# ===================================================================
# CMAKE のおまじない
# ===================================================================


# ===================================================================
# プロジェクト名，バージョンの設定
# ===================================================================


# ===================================================================
# オプション
# ===================================================================


# ===================================================================
# パッケージの検査
# ===================================================================


# ===================================================================
# ヘッダファイルの生成
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================


# ===================================================================
#  マクロの定義
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースの設定
# ===================================================================

set ( elaborator_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/ei/EiAttribute.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei/EiContAssign.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei/EiControl.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei/EiDelay.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei/EiFactory.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei/EiGenBlockScope.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei/EiGfRoot.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei/EiModule.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei/EiParamAssign.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei/EiPort.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei/EiPrimitive.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei/EiProcess.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei/EiRange.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei/EiScope.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei/EiStmtBlockScope.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei/EiTaskFunc.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei/EiToplevel.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei/EiUdp.cc

  ${CMAKE_CURRENT_SOURCE_DIR}/ei_decl/EiDecl.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei_decl/EiDeclArray.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei_decl/EiDeclHead.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei_decl/EiGenvar.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei_decl/EiImpNet.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei_decl/EiIODecl.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei_decl/EiParameter.cc

  ${CMAKE_CURRENT_SOURCE_DIR}/ei_expr/EiBinaryOp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei_expr/EiBitSelect.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei_expr/EiConcatOp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei_expr/EiConstant.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei_expr/EiExpr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei_expr/EiFuncCall.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei_expr/EiLhs.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei_expr/EiPartSelect.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei_expr/EiPrimary.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei_expr/EiTernaryOp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei_expr/EiUnaryOp.cc

  ${CMAKE_CURRENT_SOURCE_DIR}/ei_stmt/EiAssignment.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei_stmt/EiBlockStmt.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei_stmt/EiCondStmt.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei_stmt/EiMiscStmt.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ei_stmt/EiStmt.cc

  ${CMAKE_CURRENT_SOURCE_DIR}/elb_mgr/AttrHash.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/elb_mgr/ElbDecl.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/elb_mgr/ElbExpr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/elb_mgr/ElbMgr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/elb_mgr/ElbPrimitive.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/elb_mgr/TagDict.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/elb_mgr/VlNamedObj.cc

  ${CMAKE_CURRENT_SOURCE_DIR}/main/AttrGen.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/DeclGen.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/Elaborator.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/ElbEnv.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/ElbError.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/ElbProxy.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/ElbStubList.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/ErrorGen.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/ExprGen_funccall.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/ExprGen_main.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/ExprGen_operation.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/ExprGen_primary.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/ExprEval.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/FuncEval.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/ItemGen_main.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/ItemGen_module_inst.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/ItemGen_primitive.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/ItemGen_taskfunc.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/ModuleGen.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/ObjDict.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/StmtGen_assign.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/StmtGen_block.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/StmtGen_cond.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/StmtGen_main.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/UdpGen.cc
  PARENT_SCOPE
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================
