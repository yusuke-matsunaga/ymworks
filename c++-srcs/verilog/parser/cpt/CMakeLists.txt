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

set ( parser_cpt_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/CptAssignStmt.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptAttrInst.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptBlockStmt.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptCondStmt.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptContAssign.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptConst.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptControl.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptCtrlStmt.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptDeclHead.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptDeclItem.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptDefParam.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptExpr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptGenerate.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptIO.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptInst.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptItem.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptLoopStmt.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptMisc.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptModule.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptNameBranch.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptNet.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptOpr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptParam.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptPart.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptPort.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptPrimary.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptProcess.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptRange.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptReg.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptSpecItem.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptStmt.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptTaskFunc.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CptUdp.cc
  PARENT_SCOPE
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================
