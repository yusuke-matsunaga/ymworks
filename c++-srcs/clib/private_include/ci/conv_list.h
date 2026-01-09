#ifndef CONV_LIST_H
#define CONV_LIST_H

/// @file conv_list.h
/// @brief conv_list のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"


BEGIN_NAMESPACE_YM_CLIB

template<class T>
std::vector<const T*>
conv_list(
  const std::vector<std::unique_ptr<T>>& src_list
)
{
  auto n = src_list.size();
  std::vector<const T*> tmp_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    tmp_list[i] = src_list[i].get();
  }
  return tmp_list;
}

END_NAMESPACE_YM_CLIB

#endif // CONV_LIST_H
