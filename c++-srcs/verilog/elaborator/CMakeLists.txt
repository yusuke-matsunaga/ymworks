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
  ${CMAKE_CURRENT_SOURCE_DIR}/AttrGen.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/DeclGen.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/Elaborator.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ElbEnv.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ElbProxy.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ElbStubList.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ErrorGen.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ExprGen_funccall.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ExprGen_main.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ExprGen_operation.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ExprGen_primary.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ExprEval.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/FuncEval.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ItemGen_main.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ItemGen_module_inst.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ItemGen_primitive.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ItemGen_taskfunc.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ItemGen_error.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ModuleGen.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ModuleGen_error.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ObjDict.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/StmtGen_assign.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/StmtGen_block.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/StmtGen_cond.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/StmtGen_main.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/UdpGen.cc
  PARENT_SCOPE
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================
