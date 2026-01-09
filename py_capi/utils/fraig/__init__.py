#! /usr/bin/env python3

""" fraig サブモジュール生成用初期化スクリプト

:file: __init__.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from .fraigmgr_gen import FraigMgrGen
from .fraigliteral_gen import FraigLiteralGen
from .fraig_arg import FraigMgrArg, FraigLiteralArg


gen_list = [
    FraigMgrGen(),
    FraigLiteralGen(),
]
