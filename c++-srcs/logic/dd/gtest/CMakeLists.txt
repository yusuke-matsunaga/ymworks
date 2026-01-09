# ===================================================================
# ym-logic/gtest のための CMakeLists.txt
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ../
  )

# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================


# ===================================================================
#  ターゲットの設定
# ===================================================================

ym_add_gtest( logic_dd_DdEdge_test
  DdEdge_test.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest( logic_dd_BddMgr_test
  BddMgr_test.cc
  BddTest.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest( logic_dd_Bdd_test
  Bdd_test.cc
  BddTest.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest( logic_dd_BddVar_test
  BddVar_test.cc
  BddTest.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest( logic_dd_BddLit_test
  BddLit_test.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest( logic_dd_BddVarSet_test
  BddVarSet_test.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest( logic_dd_BddCube_test
  BddCube_test.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest( logic_dd_BddIsop_test
  BddIsopTest.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest( logic_dd_ZddTest
  ZddTest.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
