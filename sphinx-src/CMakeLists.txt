
# conf.py の生成
set ( YM_DOX_XML_DIR ${CMAKE_CURRENT_BINARY_DIR}/../dox-src )
configure_file (
  ${CMAKE_CURRENT_SOURCE_DIR}/source/conf.py.in
  ${CMAKE_CURRENT_SOURCE_DIR}/source/conf.py
  )