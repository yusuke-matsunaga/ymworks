#! /usr/bin/env python3

""" BnModel のテストプログラム

:file: BnModel_test.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2023 Yusuke Matsunaga, All rights reserved.
"""

import sys
import pytest
from ymworks.bn import BnModel, BnNodeList
from ymworks.logic import PrimType, Literal
from ymworks.logic import SopCube, SopCover
from ymworks.logic import Expr
from ymworks.logic import TvFunc
from ymworks.logic import Bdd, BddMgr


def test_constructor1():
    model = BnModel()

    assert model.name == ""
    assert model.comment_list() == []
    assert model.node_num == 0
    assert model.input_num == 0
    assert len(model.input_list()) == 0
    assert model.output_num == 0
    assert len(model.output_list()) == 0
    assert model.logic_num == 0
    assert len(model.logic_list()) == 0
    assert model.dff_num == 0
    assert model.func_num == 0

def test_constructor_bad():

    with pytest.raises(TypeError) as e:
        _ = BnModel(1)

def test_input_bad():
    model = BnModel()

    with pytest.raises(ValueError) as e:
        _ = model.input(0)

def test_output_bad():
    model = BnModel()

    with pytest.raises(ValueError) as e:
        _ = model.output(0)

def test_logic_bad():
    model = BnModel()

    with pytest.raises(ValueError) as e:
        _ = model.logic(0)

def test_seq_bad():
    model = BnModel()

    with pytest.raises(ValueError) as e:
        _ = model.input(0)

def test_func_bad():
    model = BnModel()

    with pytest.raises(ValueError) as e:
        _ = model.func(0)

def test_new_primitive():
    model = BnModel()

    prim_type = PrimType.And
    input1 = model.new_input()
    input2 = model.new_input()
    fanin_list = [input1, input2]

    node = model.new_primitive(prim_type, fanin_list)

    assert node.type == "LOGIC"
    assert not node.is_input()
    assert not node.is_primary_input()
    assert not node.is_dff_output()
    with pytest.raises(ValueError) as e:
        _ = node.input_id
    with pytest.raises(ValueError) as e:
        _ = node.dff_id
    assert node.is_logic()

    func = node.func
    assert func.type == "PRIMITIVE"
    assert func.is_primitive()
    assert not func.is_cover()
    assert not func.is_expr()
    assert not func.is_tvfunc()
    assert not func.is_bdd()
    assert func.primitive_type == prim_type
    with pytest.raises(ValueError) as e:
        _ = func.input_cover
    with pytest.raises(ValueError) as e:
        _ = func.output_inv
    with pytest.raises(ValueError) as e:
        _ = func.expr
    with pytest.raises(ValueError) as e:
        _ = func.tvfunc
    with pytest.raises(ValueError) as e:
        _ = func.bdd

    assert node.fanin_num == len(fanin_list)
    fanin_list2 = node.fanin_list()
    for i, fanin in enumerate(fanin_list2):
        assert fanin == fanin_list[i]
    fanin_list2[-1] == fanin_list[-1]
    fanin_list2[-2] == fanin_list[-2]

def test_new_cover():
    model = BnModel()

    input1 = model.new_input()
    input2 = model.new_input()
    fanin_list = [input1, input2]
    ni = 2
    lit0 = Literal(0)
    lit1 = Literal(1)
    cube1 = SopCube(ni, literal_list=[lit0, lit1])
    input_cover = SopCover(ni, cube_list=[cube1,])
    output_inv = True

    node = model.new_cover(input_cover, output_inv, fanin_list)

    assert node.type == "LOGIC"
    assert not node.is_input()
    assert not node.is_primary_input()
    assert not node.is_dff_output()
    with pytest.raises(ValueError) as e:
        _ = node.input_id
    with pytest.raises(ValueError) as e:
        _ = node.dff_id
    assert node.is_logic()

    func = node.func
    assert func.type == "COVER"
    assert not func.is_primitive()
    assert func.is_cover()
    assert not func.is_expr()
    assert not func.is_tvfunc()
    assert not func.is_bdd()
    with pytest.raises(ValueError) as e:
        _ = func.primitive_type
    assert func.input_cover == input_cover
    assert func.output_inv == output_inv
    with pytest.raises(ValueError) as e:
        _ = func.expr
    with pytest.raises(ValueError) as e:
        _ = func.tvfunc
    with pytest.raises(ValueError) as e:
        _ = func.bdd

    assert node.fanin_num == len(fanin_list)
    for i, fanin in enumerate(node.fanin_list()):
        assert fanin == fanin_list[i]

def test_new_cover_bad():
    model = BnModel()

    input1 = model.new_input()
    input2 = model.new_input()
    fanin_list = [input1, input2]

    ni = 3
    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    cube1 = SopCube(ni, literal_list=[lit0, lit1])
    cube2 = SopCube(ni, literal_list=[lit2])
    input_cover = SopCover(ni, cube_list=[cube1, cube2])
    output_inv = True

    with pytest.raises(ValueError) as e:
        _ = model.new_cover(input_cover, output_inv, fanin_list)

