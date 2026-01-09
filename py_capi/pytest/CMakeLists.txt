
# ===================================================================
# パッケージの検査
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================

foreach ( submodule ${YM_SUBMODULE_LIST} )
  add_subdirectory( ${submodule} )
endforeach ()


# ===================================================================
#  ソースファイルの設定
# ===================================================================


# ===================================================================
#  executable ターゲットの設定
# ===================================================================

add_executable( py_ymworks
  py_ymworks.cc
  $<TARGET_OBJECTS:py_ymworks_obj>
  ${YM_SUBMODULE_OBJ_LIST}
  )

target_compile_options( py_ymworks
  PRIVATE "-O3"
  )

target_compile_definitions( py_ymworks
  PRIVATE "-DNODEBUG"
  )

target_link_libraries ( py_ymworks
  ${YM_LIB_DEPENDS}
  ${Python3_LIBRARIES}
  )

add_executable( py_ymworks_d
  py_ymworks.cc
  $<TARGET_OBJECTS:py_ymworks_obj_d>
  ${YM_SUBMODULE_OBJ_D_LIST}
  )

target_compile_options( py_ymworks_d
  PRIVATE "-g"
  )

target_link_libraries ( py_ymworks_d
  ${YM_LIB_DEPENDS}
  ${Python3_LIBRARIES}
  )


# ===================================================================
# インストールターゲット
# ===================================================================

install ( TARGETS py_ymworks py_ymworks_d
  DESTINATION bin
  )


# ===================================================================
# テストターゲット
# ===================================================================
