# ===================================================================
# ym-logic/gtest のための CMakeLists.txt
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


# ===================================================================
#  ターゲットの設定
# ===================================================================

ym_add_gtest ( verilog_ParserTest_decl
  ParserTest_decl.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  $<TARGET_OBJECTS:ym_verilog_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( verilog_ParserTest_item1
  ParserTest_item1.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  $<TARGET_OBJECTS:ym_verilog_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( verilog_ParserTest_item2
  ParserTest_item2.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  $<TARGET_OBJECTS:ym_verilog_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( verilog_ParserTest_item3
  ParserTest_item3.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  $<TARGET_OBJECTS:ym_verilog_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( verilog_ParserTest_item4
  ParserTest_item4.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  $<TARGET_OBJECTS:ym_verilog_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( verilog_ParserTest_stmt1
  ParserTest_stmt1.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  $<TARGET_OBJECTS:ym_verilog_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( verilog_ParserTest_stmt2
  ParserTest_stmt2.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  $<TARGET_OBJECTS:ym_verilog_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( verilog_ParserTest_expr1
  ParserTest_expr1.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  $<TARGET_OBJECTS:ym_verilog_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( verilog_ParserTest_expr2
  ParserTest_expr2.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  $<TARGET_OBJECTS:ym_verilog_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( verilog_ParserTest_expr3
  ParserTest_expr3.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  $<TARGET_OBJECTS:ym_verilog_obj_d>
  ADDRESS_SANITIZE
  )


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
