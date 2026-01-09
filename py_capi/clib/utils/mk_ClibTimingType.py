#! /usr/bin/env python3

""" PyClibTimingType 用のファイルを生成するスクリプト

:file: mk_ClibTimingType.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2024 Yusuke Matsunaga, All rights reserved.
"""

from mk_enumobj import mk_enumobj
from enumspec import EnumSpec
import os.path

basedir = os.path.join(os.path.dirname(__file__), "..")
dist_h_file = os.path.join(basedir, "..", "include", "pym", "PyClibTimingType.h")
dist_cc_file = os.path.join(basedir, "PyClibTimingType.cc")
dist_py_file = os.path.join(basedir, "..", "tests", "pytest", "py_ClibTimingType_test.py")

keywords = [ "combinational",
             "combinational_rise",
             "combinational_fall",
             "three_state_enable",
             "three_state_disable",
             "three_state_enable_rise",
             "three_state_enable_fall",
             "three_state_disable_rise",
             "three_state_disable_fall",
             "rising_edge",
             "falling_edge",
             "preset",
             "clear",
             "hold_rising",
             "hold_falling",
             "setup_rising",
             "setup_falling",
             "recovery_rising",
             "recovery_falling",
             "skew_rising",
             "skew_falling",
             "removal_rising",
             "removal_falling",
             "non_seq_setup_rising",
             "non_seq_setup_falling",
             "non_seq_hold_rising",
             "non_seq_hold_falling",
             "nochange_high_high",
             "nochange_high_low",
             "nochange_low_high",
             "nochange_low_low",
             "none" ]
mk_enumobj("ClibTimingType", keywords, "YM", "ymcell",
           "ym/clib.h", dist_h_file, dist_cc_file, dist_py_file)

