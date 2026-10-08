

# ===================================================================
# パッケージの検査
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${Python3_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)


# ===================================================================
# サブディレクトリの設定
# ===================================================================
add_subdirectory( misc )
add_subdirectory( combopt )
add_subdirectory( logic )
add_subdirectory( clib )
add_subdirectory( bn )
add_subdirectory( aig )
add_subdirectory( sat )
add_subdirectory( fraig )
add_subdirectory( pytest )


# ===================================================================
#  ソースファイルの設定
# ===================================================================


# ===================================================================
#  executable ターゲットの設定
# ===================================================================
ym_add_object_library( py_ymworks
  ymworks_module.cc
  ${misc_SOURCES}
  ${combopt_SOURCES}
  ${logic_SOURCES}
  ${clib_SOURCES}
  ${bn_SOURCES}
  ${aig_SOURCES}
  ${sat_SOURCES}
  ${fraig_SOURCES}
  )

add_library( ymworkslib
  SHARED
  $<TARGET_OBJECTS:py_ymworks_obj>
  ${YM_SUBMODULE_OBJ_LIST}
  )

target_link_libraries( ymworkslib
  ${YM_LIB_DEPENDS}
  ${Python3_LIBRARIES}
  )

set_target_properties( ymworkslib
  PROPERTIES
  PREFIX ""
  OUTPUT_NAME "ymworks"
  )


# ===================================================================
# テストターゲット
# ===================================================================
