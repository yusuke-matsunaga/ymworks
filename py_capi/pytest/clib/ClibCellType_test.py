#! /usr/bin/env python3

""" ClibCellTypeのテストスクリプト

:file: py_ClibCellType_test.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2024 Yusuke Matsunaga, All rights reserved.
"""

import pytest
from ymworks.clib import ClibCellType

def test_clibcelltype_Logic():
    val1 = ClibCellType("Logic")
    assert val1 == ClibCellType.Logic
    assert val1.__repr__() == "Logic"

def test_clibcelltype_FF():
    val1 = ClibCellType("FF")
    assert val1 == ClibCellType.FF
    assert val1.__repr__() == "FF"

def test_clibcelltype_Latch():
    val1 = ClibCellType("Latch")
    assert val1 == ClibCellType.Latch
    assert val1.__repr__() == "Latch"

def test_clibcelltype_FSM():
    val1 = ClibCellType("FSM")
    assert val1 == ClibCellType.FSM
    assert val1.__repr__() == "FSM"

def test_clibcelltype_bad():
    with pytest.raises(ValueError):
        ClibCellType("abc")
