

# ===================================================================
# doxygen 起動用のコマンド
# ===================================================================

if (DOXYGEN_FOUND)
  # doxygen の対象となるヘッダファイル(のパタン)を YM_DOXY_SRCS に入れる．
  set ( YM_DOXY_SRCS
    "${PROJECT_BINARY_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../py_capi/include"
    )

  # YM_DOXY_SRCS の内容を空白で区切られた文字列に変換する．
  set ( YM_DOXY_INPUTS "" )
  foreach ( _ym_arg ${YM_DOXY_SRCS} )
    set ( YM_DOXY_INPUTS "${YM_DOXY_INPUTS} ${_ym_arg}" )
  endforeach ()
  set ( YM_DOXY_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
  set ( YM_DOXY_INDEX_FILE ${YM_DOXY_OUTPUT_DIR}/xml/index.xml )
  set ( YM_DOXYFILE ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile )

  # Doxyfile の生成
  configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
    ${YM_DOXYFILE}
    @ONLY
    )

  # 対象のヘッダーファイル
  file ( GLOB_RECURSE PUBLIC_HEADERS ${YM_DOXY_SRCS} )

  # doxygen 用ターゲット
  add_custom_target(Doxygen ALL
    DEPENDS ${YM_DOXY_INDEX_FILE}
    )

  cmake_policy( SET CMP0058 NEW )

  # doxygen 起動ルール
  add_custom_command(
    COMMAND "${DOXYGEN_EXECUTABLE}" ${YM_DOXYFILE} ">" "doxygen.log"
    MAIN_DEPENDENCY ${YM_DOXYFILE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${PUBLIC_HEADERS}
    OUTPUT ${YM_DOXY_INDEX_FILE}
    COMMENT "generating doxygen documents"
    )

endif (DOXYGEN_FOUND)
