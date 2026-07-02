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

ym_add_gtest ( logic_rc_RcMatrix_test
  RcMatrixTest.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( logic_rc_CkMatrix_test
  CkMatrixTest.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( logic_rc_FindRect_test
  FindRectTest.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( logic_rc_EnumRect_test
  EnumRectTest.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( logic_rc_KIX_test
  KIXTest.cc
  $<TARGET_OBJECTS:ym_misc_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  ADDRESS_SANITIZE
  )


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
