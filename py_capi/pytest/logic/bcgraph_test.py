#! /usr/bin/env python3

""" BcGraph のテストスクリプト

:file: bcgraph_test.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

import pytest
from ymworks.logic import BcGraph, BcEdge, BcOpType


def test_constr1():
    g = BcGraph(input_num=2)

    assert g.input_num == 2
    assert g.output_num == 0
    assert g.op_num == 0

def test_add_output1():
    g = BcGraph(input_num=1)

    e = BcEdge.input(0)
    output_id = g.add_output(e)

    e2 = g.output(output_id)
    assert e2 == e

    assert g.edge_level(e) == 0

def test_add_zero_output1():
    g = BcGraph()

    output_id = g.add_zero_output()
    e = g.output(output_id)

    assert e.is_zero()

    assert g.edge_level(e) == 0

def test_add_one_output1():
    g = BcGraph()

    output_id = g.add_one_output()
    e = g.output(output_id)

    assert e.is_one()

    assert g.edge_level(e) == 0

def test_add_op1():
    g = BcGraph(input_num=2)

    e1 = BcEdge.input(0)
    e2 = BcEdge.input(1)
    op_id = g.add_op(BcOpType.AND, (e1, e2))
    e3 = BcEdge.op(op_id)
    output_id = g.add_output(e3)

    assert g.output_level(output_id) == 1
    assert g.max_level == 1

def test_add_op2():
    g = BcGraph(input_num=3)

    e1 = BcEdge.input(0)
    e2 = BcEdge.input(1)
    e3 = BcEdge.input(2)
    op1_id = g.add_op(BcOpType.AND, (e1, e2))
    e4 = BcEdge.op(op1_id)
    op2_id = g.add_op(BcOpType.OR, (e3, e4))
    e5 = BcEdge.op(op2_id)
    output_id = g.add_output(e5)

    assert g.output_level(output_id) == 2
    assert g.max_level == 2
