#! /usr/bin/env python3

""" PyClibCapacitance を生成するスクリプト

:file: clibcapacitance_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import OptArg, KwdArg, RawObjArg, ObjConvArg
from mk_py_capi import IntArg, Uint64Arg, UlongArg
from mk_py_capi import BoolArg, StringArg


class ClibCapacitanceGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='ClibCapacitance',
                         pyname='ClibCapacitance',
                         namespace='YM',
                         header_include_files=['ym/ClibCapacitance.h'],
                         source_include_files=['pym/PyClibCapacitance.h',
                                               'pym/PyFloat.h'])

        self.add_dealloc('default')

        def meth_infty(writer):
            return writer.gen_return_pyobject('PyClibCapacitance',
                                              'ClibCapacitance::infty()')
        self.add_static_method('infty',
                               func_body=meth_infty,
                               doc_str='return Infinity Object')

        def get_value(writer):
            return writer.gen_return_py_float('val.value()')
        self.add_getter('get_value',
                        func_body=get_value)
        self.add_attr('value',
                      getter_name='get_value',
                      doc_str='value')

        self.add_richcompare('eq_default')

        self.add_conv('default')
        self.add_deconv('default')
