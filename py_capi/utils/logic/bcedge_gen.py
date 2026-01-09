#! /usr/bin/env python3

""" PyBcEdge を生成するスクリプト

:file: bcedge_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen, OptArg, KwdArg
from mk_py_capi import UlongArg, StringArg, BoolArg


class BcEdgeGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='BcEdge',
                         pyname='BcEdge',
                         namespace='YM',
                         header_include_files=['ym/BcGraph.h'],
                         source_include_files=['pym/PyBcEdge.h',
                                               'pym/PyString.h',
                                               'pym/PyUlong.h'])

        self.add_dealloc()

        def meth_zero(writer):
            writer.gen_return_pyobject('PyBcEdge',
                                       'BcEdge::zero()')
        self.add_static_method('zero',
                               func_body=meth_zero,
                               doc_str='make ZERO edge')

        def meth_one(writer):
            writer.gen_return_pyobject('PyBcEdge',
                                       'BcEdge::one()')
        self.add_static_method('one',
                               func_body=meth_one,
                               doc_str='make ONE edge')

        def meth_input(writer):
            writer.gen_return_pyobject('PyBcEdge',
                                       'BcEdge::input(input_id, inv)')
        self.add_static_method('input',
                               func_body=meth_input,
                               arg_list=[UlongArg(name='input_id',
                                                  cvarname='input_id'),
                                         OptArg(),
                                         KwdArg(),
                                         BoolArg(name='inv',
                                                 cvarname='inv',
                                                 cvardefault='false')],
                               doc_str='make INPUT edge')

        def meth_op(writer):
            writer.gen_return_pyobject('PyBcEdge',
                                       'BcEdge::op(op_id, inv)')
        self.add_static_method('op',
                               func_body=meth_op,
                               arg_list=[UlongArg(name='op_id',
                                                  cvarname='op_id'),
                                         OptArg(),
                                         KwdArg(),
                                         BoolArg(name='inv',
                                                 cvarname='inv',
                                                 cvardefault='false')],
                               doc_str='make OP edge')

        def meth_is_input(writer):
            writer.gen_return_py_bool('val.is_input()')
        self.add_method('is_input',
                        func_body=meth_is_input,
                        doc_str='True if INPUT edge')

        def meth_is_op(writer):
            writer.gen_return_py_bool('val.is_op()')
        self.add_method('is_op',
                        func_body=meth_is_op,
                        doc_str='True if OP edge')

        def meth_is_zero(writer):
            writer.gen_return_py_bool('val.is_zero()')
        self.add_method('is_zero',
                        func_body=meth_is_zero,
                        doc_str='True if ZERO edge')

        def meth_is_one(writer):
            writer.gen_return_py_bool('val.is_one()')
        self.add_method('is_one',
                        func_body=meth_is_one,
                        doc_str='True if ONE edge')

        def get_input_id(writer):
            writer.gen_return_py_ulong('val.input_id()')
        self.add_getter('get_input_id',
                        func_body=get_input_id,
                        has_catch_block=True)
        self.add_attr('input_id',
                      getter_name='get_input_id')

        def get_op_id(writer):
            writer.gen_return_py_ulong('val.op_id()')
        self.add_getter('get_op_id',
                        func_body=get_op_id,
                        has_catch_block=True)
        self.add_attr('op_id',
                      getter_name='get_op_id')

        def get_inv(writer):
            writer.gen_return_py_bool('val.inv()')
        self.add_getter('get_inv',
                        func_body=get_inv)
        self.add_attr('inv',
                      getter_name='get_inv')

        self.add_nb_invert()

        self.add_richcompare('eq_default')

        def meth_to_string(writer):
            writer.gen_return_py_string('val.to_string()')
        self.add_method('to_string',
                        func_body=meth_to_string,
                        doc_str='return string representation')

        self.add_conv('default')
        self.add_deconv('default')
