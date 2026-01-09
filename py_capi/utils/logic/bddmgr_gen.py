#! /usr/bin/env python3

""" PyBddMgr を生成するスクリプト

:file: bddmgr_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import IntArg, StringArg, RawObjArg, BoolArg
from mk_py_capi import OptArg, KwdArg
from .logic_arg import BddArg, BddVarListArg
from .logic_arg import ExprArg


class BddMgrGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='BddMgr',
                         pyname='BddMgr',
                         namespace='YM',
                         header_include_files=['ym/BddMgr.h'],
                         source_include_files=['pym/PyBddMgr.h',
                                               'pym/PyBdd.h',
                                               'pym/PyBddVar.h',
                                               'pym/PyBddLit.h',
                                               'pym/PyExpr.h',
                                               'pym/PyLong.h',
                                               'pym/PyJsonValue.h',
                                               'pym/PyList.h',
                                               'ym/BddVar.h'])

        def new_body(writer):
            writer.gen_auto_assign('self', 'type->tp_alloc(type, 0)')
            self.gen_obj_conv(writer, objname='self', varname='my_obj')
            writer.write_line('new (&my_obj->mVal) BddMgr();')
            writer.gen_return_self()
        self.add_new(new_body)

        self.add_dealloc('default')

        def meth_is_valid(writer):
            writer.gen_return_py_bool('val.is_valid()')
        self.add_method('is_valid',
                        func_body=meth_is_valid,
                        doc_str='True if valid mgr')

        def meth_variable(writer):
            writer.gen_return_pyobject('PyBddVar',
                                       'val.variable(varid)')
        self.add_method('variable',
                        func_body=meth_variable,
                        arg_list=[IntArg(name='varid',
                                         cvarname='varid')],
                        doc_str='make a variable')

        def meth_variable_bdd(writer):
            writer.gen_return_pyobject('PyBdd',
                                       'val.variable_bdd(varid)')
        self.add_method('variable_bdd',
                        func_body=meth_variable_bdd,
                        arg_list=[IntArg(name='varid',
                                         cvarname='varid')],
                        doc_str='make a variable BDD')

        def meth_literal(writer):
            writer.gen_return_pyobject('PyBddLit',
                                       'val.literal(varid, inv)')
        self.add_method('literal',
                        func_body=meth_literal,
                        arg_list=[IntArg(name='varid',
                                         cvarname='varid'),
                                  OptArg(),
                                  KwdArg(),
                                  BoolArg(name='inv',
                                          cvarname='inv')])

        def meth_literal_bdd(writer):
            writer.gen_return_pyobject('PyBdd',
                                       'val.literal_bdd(varid, inv)')
        self.add_method('literal_bdd',
                        func_body=meth_literal_bdd,
                        arg_list=[IntArg(name='varid',
                                         cvarname='varid'),
                                  OptArg(),
                                  KwdArg(),
                                  BoolArg(name='inv',
                                          cvarname='inv')])

        def meth_positive_literal(writer):
            writer.gen_return_pyobject('PyBddLit',
                                       'val.positive_literal(varid)')
        self.add_method('positive_literal',
                        func_body=meth_positive_literal,
                        arg_list=[IntArg(name='varid',
                                         cvarname='varid')])

        def meth_positive_literal_bdd(writer):
            writer.gen_return_pyobject('PyBdd',
                                       'val.positive_literal_bdd(varid)')
        self.add_method('positive_literal_bdd',
                        func_body=meth_positive_literal_bdd,
                        arg_list=[IntArg(name='varid',
                                         cvarname='varid')])

        def meth_negative_literal(writer):
            writer.gen_return_pyobject('PyBddLit',
                                       'val.negative_literal(varid)')
        self.add_method('negative_literal',
                        func_body=meth_negative_literal,
                        arg_list=[IntArg(name='varid',
                                         cvarname='varid')])

        def meth_negative_literal_bdd(writer):
            writer.gen_return_pyobject('PyBdd',
                                       'val.negative_literal_bdd(varid)')
        self.add_method('negative_literal_bdd',
                        func_body=meth_negative_literal_bdd,
                        arg_list=[IntArg(name='varid',
                                         cvarname='varid')])

        def meth_copy(writer):
            writer.gen_return_pyobject('PyBdd', 'val.copy(src)')
        self.add_method('copy',
                        func_body=meth_copy,
                        arg_list=[BddArg(name='src',
                                         cvarname='src')],
                        doc_str='make a copy of BDD')

        def meth_from_truth(writer):
            with writer.gen_if_block('var_list.empty()'):
                writer.gen_return_pyobject('PyBdd', 'val.from_truth(func_str)')
            with writer.gen_else_block():
                writer.gen_return_pyobject('PyBdd', 'val.from_truth(func_str, var_list)')
        self.add_method('from_truth',
                        func_body=meth_from_truth,
                        arg_list=[StringArg(name='func_str',
                                            cvarname='func_str'),
                                  OptArg(),
                                  KwdArg(),
                                  BddVarListArg(name='var_list',
                                                cvarname='var_list')],
                        has_catch_block=True,
                        doc_str='make a BDD from truth table')

        def meth_from_expr(writer):
            with writer.gen_if_block('var_list.empty()'):
                writer.gen_return_pyobject('PyBdd',
                                           'val.from_expr(expr)')
            with writer.gen_else_block():
                writer.gen_return_pyobject('PyBdd',
                                           'val.from_expr(expr, var_list)')
        self.add_method('from_expr',
                        func_body=meth_from_expr,
                        arg_list=[ExprArg(name='expr',
                                          cvarname='expr'),
                                  OptArg(),
                                  KwdArg(),
                                  BddVarListArg(name='var_list',
                                                cvarname='var_list')],
                        has_catch_block=True,
                        doc_str='make a BDD from Expr')

        def meth_enable_gc(writer):
            writer.write_line('val.enable_gc();')
            writer.gen_return_py_none()
        self.add_method('enable_gc',
                        func_body=meth_enable_gc,
                        doc_str='enable GC')

        def meth_disable_gc(writer):
            writer.write_line('val.disable_gc();')
            writer.gen_return_py_none()
        self.add_method('disable_gc',
                        func_body=meth_enable_gc,
                        doc_str='disable GC')

        def get_node_num(writer):
            writer.gen_return_py_long('val.node_num()')
        self.add_getter('get_node_num',
                        func_body=get_node_num)
        self.add_attr('node_num',
                      getter_name='get_node_num',
                      doc_str='number of nodes')

        def get_gc_limit(writer):
            writer.gen_return_py_long('val.gc_limit()')
        self.add_getter('get_gc_limit',
                        func_body=get_gc_limit)
        def set_gc_limit(writer):
            with writer.gen_if_block('!PyLong::Check(obj)'):
                writer.gen_type_error('"integer type is expected"',
                                      error_val='-1')
            writer.gen_auto_assign('gc_limit', 'PyLong::Get(obj)')
            writer.write_line('val.set_gc_limit(gc_limit);')
            writer.gen_return('0')
        self.add_setter('set_gc_limit',
                        func_body=set_gc_limit)
        self.add_attr('gc_limit',
                      getter_name='get_gc_limit',
                      setter_name='set_gc_limit',
                      doc_str='garbage collection limit')

        self.add_richcompare('eq_default')

        self.add_conv('default')
        self.add_deconv('default')


if __name__ == '__main__':

    gen = BddMgrGen()

    gen.make_header()

    gen.make_source()
