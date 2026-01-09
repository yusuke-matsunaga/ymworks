
/// @file py_ymworks.cc
/// @brief py_ymworks の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "pym/ymworks.h"


BEGIN_NAMESPACE_YM

static struct _inittab init_table[] = {
  {"ymworks", PyInit_ymworks},
  {nullptr, nullptr}
};

void
ymworks_init()
{
  PyImport_ExtendInittab(init_table);
}

END_NAMESPACE_YM

int
main(
  int argc,
  char** argv
)
{
  YM_NAMESPACE::ymworks_init();

  return Py_BytesMain(argc, argv);
}
