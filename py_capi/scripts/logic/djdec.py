#! /usr/bin/env python3

""" djdec 用のスクリプト

:file: djdec.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from argparse import ArgumentParser
import re
from ymworks.logic import TvFunc, DjDec


parser = ArgumentParser()
parser.add_argument('filename', type=str)

args = parser.parse_args()

pat = re.compile(r'\d+:1:([0-9a-fA-F]+)')
filename = args.filename
with open(filename, 'rt') as fin:
    for line in fin:
        line = line.rstrip()
        result = pat.match(line)
        if not result:
            print(f'invalid line: {line}')
            continue
        fstr = result.group(1)
        func = TvFunc.from_hex_str(fstr)
        print(fstr)
        g = DjDec.decompose(func)
        if g.op_num > 1:
            print(f'{fstr}: decomposable')
