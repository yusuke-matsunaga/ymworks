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

set ( ast_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/AstAttrInst.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AstAttrSpec.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AstBase.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AstCaseItem.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AstConnection.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AstContAssign.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AstControl.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AstDeclHead.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AstDeclItem.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AstDefParam.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AstDelay.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AstDumper.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AstExpr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AstGenCaseItem.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AstInst.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AstIOHead.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AstIOItem.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AstItem.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AstMgr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AstModule.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AstNameBranch.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AstPart.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AstPathDecl.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AstPathDelay.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AstPort.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AstRange.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AstStmt.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AstStrength.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AstUdp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AstUdpEntry.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AstUdpValue.cc
  PARENT_SCOPE
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================
