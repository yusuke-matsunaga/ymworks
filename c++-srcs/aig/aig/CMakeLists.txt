
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================

add_subdirectory( gtest )


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set ( aig_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/AigEdge.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AigMgr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AigMgr_cofactor.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AigMgr_expr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AigMgr_op.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AigMgr_sop.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AigMgrImpl.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AigMgrImpl_check.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AigMgrImpl_copy.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AigMgrImpl_print.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AigMgrImpl_replace.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AigMgrImpl_set.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AigMgrImpl_simulate.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/DotGen.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/IdentOp.cc
  PARENT_SCOPE
  )


# ===================================================================
# ターゲットの設定
# ===================================================================


# ===================================================================
#  テストの設定
# ===================================================================


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
