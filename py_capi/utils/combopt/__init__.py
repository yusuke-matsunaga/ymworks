#! /usr/bin/env python3

""" combopt サブモジュールの初期化用スクリプト

:file: __init__.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from .mincov_gen import MinCovGen
from .udgraph_gen import UdGraphGen
from .udedge_gen import UdEdgeGen
from .combopt_arg import MinCovArg
from .combopt_arg import UdGraphArg
from .combopt_arg import UdEdgeArg
from .combopt_arg import UdEdgeListArg


gen_list = [
    MinCovGen(),
    UdGraphGen(),
    UdEdgeGen(),
]
