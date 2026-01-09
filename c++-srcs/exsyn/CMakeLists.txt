
# ===================================================================
# インクルードパスの設定
# ===================================================================

include_directories (
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================

add_subdirectory ( gtest )
add_subdirectory ( programs )


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set ( exsyn_SOURCES
  ExSyn.cc
  FenceGen.cc
  PdGen.cc
  BdGen.cc
  EngineBase.cc
  Engine_Naive.cc
  Engine_PD.cc
  Engine_BD.cc
  MakeBcGraph.cc
  XformInfo.cc
  TemplNode.cc
  TemplNode2.cc
  TemplNode3.cc
  TemplNode4.cc
  OpNode.cc
  OpNode2.cc
  OpNode2_SSV.cc
  OpNode2_MSV.cc
  OpNode2_OSV.cc
  OpNode3.cc
  OpNode4.cc
  PdNode2.cc
  PdNode2_0.cc
  PdNode2_1.cc
  PdNode2_1_OneHot.cc
  PdNode2_1_OSV.cc
  PdNode2_1_OSV2.cc
  PdNode2_2_SSV.cc
  PdNode2_2_MSV.cc
  PdNode2_2_OSV.cc
  PdNode2_2_OSSV.cc
  PdNode2_2_OSV2.cc
  PdNode3.cc
  PdNode3_0.cc
  PdNode3_1.cc
  PdNode3_1_OneHot.cc
  PdNode3_2.cc
  PdNode3_2_SSV.cc
  PdNode3_2_MSV.cc
  PdNode3_2_OSV.cc
  PdNode3_3_SSV.cc
  PdNode3_3_MSV.cc
  PdNode3_3_OSV.cc
  PdNode4.cc
  PdNode4_0.cc
  PdNode4_1.cc
  PdNode4_1_OneHot.cc
  PdNode4_2.cc
  PdNode4_2_SSV.cc
  PdNode4_2_MSV.cc
  PdNode4_2_OSV.cc
  PdNode4_3.cc
  PdNode4_3_SSV.cc
  PdNode4_3_MSV.cc
  PdNode4_3_OSV.cc
  PdNode4_4_SSV.cc
  PdNode4_4_MSV.cc
  PdNode4_4_OSV.cc
  BdNode.cc
  )


# ===================================================================
# ターゲットの設定
# ===================================================================

ym_add_object_library ( ym_exsyn
  ${exsyn_SOURCES}
  )


# ===================================================================
#  テストの設定
# ===================================================================


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
