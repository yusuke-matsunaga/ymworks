#! /usr/bin/env python3

""" ランダムに関数を生成するスクリプト

:file: sample_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from argparse import ArgumentParser
import random


parser = ArgumentParser(prog='sample_gen',
                        description='Sample Generator')
parser.add_argument('input_num', type=int,
                    help='# of input')
parser.add_argument('num', type=int,
                    help='# of samples')
parser.add_argument('-o', '--output_num',
                    type=int, default=1,
                    help='# of output')

args = parser.parse_args()
if args is None:
    exit(1)

ni = args.input_num
no = args.output_num
num = args.num
ni_exp = 1 << ni
ni_exp_exp = 1 << ni_exp
ni_exp_exp1 = ni_exp_exp - 1

for _ in range(num):
    print(f'{ni}:{no}:', end='')
    for o in range(no):
        v = random.randint(0, ni_exp_exp1)
        if ni == 2:
            print(f'{v:X}', end='')
        elif ni == 3:
            print(f'{v:02X}', end='')
        elif ni == 4:
            print(f'{v:04X}', end='')
        elif ni == 5:
            print(f'{v:08X}', end='')
        elif ni == 6:
            print(f'{v:016X}', end='')
    print()
