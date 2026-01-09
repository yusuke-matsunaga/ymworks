#! /usr/bin/env python3

""" BcEdge のテストスクリプト

:file: bcedge_test.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

import pytest
from ymworks.logic import BcEdge, BcGraph, BcOpType


def test_input1():
    input_id = 7
    e = BcEdge.input(input_id)

    assert e.is_input()
    assert not e.is_op()
    assert not e.is_zero()
    assert not e.is_one()
    assert not e.inv

    assert e.input_id == input_id
    with pytest.raises(ValueError) as err:
        e.op_id

    exp_str = f'I#{input_id}'
    assert e.to_string() == exp_str

def test_input2():
    input_id = 7
    e = BcEdge.input(input_id, inv=False)

    assert e.is_input()
    assert not e.is_op()
    assert not e.is_zero()
    assert not e.is_one()
    assert not e.inv

    assert e.input_id == input_id
    with pytest.raises(ValueError) as err:
        e.op_id

    exp_str = f'I#{input_id}'
    assert e.to_string() == exp_str

def test_input_inv1():
    input_id = 7
    e = BcEdge.input(input_id, inv=True)

    assert e.is_input()
    assert not e.is_op()
    assert not e.is_zero()
    assert not e.is_one()
    assert e.inv

    assert e.input_id == input_id
    with pytest.raises(ValueError) as err:
        e.op_id

    exp_str = f'~I#{input_id}'
    assert e.to_string() == exp_str

def test_op1():
    g = BcGraph(input_num=2)

    e1 = BcEdge.input(0)
    e2 = BcEdge.input(1)
    op_id = g.add_op(BcOpType.AND, (e1, e2))
    e = BcEdge.op(op_id)

    assert not e.is_input()
    assert     e.is_op()
    assert not e.is_zero()
    assert not e.is_one()
    assert not e.inv

    with pytest.raises(ValueError):
        e.input_id
    assert e.op_id == op_id

    exp_str = f'OP#{op_id}'
    assert e.to_string() == exp_str

def test_op2():
    g = BcGraph(input_num=2)

    e1 = BcEdge.input(0)
    e2 = BcEdge.input(1)
    op_id = g.add_op(BcOpType.AND, (e1, e2))
    e = BcEdge.op(op_id, inv=False)

    assert not e.is_input()
    assert     e.is_op()
    assert not e.is_zero()
    assert not e.is_one()
    assert not e.inv

    with pytest.raises(ValueError):
        e.input_id
    assert e.op_id == op_id

    exp_str = f'OP#{op_id}'
    assert e.to_string() == exp_str

def test_op_inv1():
    g = BcGraph(input_num=2)

    e1 = BcEdge.input(0)
    e2 = BcEdge.input(1)
    op_id = g.add_op(BcOpType.AND, (e1, e2))
    e = BcEdge.op(op_id, inv=True)

    assert not e.is_input()
    assert     e.is_op()
    assert not e.is_zero()
    assert not e.is_one()
    assert     e.inv

    with pytest.raises(ValueError):
        e.input_id
    assert e.op_id == op_id

    exp_str = f'~OP#{op_id}'
    assert e.to_string() == exp_str

def test_zero():
    e = BcEdge.zero()

    assert not e.is_input()
    assert not e.is_op()
    assert     e.is_zero()
    assert not e.is_one()
    assert not e.inv

    with pytest.raises(ValueError):
        e.input_id
    with pytest.raises(ValueError):
        e.op_id

    assert e.to_string() == "0"

def test_one():
    e = BcEdge.one()

    assert not e.is_input()
    assert not e.is_op()
    assert not e.is_zero()
    assert     e.is_one()
    assert     e.inv

    with pytest.raises(ValueError):
        e.input_id
    with pytest.raises(ValueError):
        e.op_id

    assert e.to_string() == "1"

def test_invert1():
    g = BcGraph(input_num=2)

    input1 = BcEdge.input(0)
    input1_inv = BcEdge.input(0, inv=True)
    assert ~input1 == input1_inv
    assert ~input1_inv == input1

    input2 = BcEdge.input(1)
    op1_id = g.add_op(BcOpType.AND, (input1, input2))
    op1_edge = BcEdge.op(op1_id)
    op1_inv_edge = BcEdge.op(op1_id, inv=True)
    assert ~op1_edge == op1_inv_edge
    assert ~op1_inv_edge == op1_edge

    zero = BcEdge.zero()
    one = BcEdge.one()
    assert ~zero == one
    assert ~one == zero
