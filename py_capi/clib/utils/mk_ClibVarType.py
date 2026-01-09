#! /usr/bin/env python3

""" PyClibVarType 用のファイルを生成するスクリプト

:file: mk_ClibVarType.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2024 Yusuke Matsunaga, All rights reserved.
"""

from mk_enumobj import mk_enumobj
from enumspec import EnumSpec
import os.path

basedir = os.path.join(os.path.dirname(__file__), "..")
dist_h_file = os.path.join(basedir, "..", "include", "pym", "PyClibVarType.h")
dist_cc_file = os.path.join(basedir, "PyClibVarType.cc")
dist_py_file = os.path.join(basedir, "..", "tests", "pytest", "py_ClibVarType_test.py")

keywords = [ "input_net_transition",
             "total_output_net_capacitance",
             "output_net_length",
             "output_net_wire_cap",
             "output_net_pin_cap",
             "equal_or_opposite_output_net_capacitance",
             "input_transition_time",
             "related_out_total_output_net_capacitance",
             "related_out_output_net_length",
             "related_out_output_net_wire_cap",
             "related_out_output_net_pin_cap",
             "constrained_pin_transition",
             "related_pin_transition",
             "none"
            ]
mk_enumobj("ClibVarType", keywords, "YM", "ymcell",
           "ym/clib.h", dist_h_file, dist_cc_file, dist_py_file)

