# ===================================================================
# programs/fraig のための CMakeLists.txt
# ===================================================================


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
#  ターゲットの設定
# ===================================================================

add_executable( exsyn
  exsyn.cc
  ${YM_SUBMODULE_OBJ_LIST}
  )

target_link_libraries ( exsyn
  ${YM_LIB_DEPENDS}
  )

target_compile_options( exsyn
  PRIVATE "-O3"
  )

target_compile_definitions ( exsyn
  PRIVATE "-DNDEBUG"
  )

add_executable( exsyn_d
  exsyn.cc
  ${YM_SUBMODULE_OBJ_D_LIST}
  )

target_link_libraries ( exsyn_d
  ${YM_LIB_DEPENDS}
  )

target_compile_options( exsyn_d
  PRIVATE "-g"
  )

add_executable( all_4
  all_4.cc
  ${YM_SUBMODULE_OBJ_LIST}
  )

target_link_libraries ( all_4
  ${YM_LIB_DEPENDS}
  )

target_compile_options( all_4
  PRIVATE "-O3"
  )

target_compile_definitions ( all_4
  PRIVATE "-DNDEBUG"
  )

add_executable( all_4_d
  all_4.cc
  ${YM_SUBMODULE_OBJ_D_LIST}
  )

target_link_libraries ( all_4_d
  ${YM_LIB_DEPENDS}
  )

target_compile_options( all_4_d
  PRIVATE "-g"
  )


add_executable( all_5
  all_5.cc
  ${YM_SUBMODULE_OBJ_LIST}
  )

target_link_libraries ( all_5
  ${YM_LIB_DEPENDS}
  )

target_compile_options( all_5
  PRIVATE "-O3"
  )

target_compile_definitions ( all_5
  PRIVATE "-DNDEBUG"
  )

add_executable( all_5_d
  all_5.cc
  ${YM_SUBMODULE_OBJ_D_LIST}
  )

target_link_libraries ( all_5_d
  ${YM_LIB_DEPENDS}
  )

target_compile_options( all_5_d
  PRIVATE "-g"
  )


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
