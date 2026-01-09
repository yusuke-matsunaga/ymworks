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
set (ci_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/CiBundle.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CiBus.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CiBusType.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CiCell.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CiLogicCell.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CiFFCell.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CiFLCell.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CiLatchCell.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CiFsmCell.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CiCellClass.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CiCellGroup.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CiCellLibrary.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CiLut.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CiLutTemplate.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CiPatGraph.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CiPatMgr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CiPin.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CiStLut.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CiTiming.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/dump.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/restore.cc
  PARENT_SCOPE
  )

# ===================================================================
#  ターゲットの設定
# ===================================================================


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
