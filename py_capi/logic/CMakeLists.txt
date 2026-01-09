

# ===================================================================
# パッケージの検査
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
set ( logic_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/logic_module.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyVal3.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyPrimType.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyLiteral.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyNpnMap.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyExpr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyTvFunc.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PySopCube.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PySopCover.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyBdd.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyBddVar.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyBddLit.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyBddVarSet.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyBddCube.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyBddMgr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyBddCompMap.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyBddVarMap.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyTruthTable.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyBcOpType.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyBcOp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyBcEdge.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyBcGraph.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PyDjDec.cc
  PARENT_SCOPE
  )


# ===================================================================
#  executable ターゲットの設定
# ===================================================================


# ===================================================================
# テストターゲット
# ===================================================================
