
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

add_executable( djdec
  djdec.cc
  ${YM_SUBMODULE_OBJ_LIST}
  )

target_link_libraries ( djdec
  ${YM_LIB_DEPENDS}
  )

target_compile_options( djdec
  PRIVATE "-O3"
  )

target_compile_definitions ( djdec
  PRIVATE "-DNDEBUG"
  )

add_executable( djdec_d
  djdec.cc
  ${YM_SUBMODULE_OBJ_D_LIST}
  )

target_link_libraries ( djdec_d
  ${YM_LIB_DEPENDS}
  )

target_compile_options( djdec_d
  PRIVATE "-g"
  )
