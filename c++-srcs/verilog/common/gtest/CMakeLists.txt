
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

ym_add_gtest ( verilog_VlValueType_test
  VlValueType_test.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  $<TARGET_OBJECTS:ym_verilog_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( verilog_VlUdpVal_test
  VlUdpVal_test.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  $<TARGET_OBJECTS:ym_verilog_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( verilog_BitVectorTest
  BitVectorTest.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  $<TARGET_OBJECTS:ym_verilog_obj_d>
  ADDRESS_SANITIZE
  )
