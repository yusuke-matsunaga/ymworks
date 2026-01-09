#! /usr/bin/env python3

""" PyBnFunc を生成するスクリプト

:file: bnfunc_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen, OptArg, KwdArg
from mk_py_capi import RawObjArg, TypedRawObjArg, ObjConvArg
from mk_py_capi import IntArg, UintArg, LongArg, UlongArg
from mk_py_capi import BoolArg, StringArg, TypedObjConvArg


class BnFuncGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='BnFunc',
                         pyname='BnFunc',
                         namespace='YM',
                         header_include_files=['ym/BnFunc.h'],
                         source_include_files=['pym/PyBnFunc.h',
                                               'pym/PyPrimType.h',
                                               'pym/PySopCover.h',
                                               'pym/PyExpr.h',
                                               'pym/PyTvFunc.h',
                                               'pym/PyBdd.h',
                                               'pym/PyUlong.h',
                                               'pym/PyString.h'])

        self.add_dealloc('default')

        def meth_is_primitive(writer):
            writer.gen_return_py_bool('val.is_primitive()')
        self.add_method('is_primitive',
                        func_body=meth_is_primitive,
                        doc_str='True if PRIMITIVE type')

        def meth_is_cover(writer):
            writer.gen_return_py_bool('val.is_cover()')
        self.add_method('is_cover',
                        func_body=meth_is_cover,
                        doc_str='True if COVER type')

        def meth_is_expr(writer):
            writer.gen_return_py_bool('val.is_expr()')
        self.add_method('is_expr',
                        func_body=meth_is_expr,
                        doc_str='True if EXPR type')

        def meth_is_tvfunc(writer):
            writer.gen_return_py_bool('val.is_tvfunc()')
        self.add_method('is_tvfunc',
                        func_body=meth_is_tvfunc,
                        doc_str='True if TVFUNC type')

        def meth_is_bdd(writer):
            writer.gen_return_py_bool('val.is_bdd()')
        self.add_method('is_bdd',
                        func_body=meth_is_bdd,
                        doc_str='True if BDD type')

        def meth_print(writer):
            writer.write_line('val.print(std::cout);')
            writer.gen_return_py_none()
        self.add_method('print',
                        func_body=meth_print,
                        doc_str='print contents')

        def get_id(writer):
            writer.gen_return_py_ulong('val.id()')
        self.add_getter('get_id',
                        func_body=get_id)
        self.add_attr('id',
                      getter_name='get_id',
                      doc_str="ID")

        def get_input_num(writer):
            writer.gen_return_py_ulong('val.input_num()')
        self.add_getter('get_input_num',
                        func_body=get_input_num)
        self.add_attr('input_num',
                      getter_name='get_input_num',
                      doc_str="number of inputs")

        def get_type(writer):
            writer.gen_return_py_string('val.type_str()')
        self.add_getter('get_type',
                        func_body=get_type)
        self.add_attr('type',
                      getter_name='get_type',
                      doc_str='type(\\\"PRIMITIVE\\\", \\\"COVER\\\", \\\"EXPR\\\", \\\"TVFUNC\\\", \\\"BDD\\\")')

        def get_primitive_type(writer):
            writer.gen_return_pyobject('PyPrimType', 'val.primitive_type()')
        self.add_getter('get_primitive_type',
                        func_body=get_primitive_type)
        self.add_attr('primitive_type',
                      getter_name='get_primitive_type',
                      doc_str="primitive type")

        def get_input_cover(writer):
            writer.gen_return_pyobject('PySopCover', 'val.input_cover()')
        self.add_getter('get_input_cover',
                        func_body=get_input_cover)
        self.add_attr('input_cover',
                      getter_name='get_input_cover',
                      doc_str="input cover")

        def get_output_inv(writer):
            writer.gen_return_py_bool('val.output_inv()')
        self.add_getter('get_output_inv',
                        func_body=get_output_inv)
        self.add_attr('output_inv',
                      getter_name='get_output_inv',
                      doc_str="True if output is inverted")

        def get_expr(writer):
            writer.gen_return_pyobject('PyExpr', 'val.expr()')
        self.add_getter('get_expr',
                        func_body=get_expr)
        self.add_attr('expr',
                      getter_name='get_expr',
                      doc_str="Expr")

        def get_tvfunc(writer):
            writer.gen_return_pyobject('PyTvFunc', 'val.tvfunc()')
        self.add_getter('get_tvfunc',
                        func_body=get_tvfunc)
        self.add_attr('tvfunc',
                      getter_name='get_tvfunc',
                      doc_str="TvFunc")

        def get_bdd(writer):
            writer.gen_return_pyobject('PyBdd', 'val.bdd()')
        self.add_getter('get_bdd',
                        func_body=get_bdd)
        self.add_attr('bdd',
                      getter_name='get_bdd',
                      doc_str="Bdd")

        self.add_richcompare('eq_default')

        self.add_conv('default')
        self.add_deconv('default')
