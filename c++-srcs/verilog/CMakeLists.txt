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

add_subdirectory ( alloc )
add_subdirectory ( common )
add_subdirectory ( parser )
add_subdirectory ( scanner )
add_subdirectory ( ast )
add_subdirectory ( cpt/decl )
add_subdirectory ( cpt/expr )
add_subdirectory ( cpt/item )
add_subdirectory ( cpt/misc )
add_subdirectory ( cpt/stmt )
add_subdirectory ( elaborator )
add_subdirectory ( elb_mgr )
add_subdirectory ( ei/decl )
add_subdirectory ( ei/expr )
add_subdirectory ( ei/item )
add_subdirectory ( ei/misc )
add_subdirectory ( ei/stmt )

add_subdirectory ( gtest )
#add_subdirectory ( tests/programs )


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
  "${CMAKE_CURRENT_SOURCE_DIR}/parser/verilog_grammer.yy"
  -o "${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.cc"
  -d -v
  -Wcounterexamples
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.cc"
  "${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.hh"
  "${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.output"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/parser/verilog_grammer.yy"
  )

# Since verilog_grammer.cc does not exists yet when cmake is run, mark
# it as generated
set_source_files_properties ( ${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.cc GENERATED )


# ===================================================================
#  ターゲットの設定
# ===================================================================
ym_add_object_library ( ym_verilog
  ${common_SOURCES}
  ${parser_SOURCES}
  ${scanner_SOURCES}
  ${ast_SOURCES}
  ${cpt_decl_SOURCES}
  ${cpt_expr_SOURCES}
  ${cpt_item_SOURCES}
  ${cpt_misc_SOURCES}
  ${cpt_stmt_SOURCES}
  ${elaborator_SOURCES}
  ${elb_mgr_SOURCES}
  ${ei_decl_SOURCES}
  ${ei_expr_SOURCES}
  ${ei_item_SOURCES}
  ${ei_misc_SOURCES}
  ${ei_stmt_SOURCES}
  ${alloc_SOURCES}
  ${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.cc
  )
