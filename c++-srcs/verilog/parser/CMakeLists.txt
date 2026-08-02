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

add_subdirectory ( gtest )


# ===================================================================
#  ソースの設定
# ===================================================================

set ( parser_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/cpt/CptDecl.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/cpt/CptExpr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/cpt/CptFactory.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/cpt/CptGenerate.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/cpt/CptIO.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/cpt/CptInst.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/cpt/CptItem.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/cpt/CptMisc.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/cpt/CptModule.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/cpt/CptPrimary.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/cpt/CptSpecItem.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/cpt/CptStmt.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/cpt/CptUdp.cc

  ${CMAKE_CURRENT_SOURCE_DIR}/main/Parser.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/Parser_decl.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/Parser_expr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/Parser_generate.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/Parser_inst.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/Parser_item.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/Parser_misc.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/Parser_module.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/Parser_port.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/Parser_stmt.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/main/Parser_udp.cc

  ${CMAKE_CURRENT_SOURCE_DIR}/pt_mgr/PtDumper.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/pt_mgr/PtMgr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/pt_mgr/PtiBase.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/pt_mgr/PtiFactory.cc

  ${CMAKE_CURRENT_SOURCE_DIR}/scanner/InputMgr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/scanner/Lex.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/scanner/LexCondPlugin.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/scanner/LexMacroPlugin.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/scanner/LexModulePlugin.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/scanner/LexModuleState.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/scanner/LexPlugin.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/scanner/LexState.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/scanner/MacroSource.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/scanner/RawLex.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/scanner/RsrvWordDic.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/scanner/StdLexPlugin.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/scanner/TokenInfo.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/scanner/TokenList.cc

  ${CMAKE_CURRENT_SOURCE_DIR}/spt/SptDecl.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/spt/SptExpr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/spt/SptFactory.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/spt/SptGenerate.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/spt/SptItem.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/spt/SptMisc.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/spt/SptModule.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/spt/SptSpecItem.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/spt/SptStmt.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/spt/SptUdp.cc

  PARENT_SCOPE
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================
