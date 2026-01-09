#! /usr/bin/env python3

""" BcOp のテストスクリプト

:file: bcop_test.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

import pytest
from ymworks.logic import BcOp, BcGraph, BcEdge, BcOpType


def test_and1():
    g = BcGraph(input_num=2)

    e1 = BcEdge.input(0)
    e2 = BcEdge.input(1)
    op_id = g.add_op(BcOpType.AND, (e1, e2))

    op = g.op(op_id)
    assert op.id == op_id
    assert op.type == BcOpType.AND

    e3 = op.operand(0)
    assert e3.is_input()
    assert e3.input_id == 0

    e4 = op.operand(1)
    assert e4.is_input()
    assert e4.input_id == 1

    assert op.level == 1

    assert op.to_string() == "AND(I#0, I#1)"

def test_or():
    g = BcGraph(input_num=2)

    e1 = BcEdge.input(0)
    e2 = BcEdge.input(1)
    op_id = g.add_op(BcOpType.OR, (e1, e2))

    op = g.op(op_id)
    assert op.id == op_id
    assert op.type == BcOpType.OR

    e3 = op.operand(0)
    assert e3.is_input()
    assert e3.input_id == 0

    e4 = op.operand(1)
    assert e4.is_input()
    assert e4.input_id == 1

    assert op.level == 1

    assert op.to_string() == "OR(I#0, I#1)"

def test_xor1():
    g = BcGraph(input_num=2)

    e1 = BcEdge.input(0)
    e2 = BcEdge.input(1)
    op_id = g.add_op(BcOpType.XOR, (e1, e2))

    op = g.op(op_id)
    assert op.id == op_id
    assert op.type == BcOpType.XOR

    e3 = op.operand(0)
    assert e3.is_input()
    assert e3.input_id == 0

    e4 = op.operand(1)
    assert e4.is_input()
    assert e4.input_id == 1

    assert op.level == 1

    assert op.to_string() == "XOR(I#0, I#1)"

def test_equal():
    g = BcGraph(input_num=2)

    e1 = BcEdge.input(0)
    e2 = BcEdge.input(1)

    op1_id = g.add_op(BcOpType.AND, (e1, e2))
    op2_id = g.add_op(BcOpType.AND, (e1, ~e2))
    op3_id = g.add_op(BcOpType.OR, (e1, e2))
    op4_id = g.add_op(BcOpType.OR, (~e1, e2))
    op5_id = g.add_op(BcOpType.XOR, (e1, e2))
    op6_id = g.add_op(BcOpType.XOR, (e1, ~e2))

    op1 = g.op(op1_id)
    op2 = g.op(op2_id)
    op3 = g.op(op3_id)
    op4 = g.op(op4_id)
    op5 = g.op(op5_id)
    op6 = g.op(op6_id)

    op_list = [op1, op2, op3, op4, op5, op6]
    n = len(op_list)
    for i1 in range(n):
        op_1 = op_list[i1]
        for i2 in range(n):
            op_2 = op_list[i2]
            if i1 == i2:
                assert     op_1 == op_2
                assert not op_1 != op_2
            else:
                assert not op_1 == op_2
                assert     op_1 != op_2

    op1_dup_id = g.add_op(BcOpType.AND, (e1, e2))
    op1_dup = g.op(op1_dup_id)
    assert op1 == op1_dup
