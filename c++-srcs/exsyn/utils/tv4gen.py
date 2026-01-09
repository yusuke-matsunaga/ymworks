#! /usr/bin/env python3

"""

:file: tv4gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from logictools import BoolFunc, Bool3

def decode_index(index, ni):
    ival_list = []
    for i in range(ni):
        if index & (1 << (ni - i - 1)):
            ival_list.append(Bool3._1)
        else:
            ival_list.append(Bool3._0)
    return ival_list

def check_sup(f, var):
    """var が f のサポートになっている時 True を返す．
    """
    ni = f.input_num
    ni_exp = 1 << ni
    for b in range(ni_exp):
        ival_list = decode_index(b, ni)
        val1 = f.val(ival_list)
        ival_list2 = ival_list[:]
        ival_list2[var] = ~ival_list2[var]
        val2 = f.val(ival_list2)
        if val1 != val2:
            return True
    return False

def check_func(f):
    """f がサポートでない変数を含んでいない時 True を返す．
    """
    ni = f.input_num
    for i in range(ni):
        if not check_sup(f, i):
            return False
    return True

ni = 4

ni_exp = 1 << ni
ni_exp_exp = 1 << ni_exp
for f_index in range(ni_exp_exp):
    val_list = [ ]
    for b in range(ni_exp):
        if f_index & (1 << b) != 0:
            val_list.append(Bool3._1)
        else:
            val_list.append(Bool3._0)
    if val_list[0] == Bool3._1:
        continue

    f = BoolFunc(val_list)
    if not check_func(f):
        print(f'  add_ngpat4(solver', end='')
        first = True
        for val in val_list:
            if first:
                # (0, 0, 0, 0) はスキップ
                first = False
                continue
            if val == Bool3._1:
                print(', 1', end='')
            else:
                print(', 0', end='')
        print(', mTval);')
