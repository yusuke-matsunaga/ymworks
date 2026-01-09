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

ym_add_gtest ( logic_tv_TvFunc_base_test
  TvFuncTest_base.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest ( logic_tv_TvFunc_bind_test
  TvFuncTest_bind.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest ( logic_tv_TvFunc_move_test
  TvFuncTest_move.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest ( logic_tv_TvFunc_analyze_test
  TvFuncTest_analyze.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest ( logic_tv_all_primes_test
  all_primes_test.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest ( logic_tv_isop_test
  isop_test.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest ( logic_tv_Tv2Bdd_test
  Tv2BddTest.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
