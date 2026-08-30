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

ym_add_gtest ( verilog_PtListTest
  PtListTest.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  $<TARGET_OBJECTS:ym_verilog_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( verilog_PtHierNameTest
  PtHierNameTest.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  $<TARGET_OBJECTS:ym_verilog_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( verilog_PtIODListTest
  PtIODListTest.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  $<TARGET_OBJECTS:ym_verilog_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( verilog_PtDeclTest
  PtDeclTest.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  $<TARGET_OBJECTS:ym_verilog_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( verilog_PtItemTest1
  PtItemTest1.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  $<TARGET_OBJECTS:ym_verilog_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( verilog_PtItemTest2
  PtItemTest2.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  $<TARGET_OBJECTS:ym_verilog_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( verilog_PtItemTest3
  PtItemTest3.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  $<TARGET_OBJECTS:ym_verilog_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( verilog_PtItemTest4
  PtItemTest4.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  $<TARGET_OBJECTS:ym_verilog_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( verilog_PtStmtTest1
  PtStmtTest1.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  $<TARGET_OBJECTS:ym_verilog_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( verilog_PtStmtTest2
  PtStmtTest2.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  $<TARGET_OBJECTS:ym_verilog_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( verilog_PtExprTest1
  PtExprTest1.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  $<TARGET_OBJECTS:ym_verilog_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( verilog_PtExprTest2
  PtExprTest2.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  $<TARGET_OBJECTS:ym_verilog_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( verilog_PtExprTest3
  PtExprTest3.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  $<TARGET_OBJECTS:ym_verilog_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( verilog_PtMiscTest
  PtMiscTest.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  $<TARGET_OBJECTS:ym_verilog_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( verilog_PtUdpTest
  PtUdpTest.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  $<TARGET_OBJECTS:ym_verilog_obj_d>
  ADDRESS_SANITIZE
  )


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
