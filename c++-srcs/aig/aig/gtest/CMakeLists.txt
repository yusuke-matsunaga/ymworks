
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${GTEST_INCLUDE_DIR}
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================


# ===================================================================
#  テスト用のターゲットの設定
# ===================================================================

ym_add_gtest ( aig_AigNode_test
  AigNode_test.cc
  ${YM_SUBMODULE_OBJ_D_LIST}
  DEFINITIONS "-DTESTDATA_DIR=\"${TESTDATA_DIR}\""
  ADDRESS_SANITIZE
  )

ym_add_gtest ( aig_AigEdge_test
  AigEdge_test.cc
  ${YM_SUBMODULE_OBJ_D_LIST}
  DEFINITIONS "-DTESTDATA_DIR=\"${TESTDATA_DIR}\""
  ADDRESS_SANITIZE
  )

ym_add_gtest ( aig_AigMgr_test
  AigMgr_test.cc
  ${YM_SUBMODULE_OBJ_D_LIST}
  DEFINITIONS "-DTESTDATA_DIR=\"${TESTDATA_DIR}\""
  ADDRESS_SANITIZE
  )

ym_add_gtest ( aig_AigMgrImpl_test
  AigMgrImpl_test.cc
  ${YM_SUBMODULE_OBJ_D_LIST}
  DEFINITIONS "-DTESTDATA_DIR=\"${TESTDATA_DIR}\""
  ADDRESS_SANITIZE
  )

ym_add_gtest ( aig_Cov2Aig_test
  cov2aig_test.cc
  ${YM_SUBMODULE_OBJ_D_LIST}
  DEFINITIONS "-DTESTDATA_DIR=\"${TESTDATA_DIR}\""
  ADDRESS_SANITIZE
  )

ym_add_gtest( aig_ident_test
  ident_test.cc
  ${YM_SUBMODULE_OBJ_D_LIST}
  DEFINITIONS
  "-DDATAPATH=\"${TESTDATA_DIR}\""
  ADDRESS_SANITIZE
  )

ym_add_gtest ( aig_simulate_test
  simulate_test.cc
  ${YM_SUBMODULE_OBJ_D_LIST}
  ADDRESS_SANITIZE
  )
