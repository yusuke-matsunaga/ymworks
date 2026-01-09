#! /usr/bin/env python3

""" PyBddVarSet を生成するスクリプト

:file: bddvarset_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import RawObjArg, TypedRawObjArg, OptArg
from mk_py_capi import AddOp, AddIop
from mk_py_capi import SubOp, SubIop
from mk_py_capi import AndOp, AndIop


class BddVarSetGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='BddVarSet',
                         pyname='BddVarSet',
                         namespace='YM',
                         header_include_files=['ym/BddVarSet.h'],
                         source_include_files=['pym/PyBddVarSet.h',
                                               'pym/PyBddVar.h',
                                               'pym/PyBdd.h',
                                               'pym/PyBddMgr.h',
                                               'pym/PyList.h',
                                               'pym/PyLong.h'])

        def new_body(writer):
            writer.gen_auto_assign('self', 'type->tp_alloc(type, 0)')
            self.gen_obj_conv(writer, objname='self', varname='my_obj')
            with writer.gen_if_block('arg_obj == nullptr'):
                writer.write_line('new (&my_obj->mVal) BddVarSet();')
            with writer.gen_elseif_block('PyBddVar::Check(arg_obj)'):
                writer.gen_autoref_assign('var', 'PyBddVar::_get_ref(arg_obj)')
                writer.write_line('new (&my_obj->mVal) BddVarSet(var);')
            with writer.gen_elseif_block('PyList<BddVar, PyBddVar>::Check(arg_obj)'):
                writer.gen_auto_assign('var_list', 'PyList<BddVar, PyBddVar>::Get(arg_obj)')
                writer.write_line('new (&my_obj->mVal) BddVarSet(var_list);')
            with writer.gen_else_block():
                writer.gen_type_error('"argument 2 should be a sequence of \'BddVar\'"')
            writer.gen_return_self()
        self.add_new(new_body,
                     arg_list=[OptArg(),
                               RawObjArg(name='arg',
                                         cvarname='arg_obj')])
        self.add_dealloc('default')

        self.add_nb_add(
            op_list1=[AddOp('PyBddVar')],
            op_list2=[AddOp('PyBddVar')]
        )
        self.add_nb_subtract(
            op_list1=[SubOp('PyBddVar')]
        )
        self.add_nb_and(
            op_list1=[AndOp('PyBddVar')]
        )
        self.add_nb_inplace_add(
            op_list1=[AddIop('PyBddVar')]
        )
        self.add_nb_inplace_subtract(
            op_list1=[SubIop('PyBddVar')]
        )
        self.add_nb_inplace_and(
            op_list1=[AndIop('PyBddVar')]
        )

        self.add_richcompare('eq_default')

        def meth_is_empty_set(writer):
            writer.gen_return_py_bool('val.is_empty_set()')
        self.add_method('is_empty_set',
                        func_body=meth_is_empty_set,
                        doc_str='True if empty set')

        def meth_size(writer):
            writer.gen_return_py_long('val.size()')
        self.add_method('size',
                        func_body=meth_size,
                        has_catch_block=True,
                        doc_str='return size')

        def meth_var_list(writer):
            writer.gen_return_pyobject('PyList<BddVar, PyBddVar>',
                                       'val.var_list()')
        self.add_method('var_list',
                        func_body=meth_var_list,
                        has_catch_block=True,
                        doc_str='convert to list of \'BddVar\'s')

        def meth_bdd(writer):
            writer.gen_return_pyobject('PyBdd',
                                       'val.bdd()')
        self.add_method('bdd',
                        func_body=meth_bdd)

        self.add_conv('default')
        self.add_deconv('default')


if __name__ == '__main__':

    gen = BddVarSetGen()

    gen.make_header()

    gen.make_source()
