#! /usr/bin/env python3

""" PyClibBundle を生成するスクリプト

:file: clibbundle_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import OptArg, KwdArg
from mk_py_capi import RawObjArg, TypedRawObjArg, ObjConvArg
from mk_py_capi import IntArg, UintArg, LongArg, UlongArg
from mk_py_capi import BoolArg, StringArg, TypedObjConvArg


class ClibBundleGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='ClibBundle',
                         pyname='ClibBundle',
                         namespace='YM',
                         header_include_files=['ym/ClibBundle.h',
                                               'ym/ClibPin.h'],
                         source_include_files=['pym/PyClibBundle.h',
                                               'pym/PyClibPin.h',
                                               'pym/PyClibList.h',
                                               'pym/PyString.h',
                                               'pym/PyUlong.h'])

        self.add_dealloc('default')

        def get_name(writer):
            writer.gen_return_py_string('val.name()')
        self.add_getter('get_name',
                        func_body=get_name)
        self.add_attr('name',
                      getter_name='get_name',
                      doc_str='name')

        def get_pin_num(writer):
            writer.gen_return_py_ulong('val.pin_num()')
        self.add_getter('get_pin_num',
                        func_body=get_pin_num)
        self.add_attr('pin_num',
                      getter_name='get_pin_num',
                      doc_str='number of pins')

        def meth_pin(writer):
            writer.gen_return_pyobject('PyClibPin', 'val.pin(pos)')
        self.add_method('pin',
                        func_body=meth_pin,
                        arg_list=[UlongArg(name='pos',
                                           cvarname='pos')],
                        doc_str='return PIN')

        def meth_pin_list(writer):
            writer.gen_return_pyobject('PyClibPinList',
                                       'val.pin_list()')
        self.add_method('meth_pin_list',
                        func_body=meth_pin_list,
                        doc_str='return list of pins')

        self.add_richcompare('eq_default')

        self.add_conv('default')
        self.add_deconv('default')
