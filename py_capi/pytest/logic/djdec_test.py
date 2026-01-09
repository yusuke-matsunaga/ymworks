#! /usr/bin/env python3

""" DjDec のテストスクリプト

:file: djdec_test.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

import pytest
from ymworks.logic import TvFunc, DjDec


def test1():
    func = TvFunc.from_hex_str("0660")
    g = DjDec.decompose(func)

    assert g.op_num == 3
