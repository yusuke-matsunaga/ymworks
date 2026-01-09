#! /usr/bin/env python3

""" PyClibArea を生成するスクリプト

:file: clibarea_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen, OptArg, KwdArg
from mk_py_capi import RawObjArg, TypedRawObjArg, ObjConvArg
from mk_py_capi import IntArg, UintArg, LongArg, UlongArg
from mk_py_capi import BoolArg, StringArg, TypedObjConvArg


class ClibAreaGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='ClibArea',
                         pyname='ClibArea',
                         namespace='YM',
                         header_include_files=['ym/ClibArea.h'],
                         source_include_files=['pym/PyClibArea.h',
                                               'pym/PyFloat.h'])

        def new_body(writer):
            writer.gen_auto_assign('self', 'type->tp_alloc(type, 0)')
            self.gen_obj_conv(writer, objname='self', varname='my_obj')
            writer.write_line('new (&my_obj->mVal) ClibArea(val);')
            writer.gen_return_self()
            self.add_new(new_body,
                         arg_list=[OptArg(),
                                   DoubleArg(name='val',
                                             cvarname='val',
                                             cvardefault=0.0)])

        self.add_dealloc('default')

        def meth_infinity(writer):
            writer.gen_return_pyobject('PyClibArea', 'ClibArea::infinity()')

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

        self.add_conv('default')
        self.add_deconv('default')
