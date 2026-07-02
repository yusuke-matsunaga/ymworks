
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${GTEST_INCLUDE_DIR}
  ../
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

ym_add_gtest ( fraig_FraigLiteral_test
  FraigLiteral_test.cc
  ${YM_SUBMODULE_OBJ_D_LIST}
  DEFINITIONS "-DTESTDATA_DIR=\"${TESTDATA_DIR}\""
  ADDRESS_SANITIZE
  )

ym_add_gtest ( fraig_FraigNode_test
  FraigNode_test.cc
  ${YM_SUBMODULE_OBJ_D_LIST}
  DEFINITIONS "-DTESTDATA_DIR=\"${TESTDATA_DIR}\""
  ADDRESS_SANITIZE
  )

ym_add_gtest ( fraig_FraigMgrImpl_test
  FraigMgrImpl_test.cc
  ${YM_SUBMODULE_OBJ_D_LIST}
  DEFINITIONS "-DTESTDATA_DIR=\"${TESTDATA_DIR}\""
  ADDRESS_SANITIZE
  )

ym_add_gtest ( fraig_FraigMgr_test
  FraigMgr_test.cc
  ${YM_SUBMODULE_OBJ_D_LIST}
  DEFINITIONS "-DTESTDATA_DIR=\"${TESTDATA_DIR}\""
  ADDRESS_SANITIZE
  )
