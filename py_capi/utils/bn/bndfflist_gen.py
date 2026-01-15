#! /usr/bin/env python3

""" PyBnDffList を生成するスクリプト

:file: bndfflist_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import OptArg, RawObjArg


class BnDffListGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='BnDffList',
                         pyname='BnDffList',
                         namespace='YM',
                         header_include_files=['ym/BnDffList.h'],
                         source_include_files=['pym/PyBnDffList.h',
                                               'pym/PyBnDff.h',
                                               'pym/PyBnDffIter2.h',
                                               'pym/PyList.h',
                                               'pym/PyUlong.h'])

        self.add_dealloc('default')

        def sq_length(writer):
            writer.gen_auto_assign('len_val', 'val.size()')
        def sq_item(writer):
            writer.gen_return_pyobject('PyBnDff',
                                       'val[index]')
        self.add_sequence(sq_length=sq_length,
                          sq_item=sq_item)

        def iter_func(writer):
            writer.gen_return_pyobject('PyBnDffIter2',
                                       'val.iter()')
        self.add_iter(iter_func)

        self.add_conv('default')
