#! /usr/bin/env python3

""" PyBddLit を生成するスクリプト

:file: bddlit_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen, OptArg, KwdArg
from mk_py_capi import RawObjArg, TypedRawObjArg, ObjConvArg
from mk_py_capi import IntArg, UintArg, LongArg, UlongArg
from mk_py_capi import BoolArg, StringArg, TypedObjConvArg
from mk_py_capi import MulOp, MulIop


class BddLitGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='BddLit',
                         pyname='BddLit',
                         namespace='YM',
                         header_include_files=['ym/BddLit.h'],
                         source_include_files=['pym/PyBddLit.h',
                                               'pym/PyBddMgr.h',
                                               'pym/PyBddVar.h',
                                               'pym/PyBddCube.h',
                                               'pym/PyBdd.h',
                                               'pym/PyBool.h'])

        def new_body(writer):
            writer.gen_auto_assign('self', 'type->tp_alloc(type, 0)')
            self.gen_obj_conv(writer, objname='self', varname='my_obj')
            writer.write_line('new (&my_obj->mVal) BddLit();')
            writer.gen_return_self()
        self.add_new(new_body)

        self.add_dealloc('default')

        self.add_nb_invert()
        self.add_nb_multiply(
            expr=None,
            op_list1=[MulOp('PyBool', useref=False)]
        )
        self.add_nb_inplace_multiply(
            stmt=None,
            op_list1=[MulIop('PyBool', useref=False)]
        )
        self.add_nb_and(
            retclassname='PyBddCube'
        )

        def meth_invalid(writer):
            writer.gen_return_pyobject('PyBddLit', 'BddLit::invalid()')
        self.add_static_method('invalid',
                               func_body=meth_invalid,
                               doc_str='make invalid BddLit object')

        def meth_is_invalid(writer):
            writer.gen_return_py_bool('val.is_invalid()')
        self.add_method('is_invalid',
                        func_body=meth_is_invalid,
                        doc_str='return True if invalid')

        def meth_is_positive(writer):
            writer.gen_return_py_bool('val.is_positive()')
        self.add_method('is_positive',
                        func_body=meth_is_positive,
                        doc_str='return True if positive literal')

        def meth_is_negative(writer):
            writer.gen_return_py_bool('val.is_negative()')
        self.add_method('is_negative',
                        func_body=meth_is_negative,
                        doc_str='return True if negative literal')

        def meth_positive_literal(writer):
            writer.gen_return_pyobject('PyBddLit', 'val.positive_literal()')
        self.add_method('positive_literal',
                        func_body=meth_positive_literal,
                        has_catch_block=True,
                        doc_str='return positive literal of the same variable')

        def meth_negative_literal(writer):
            writer.gen_return_pyobject('PyBddLit', 'val.negative_literal()')
        self.add_method('negative_literal',
                        func_body=meth_negative_literal,
                        has_catch_block=True,
                        doc_str='return negative literal of the same variable')

        def meth_bdd(writer):
            writer.gen_return_pyobject('PyBdd',
                                       'val.bdd()')
        self.add_method('bdd',
                        func_body=meth_bdd)

        def meth_complement(writer):
            writer.gen_return_pyobject('PyBddLit', 'val.complement()')
        self.add_method('complement',
                        func_body=meth_complement,
                        has_catch_block=True,
                        doc_str='return the complement of this literal')

        def get_var(writer):
            writer.gen_return_pyobject('PyBddVar', 'val.var()')
        self.add_getter('get_var',
                        func_body=get_var)
        self.add_attr('var',
                      getter_name='get_var',
                      doc_str='BddVar')

        self.add_richcompare('eq_default')

        def hash_func(writer):
            writer.gen_return('val.hash()')
        self.add_hash(hash_func)

        self.add_conv('default')
        self.add_deconv('default')
