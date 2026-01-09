#! /usr/bin/env python3

""" aig サブモジュール生成用初期化スクリプト

:file: __init__.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from .aigmgr_gen import AigMgrGen
from .aigliteral_gen import AigLiteralGen
from .aig_arg import AigMgrArg, AigLiteralArg


gen_list = [
    AigMgrGen(),
    AigLiteralGen(),
]
