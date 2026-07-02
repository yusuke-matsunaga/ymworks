
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

ym_add_gtest ( sat_CnfSize_test
  CnfSizeTest.cc
  $<TARGET_OBJECTS:ym_sat_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_aig_obj_d>
  $<TARGET_OBJECTS:ym_misc_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( sat_SatInitParam_test
  SatInitParamTest.cc
  $<TARGET_OBJECTS:ym_sat_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_aig_obj_d>
  $<TARGET_OBJECTS:ym_misc_obj_d>
  DEFINITIONS
  "-DTESTDATA_DIR=\"${TESTDATA_DIR}\""
  ADDRESS_SANITIZE
  )

ym_add_gtest ( sat_SatInitParam2_test
  SatInitParamEnvTest.cc
  $<TARGET_OBJECTS:ym_sat_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_aig_obj_d>
  $<TARGET_OBJECTS:ym_misc_obj_d>
  ADDRESS_SANITIZE
  )

set_property ( TEST sat_SatInitParam2_test
  PROPERTY ENVIRONMENT YMSAT_CONF=${TESTDATA_DIR}/sat/ymsat.json
  )

ym_add_gtest ( sat_SatInitParam3_test
  SatInitParamEnvTest.cc
  $<TARGET_OBJECTS:ym_sat_obj_d>
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_aig_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_misc_obj_d>
  ADDRESS_SANITIZE
  )

set_property ( TEST sat_SatInitParam3_test
  PROPERTY ENVIRONMENT YMSAT_CONFDIR=${TESTDATA_DIR}/sat
  )

ym_add_gtest ( sat_SatBool3_test
  SatBool3Test.cc
  $<TARGET_OBJECTS:ym_sat_obj_d>
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_aig_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_misc_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( sat_SatLiteral_test
  SatLiteralTest.cc
  $<TARGET_OBJECTS:ym_sat_obj_d>
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_aig_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_misc_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( sat_add_clause_test
  add_clause_test.cc
  SatTestFixture.cc
  $<TARGET_OBJECTS:ym_sat_obj_d>
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_aig_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_misc_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( sat_tseitinenc_test
  tseitinenc_test.cc
  SatTestFixture.cc
  $<TARGET_OBJECTS:ym_sat_obj_d>
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_aig_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_misc_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( sat_add_expr_test
  add_expr_test.cc
  $<TARGET_OBJECTS:ym_sat_obj_d>
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_aig_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_misc_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( sat_add_aig_test
  add_aig_test.cc
  $<TARGET_OBJECTS:ym_sat_obj_d>
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_aig_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_misc_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( sat_adder_test
  adder_test.cc
  SatTestFixture.cc
  $<TARGET_OBJECTS:ym_sat_obj_d>
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_aig_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_misc_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( sat_counter_test
  counter_test.cc
  SatTestFixture.cc
  $<TARGET_OBJECTS:ym_sat_obj_d>
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_aig_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_misc_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( sat_at_most_test
  at_most_test.cc
  SatTestFixture.cc
  $<TARGET_OBJECTS:ym_sat_obj_d>
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_aig_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_misc_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( sat_comp_test
  comp_test.cc
  SatTestFixture.cc
  $<TARGET_OBJECTS:ym_sat_obj_d>
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_aig_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_misc_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( sat_comp_const_test
  comp_const_test.cc
  SatTestFixture.cc
  $<TARGET_OBJECTS:ym_sat_obj_d>
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_aig_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_misc_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( sat_conflict_test
  conflict_test.cc
  SatTestFixture.cc
  $<TARGET_OBJECTS:ym_sat_obj_d>
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_aig_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_misc_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( sat_SatOrderedSet_test
  SatOrderedSet_test.cc
  $<TARGET_OBJECTS:ym_sat_obj_d>
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_aig_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_misc_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( sat_SatOneHotVar_test
  SatOneHotVar_test.cc
  $<TARGET_OBJECTS:ym_sat_obj_d>
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_aig_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_misc_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( sat_SatOneHotMatrix_test
  SatOneHotMatrix_test.cc
  $<TARGET_OBJECTS:ym_sat_obj_d>
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_aig_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_misc_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( sat_SatOneHotMatrix_test2
  SatOneHotMatrix_test2.cc
  $<TARGET_OBJECTS:ym_sat_obj_d>
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_aig_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_misc_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( sat_Dimacs_test
  DimacsTest.cc
  $<TARGET_OBJECTS:ym_sat_obj_d>
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_aig_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_misc_obj_d>
  DEFINITIONS
  "-DTESTDATA_DIR=\"${TESTDATA_DIR}\""
  ADDRESS_SANITIZE
  )

ym_add_gtest ( sat_SatSolver_test
  SatSolver_test.cc
  $<TARGET_OBJECTS:ym_sat_obj_d>
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_aig_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_misc_obj_d>
  ADDRESS_SANITIZE
  )

ym_add_gtest ( sat_timer_test
  timer_test.cc
  $<TARGET_OBJECTS:ym_sat_obj_d>
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_aig_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_misc_obj_d>
  ADDRESS_SANITIZE
  )
