
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

ym_add_gtest ( exsyn_ExSyn_test
  ExSyn_test.cc
  ${YM_SUBMODULE_OBJ_D_LIST}
  DEFINITIONS "-DTESTDATA_DIR=\"${TESTDATA_DIR}\""
  ADDRESS_SANITIZE
  )

ym_add_gtest ( test1
  test1.cc
  ${YM_SUBMODULE_OBJ_D_LIST}
  DEFINITIONS "-DTESTDATA_DIR=\"${TESTDATA_DIR}\""
  ADDRESS_SANITIZE
  )

ym_add_gtest ( test2
  test2.cc
  ${YM_SUBMODULE_OBJ_D_LIST}
  DEFINITIONS "-DTESTDATA_DIR=\"${TESTDATA_DIR}\""
  ADDRESS_SANITIZE
  )

ym_add_gtest ( test3
  test3.cc
  ${YM_SUBMODULE_OBJ_D_LIST}
  DEFINITIONS "-DTESTDATA_DIR=\"${TESTDATA_DIR}\""
  ADDRESS_SANITIZE
  )

ym_add_gtest ( exsyn_FenceGen_test
  FenceGen_test.cc
  ${YM_SUBMODULE_OBJ_D_LIST}
  ADDRESS_SANITIZE
  )

ym_add_gtest ( exsyn_PdGen_test
  PdGen_test.cc
  ${YM_SUBMODULE_OBJ_D_LIST}
  ADDRESS_SANITIZE
  )

if ( 0 )
ym_add_gtest ( exsyn_BdGen_test
  BdGen_test.cc
  ${YM_SUBMODULE_OBJ_D_LIST}
  )
endif ()

add_executable ( and2_test
  and2_test.cc
  ${YM_SUBMODULE_OBJ_D_LIST}
  ADDRESS_SANITIZE
  )

ym_add_gtest ( exsyn_XformInfo_test
  XformInfo_test.cc
  ${YM_SUBMODULE_OBJ_D_LIST}
  ADDRESS_SANITIZE
  )

target_compile_options ( and2_test
  PRIVATE "-g"
  )

target_link_libraries ( and2_test
  ${YM_LIB_DEPENDS}
  )

add_executable ( or3_test
  or3_test.cc
  ${YM_SUBMODULE_OBJ_D_LIST}
  )

target_compile_options ( or3_test
  PRIVATE "-g"
  )

target_link_libraries ( or3_test
  ${YM_LIB_DEPENDS}
  )

add_executable ( level_test
  level_test.cc
  ${YM_SUBMODULE_OBJ_D_LIST}
  ADDRESS_SANITIZE
  )

target_compile_options ( level_test
  PRIVATE "-g"
  )

target_link_libraries ( level_test
  ${YM_LIB_DEPENDS}
  )
