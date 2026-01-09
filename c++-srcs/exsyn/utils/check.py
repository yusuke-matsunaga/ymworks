#! /usr/bin/env python3

"""

:file: check.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

import sys


def analyze(func):
    has1 = False
    has1_array = [ False for _ in range(4) ]
    ni = 2
    ni_exp = 1 << ni
    for b in range(ni_exp):
        val = func[b]
        print(val, end='')
        if val == 0:
            for i in range(ni):
                if (b & (1 << (ni - i - 1))) == 0:
                    has1_array[i] = True
        else:
            has1 = True
            for i in range(ni):
                if (b & (1 << (ni - i - 1))) != 0:
                    has1_array[i] = True
    print()
    print(f'has1 = {has1}')
    for i in range(ni):
        print(f'I#[{i}]: has1 = {has1_array[i]}')


analyze([0, 0, 0, 0])

analyze([1, 0, 1, 0])

analyze([1, 1, 0, 0])
