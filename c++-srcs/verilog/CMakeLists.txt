# ===================================================================
# CMAKE のおまじない
# ===================================================================


# ===================================================================
# プロジェクト名，バージョンの設定
# ===================================================================


# ===================================================================
# オプション
# ===================================================================


# ===================================================================
# パッケージの検査
# ===================================================================
find_package(BISON 3.0.4 REQUIRED)


# ===================================================================
# ヘッダファイルの生成
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/private_include
  ${CMAKE_CURRENT_BINARY_DIR}
  )


# ===================================================================
#  マクロの定義
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================

#add_subdirectory ( tests/programs )
add_subdirectory ( alloc )
add_subdirectory ( common )
add_subdirectory ( elaborator )
add_subdirectory ( parser )
add_subdirectory ( parser/main )
add_subdirectory ( parser/scanner )
#add_subdirectory ( parser/pt_mgr )
add_subdirectory ( parser/ast )
add_subdirectory ( parser/cpt )


# ===================================================================
#  ソースの設定
# ===================================================================

# Create target for the parser
add_custom_target ( verilog_grammer ALL
  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.hh"
  "${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.cc"
  )

# Create custom command for bison/yacc (note the DEPENDS)
add_custom_command (
  COMMAND "${BISON_EXECUTABLE}"
  "${CMAKE_CURRENT_SOURCE_DIR}/parser/main/verilog_grammer.yy"
  -o "${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.cc"
  -d -v
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.cc"
  "${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.hh"
  "${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.output"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/parser/main/verilog_grammer.yy"
  )

# Since verilog_grammer.cc does not exists yet when cmake is run, mark
# it as generated
set_source_files_properties ( ${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.cc GENERATED )


# ===================================================================
#  ターゲットの設定
# ===================================================================
ym_add_object_library ( ym_verilog
  ${common_SOURCES}
  ${elaborator_SOURCES}
  ${parser_SOURCES}
  ${parser_main_SOURCES}
  ${parser_scanner_SOURCES}
  ${parser_pt_mgr_SOURCES}
  ${parser_ast_SOURCES}
  ${parser_cpt_SOURCES}
  ${alloc_SOURCES}
  ${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.cc
  )
