#! /usr/bin/env python3

""" PyClibCellType 用のファイルを生成するスクリプト

:file: mk_ClibCellType.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2024 Yusuke Matsunaga, All rights reserved.
"""

from mk_enumobj import mk_enumobj
from enumspec import EnumSpec
import os.path

basedir = os.path.join(os.path.dirname(__file__), "..")
dist_h_file = os.path.join(basedir, "..", "include", "pym", "PyClibCellType.h")
dist_cc_file = os.path.join(basedir, "PyClibCellType.cc")
dist_py_file = os.path.join(basedir, "..", "tests", "pytest", "py_ClibCellType_test.py")

keywords = [ "none", "Logic", "FF", "Latch", "FSM" ]
mk_enumobj("ClibCellType", keywords, "YM", "ymcell",
           "ym/clib.h", dist_h_file, dist_cc_file, dist_py_file)

