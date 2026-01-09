#! /usr/bin/env python3

""" モジュールのセットアップスクリプト

:file: __init__.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from .mt19937_gen import Mt19937Gen
from .jsonvalue_gen import JsonValueGen
from .misc_arg import Mt19937Arg, JsonValueArg


gen_list = [
    Mt19937Gen(),
    JsonValueGen(),
]
