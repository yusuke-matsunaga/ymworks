#! /usr/bin/env python3

""" PyClibPinMap を生成するスクリプト

:file: clibpinmap_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import OptArg, KwdArg, RawObjArg, ObjConvArg
from mk_py_capi import LongArg, Uint64Arg, UlongArg
from mk_py_capi import BoolArg, StringArg
from .clib_arg import ClibTimeArg, ClibCapacitanceArg


class ClibPinMapGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='ClibPinMap',
                         pyname='ClibPinMap',
                         namespace='YM',
                         header_include_files=['ym/ClibPinMap.h'],
                         source_include_files=['pym/PyClibPinMap.h',
                                               'pym/PyUlong.h'])

        self.add_dealloc('default')

        def meth_invalid(writer):
            writer.gen_return_pyobject('PyClibPinMap',
                                       'ClibPinMap::invalid()')
        self.add_static_method('invalid',
                               func_body=meth_invalid,
                               doc_str='make INVALID object')

        def get_id(writer):
            writer.gen_return_py_ulong('val.id()')
        self.add_getter('get_id',
                        func_body=get_id)
        self.add_attr('pin_id',
                      getter_name='get_id',
                      doc_str='pin ID')

        def get_inv(writer):
            writer.gen_return_py_bool('val.inv()')
        self.add_getter('get_inv',
                        func_body=get_inv)
        self.add_attr('inv',
                      getter_name='get_inv',
                      doc_str='True if inverted')

        def meth_is_valid(writer):
            writer.gen_return_py_bool('val.is_valid()')
        self.add_method('is_valid',
                        func_body=meth_is_valid,
                        doc_str='True if valid')

        self.add_richcompare('eq_default')

        self.add_conv('default')
        self.add_deconv('default')
