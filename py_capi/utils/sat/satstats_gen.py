#! /usr/bin/env python3

""" PySatStats を生成するスクリプト

:file: satstats_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2026 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen


class SatStatsGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='SatStats',
                         pyname='SatStats',
                         namespace='YM',
                         header_include_files=['ym/SatStats.h'],
                         source_include_files=['pym/PySatStats.h'])

        self.add_dealloc('default')

        self.add_conv('default')
        self.add_deconv('default')
