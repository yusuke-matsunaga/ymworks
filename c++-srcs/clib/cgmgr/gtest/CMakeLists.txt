
# ===================================================================
# インクルードパスの設定
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================



# ===================================================================
#  テスト用のターゲットの設定
# ===================================================================


# ===================================================================
#  CgLogicSig_test
# ===================================================================
ym_add_gtest ( clib_CgLogicSig_test
  CgLogicSig_test.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  )

target_include_directories( clib_CgLogicSig_test
  PRIVATE
  ../
  )

if (0)
add_executable( foo_test
  foo.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  )

target_compile_options ( foo_test
  PRIVATE "-g" "-fPIC"
  )

target_link_libraries( foo_test
  ${YM_LIB_DEPENDS}
  pthread
  )

target_include_directories( foo_test
  PRIVATE
  ../
  )
endif ()

# ===================================================================
#  CgGenLogicSig_test
# ===================================================================
ym_add_gtest ( clib_CgGenLogicSig_test
  CgGenLogicSig_test.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  )

target_include_directories( clib_CgGenLogicSig_test
  PRIVATE
  ../
  )


# ===================================================================
#  CgFFSig_test
# ===================================================================
ym_add_gtest ( clib_CgFFSig_test
  CgFFSig_test.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  )

target_include_directories( clib_CgFFSig_test
  PRIVATE
  ../
  )


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
