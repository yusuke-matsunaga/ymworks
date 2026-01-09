#! /usr/bin/env python3

""" PyClibTechnology 用のファイルを生成するスクリプト

:file: mk_ClibTechnology.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2024 Yusuke Matsunaga, All rights reserved.
"""

from mk_enumobj import mk_enumobj
from enumspec import EnumSpec
import os.path

basedir = os.path.join(os.path.dirname(__file__), "..")
dist_h_file = os.path.join(basedir, "..", "include", "pym", "PyClibTechnology.h")
dist_cc_file = os.path.join(basedir, "PyClibTechnology.cc")
dist_py_file = os.path.join(basedir, "..", "tests", "pytest", "py_ClibTechnology_test.py")

keywords = [ "cmos", "fpga", "none" ]
mk_enumobj("ClibTechnology", keywords, "YM", "ymcell",
           "ym/clib.h", dist_h_file, dist_cc_file, dist_py_file)

