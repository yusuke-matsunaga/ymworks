# ===================================================================
# パッケージの検査
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================

add_subdirectory( gtest )


# ===================================================================
#  ソースファイルの設定
# ===================================================================
set (clib_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/ClibBundle.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ClibBus.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ClibBusType.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ClibCell.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ClibCellClass.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ClibCellGroup.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ClibCellLibrary.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ClibIOMap.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ClibLibraryPtr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ClibLut.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ClibPatGraph.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ClibPin.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ClibTiming.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/Writer.cc
  PARENT_SCOPE
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
