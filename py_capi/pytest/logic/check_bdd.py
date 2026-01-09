#! /usr/bin/env python3

"""

:file: check_bdd.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""


def check(bdd, mgr, exp_str):
    n = len(exp_str)
    ni = 0
    while ( (1 << ni) < n ):
        ni += 1
    assert n == (1 << ni)
    inputs = [ False for _ in range(ni) ]
    for p in range(n):
        for i in range(ni):
            if p & (1 << (ni - i - 1)) != 0:
                inputs[i] = True
            else:
                inputs[i] = False
        val = bdd.eval(inputs)
        exp_val = True if exp_str[n - p - 1] == '1' else False
        if exp_val != val:
            print(f"val = {val}")
            print(f"exp_val = {exp_val}")
            return False
    var_list = [ mgr.variable(i) for i in range(ni) ]
    truth_str = bdd.to_truth(var_list)
    if truth_str != exp_str:
        print(f'truth_str = {truth_str}')
        return False
    return True
