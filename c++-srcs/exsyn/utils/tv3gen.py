#! /usr/bin/env python3

"""

:file: tv3gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from logictools import BoolFunc, Bool3



x0 = BoolFunc.make_literal(3, 0)
x1 = BoolFunc.make_literal(3, 1)
x2 = BoolFunc.make_literal(3, 2)

def make_tv(f):
    tv_list = []
    for b in range(1, 8):
        i0 = Bool3._1 if b & 4 else Bool3._0
        i1 = Bool3._1 if b & 2 else Bool3._0
        i2 = Bool3._1 if b & 1 else Bool3._0
        val = f.val((i0, i1, i2))
        tv_list.append(val)
    return tv_list

# 定数0
tv_list = make_tv(BoolFunc.make_const0(3))
print(tv_list)
