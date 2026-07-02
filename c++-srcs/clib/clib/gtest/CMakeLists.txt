
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
#  ClibArea_test
# ===================================================================
ym_add_gtest ( clib_ClibArea_test
  # 実は ClibArea.h のみに依存している．
  ClibAreaTest.cc
  ADDRESS_SANITIZE
  )


# ===================================================================
#  ClibCapacitance_test
# ===================================================================
ym_add_gtest ( clib_ClibCapacitance_test
  # 実は ClibCapacitance.h のみに依存している．
  ClibCapacitanceTest.cc
  ADDRESS_SANITIZE
  )


# ===================================================================
#  ClibResistance_test
# ===================================================================
ym_add_gtest ( clib_ClibResistance_test
  # 実は ClibResistance.h のみに依存している．
  ClibResistanceTest.cc
  ADDRESS_SANITIZE
  )

# ===================================================================
#  ClibSeqAttr_test
# ===================================================================
ym_add_gtest ( clib_ClibSeqAttr_test
  ClibSeqAttrTest.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  ADDRESS_SANITIZE
  )


# ===================================================================
#  ClibTime_test
# ===================================================================
ym_add_gtest ( clib_ClibTime_test
  # 実は ClibTime.h のみに依存している．
  ClibTimeTest.cc
  ADDRESS_SANITIZE
  )


# ===================================================================
#  ClibClib_test
# ===================================================================
ym_add_gtest ( clib_ClibClib_test
  ClibCellTest.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  DEFINITIONS
  "-DDATA_DIR=\"${TESTDATA_DIR}\""
  ADDRESS_SANITIZE
  )


# ===================================================================
#  ClibCellLibrary_test
# ===================================================================
ym_add_gtest ( clib_ClibCellLibrary_test
  ClibCellLibraryTest.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  DEFINITIONS
  "-DDATA_DIR=\"${TESTDATA_DIR}\""
  ADDRESS_SANITIZE
  )


# ===================================================================
#  ClibIOMap_test
# ===================================================================
ym_add_gtest ( clib_ClibIOMap_test
  ClibIOMapTest.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_clib_obj_d>
  ADDRESS_SANITIZE
  )


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
