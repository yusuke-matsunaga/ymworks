# ===================================================================
# ym-logic/gtest のための CMakeLists.txt
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories()


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================


# ===================================================================
#  ターゲットの設定
# ===================================================================

add_executable( aig_read_blif
  read_blif.cc
  $<TARGET_OBJECTS:ym_misc_obj>
  $<TARGET_OBJECTS:ym_logic_obj>
  $<TARGET_OBJECTS:ym_bn_obj>
  $<TARGET_OBJECTS:ym_aig_obj>
  )

target_link_libraries ( aig_read_blif
  ${YM_LIB_DEPENDS}
  )

target_compile_options ( aig_read_blif
  PRIVATE "-O3"
  )

target_compile_definitions ( aig_read_blif
  PRIVATE "-DNDEBUG"
  )

add_executable( aig_read_blif_d
  read_blif.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_aig_obj_d>
  )

target_link_libraries ( aig_read_blif_d
  ${YM_LIB_DEPENDS}
  )

target_compile_options ( aig_read_blif_d
  PRIVATE "-g"
  )

add_executable( aig_read_write_test
  read_write_test.cc
  $<TARGET_OBJECTS:ym_misc_obj>
  $<TARGET_OBJECTS:ym_logic_obj>
  $<TARGET_OBJECTS:ym_bn_obj>
  $<TARGET_OBJECTS:ym_aig_obj>
  )

target_link_libraries ( aig_read_write_test
  ${YM_LIB_DEPENDS}
  )

target_compile_options ( aig_read_write_test
  PRIVATE "-O3"
  )

target_compile_definitions ( aig_read_write_test
  PRIVATE "-DNDEBUG"
  )

add_executable( aig_read_write_test_d
  read_write_test.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_aig_obj_d>
  )

target_link_libraries ( aig_read_write_test_d
  ${YM_LIB_DEPENDS}
  )

target_compile_options ( aig_read_write_test_d
  PRIVATE "-g"
  )

add_executable( aig_rwt_test
  rwt_test.cc
  $<TARGET_OBJECTS:ym_misc_obj>
  $<TARGET_OBJECTS:ym_logic_obj>
  $<TARGET_OBJECTS:ym_bn_obj>
  $<TARGET_OBJECTS:ym_aig_obj>
  )

target_link_libraries ( aig_rwt_test
  ${YM_LIB_DEPENDS}
  )

target_compile_options ( aig_rwt_test
  PRIVATE "-O3"
  )

target_compile_definitions ( aig_rwt_test
  PRIVATE "-DNDEBUG"
  )

add_executable( aig_rwt_test_d
  rwt_test.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_aig_obj_d>
  )

target_link_libraries ( aig_rwt_test_d
  ${YM_LIB_DEPENDS}
  )

target_compile_options ( aig_rwt_test_d
  PRIVATE "-g"
  )



# ===================================================================
#  インストールターゲットの設定
# ===================================================================
