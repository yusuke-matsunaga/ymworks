#! /usr/bin/env python3

""" PyAigLiteral を生成するスクリプト

:file: aigliteral_gen.py
:author: Yusuke Matsunaga (松永 裕介)
p:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen, OptArg, KwdArg
from mk_py_capi import RawObjArg, TypedRawObjArg, ObjConvArg
from mk_py_capi import IntArg, UintArg, LongArg, UlongArg
from mk_py_capi import BoolArg, StringArg, TypedObjConvArg
from mk_py_capi import MulOp, MulIop


class AigLiteralGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='AigLiteral',
                         pyname='AigLiteral',
                         namespace='YM',
                         header_include_files=['ym/AigLiteral.h'],
                         source_include_files=['pym/PyAigLiteral.h',
                                               'pym/PyUlong.h',
                                               'pym/PyBool.h'])

        self.add_dealloc('default')

        def meth_invalid(writer):
            writer.gen_return_pyobject('PyAigLiteral',
                                       'AigLiteral::invalid()')
        self.add_static_method('invalid',
                               func_body=meth_invalid,
                               doc_str='return INVALID literal')

        def meth_zero(writer):
            writer.gen_return_pyobject('PyAigLiteral',
                                       'AigLiteral::zero()')
        self.add_static_method('zero',
                               func_body=meth_zero,
                               doc_str='return ZERO literal')

        def meth_one(writer):
            writer.gen_return_pyobject('PyAigLiteral',
                                       'AigLiteral::one()')
        self.add_static_method('one',
                               func_body=meth_one,
                               doc_str='return ONE literal')

        def meth_is_valid(writer):
            writer.gen_return_py_bool('val.is_valid()')
        self.add_method('is_valid',
                        func_body=meth_is_valid,
                        doc_str='True if valid')

        def meth_is_invalid(writer):
            writer.gen_return_py_bool('val.is_invalid()')
        self.add_method('is_invalid',
                        func_body=meth_is_invalid,
                        doc_str='True if invalid')

        def meth_inv(writer):
            writer.gen_return_py_bool('val.inv()')
        self.add_method('inv',
                        func_body=meth_inv,
                        doc_str='True if inverted')

        def meth_is_zero(writer):
            writer.gen_return_py_bool('val.is_zero()')
        self.add_method('is_zero',
                        func_body=meth_is_zero,
                        doc_str='True if ZERO')

        def meth_is_one(writer):
            writer.gen_return_py_bool('val.is_one()')
        self.add_method('is_one',
                        func_body=meth_is_one,
                        doc_str='True if ONE')

        def meth_is_const(writer):
            writer.gen_return_py_bool('val.is_const()')
        self.add_method('is_const',
                        func_body=meth_is_const,
                        doc_str='True if constant(ZERO or ONE)')

        def get_node_id(writer):
            writer.gen_return_py_ulong('val.node_id()')
        self.add_getter('get_node_id',
                        func_body=get_node_id)
        self.add_attr('node_id',
                      getter_name='get_node_id',
                      doc_str='node ID')

        def meth_invert(writer):
            writer.gen_return_pyobject('PyAigLiteral', '~val')
        self.add_method('invert',
                        func_body=meth_invert,
                        doc_str='INVERT OP')

        self.add_richcompare('eq_default')

        self.add_nb_invert()
        self.add_nb_multiply(
            expr=None,
            op_list1=[MulOp('PyBool', useref=False)]
        )
        self.add_nb_inplace_multiply(
            stmt=None,
            op_list1=[MulIop('PyBool', useref=False)]
        )

        def hash_func(writer):
            writer.gen_return('val.index()')
        self.add_hash(hash_func)

        def conv_func(writer):
            self.gen_alloc_code(writer, varname='obj')
            self.gen_obj_conv(writer, objname='obj', varname='my_obj')
            writer.write_line('new (&my_obj->mVal) AigLiteral(val);')
            writer.gen_return('obj')
        self.add_conv(conv_func)

        self.add_deconv('default')
