#! /usr/bin/env python3

""" ClibDelayModelのテストスクリプト

:file: py_ClibDelayModel_test.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2024 Yusuke Matsunaga, All rights reserved.
"""

import pytest
from ymworks.clib import ClibDelayModel


def test_clibdelaymodel_generic_cmos():
    val1 = ClibDelayModel("generic_cmos")
    assert val1 == ClibDelayModel.generic_cmos
    assert val1.__repr__() == "generic_cmos"

def test_clibdelaymodel_table_lookup():
    val1 = ClibDelayModel("table_lookup")
    assert val1 == ClibDelayModel.table_lookup
    assert val1.__repr__() == "table_lookup"

def test_clibdelaymodel_piecewise_cmos():
    val1 = ClibDelayModel("piecewise_cmos")
    assert val1 == ClibDelayModel.piecewise_cmos
    assert val1.__repr__() == "piecewise_cmos"

def test_clibdelaymodel_cmos2():
    val1 = ClibDelayModel("cmos2")
    assert val1 == ClibDelayModel.cmos2
    assert val1.__repr__() == "cmos2"

def test_clibdelaymodel_dcm():
    val1 = ClibDelayModel("dcm")
    assert val1 == ClibDelayModel.dcm
    assert val1.__repr__() == "dcm"

def test_clibdelaymodel_polynomial():
    val1 = ClibDelayModel("polynomial")
    assert val1 == ClibDelayModel.polynomial
    assert val1.__repr__() == "polynomial"

def test_clibdelaymodel_bad():
    with pytest.raises(ValueError):
        ClibDelayModel("abc")
