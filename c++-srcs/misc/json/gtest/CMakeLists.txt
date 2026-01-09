
# ===================================================================
# インクルードパスの設定
# ===================================================================

include_directories (
  ../
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================


# ===================================================================
#  テスト用のターゲットの設定
# ===================================================================

ym_add_gtest ( misc_JsonObjTest
  JsonObjTest.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  )

ym_add_gtest ( misc_JsonValueTest
  JsonValueTest.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  )

ym_add_gtest ( misc_JsonScannerTest
  JsonScannerTest.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  )

ym_add_gtest ( misc_JsonParserTest
  JsonParserTest.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  )

ym_add_gtest ( misc_JsonTest
  JsonTest.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  DEFINITIONS
  "-DTESTDATA_DIR=\"${TESTDATA_DIR}/json/\""
  )
