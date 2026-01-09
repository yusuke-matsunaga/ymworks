

# ===================================================================
# パッケージの検査
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
set ( clib_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/clib_module.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyClibArea.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyClibTechnology.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyClibDelayModel.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyClibDirection.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyClibTimingSense.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyClibTimingType.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyClibVarType.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyClibCellType.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyClibCPV.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyClibPatType.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyClibBundle.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyClibBus.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyClibBusType.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyClibCapacitance.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyClibCell.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyClibCellClass.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyClibCellGroup.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyClibCellLibrary.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyClibLut.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyClibIOMap.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyClibPatGraph.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyClibPin.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyClibPinMap.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyClibResistance.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyClibSeqAttr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyClibTime.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyClibTiming.cc
  PARENT_SCOPE
  )


# ===================================================================
#  executable ターゲットの設定
# ===================================================================


# ===================================================================
# テストターゲット
# ===================================================================
