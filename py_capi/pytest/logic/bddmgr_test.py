#! /usr/bin/env python3

""" BddMgr のテストプログラム

:file: bddmgr_test.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2024 Yusuke Matsunaga, All rights reserved.
"""

import pytest
from ymworks.logic import Bdd, BddMgr
from ymworks.logic import Expr
from check_bdd import check


def test_variable1():
    mgr = BddMgr()

    var = mgr.variable(0)
    assert not var.is_invalid()
    assert var.varid == 0

    bdd = var.bdd()
    assert not bdd.is_invalid()
    assert not bdd.is_zero()
    assert not bdd.is_one()

    bdd2 = mgr.variable_bdd(0)
    assert bdd2 == bdd

    var1, f0, f1 = bdd.root_decomp()

    assert var1 == var
    assert f0.is_zero()
    assert f1.is_one()

    assert check(bdd, mgr, "10")

def test_variable2():
    mgr = BddMgr()

    var = mgr.variable(1)
    assert not var.is_invalid()
    assert var.varid == 1

    bdd = ~var.bdd()
    assert not bdd.is_invalid()
    assert not bdd.is_zero()
    assert not bdd.is_one()

    bdd2 = ~mgr.variable_bdd(1)
    assert bdd2 == bdd

    var1, f0, f1 = bdd.root_decomp()

    assert var1 == var
    assert f0.is_one()
    assert f1.is_zero()

    assert check(bdd, mgr, "0101")

def test_variable3():
    mgr = BddMgr()

    var = mgr.variable(2)
    assert not var.is_invalid()
    assert var.varid == 2

    bdd = var.bdd()
    assert not bdd.is_invalid()
    assert not bdd.is_zero()
    assert not bdd.is_one()

    bdd2 = mgr.variable_bdd(2)
    assert bdd2 == bdd

    var1, f0, f1 = bdd.root_decomp()

    assert var1 == var
    assert f0.is_zero()
    assert f1.is_one()

    assert check(bdd, mgr, "10101010")

def test_variable4():
    mgr = BddMgr()

    var = mgr.variable(3)
    assert not var.is_invalid()
    assert var.varid == 3

    bdd = ~var.bdd()
    assert not bdd.is_invalid()
    assert not bdd.is_zero()
    assert not bdd.is_one()

    bdd2 = ~mgr.variable_bdd(3)
    assert bdd2 == bdd

    var1, f0, f1 = bdd.root_decomp()

    assert var1 == var
    assert f0.is_one()
    assert f1.is_zero()

    assert check(bdd, mgr, "0101010101010101")

def test_from_truth1():
    mgr = BddMgr()

    exp_str = "10010110"
    bdd = mgr.from_truth(exp_str)

    assert not bdd.is_invalid()

    assert check(bdd, mgr, exp_str)

def test_from_truth2():
    mgr = BddMgr()

    exp_str = "10010101"
    bdd = mgr.from_truth(exp_str)

    assert not bdd.is_invalid()

    assert check(bdd, mgr, exp_str)

def test_from_truth3():
    mgr = BddMgr()

    var0 = mgr.variable(0)
    var1 = mgr.variable(1)
    var2 = mgr.variable(2)
    var_list = ( var2, var1, var0 )
    exp_str = "10010101"
    bdd = mgr.from_truth(exp_str,
                         var_list=var_list)

    assert not bdd.is_invalid()

    assert check(bdd, mgr, exp_str)

def test_from_expr1():
    mgr = BddMgr()

    var_list = [ mgr.variable(i) for i in range(3) ]
    expr_str = "0 + (1 * ~2)"
    expr = Expr.from_string(expr_str)
    bdd = mgr.from_expr(expr,
                        var_list=var_list)

    assert not bdd.is_invalid()

    print(bdd.to_truth(var_list))

def test_copy():
    mgr1 = BddMgr()

    var_list = [ mgr1.variable(i) for i in range(3) ]
    exp_str = "10010110"
    bdd = mgr1.from_truth(exp_str,
                          var_list=var_list)

    mgr2 = BddMgr()
    bdd2 = mgr2.copy(bdd)

    assert check(bdd2, mgr2, exp_str)
