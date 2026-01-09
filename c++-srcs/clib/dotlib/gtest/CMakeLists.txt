
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
#  テスト用のターゲットの設定
# ===================================================================


# ===================================================================
#  DotlibScanner_test
# ===================================================================
ym_add_gtest ( clib_DotlibScanner_test
  ScannerTest.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  )


# ===================================================================
#  FuncScanner_test
# ===================================================================
ym_add_gtest ( clib_FuncScanner_test
  FuncScannerTest.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  )

target_include_directories( clib_FuncScanner_test
  PRIVATE
  ../parser
  )


# ===================================================================
#  DotlibParser_test
# ===================================================================
ym_add_gtest ( clib_DotlibParser_test
  ParserTest_simple.cc
  ParserTest_complex.cc
  ParserTest_group.cc
  ParserTest_library_simple.cc
  ParserTest_library_default.cc
  ParserTest_library_k_process.cc
  ParserTest_library_k_temp.cc
  ParserTest_library_k_volt.cc
  ParserTest_library_complex.cc
  ParserTest_library_group.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  )

target_include_directories( clib_DotlibParser_test
  PRIVATE
  ../parser
  )


# ===================================================================
#  DotlibParser_test2
# ===================================================================
ym_add_gtest ( clib_DotlibParser_test2
  ParserTest_ast.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  DEFINITIONS
  "-DDATA_DIR=\"${TESTDATA_DIR}\""
  )

target_include_directories( clib_DotlibParser_test2
  PRIVATE
  ../parser
  )


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
