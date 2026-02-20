#! /usr/bin/env python3

""" MinCov 用のスクリプト

:file: mincov_test.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2026 Yusuke Matsunaga, All rights reserved.
"""

from argparse import ArgumentParser
import re
import time
from ymworks.combopt import MinCov
from ymworks.misc import JsonValue

pat1 = re.compile(r'(\d+)\s*(\d+)')
pat2 = re.compile(r'\s*-1\s*-1\s*')


def read_pi(filename):
    with open(filename, 'rt') as fin:
        elem_list = []
        max_row = 0
        max_col = 0
        for line in fin:
            if pat2.match(line):
                break;
            result = pat1.match(line)
            if not result:
                print(f'invalid line: {line}, ignored')
                continue
            row = int(result.group(1))
            col = int(result.group(2))
            elem_list.append((row, col))
        max_row = max(max_row, row)
        max_col = max(max_col, col)
        row_size = max_row + 1
        col_size = max_col + 1
        mincov = MinCov(row_size=row_size, col_size=col_size)
        for row, col in elem_list:
            mincov.insert_elem(row_pos=row, col_pos=col)
        return mincov
    assert False


parser = ArgumentParser()
parser.add_argument("filename", type=str)
parser.add_argument('--algorithm', type=str)

args = parser.parse_args()
if args is None:
    parser.usage()
    exit(1)

filename = args.filename
if args.algorithm is None:
    algorithm = "greedy"
else:
    algorithm = args.algorithm

s_time = time.time()
mincov = read_pi(filename)
e_time = time.time()
print(f'read_pi({filename}) end. Time: {e_time - s_time}')

option = JsonValue.object()
option.add_with_key('algorithm', JsonValue(algorithm))

s_time = time.time()
solution, nc = mincov.solve(option=option)
e_time = time.time()
print(f'solve end. Time: {e_time - s_time}')
