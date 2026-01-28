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
find_package(BISON 3.0.4 REQUIRED)


# ===================================================================
# ヘッダファイルの生成
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/private_include
  ${CMAKE_CURRENT_BINARY_DIR}
  )


# ===================================================================
#  マクロの定義
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================

#add_subdirectory ( tests/programs )
add_subdirectory ( common )


# ===================================================================
#  ソースの設定
# ===================================================================

set ( elaborator_SOURCES
  elaborator/ei/EiAttribute.cc
  elaborator/ei/EiContAssign.cc
  elaborator/ei/EiControl.cc
  elaborator/ei/EiDelay.cc
  elaborator/ei/EiFactory.cc
  elaborator/ei/EiGenBlockScope.cc
  elaborator/ei/EiGfRoot.cc
  elaborator/ei/EiModule.cc
  elaborator/ei/EiParamAssign.cc
  elaborator/ei/EiPort.cc
  elaborator/ei/EiPrimitive.cc
  elaborator/ei/EiProcess.cc
  elaborator/ei/EiRange.cc
  elaborator/ei/EiScope.cc
  elaborator/ei/EiStmtBlockScope.cc
  elaborator/ei/EiTaskFunc.cc
  elaborator/ei/EiToplevel.cc
  elaborator/ei/EiUdp.cc

  elaborator/ei_decl/EiDecl.cc
  elaborator/ei_decl/EiDeclArray.cc
  elaborator/ei_decl/EiDeclHead.cc
  elaborator/ei_decl/EiGenvar.cc
  elaborator/ei_decl/EiImpNet.cc
  elaborator/ei_decl/EiIODecl.cc
  elaborator/ei_decl/EiParameter.cc

  elaborator/ei_expr/EiBinaryOp.cc
  elaborator/ei_expr/EiBitSelect.cc
  elaborator/ei_expr/EiConcatOp.cc
  elaborator/ei_expr/EiConstant.cc
  elaborator/ei_expr/EiExpr.cc
  elaborator/ei_expr/EiFuncCall.cc
  elaborator/ei_expr/EiLhs.cc
  elaborator/ei_expr/EiPartSelect.cc
  elaborator/ei_expr/EiPrimary.cc
  elaborator/ei_expr/EiTernaryOp.cc
  elaborator/ei_expr/EiUnaryOp.cc

  elaborator/ei_stmt/EiAssignment.cc
  elaborator/ei_stmt/EiBlockStmt.cc
  elaborator/ei_stmt/EiCondStmt.cc
  elaborator/ei_stmt/EiMiscStmt.cc
  elaborator/ei_stmt/EiStmt.cc

  elaborator/elb_mgr/AttrHash.cc
  elaborator/elb_mgr/ElbDecl.cc
  elaborator/elb_mgr/ElbExpr.cc
  elaborator/elb_mgr/ElbMgr.cc
  elaborator/elb_mgr/ElbPrimitive.cc
  elaborator/elb_mgr/TagDict.cc
  elaborator/elb_mgr/VlNamedObj.cc

  elaborator/main/AttrGen.cc
  elaborator/main/DeclGen.cc
  elaborator/main/Elaborator.cc
  elaborator/main/ElbEnv.cc
  elaborator/main/ElbError.cc
  elaborator/main/ElbProxy.cc
  elaborator/main/ElbStubList.cc
  elaborator/main/ErrorGen.cc
  elaborator/main/ExprGen_funccall.cc
  elaborator/main/ExprGen_main.cc
  elaborator/main/ExprGen_operation.cc
  elaborator/main/ExprGen_primary.cc
  elaborator/main/ExprEval.cc
  elaborator/main/FuncEval.cc
  elaborator/main/ItemGen_main.cc
  elaborator/main/ItemGen_module_inst.cc
  elaborator/main/ItemGen_primitive.cc
  elaborator/main/ItemGen_taskfunc.cc
  elaborator/main/ModuleGen.cc
  elaborator/main/ObjDict.cc
  elaborator/main/StmtGen_assign.cc
  elaborator/main/StmtGen_block.cc
  elaborator/main/StmtGen_cond.cc
  elaborator/main/StmtGen_main.cc
  elaborator/main/UdpGen.cc
  )

set ( parser_SOURCES
  parser/cpt/CptDecl.cc
  parser/cpt/CptExpr.cc
  parser/cpt/CptFactory.cc
  parser/cpt/CptGenerate.cc
  parser/cpt/CptIO.cc
  parser/cpt/CptInst.cc
  parser/cpt/CptItem.cc
  parser/cpt/CptMisc.cc
  parser/cpt/CptModule.cc
  parser/cpt/CptPrimary.cc
  parser/cpt/CptSpecItem.cc
  parser/cpt/CptStmt.cc
  parser/cpt/CptUdp.cc

  parser/main/Parser.cc
  parser/main/Parser_decl.cc
  parser/main/Parser_expr.cc
  parser/main/Parser_generate.cc
  parser/main/Parser_inst.cc
  parser/main/Parser_item.cc
  parser/main/Parser_misc.cc
  parser/main/Parser_module.cc
  parser/main/Parser_port.cc
  parser/main/Parser_stmt.cc
  parser/main/Parser_udp.cc

  parser/pt_mgr/PtDumper.cc
  parser/pt_mgr/PtMgr.cc
  parser/pt_mgr/PtiBase.cc
  parser/pt_mgr/PtiFactory.cc

  parser/scanner/InputMgr.cc
  parser/scanner/Lex.cc
  parser/scanner/LexCondPlugin.cc
  parser/scanner/LexMacroPlugin.cc
  parser/scanner/LexModulePlugin.cc
  parser/scanner/LexModuleState.cc
  parser/scanner/LexPlugin.cc
  parser/scanner/LexState.cc
  parser/scanner/MacroSource.cc
  parser/scanner/RawLex.cc
  parser/scanner/RsrvWordDic.cc
  parser/scanner/StdLexPlugin.cc
  parser/scanner/TokenInfo.cc
  parser/scanner/TokenList.cc

  parser/spt/SptDecl.cc
  parser/spt/SptExpr.cc
  parser/spt/SptFactory.cc
  parser/spt/SptGenerate.cc
  parser/spt/SptItem.cc
  parser/spt/SptMisc.cc
  parser/spt/SptModule.cc
  parser/spt/SptSpecItem.cc
  parser/spt/SptStmt.cc
  parser/spt/SptUdp.cc

  ${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.cc
  )

set ( alloc_SOURCES
  alloc/Alloc.cc
  alloc/SimpleAlloc.cc
  )


# Create target for the parser
add_custom_target ( verilog_grammer ALL
  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.hh"
  "${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.cc"
  )

# Create custom command for bison/yacc (note the DEPENDS)
add_custom_command (
  COMMAND "${BISON_EXECUTABLE}"
  "${CMAKE_CURRENT_SOURCE_DIR}/parser/main/verilog_grammer.yy"
  -o "${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.cc"
  -d -v
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.cc"
  "${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.hh"
  "${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.output"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/parser/main/verilog_grammer.yy"
  )

# Since verilog_grammer.cc does not exists yet when cmake is run, mark
# it as generated
set_source_files_properties ( ${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.cc GENERATED )


# ===================================================================
#  ターゲットの設定
# ===================================================================
ym_add_object_library ( ym_verilog
  ${common_SOURCES}
  ${elaborator_SOURCES}
  ${parser_SOURCES}
  ${alloc_SOURCES}
  )