def test_new_expr():
    model = BnModel()

    lit0 = Expr.literal(0)
    lit1 = Expr.literal(1)
    expr = lit0 | ~lit1

    input1 = model.new_input()
    input2 = model.new_input()
    fanin_list = [input1, input2]

    node = model.new_expr(expr, fanin_list)

    assert node.type == "LOGIC"
    assert not node.is_input()
    assert not node.is_primary_input()
    assert not node.is_dff_output()
    with pytest.raises(ValueError) as e:
        _ = node.input_id
    with pytest.raises(ValueError) as e:
        _ = node.dff_id
    assert node.is_logic()

    func = node.func
    assert func.type == "EXPR"
    assert not func.is_primitive()
    assert not func.is_cover()
    assert func.is_expr()
    assert not func.is_tvfunc()
    assert not func.is_bdd()
    with pytest.raises(ValueError) as e:
        _ = func.primitive_type
    with pytest.raises(ValueError) as e:
        _ = func.input_cover
    with pytest.raises(ValueError) as e:
        _ = func.output_inv
    assert func.expr == expr
    with pytest.raises(ValueError) as e:
        _ = func.tvfunc
    with pytest.raises(ValueError) as e:
        _ = func.bdd

    assert node.fanin_num == len(fanin_list)
    for i, fanin in enumerate(node.fanin_list()):
        assert fanin == fanin_list[i]

def test_new_expr_bad():
    model = BnModel()

    input1 = model.new_input()
    input2 = model.new_input()
    fanin_list = [input1, input2]

    lit0 = Expr.literal(0)
    lit1 = Expr.literal(1)
    lit2 = Expr.literal(2)
    expr = lit0 | ~lit1 & lit2

    with pytest.raises(ValueError) as e:
        _ = model.new_expr(expr, fanin_list)

def test_new_tvfunc():
    model = BnModel()

    ni = 3
    v0 = TvFunc.positive_literal(ni, 0)
    v1 = TvFunc.positive_literal(ni, 1)
    v2 = TvFunc.positive_literal(ni, 2)
    tvfunc = v0 | v1 | v2

    input1 = model.new_input()
    input2 = model.new_input()
    input3 = model.new_input()
    fanin_list = [input1, input2, input3]

    node = model.new_tvfunc(tvfunc, fanin_list)

    assert node.type == "LOGIC"
    assert not node.is_input()
    assert not node.is_primary_input()
    assert not node.is_dff_output()
    with pytest.raises(ValueError) as e:
        _ = node.input_id
    with pytest.raises(ValueError) as e:
        _ = node.dff_id
    assert node.is_logic()

    func = node.func
    assert func.type == "TVFUNC"
    assert not func.is_primitive()
    assert not func.is_cover()
    assert not func.is_expr()
    assert func.is_tvfunc()
    assert not func.is_bdd()
    with pytest.raises(ValueError) as e:
        _ = func.primitive_type
    with pytest.raises(ValueError) as e:
        _ = func.input_cover
    with pytest.raises(ValueError) as e:
        _ = func.output_inv
    with pytest.raises(ValueError) as e:
        _ = func.expr
    assert func.tvfunc == tvfunc
    with pytest.raises(ValueError) as e:
        _ = func.bdd

    assert node.fanin_num == len(fanin_list)
    for i, fanin in enumerate(node.fanin_list()):
        assert fanin == fanin_list[i]

def test_new_tvfunc_bad():
    model = BnModel()

    ni = 3
    v0 = TvFunc.positive_literal(ni, 0)
    v1 = TvFunc.positive_literal(ni, 1)
    v2 = TvFunc.positive_literal(ni, 2)
    func = v0 | v1 | v2

    input1 = model.new_input()
    input2 = model.new_input()
    fanin_list = [input1, input2]

    with pytest.raises(ValueError) as e:
        _ = model.new_tvfunc(func, fanin_list)

def test_new_bdd():
    mgr = BddMgr()

    var0 = mgr.variable_bdd(0)
    var1 = mgr.variable_bdd(1)
    bdd = var0 & ~var1

    model = BnModel()

    input1 = model.new_input()
    input2 = model.new_input()
    fanin_list = [input1, input2]

    node = model.new_bdd(bdd, fanin_list)

    assert node.type == "LOGIC"
    assert not node.is_input()
    assert not node.is_primary_input()
    assert not node.is_dff_output()
    with pytest.raises(ValueError) as e:
        _ = node.input_id
    with pytest.raises(ValueError) as e:
        _ = node.dff_id
    assert node.is_logic()

    func = node.func
    assert func.type == "BDD"
    assert not func.is_primitive()
    assert not func.is_cover()
    assert not func.is_expr()
    assert not func.is_tvfunc()
    assert func.is_bdd()
    with pytest.raises(ValueError) as e:
        _ = func.primitive_type
    with pytest.raises(ValueError) as e:
        _ = func.input_cover
    with pytest.raises(ValueError) as e:
        _ = func.output_inv
    with pytest.raises(ValueError) as e:
        _ = func.expr
    with pytest.raises(ValueError) as e:
        _ = func.tvfunc
    assert mgr.copy(func.bdd) == bdd

    assert node.fanin_num == len(fanin_list)
    for i, fanin in enumerate(node.fanin_list()):
        assert fanin == fanin_list[i]

def test_new_bdd_bd():
    mgr = BddMgr()

    var0 = mgr.variable_bdd(0)
    var1 = mgr.variable_bdd(1)
    var2 = mgr.variable_bdd(2)
    bdd = var0 & ~var1 | var2

    model = BnModel()

    input1 = model.new_input()
    input2 = model.new_input()
    fanin_list = [input1, input2]

    with pytest.raises(ValueError) as e:
        _ = model.new_bdd(bdd, fanin_list)

def test_fsm1(capfd):
    model = BnModel()

    input1 = model.new_input()
    dff1 = model.new_dff()
    dff1_output = dff1.output
    and_node = model.new_primitive(PrimType.And, [input1, dff1_output])
    model.set_dff_src(dff1, and_node)
    model.new_output(dff1_output)

    model.write(sys.stdout)

    out, err = capfd.readouterr()

    exp_str = """I#0: N#0
O#0: N#1
Q#0: output = N#1, src = N#2
N#2 = F#0(N#0, N#1)
F#0: Primitive: And(2)
"""
    assert out == exp_str
