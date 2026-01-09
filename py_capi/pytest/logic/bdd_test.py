#! /usr/bin/env python3

""" Bdd のテストスクリプト

:file: bdd_test.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

import pytest
from ymworks.logic import Bdd, BddVar, BddLit, BddCube, BddMgr
from check_bdd import check


def test_invalid1():
    # 不正値になっているはず
    bdd = Bdd()

    assert not bdd.is_zero()
    assert not bdd.is_one()
    assert not bdd.is_const()
    assert bdd.is_invalid()

    mgr = bdd.mgr
    assert not mgr.is_valid()

    # 不正値でも否定は計算できる．
    ibdd = ~bdd
    assert not ibdd.is_zero()
    assert not ibdd.is_one()
    assert not ibdd.is_const()
    assert ibdd.is_invalid()

    mgr2 = ibdd.mgr
    assert not mgr2.is_valid()

def test_invalid2():
    # 不正値になっているはず
    bdd = Bdd.invalid()

    assert not bdd.is_zero()
    assert not bdd.is_one()
    assert not bdd.is_const()
    assert bdd.is_invalid()

    mgr = bdd.mgr
    assert not mgr.is_valid()

    # 不正値でも否定は計算できる．
    ibdd = ~bdd
    assert not ibdd.is_zero()
    assert not ibdd.is_one()
    assert not ibdd.is_const()
    assert ibdd.is_invalid()

    mgr2 = ibdd.mgr
    assert not mgr2.is_valid()

def test_zero():
    bdd = Bdd.zero()

    assert bdd.is_zero()
    assert not bdd.is_one()
    assert bdd.is_const()
    assert not bdd.is_invalid()

    mgr = bdd.mgr
    assert not mgr.is_valid()

    ibdd = ~bdd
    assert not ibdd.is_zero()
    assert ibdd.is_one()
    assert ibdd.is_const()
    assert not ibdd.is_invalid()

    mgr2 = ibdd.mgr
    assert not mgr2.is_valid()

    mgr = BddMgr() # ダミー
    assert check(bdd, mgr, "0")

def test_one():
    bdd = Bdd.one()

    assert not bdd.is_zero()
    assert bdd.is_one()
    assert bdd.is_const()
    assert not bdd.is_invalid()

    mgr = bdd.mgr
    assert not mgr.is_valid()

    ibdd = ~bdd
    assert ibdd.is_zero()
    assert not ibdd.is_one()
    assert ibdd.is_const()
    assert not ibdd.is_invalid()

    mgr2 = ibdd.mgr
    assert not mgr2.is_valid()

    mgr = BddMgr() # ダミー
    assert check(bdd, mgr, "1")

def test_variable1():
    mgr = BddMgr()
    var = mgr.variable(0)
    bdd = var.bdd()

    assert check(bdd, mgr, "10")

def test_variable2():
    mgr = BddMgr()
    var = mgr.variable(1)
    bdd = ~var.bdd()

    assert check(bdd, mgr, "0101")

def test_variable3():
    mgr = BddMgr()
    var = mgr.variable(2)
    bdd = var.bdd()

    assert check(bdd, mgr, "10101010")

def test_variable4():
    mgr = BddMgr()
    var = mgr.variable(3)
    bdd = ~var.bdd()

    assert check(bdd, mgr, "0101010101010101")

def test_invert1():
    mgr = BddMgr()
    src_str = "1101"
    bdd = mgr.from_truth(src_str)

    bdd1 = ~bdd
    exp_str = "0010"

    assert check(bdd1, mgr, exp_str)

def test_invert_invalid():
    mgr = BddMgr()
    bdd = Bdd.invalid()

    assert bdd.is_invalid()

    bdd1 = ~bdd

    assert bdd1.is_invalid()

def test_pol_inv1():
    mgr = BddMgr()
    src_str = "1101"
    bdd = mgr.from_truth(src_str)

    bdd1 = bdd * True

    exp_str = "0010"
    assert check(bdd1, mgr, exp_str)

def test_pol_inv_int1():
    mgr = BddMgr()
    src_str = "1101"
    bdd = mgr.from_truth(src_str)

    bdd *= True
    exp_str = "0010"
    assert check(bdd, mgr, exp_str)

def test_pol_inv1_invalid():
    bdd = Bdd.invalid()

    assert bdd.is_invalid()

    bdd1 = bdd * True
    assert bdd1.is_invalid()

def test_pol_inv_int1_invalid():
    bdd = Bdd.invalid()

    assert bdd.is_invalid()

    bdd *= True
    assert bdd.is_invalid()

def test_pol_inv2():
    mgr = BddMgr()
    src_str = "1101"
    bdd = mgr.from_truth(src_str)

    bdd1 = bdd * False

    assert check(bdd1, mgr, src_str)

def test_pol_inv_int2():
    mgr = BddMgr()
    src_str = "1101"
    bdd = mgr.from_truth(src_str)

    bdd *= False

    assert check(bdd, mgr, src_str)

def test_pol_inv2_invalid():
    bdd = Bdd.invalid()

    bdd1 = bdd * False
    assert bdd1.is_invalid()

def test_pol_inv_int2_invalid():
    bdd = Bdd.invalid()

    bdd *= False
    assert bdd.is_invalid()

def test_and_op1():
    mgr = BddMgr()
    bdd1 = mgr.variable_bdd(0)
    bdd2 = mgr.variable_bdd(1)
    bdd = bdd1 & bdd2

    assert check(bdd, mgr, "1000")

def test_and_op2():
    mgr = BddMgr()
    bdd1 = Bdd.zero()
    bdd2 = mgr.variable_bdd(1)
    bdd = bdd1 & bdd2

    assert check(bdd, mgr, "0000")

def test_and_op3():
    mgr = BddMgr()
    bdd1 = mgr.variable_bdd(0)
    bdd2 = Bdd.zero()
    bdd = bdd1 & bdd2

    assert check(bdd, mgr, "0000")

def test_and_op4():
    mgr = BddMgr()
    bdd1 = Bdd.one()
    bdd2 = mgr.variable_bdd(1)
    bdd = bdd1 & bdd2

    assert check(bdd, mgr, "1010")

def test_and_op5():
    mgr = BddMgr()
    bdd1 = mgr.variable_bdd(0)
    bdd2 = Bdd.one()
    bdd = bdd1 & bdd2

    assert check(bdd, mgr, "1100")

def test_and_op_invalid1():
    mgr = BddMgr()
    bdd1 = Bdd.invalid()
    bdd2 = mgr.variable_bdd(1)

    with pytest.raises(ValueError) as e:
        bdd1 & bdd2

def test_and_op_invalid2():
    mgr = BddMgr()
    bdd1 = mgr.variable_bdd(0)
    bdd2 = Bdd.invalid()

    with pytest.raises(ValueError) as e:
        bdd1 & bdd2

def test_and_op_invalid3():
    mgr = BddMgr()
    bdd1 = mgr.variable_bdd(0)
    mgr2 = BddMgr()
    bdd2 = mgr2.variable_bdd(1)

    with pytest.raises(ValueError) as e:
        bdd1 & bdd2

def test_iand_op1():
    mgr = BddMgr()
    bdd1 = mgr.variable_bdd(0)
    bdd2 = mgr.variable_bdd(1)
    bdd1 &= bdd2

    exp_str = "1000"
    assert check(bdd1, mgr, exp_str)

def test_iand_op2():
    mgr = BddMgr()
    bdd1 = Bdd.zero()
    bdd2 = mgr.variable_bdd(1)
    bdd1 &= bdd2

    exp_str = "0000"
    assert check(bdd1, mgr, exp_str)

def test_iand_op3():
    mgr = BddMgr()
    bdd1 = mgr.variable_bdd(0)
    bdd2 = Bdd.zero()
    bdd1 &= bdd2

    exp_str = "0000"
    assert check(bdd1, mgr, exp_str)

def test_iand_op4():
    mgr = BddMgr()
    bdd1 = Bdd.one()
    bdd2 = mgr.variable_bdd(1)
    bdd1 &= bdd2

    exp_str = "1010"
    assert check(bdd1, mgr, exp_str)

def test_iand_op5():
    mgr = BddMgr()
    bdd1 = mgr.variable_bdd(0)
    bdd2 = Bdd.one()
    bdd1 &= bdd2

    exp_str = "1100"
    assert check(bdd1, mgr, exp_str)

def test_iand_op_invalid1():
    mgr = BddMgr()
    bdd1 = Bdd.invalid()
    bdd2 = mgr.variable_bdd(1)

    with pytest.raises(ValueError) as e:
        bdd1 &= bdd2

def test_iand_op_invalid2():
    mgr = BddMgr()
    bdd1 = mgr.variable_bdd(0)
    bdd2 = Bdd.invalid()

    with pytest.raises(ValueError) as e:
        bdd1 &= bdd2

def test_iand_op_invalid3():
    mgr = BddMgr()
    bdd1 = mgr.variable_bdd(0)
    mgr2 = BddMgr()
    bdd2 = mgr2.variable_bdd(1)

    with pytest.raises(ValueError) as e:
        bdd1 &= bdd2

def test_or_op1():
    mgr = BddMgr()
    bdd1 = mgr.variable_bdd(0)
    bdd2 = mgr.variable_bdd(1)
    bdd = bdd1 | bdd2

    exp_str = "1110"
    assert check(bdd, mgr, exp_str)

def test_or_op_invalid1():
    mgr = BddMgr()
    bdd1 = Bdd.invalid()
    bdd2 = mgr.variable_bdd(1)

    with pytest.raises(ValueError) as e:
        bdd1 | bdd2

def test_or_op_invalid2():
    mgr = BddMgr()
    bdd1 = mgr.variable_bdd(0)
    bdd2 = Bdd.invalid()

    with pytest.raises(ValueError) as e:
        bdd1 | bdd2

def test_or_op_invalid3():
    mgr = BddMgr()
    bdd1 = mgr.variable_bdd(0)
    mgr2 = BddMgr()
    bdd2 = mgr2.variable_bdd(1)

    with pytest.raises(ValueError) as e:
        bdd1 | bdd2

def test_ior_op1():
    mgr = BddMgr()
    bdd1 = mgr.variable_bdd(0)
    bdd2 = mgr.variable_bdd(1)
    bdd1 |= bdd2

    exp_str = "1110"
    assert check(bdd1, mgr, exp_str)

def test_ior_op_invalid1():
    mgr = BddMgr()
    bdd1 = Bdd.invalid()
    bdd2 = mgr.variable_bdd(1)

    with pytest.raises(ValueError) as e:
        bdd1 |= bdd2

def test_ior_op_invalid2():
    mgr = BddMgr()
    bdd1 = mgr.variable_bdd(0)
    bdd2 = Bdd.invalid()

    with pytest.raises(ValueError) as e:
        bdd1 |= bdd2

def test_ior_op_invalid3():
    mgr = BddMgr()
    bdd1 = mgr.variable_bdd(0)
    mgr2 = BddMgr()
    bdd2 = mgr2.variable_bdd(1)

    with pytest.raises(ValueError) as e:
        bdd1 |= bdd2

def test_xor_op1():
    mgr = BddMgr()
    bdd1 = mgr.variable_bdd(0)
    bdd2 = mgr.variable_bdd(1)
    bdd = bdd1 ^ bdd2

    exp_str = "0110"
    assert check(bdd, mgr, exp_str)

def test_xor_op_invalid1():
    mgr = BddMgr()
    bdd1 = Bdd.invalid()
    bdd2 = mgr.variable_bdd(1)

    with pytest.raises(ValueError) as e:
        bdd1 ^ bdd2

def test_xor_op_invalid2():
    mgr = BddMgr()
    bdd1 = mgr.variable_bdd(0)
    bdd2 = Bdd.invalid()

    with pytest.raises(ValueError) as e:
        bdd1 ^ bdd2

def test_ixor_op1():
    mgr = BddMgr()
    bdd1 = mgr.variable_bdd(0)
    bdd2 = mgr.variable_bdd(1)
    bdd1 ^= bdd2

    exp_str = "0110"
    assert check(bdd1, mgr, exp_str)

def test_ixor_op_invalid1():
    mgr = BddMgr()
    bdd1 = Bdd.invalid()
    bdd2 = mgr.variable_bdd(1)

    with pytest.raises(ValueError) as e:
        bdd1 ^= bdd2

def test_ixor_op_invalid2():
    mgr = BddMgr()
    bdd1 = mgr.variable_bdd(0)
    bdd2 = Bdd.invalid()

    with pytest.raises(ValueError) as e:
        bdd1 ^= bdd2

def test_xor3():
    mgr = BddMgr()
    bdd1 = mgr.variable_bdd(0)
    bdd2 = mgr.variable_bdd(1)
    bdd3 = mgr.variable_bdd(2)
    bdd = bdd1 ^ bdd2 ^ bdd3

    assert check(bdd, mgr, "10010110")

def test_complex_expr1():
    mgr = BddMgr()
    bdd1 = mgr.variable_bdd(0)
    bdd2 = mgr.variable_bdd(1)
    bdd3 = mgr.variable_bdd(2)
    bdd = (bdd1 & ~bdd2) | bdd3

    assert check(bdd, mgr, "10111010")

def test_complex_expr_invalid1():
    mgr = BddMgr()
    bdd1 = mgr.variable_bdd(0)
    bdd2 = mgr.variable_bdd(1)
    bdd3 = mgr.variable_bdd(2)
    mgr2 = BddMgr()
    bdd4 = mgr2.variable_bdd(3)

    with pytest.raises(ValueError) as e:
        (bdd1 & ~bdd2) | (bdd3 & ~bdd4)

def test_ite1():
    mgr = BddMgr()
    bdd1 = mgr.from_truth("10101010")
    bdd2 = mgr.from_truth("11001100")
    bdd3 = mgr.from_truth("11110000")
    bdd = Bdd.ite(bdd1, bdd2, bdd3)

    exp_bdd = bdd1 & bdd2 | ~bdd1 & bdd3
    assert exp_bdd == bdd

def test_ite2():
    mgr = BddMgr()
    bdd1 = Bdd.zero()
    bdd2 = mgr.from_truth("11001100")
    bdd3 = mgr.from_truth("11110000")
    bdd = Bdd.ite(bdd1, bdd2, bdd3)

    exp_bdd = bdd3
    assert exp_bdd == bdd

def test_ite3():
    mgr = BddMgr()
    bdd1 = Bdd.one()
    bdd2 = mgr.from_truth("11001100")
    bdd3 = mgr.from_truth("11110000")
    bdd = Bdd.ite(bdd1, bdd2, bdd3)

    exp_bdd = bdd2
    assert exp_bdd == bdd

def test_ite1():
    mgr = BddMgr()
    bdd1 = Bdd.one()
    bdd2 = Bdd.zero()
    bdd3 = Bdd.one()
    bdd = Bdd.ite(bdd1, bdd2, bdd3)

    exp_bdd = Bdd.zero()
    assert exp_bdd == bdd

def test_ite_invalid1():
    mgr = BddMgr()
    bdd1 = Bdd.invalid()
    bdd2 = mgr.from_truth("11001100")
    bdd3 = mgr.from_truth("11110000")

    with pytest.raises(ValueError) as e:
        Bdd.ite(bdd1, bdd2, bdd3)

def test_ite_invalid2():
    mgr = BddMgr()
    bdd1 = mgr.from_truth("10101010")
    bdd2 = Bdd.invalid()
    bdd3 = mgr.from_truth("11110000")

    with pytest.raises(ValueError) as e:
        Bdd.ite(bdd1, bdd2, bdd3)


def test_ite_invalid3():
    mgr = BddMgr()
    bdd1 = mgr.from_truth("10101010")
    bdd2 = mgr.from_truth("11001100")
    bdd3 = Bdd.invalid()

    with pytest.raises(ValueError) as e:
        Bdd.ite(bdd1, bdd2, bdd3)

def test_simplify1():
    mgr = BddMgr()
    src_str = "10010111"
    bdd1 = mgr.from_truth(src_str)
    bdd2 = Bdd.zero()
    bdd = Bdd.simplify(bdd1, bdd2)
    assert bdd1 == bdd

def test_simplify_invalid1():
    mgr = BddMgr()
    bdd1 = Bdd.invalid()
    bdd2 = Bdd.zero()

    with pytest.raises(ValueError) as e:
        Bdd.simplify(bdd1, bdd2)

def test_simplify_invalid2():
    mgr = BddMgr()
    src_str = "10010111"
    bdd1 = mgr.from_truth(src_str)
    bdd2 = Bdd.invalid()

    with pytest.raises(ValueError) as e:
        Bdd.simplify(bdd1, bdd2)

def test_cofactor1():
    # (BddVar) のみのバージョン
    mgr = BddMgr()
    src_str = "10010111"
    bdd = mgr.from_truth(src_str)
    var = mgr.variable(0)
    bdd1 = bdd.cofactor(var)

    exp_str = "10011001"

    assert check(bdd1, mgr, exp_str)

def test_cofactor1_invalid1():
    mgr = BddMgr()
    bdd = Bdd.invalid()
    var = mgr.variable(0)

    with pytest.raises(ValueError) as e:
        bdd.cofactor(var)

def test_cofactor1_invalid2():
    mgr = BddMgr()
    src_str = "10010111"
    bdd = mgr.from_truth(src_str)
    var = BddVar.invalid()

    with pytest.raises(ValueError) as e:
        bdd.cofactor(var)

def test_cofactor2():
    # (BddVar, inv=Bool) のバージョン
    mgr = BddMgr()
    src_str = "10010111"
    bdd = mgr.from_truth(src_str)
    var = mgr.variable(0)
    bdd1 = bdd.cofactor(var, inv=True)

    exp_str = "01110111"
    assert check(bdd1, mgr, exp_str)

def test_cofactor2_invalid1():
    mgr = BddMgr()
    bdd = Bdd.invalid()
    var = mgr.variable(0)

    with pytest.raises(ValueError) as e:
        bdd.cofactor(var, inv=True)

def test_cofactor2_invalid2():
    mgr = BddMgr()
    src_str = "10010111"
    bdd = mgr.from_truth(src_str)
    var = BddVar.invalid()

    with pytest.raises(ValueError) as e:
        bdd.cofactor(var, inv=True)

def test_cofactor3():
    # (BddLit) のバージョン
    mgr = BddMgr()
    src_str = "10010111"
    bdd = mgr.from_truth(src_str)
    lit = mgr.literal(0, inv=True)
    bdd1 = bdd.cofactor(lit)

    exp_str = "01110111"
    assert check(bdd1, mgr, exp_str)

def test_cofactor3_invalid1():
    mgr = BddMgr()
    bdd = Bdd.invalid()
    lit = mgr.negative_literal(0)

    with pytest.raises(ValueError) as e:
        bdd.cofactor(lit)

def test_cofactor3_invalid2():
    mgr = BddMgr()
    src_str = "10010111"
    bdd = mgr.from_truth(src_str)
    lit = BddLit.invalid()

    with pytest.raises(ValueError) as e:
        bdd.cofactor(lit)

def test_cofactor4():
    # (BddCube) のバージョン
    mgr = BddMgr()
    src_str = "10010111"
    bdd = mgr.from_truth(src_str)
    lit0 = mgr.negative_literal(0)
    lit1 = mgr.positive_literal(1)
    cube = lit0 & lit1

    bdd1 = bdd.cofactor(cube)

    exp_str = "01010101"
    assert check(bdd1, mgr, exp_str)

def test_cofactor4_invalid1():
    mgr = BddMgr()
    bdd = Bdd.invalid()
    lit0 = mgr.negative_literal(0)
    lit1 = mgr.positive_literal(1)
    cube = lit0 & lit1

    with pytest.raises(ValueError) as e:
        bdd.cofactor(cube)

def test_cofactor4_invalid2():
    mgr = BddMgr()
    src_str = "10010111"
    bdd = mgr.from_truth(src_str)
    cube = BddCube.invalid()

    with pytest.raises(ValueError) as e:
        bdd.cofactor(cube)

def test_cofactor4_invalid3():
    # (BddCube, inv=True)
    mgr = BddMgr()
    src_str = "10010111"
    bdd = mgr.from_truth(src_str)
    lit0 = mgr.negative_literal(0)
    lit1 = mgr.positive_literal(1)
    cube = lit0 & lit1

    with pytest.raises(TypeError) as e:
        bdd.cofactor(cube, inv=True)

def test_div1():
    # (BddLit) のバージョン
    mgr = BddMgr()
    src_str = "10010111"
    bdd = mgr.from_truth(src_str)
    lit = mgr.literal(0, inv=True)
    bdd1 = bdd / lit

    exp_str = "01110111"
    assert check(bdd1, mgr, exp_str)

def test_div1_invalid1():
    mgr = BddMgr()
    bdd = Bdd.invalid()
    lit = mgr.negative_literal(0)

    with pytest.raises(ValueError) as e:
        bdd / lit

def test_div1_invalid2():
    mgr = BddMgr()
    src_str = "10010111"
    bdd = mgr.from_truth(src_str)
    lit = BddLit.invalid()

    with pytest.raises(ValueError) as e:
        bdd / lit

def test_idiv1():
    # (BddLit) のバージョン
    mgr = BddMgr()
    src_str = "10010111"
    bdd = mgr.from_truth(src_str)
    lit = mgr.literal(0, inv=True)
    bdd /= lit

    exp_str = "01110111"
    assert check(bdd, mgr, exp_str)

def test_idiv1_invalid1():
    mgr = BddMgr()
    bdd = Bdd.invalid()
    lit = mgr.negative_literal(0)

    with pytest.raises(ValueError) as e:
        bdd /= lit

def test_idiv1_invalid2():
    mgr = BddMgr()
    src_str = "10010111"
    bdd = mgr.from_truth(src_str)
    lit = BddLit.invalid()

    with pytest.raises(ValueError) as e:
        bdd /= lit

def test_idiv2():
    # (BddCube) のバージョン
    mgr = BddMgr()
    src_str = "10010111"
    bdd = mgr.from_truth(src_str)
    lit0 = mgr.negative_literal(0)
    lit1 = mgr.positive_literal(1)
    cube = lit0 & lit1

    bdd /= cube

    exp_str = "01010101"
    assert check(bdd, mgr, exp_str)

def test_idiv2_invalid1():
    mgr = BddMgr()
    bdd = Bdd.invalid()
    lit0 = mgr.negative_literal(0)
    lit1 = mgr.positive_literal(1)
    cube = lit0 & lit1

    with pytest.raises(ValueError) as e:
        bdd /= cube

def test_idiv2_invalid2():
    mgr = BddMgr()
    src_str = "10010111"
    bdd = mgr.from_truth(src_str)
    cube = BddCube.invalid()

    with pytest.raises(ValueError) as e:
        bdd /= cube

def check_sup(bdd, mgr, var_id):
    lit = mgr.literal(var_id)
    bdd0 = bdd / ~lit
    bdd1 = bdd /  lit
    exp_ans = (bdd0 != bdd1)
    var = mgr.variable(var_id)
    assert exp_ans == bdd.check_sup(var)

def test_get_support1():
    mgr = BddMgr()
    lit1 = mgr.literal_bdd(1)
    lit3 = mgr.literal_bdd(3)
    lit5 = mgr.literal_bdd(5)
    lit7 = mgr.literal_bdd(7)
    bdd = (lit1 & lit3) | (~lit1 & lit5) | (lit3 & ~lit7)
    sup = bdd.get_support()

    check_sup(bdd, mgr, 8)

def test_get_support2():
    mgr = BddMgr()
    lit1 = mgr.literal_bdd(1)
    lit2 = mgr.literal_bdd(2)
    lit3 = mgr.literal_bdd(3)
    bdd = (lit1 & lit2) | (~lit1 & lit2) | lit3
    sup = bdd.get_support()

    check_sup(bdd, mgr, 4)

def test_get_support_invalid():
    bdd = Bdd.invalid()

    with pytest.raises(ValueError) as e:
        bdd.get_support()

def test_check_sup1():
    mgr = BddMgr()
    bdd = mgr.from_truth("1100")

    check_sup(bdd, mgr, 0)
    check_sup(bdd, mgr, 1)

def check_sym(bdd, mgr, var_id1, var_id2):
    lit1 = mgr.literal(var_id1)
    lit2 = mgr.literal(var_id2)
    bdd00 = bdd / (~lit1 & ~lit2)
    bdd01 = bdd / (~lit1 &  lit2)
    bdd10 = bdd / ( lit1 & ~lit2)
    bdd11 = bdd / ( lit1 &  lit2)
    exp_ans1 = (bdd01 == bdd10)
    exp_ans2 = (bdd00 == bdd11)
    var1 = mgr.variable(var_id1)
    var2 = mgr.variable(var_id2)

    assert exp_ans1 == bdd.check_sym(var1, var2, inv=False)
    assert exp_ans2 == bdd.check_sym(var1, var2, inv=True)

def test_check_sym1():
    mgr = BddMgr()
    bdd = mgr.from_truth("11101010")

    check_sym(bdd, mgr, 0, 1)
    check_sym(bdd, mgr, 0, 2)
    check_sym(bdd, mgr, 1, 2)

def test_check_sym2():
    mgr = BddMgr()
    bdd = mgr.from_truth("01001011")

    check_sym(bdd, mgr, 0, 1)
    check_sym(bdd, mgr, 0, 2)
    check_sym(bdd, mgr, 1, 2)

def test_check_sym3():
    mgr = BddMgr()
    bdd = mgr.from_truth("0110")

    check_sym(bdd, mgr, 0, 1)

def test_check_sym4():
    mgr = BddMgr()
    bdd = mgr.from_truth("01100101")

    check_sym(bdd, mgr, 0, 1)
    check_sym(bdd, mgr, 0, 2)
    check_sym(bdd, mgr, 1, 2)

def test_check_sym5():
    mgr = BddMgr()
    bdd = mgr.from_truth("0100")

    check_sym(bdd, mgr, 0, 1)

def test_get_onepath1():
    mgr = BddMgr()
    lit1 = mgr.literal_bdd(0)
    lit2 = mgr.literal_bdd(1)
    lit3 = mgr.literal_bdd(2)
    bdd = lit1 & ~lit2 & lit3
    cube = bdd.get_onepath()
    bdd1 = bdd / cube
    assert bdd1.is_one()

def test_get_onepath2():
    bdd = Bdd.zero()
    cube = bdd.get_onepath()

    assert cube.is_invalid()

def test_get_onepath3():
    mgr = BddMgr()
    lit1 = mgr.literal_bdd(0)
    lit2 = mgr.literal_bdd(1)
    lit3 = mgr.literal_bdd(2)
    bdd = lit1 & lit2 | ~lit1 & lit3
    cube = bdd.get_onepath()

    bdd1 = bdd / cube
    assert bdd1.is_one()

def test_get_onepath_invalid():
    bdd = Bdd.invalid()

    with pytest.raises(ValueError) as e:
        bdd.get_onepath()

def test_get_zeropath1():
    mgr = BddMgr()
    lit1 = mgr.literal_bdd(0)
    lit2 = mgr.literal_bdd(1)
    lit3 = mgr.literal_bdd(2)
    bdd = lit1 | ~lit2 | lit3
    cube = bdd.get_zeropath()
    exp_cube = ~lit1 & lit2 & ~lit3
    var1 = mgr.variable(0)
    var2 = mgr.variable(1)
    var3 = mgr.variable(2)
    assert cube.bdd() == exp_cube

def test_get_zeropath2():
    mgr = BddMgr()
    lit1 = mgr.literal_bdd(0)
    lit2 = mgr.literal_bdd(1)
    lit3 = mgr.literal_bdd(2)
    bdd = lit1 & lit2 | ~lit1 & lit3
    cube = bdd.get_zeropath()
    bdd1 = bdd / cube
    assert bdd1.is_zero()

def test_get_zeropath_invalid():
    bdd = Bdd.invalid()

    with pytest.raises(ValueError) as e:
        bdd.get_zeropath()

def test_root_decomp1():
    mgr = BddMgr()
    bdd = mgr.from_truth("1000")

    var0, bdd0, bdd1 = bdd.root_decomp()

    assert var0.varid == 0
    assert bdd0.is_zero()

    var1, bdd10, bdd11 = bdd1.root_decomp()

    assert var1.varid == 1
    assert bdd10.is_zero()
    assert bdd11.is_one()

def test_root_decomp_invalid1():
    bdd = Bdd.invalid()

    with pytest.raises(ValueError) as e:
        bdd.root_decomp()

def test_root_inv1():
    bdd = Bdd.zero()

    assert not bdd.root_inv

def test_root_inv2():
    bdd = Bdd.one()

    assert bdd.root_inv

def test_root_inv3():
    mgr = BddMgr()
    bdd = mgr.from_truth("1110")

    assert not bdd.root_inv

def test_root_inv4():
    mgr = BddMgr()
    bdd = mgr.from_truth("0001")

    assert bdd.root_inv

def test_root_inv_invalid1():
    bdd = Bdd.invalid()

    assert not bdd.root_inv

    ibdd = ~bdd

    assert not ibdd.root_inv

def test_size1():
    bdd = Bdd.zero()

    assert bdd.size == 0

def test_size2():
    bdd = Bdd.one()

    assert bdd.size == 0

def test_size3():
    mgr = BddMgr()
    bdd = mgr.from_truth("1000")

    assert bdd.size == 2

def test_size_invalid1():
    bdd = Bdd.invalid()

    assert bdd.size == 0

def test_bdd_size1():
    mgr = BddMgr()
    bdd1 = mgr.from_truth("1100")
    bdd2 = mgr.from_truth("1010")
    bdd3 = mgr.from_truth("1000")

    assert bdd1.size == 1
    assert bdd2.size == 1
    assert bdd3.size == 2
    assert Bdd.bdd_size([bdd1, bdd2, bdd3]) == 3

def test_is_identical1():
    mgr = BddMgr()
    bdd1 = mgr.from_truth("1011")
    bdd2 = mgr.from_truth("1100")
    bdd3 = mgr.from_truth("1011")

    assert not Bdd.is_identical(bdd1, bdd2)
    assert Bdd.is_identical(bdd1, bdd3)

    mgr2 = BddMgr()
    bdd4 = mgr2.from_truth("1011")

    assert Bdd.is_identical(bdd1, bdd4)
    assert not Bdd.is_identical(bdd2, bdd4)

def test_is_identical_invalid1():
    mgr = BddMgr()
    bdd1 = Bdd.invalid()
    bdd2 = mgr.from_truth("1011")

    assert not Bdd.is_identical(bdd1, bdd2)

def test_is_identical_invalid2():
    mgr = BddMgr()
    bdd1 = mgr.from_truth("1011")
    bdd2 = Bdd.invalid()

    assert not Bdd.is_identical(bdd1, bdd2)
