
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
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

add_executable( engine_test
  engine_test.cc
  ${YM_SUBMODULE_OBJ_LIST}
  )

target_link_libraries ( engine_test
  ${YM_LIB_DEPENDS}
  )

target_compile_options( engine_test
  PRIVATE "-O3"
  )

target_compile_definitions ( engine_test
  PRIVATE "-DNDEBUG"
  )

add_executable( engine_test_d
  engine_test.cc
  ${YM_SUBMODULE_OBJ_D_LIST}
  )

target_link_libraries ( engine_test_d
  ${YM_LIB_DEPENDS}
  )

target_compile_options( engine_test_d
  PRIVATE "-g"
  )

add_executable( bd_test
  bd_test.cc
  ${YM_SUBMODULE_OBJ_D_LIST}
  )

target_link_libraries ( bd_test
  ${YM_LIB_DEPENDS}
  )

target_compile_options( bd_test
  PRIVATE "-g"
  )

add_executable( exsyn2
  exsyn.cc
  ${YM_SUBMODULE_OBJ_LIST}
  )

target_link_libraries ( exsyn2
  ${YM_LIB_DEPENDS}
  )

target_compile_options( exsyn2
  PRIVATE "-O3"
  )

target_compile_definitions ( exsyn2
  PRIVATE "-DNDEBUG"
  )

add_executable( exsyn2_d
  exsyn.cc
  ${YM_SUBMODULE_OBJ_D_LIST}
  )

target_link_libraries ( exsyn2_d
  ${YM_LIB_DEPENDS}
  )

target_compile_options( exsyn2_d
  PRIVATE "-g"
  )
