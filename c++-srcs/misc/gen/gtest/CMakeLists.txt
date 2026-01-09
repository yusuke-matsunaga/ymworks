
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

ym_add_gtest ( misc_CombiGen_test
  CombiGen_test.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  )

ym_add_gtest ( misc_PermGen_test
  PermGen_test.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  )

ym_add_gtest ( misc_MultiCombiGen_test
  MultiCombiGen_test.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  )

ym_add_gtest ( misc_MultiPermGen_test
  MultiPermGen_test.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  )

ym_add_gtest ( misc_MultiSetCombiGen_test
  MultiSetCombiGen_test.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  )

ym_add_gtest ( misc_MultiSetPermGen_test
  MultiSetPermGen_test.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  )
