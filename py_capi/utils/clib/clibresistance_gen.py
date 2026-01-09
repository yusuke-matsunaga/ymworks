#! /usr/bin/env python3

""" PyClibResistance を生成するスクリプト

:file: clibresistance_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import OptArg, KwdArg, RawObjArg, ObjConvArg
from mk_py_capi import LongArg, Uint64Arg, UlongArg
from mk_py_capi import BoolArg, StringArg


class ClibResistanceGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='ClibResistance',
                         pyname='ClibResistance',
                         namespace='YM',
                         header_include_files=['ym/ClibResistance.h'],
                         source_include_files=['pym/PyClibResistance.h',
                                               'pym/PyFloat.h'])

        self.add_dealloc('default')

        def meth_infty(writer):
            writer.gen_return_pyobject('PyClibResistance',
                                       'ClibResistance::infty()')
        self.add_static_method('infty',
                               func_body=meth_infty,
                               doc_str='make INFINITY object')

        def get_value(writer):
            writer.gen_return_py_float('val.value()')
        self.add_getter('get_value',
                        func_body=get_value)
        self.add_attr('value',
                      getter_name='get_value',
                      doc_str='value')

        self.add_nb_add()
        self.add_nb_subtract()
        self.add_nb_inplace_add()
        self.add_nb_inplace_subtract()

        self.add_richcompare('eq_default')

        self.add_conv('default')
        self.add_deconv('default')
