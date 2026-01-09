# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ../../c++-srcs/logic/private_include
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================

#add_subdirectory(mwc_test)
add_subdirectory(bcf_test)
add_subdirectory(gen_factor)
add_subdirectory(npn_test)
add_subdirectory(npn_gen)
add_subdirectory(dd)
add_subdirectory(tt)