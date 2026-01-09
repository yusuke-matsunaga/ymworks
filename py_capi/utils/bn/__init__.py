#! /usr/bin/env python3

""" bn サブモジュール生成用初期化スクリプト

:file: __init__.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from .bnmodel_gen import BnModelGen
from .bndff_gen import BnDffGen
from .bndfflist_gen import BnDffListGen
from .bndffiter2_gen import BnDffIter2Gen
from .bnnode_gen import BnNodeGen
from .bnnodelist_gen import BnNodeListGen
from .bnnodeiter2_gen import BnNodeIter2Gen
from .bnfunc_gen import BnFuncGen
from .bn_arg import BnModelArg, BnNodeArg, BnDffArg, BnFuncArg


gen_list = [
    BnModelGen(),
    BnDffGen(),
    BnDffListGen(),
    BnDffIter2Gen(),
    BnNodeGen(),
    BnNodeListGen(),
    BnNodeIter2Gen(),
    BnFuncGen(),
]
