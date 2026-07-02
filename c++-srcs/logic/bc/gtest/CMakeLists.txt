# ===================================================================
# gtest のための CMakeLists.txt
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../
  )

# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================


# ===================================================================
#  ターゲットの設定
# ===================================================================

ym_add_gtest ( logic_bc_BcEdge_test
  BcEdge_test.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  DEFINITIONS
  "-DDATAPATH=\"${TESTDATA_DIR}\""
  ADDRESS_SANITIZE
  )

ym_add_gtest ( logic_bc_BcOp_test
  BcOp_test.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  DEFINITIONS
  "-DDATAPATH=\"${TESTDATA_DIR}\""
  ADDRESS_SANITIZE
  )

ym_add_gtest ( logic_bc_BcGraph_test
  BcGraph_test.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  DEFINITIONS
  "-DDATAPATH=\"${TESTDATA_DIR}\""
  ADDRESS_SANITIZE
  )


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
