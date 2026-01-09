#! /usr/bin/env python3

""" TruthTable のテストスクリプト

:file: truthtable_test.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

import pytest
import os
import os.path
from ymworks.logic import TruthTable, Val3


def test_constr():
    ni = 3
    no = 2
    tt = TruthTable(input_num=ni, output_num=no)

    assert tt.input_num == ni
    assert tt.output_num == no

    ni_exp = 1 << ni
    for o in range(no):
        for i in range(ni_exp):
            v = tt.get_value(i_index=i, o_index=o)
            assert v == Val3.X

    assert tt.bin_str() == "3:2:----------------"

def test_read_file():
    root_dir = os.environ.get("TESTDATA_DIR")
    filename = os.path.join(root_dir, "logic", "tt", "1_0_0.truth")
    tt = TruthTable.read_file(filename)

    assert tt.input_num == 6
    assert tt.output_num == 18
    exp_src = """000011001000100100
000001001111000000
000010000101011010
XXXXXXXXXXXXXXXXXX
111111000110101001
000011100100010010
111100010111011100
XXXXXXXXXXXXXXXXXX
000000100111001001
111001101000000010
000001000011011010
XXXXXXXXXXXXXXXXXX
111100111101001101
111010100011111001
111111010011100101
XXXXXXXXXXXXXXXXXX
000000011000010101
111010111101100110
111001111111011000
XXXXXXXXXXXXXXXXXX
000010100110100110
001001001000111000
111111010101110100
XXXXXXXXXXXXXXXXXX
000100000100001111
000010011010111101
111101001111110101
XXXXXXXXXXXXXXXXXX
000011101001000100
000011100001100110
111101001001100100
XXXXXXXXXXXXXXXXXX
000001100110011010
000001010010001000
111101000011110010
XXXXXXXXXXXXXXXXXX
111100111000100010
111110001001100101
111100000110111000
XXXXXXXXXXXXXXXXXX
111110100101000100
000010001000001001
111111101110101101
XXXXXXXXXXXXXXXXXX
000100110010110010
111011111001010010
000000100001010101
XXXXXXXXXXXXXXXXXX
111101101011000100
111101011110010110
111110110011100011
XXXXXXXXXXXXXXXXXX
111011100000111010
000010001001000011
111100010010110000
XXXXXXXXXXXXXXXXXX
111101001011100010
111000101000100110
000000111010110111
XXXXXXXXXXXXXXXXXX
000000000101111000
000110001001110111
000001011111010010
XXXXXXXXXXXXXXXXXX"""

    def char2val3(c):
        if c == '0':
            return Val3.ZERO
        if c == '1':
            return Val3.ONE
        if c == 'x':
            return Val3.X
        if c == 'X':
            return Val3.X
        if c == 'd':
            return Val3.X
        if c == 'D':
            return Val3.X
        if c == '-':
            return Val3.X
        if c == '*':
            return Val3.X

    exp_list = exp_src.split()
    ni_exp = 1 << 6
    assert len(exp_list) == ni_exp
    for i in range(ni_exp):
        line = exp_list[i]
        for o in range(18):
            v = tt.get_value(i_index=i, o_index=o)
            exp_v = char2val3(line[o])
            assert v == exp_v


def test_from_bin1():
    src_str = "3:2:01d11*0010xX0101"
    tt = TruthTable.from_bin(src_str)

    assert tt.input_num == 3
    assert tt.output_num == 2
    assert tt.bin_str() == "3:2:01-11-0010--0101"

def test_from_bin_bad1():
    src_str = "01-11*0010--0101"
    with pytest.raises(ValueError) as e:
        TruthTable.from_bin(src_str)

def test_from_bin_bad2():
    src_str = "3:2:1-11*0010--0101"
    with pytest.raises(ValueError) as e:
        TruthTable.from_bin(src_str)

def test_from_bin_bad2():
    src_str = "3:2:1A11*0010--0101D"
    with pytest.raises(ValueError) as e:
        TruthTable.from_bin(src_str)

def test_from_hex1():
    src_str = "3:2:28CA"
    tt = TruthTable.from_hex(src_str)

    assert tt.input_num == 3
    assert tt.output_num == 2
    assert tt.bin_str() == "3:2:0010100011001010"

def test_from_hex_bad1():
    src_str = "abcdefg"
    with pytest.raises(ValueError) as e:
        TruthTable.from_hex(src_str)

def test_from_hex_bad2():
    src_str = "2:1:abcdefg"
    with pytest.raises(ValueError) as e:
        TruthTable.from_hex(src_str)

def test_from_hex_bad3():
    src_str = "2:1:0X"
    with pytest.raises(ValueError) as e:
        TruthTable.from_hex(src_str)
