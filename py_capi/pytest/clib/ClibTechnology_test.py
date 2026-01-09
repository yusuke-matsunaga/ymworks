#! /usr/bin/env python3

""" ClibTechnologyのテストスクリプト

:file: py_ClibTechnology_test.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2024 Yusuke Matsunaga, All rights reserved.
"""

import pytest
from ymworks.clib import ClibTechnology


def test_clibtechnology_cmos():
    val1 = ClibTechnology("cmos")
    assert val1 == ClibTechnology.cmos
    assert val1.__repr__() == "cmos"

def test_clibtechnology_fpga():
    val1 = ClibTechnology("fpga")
    assert val1 == ClibTechnology.fpga
    assert val1.__repr__() == "fpga"

def test_clibtechnology_bad():
    with pytest.raises(ValueError):
        ClibTechnology("abc")
