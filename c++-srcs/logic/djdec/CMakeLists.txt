
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================

add_subdirectory ( gtest )
add_subdirectory ( programs )


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set ( djdec_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/BddDecMgr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/BddDec2Bc.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/BddDecNode.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/BddDecMark.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/TvDecMgr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/TvDec2Bc.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/TvDecNode.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/TvDecMark.cc
  PARENT_SCOPE
  )


# ===================================================================
# ターゲットの設定
# ===================================================================
