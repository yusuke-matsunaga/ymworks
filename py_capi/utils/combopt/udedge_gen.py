#! /usr/bin/env python3

""" PyUdEdge を生成するスクリプト

:file: udedge_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import UlongArg


class UdEdgeGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='UdEdge',
                         pyname='UdEdge',
                         namespace='YM',
                         header_include_files=['ym/UdGraph.h'],
                         source_include_files=['pym/PyUdEdge.h',
                                               'pym/PyUlong.h'])

        self.add_dealloc('default')

        def get_id1(writer):
            writer.gen_return_py_ulong('val.id1()')
        self.add_getter('get_id1',
                        func_body=get_id1)
        self.add_attr('id1',
                      getter_name='get_id1')

        def get_id2(writer):
            writer.gen_return_py_ulong('val.id2()')
        self.add_getter('get_id2',
                        func_body=get_id2)
        self.add_attr('id2',
                      getter_name='get_id2')

        self.add_conv('default')
        self.add_deconv('default')
