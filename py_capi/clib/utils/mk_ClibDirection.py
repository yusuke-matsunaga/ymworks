#! /usr/bin/env python3

""" PyClibDirection 用のファイルを生成するスクリプト

:file: mk_ClibDirection.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2024 Yusuke Matsunaga, All rights reserved.
"""

from mk_enumobj import mk_enumobj
from enumspec import EnumSpec
import os.path

basedir = os.path.join(os.path.dirname(__file__), "..")
dist_h_file = os.path.join(basedir, "..", "include", "pym", "PyClibDirection.h")
dist_cc_file = os.path.join(basedir, "PyClibDirection.cc")
dist_py_file = os.path.join(basedir, "..", "tests", "pytest", "py_ClibDirection_test.py")

keywords = [ "input", "output", "inout", "internal", "none" ]
mk_enumobj("ClibDirection", keywords, "YM", "ymcell",
           "ym/clib.h", dist_h_file, dist_cc_file, dist_py_file)

