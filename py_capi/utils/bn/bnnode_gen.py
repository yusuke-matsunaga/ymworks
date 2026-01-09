#! /usr/bin/env python3

""" PyBnNode を生成するスクリプト

:file: bnnode_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen, OptArg, KwdArg
from mk_py_capi import RawObjArg, TypedRawObjArg, ObjConvArg
from mk_py_capi import IntArg, UintArg, LongArg, UlongArg
from mk_py_capi import BoolArg, StringArg, TypedObjConvArg


class BnNodeGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='BnNode',
                         pyname='BnNode',
                         namespace='YM',
                         header_include_files=['ym/BnNode.h'],
                         source_include_files=['pym/PyBnNode.h',
                                               'pym/PyBnNodeList.h',
                                               'pym/PyBnFunc.h',
                                               'pym/PyUlong.h',
                                               'pym/PyString.h'])

        self.add_dealloc('default')

        def meth_is_input(writer):
            writer.gen_return_py_bool('val.is_input()')
        self.add_method('is_input',
                        func_body=meth_is_input,
                        doc_str='True if INPUT type')

        def meth_is_logic(writer):
            writer.gen_return_py_bool('val.is_logic()')
        self.add_method('is_logic',
                        func_body=meth_is_logic,
                        doc_str='True if LOGIC type')

        def meth_is_primary_input(writer):
            writer.gen_return_py_bool('val.is_primary_input()')
        self.add_method('is_primary_input',
                        func_body=meth_is_primary_input,
                        doc_str='True if PRIMARY INPUT type')

        def meth_is_dff_output(writer):
            writer.gen_return_py_bool('val.is_dff_output()')
        self.add_method('is_dff_output',
                        func_body=meth_is_dff_output,
                        doc_str='True if DFF OUTPUT type')

        def meth_fanin(writer):
            writer.gen_return_pyobject('PyBnNode', 'val.fanin(pos)')
        self.add_method('fanin',
                        func_body=meth_fanin,
                        arg_list=[IntArg(name='pos',
                                         cvarname='pos')],
                        doc_str='return fanin node')

        def meth_fanin_list(writer):
            writer.gen_return_pyobject('PyBnNodeList', 'val.fanin_list()')
        self.add_method('fanin_list',
                        func_body=meth_fanin_list,
                        doc_str='return list of fanin nodes')

        def get_id(writer):
            writer.gen_return_py_ulong('val.id()')
        self.add_getter('get_id',
                        func_body=get_id)
        self.add_attr('id',
                      getter_name='get_id',
                      doc_str='ID')

        def get_type(writer):
            writer.gen_return_py_string('val.type_str()')
        self.add_getter('get_type',
                        func_body=get_type)
        self.add_attr('type',
                      getter_name='get_type',
                      doc_str='type(\\\"INPUT\\\", \\\"LOGIC\\\")')

        def get_input_id(writer):
            writer.gen_return_py_ulong('val.input_id()')
        self.add_getter('get_input_id',
                        func_body=get_input_id)
        self.add_attr('input_id',
                      getter_name='get_input_id',
                      doc_str='input ID')

        def get_dff_id(writer):
            writer.gen_return_py_ulong('val.dff_id()')
        self.add_getter('get_dff_id',
                        func_body=get_dff_id)
        self.add_attr('dff_id',
                      getter_name='get_dff_id',
                      doc_str='DFF ID')

        def get_func(writer):
            writer.gen_return_pyobject('PyBnFunc', 'val.func()')
        self.add_getter('get_func',
                        func_body=get_func)
        self.add_attr('func',
                      getter_name='get_func',
                      doc_str='function')

        def get_fanin_num(writer):
            writer.gen_return_py_ulong('val.fanin_num()')
        self.add_getter('get_fanin_num',
                        func_body=get_fanin_num)
        self.add_attr('fanin_num',
                      getter_name='get_fanin_num',
                      doc_str='number of fanins')

        self.add_richcompare('eq_default')

        self.add_conv('default')
        self.add_deconv('default')
