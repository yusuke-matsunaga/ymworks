#! /usr/bin/env python3

""" ClibVarTypeのテストスクリプト

:file: py_ClibVarType_test.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2024 Yusuke Matsunaga, All rights reserved.
"""

import pytest
from ymworks.clib import ClibVarType


def test_clibvartype_input_net_transition():
    val1 = ClibVarType("input_net_transition")
    assert val1 == ClibVarType.input_net_transition
    assert val1.__repr__() == "input_net_transition"

def test_clibvartype_total_output_net_capacitance():
    val1 = ClibVarType("total_output_net_capacitance")
    assert val1 == ClibVarType.total_output_net_capacitance
    assert val1.__repr__() == "total_output_net_capacitance"

def test_clibvartype_output_net_length():
    val1 = ClibVarType("output_net_length")
    assert val1 == ClibVarType.output_net_length
    assert val1.__repr__() == "output_net_length"

def test_clibvartype_output_net_wire_cap():
    val1 = ClibVarType("output_net_wire_cap")
    assert val1 == ClibVarType.output_net_wire_cap
    assert val1.__repr__() == "output_net_wire_cap"

def test_clibvartype_output_net_pin_cap():
    val1 = ClibVarType("output_net_pin_cap")
    assert val1 == ClibVarType.output_net_pin_cap
    assert val1.__repr__() == "output_net_pin_cap"

def test_clibvartype_equal_or_opposite_output_net_capacitance():
    val1 = ClibVarType("equal_or_opposite_output_net_capacitance")
    assert val1 == ClibVarType.equal_or_opposite_output_net_capacitance
    assert val1.__repr__() == "equal_or_opposite_output_net_capacitance"

def test_clibvartype_input_transition_time():
    val1 = ClibVarType("input_transition_time")
    assert val1 == ClibVarType.input_transition_time
    assert val1.__repr__() == "input_transition_time"

def test_clibvartype_related_out_total_output_net_capacitance():
    val1 = ClibVarType("related_out_total_output_net_capacitance")
    assert val1 == ClibVarType.related_out_total_output_net_capacitance
    assert val1.__repr__() == "related_out_total_output_net_capacitance"

def test_clibvartype_related_out_output_net_length():
    val1 = ClibVarType("related_out_output_net_length")
    assert val1 == ClibVarType.related_out_output_net_length
    assert val1.__repr__() == "related_out_output_net_length"

def test_clibvartype_related_out_output_net_wire_cap():
    val1 = ClibVarType("related_out_output_net_wire_cap")
    assert val1 == ClibVarType.related_out_output_net_wire_cap
    assert val1.__repr__() == "related_out_output_net_wire_cap"

def test_clibvartype_related_out_output_net_pin_cap():
    val1 = ClibVarType("related_out_output_net_pin_cap")
    assert val1 == ClibVarType.related_out_output_net_pin_cap
    assert val1.__repr__() == "related_out_output_net_pin_cap"

def test_clibvartype_constrained_pin_transition():
    val1 = ClibVarType("constrained_pin_transition")
    assert val1 == ClibVarType.constrained_pin_transition
    assert val1.__repr__() == "constrained_pin_transition"

def test_clibvartype_related_pin_transition():
    val1 = ClibVarType("related_pin_transition")
    assert val1 == ClibVarType.related_pin_transition
    assert val1.__repr__() == "related_pin_transition"

def test_clibvartype_bad():
    with pytest.raises(ValueError):
        ClibVarType("abc")
