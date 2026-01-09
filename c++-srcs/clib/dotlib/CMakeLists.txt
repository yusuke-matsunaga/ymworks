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
set ( dotlib_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/GroupInfo.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/LibraryInfo.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ElemInfo.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CellInfo.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/FFInfo.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/FLInfo.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/FSMInfo.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/LatchInfo.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/LuTemplInfo.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PinInfo.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/TableInfo.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/TimingInfo.cc

  ${CMAKE_CURRENT_SOURCE_DIR}/parser/FuncParser.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/parser/FuncScanner.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/parser/HeaderHandler.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/parser/Parser.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/parser/DotlibScanner.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/parser/DotlibScanner_value.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/parser/DotlibScanner_expr.cc

  ${CMAKE_CURRENT_SOURCE_DIR}/ast/AstExpr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ast/AstValue.cc

  PARENT_SCOPE
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
