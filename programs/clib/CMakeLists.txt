# ===================================================================
# CMAKE のおまじない
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  )


# ===================================================================
#  マクロの定義
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================

add_executable ( read_mislib_test
  read_mislib_test.cc
  $<TARGET_OBJECTS:ym_clib_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_misc_obj_d>
  )

target_link_libraries ( read_mislib_test
  ${YM_LIB_DEPENDS}
  )

target_compile_definitions ( read_mislib_test
  PRIVATE "-DDATA_DIR=\"${TESTDATA_DIR}\""
  )


add_executable ( read_liberty_test
  read_liberty_test.cc
  $<TARGET_OBJECTS:ym_clib_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_misc_obj_d>
  )

target_link_libraries ( read_liberty_test
  ${YM_LIB_DEPENDS}
  )

target_compile_definitions ( read_liberty_test
  PRIVATE "-DDATA_DIR=\"${TESTDATA_DIR}\""
  )
