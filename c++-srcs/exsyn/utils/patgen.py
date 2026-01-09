#! /usr/bin/env python3

"""

:file: patgen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from logictools import BoolFunc, Bool3


def print_hex(f):
    v = 0
    for index in range(16):
        ival_list = []
        if index & 8:
            ival_list.append(True)
        else:
            ival_list.append(False)
        if index & 4:
            ival_list.append(True)
        else:
            ival_list.append(False)
        if index & 2:
            ival_list.append(True)
        else:
            ival_list.append(False)
        if index & 1:
            ival_list.append(True)
        else:
            ival_list.append(False)
        val = f.val(ival_list)
        if val == Bool3._1:
            v += (1 << index)
    print(f': {v:04X}')

i0 = BoolFunc.make_literal(4, 0)
i1 = BoolFunc.make_literal(4, 1)
i2 = BoolFunc.make_literal(4, 2)
i3 = BoolFunc.make_literal(4, 3)

for b in range(16):
    f = BoolFunc.make_const1(4)
    if b & 8:
        f &= ~i0
        print('N', end='')
    else:
        f &= i0
        print('P', end='')
    if b & 4:
        f &= ~i1
        print('N', end='')
    else:
        f &= i1
        print('P', end='')
    if b & 2:
        f &= ~i2
        print('N', end='')
    else:
        f &= i2
        print('P', end='')
    if b & 1:
        f &= ~i3
        print('N', end='')
    else:
        f &= i3
        print('P', end='')
    print_hex(f)
