#! /usr/bin/env python3

""" PyFraigLiteral を生成するスクリプト

:file: fraigliteral_gen.py
:author: Yusuke Matsunaga (松永 裕介)
p:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen, OptArg, KwdArg
from mk_py_capi import RawObjArg, TypedRawObjArg, ObjConvArg
from mk_py_capi import IntArg, UintArg, LongArg, UlongArg
from mk_py_capi import BoolArg, StringArg, TypedObjConvArg
from mk_py_capi import MulOp, MulIop


class FraigLiteralGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='FraigLiteral',
                         pyname='FraigLiteral',
                         namespace='YM',
                         header_include_files=['ym/FraigLiteral.h'],
                         source_include_files=['pym/PyFraigLiteral.h',
                                               'pym/PyUlong.h',
                                               'pym/PyBool.h'])

        self.add_dealloc('default')

        def meth_zero(writer):
            writer.gen_return_pyobject('PyFraigLiteral',
                                       'FraigLiteral::zero()')
        self.add_static_method('zero',
                               func_body=meth_zero,
                               doc_str='return ZERO literal')

        def meth_one(writer):
            writer.gen_return_pyobject('PyFraigLiteral',
                                       'FraigLiteral::one()')
        self.add_static_method('one',
                               func_body=meth_one,
                               doc_str='return ONE literal')

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

        def meth_is_and(writer):
            writer.gen_return_py_bool('val.is_and()')
        self.add_method('is_and',
                        func_body=meth_is_and,
                        doc_str='True if AND node')

        self.add_richcompare('eq_default')

        self.add_nb_invert()
        self.add_nb_multiply(
            expr=None,
            op_list1=[MulOp('PyBool', useref=False)]
        )
        self.add_nb_inplace_multiply(
            stmt=None,
            op_list1=[MulOp('PyBool', useref=False)]
        )

        def hash_func(writer):
            writer.gen_return('val.hash()')
        self.add_hash(hash_func)

        self.add_conv('default')
        self.add_deconv('default')
