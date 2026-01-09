#! /usr/bin/env python3

""" PyClibDelayModel 用のファイルを生成するスクリプト

:file: mk_ClibDelayModel.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2024 Yusuke Matsunaga, All rights reserved.
"""

from mk_enumobj import mk_enumobj
from enumspec import EnumSpec
import os.path

basedir = os.path.join(os.path.dirname(__file__), "..")
dist_h_file = os.path.join(basedir, "..", "include", "pym", "PyClibDelayModel.h")
dist_cc_file = os.path.join(basedir, "PyClibDelayModel.cc")
dist_py_file = os.path.join(basedir, "..", "tests", "pytest", "py_ClibDelayModel_test.py")

keywords = [ "generic_cmos", "table_lookup", "piecewise_cmos",
             "cmos2", "dcm", "polynomial", "none" ]
mk_enumobj("ClibDelayModel", keywords, "YM", "ymcell",
           "ym/clib.h", dist_h_file, dist_cc_file, dist_py_file)

