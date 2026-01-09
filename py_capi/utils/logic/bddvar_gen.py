#! /usr/bin/env python3

""" PyBddVar を生成するスクリプト

:file: bddvar_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen, OptArg, KwdArg
from mk_py_capi import RawObjArg, TypedRawObjArg, ObjConvArg
from mk_py_capi import IntArg, UintArg, LongArg, UlongArg
from mk_py_capi import BoolArg, StringArg, TypedObjConvArg


class BddVarArg(ObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='BddVar',
                         cvardefault=None,
                         pyclassname='PyBddVar')


class BddVarGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='BddVar',
                         pyname='BddVar',
                         namespace='YM',
                         header_include_files=['ym/BddVar.h'],
                         source_include_files=['pym/PyBdd.h',
                                               'pym/PyBddMgr.h',
                                               'pym/PyBddVar.h',
                                               'pym/PyBddLit.h',
                                               'pym/PyBdd.h',
                                               'pym/PyBddCompMap.h',
                                               'pym/PyBddVarMap.h',
                                               'pym/PyBddVarSet.h',
                                               'pym/PyLiteral.h',
                                               'pym/PyNpnMap.h',
                                               'pym/PyInt.h',
                                               'pym/PyLong.h',
                                               'pym/PyBool.h',
                                               'pym/PyString.h',
                                               'pym/PyModule.h',
                                               'ym/BddVar.h',
                                               'ym/BddLit.h'])

        def new_body(writer):
            writer.gen_auto_assign('self', 'type->tp_alloc(type, 0)')
            self.gen_obj_conv(writer, objname='self', varname='my_obj')
            writer.write_line('new (&my_obj->mVal) BddVar();')
            writer.gen_return_self()
        self.add_new(new_body)

        self.add_dealloc('default')

        """
        def repr_func(writer):
            writer.gen_auto_assign('str_val', 'val.str()')
        self.add_repr(func_body=repr_func)
        """

        self.add_nb_add(
            retclassname='PyBddVarSet'
        )

        def meth_invalid(writer):
            writer.gen_return_pyobject('PyBddVar',
                                       'BddVar::invalid()')
        self.add_static_method('invalid',
                               func_body=meth_invalid,
                               doc_str='make invalid BddVar object')

        def meth_is_invalid(writer):
            writer.gen_return_py_bool('val.is_invalid()')
        self.add_method('is_invalid',
                        func_body=meth_is_invalid,
                        doc_str='return True if invalid')

        def meth_positive_literal(writer):
            writer.gen_return_pyobject('PyBddLit',
                                       'val.positive_literal()')
        self.add_method('positive_literal',
                        func_body=meth_positive_literal,
                        has_catch_block=True,
                        doc_str='return positive literal of this variable')

        def meth_negative_literal(writer):
            writer.gen_return_pyobject('PyBddLit',
                                       'val.negative_literal()')
        self.add_method('negative_literal',
                        func_body=meth_negative_literal,
                        has_catch_block=True,
                        doc_str='return negative literal of this variable')

        def meth_bdd(writer):
            writer.gen_return_pyobject('PyBdd',
                                       'val.bdd()')
        self.add_method('bdd',
                        func_body=meth_bdd)

        def get_varid(writer):
            writer.gen_return_py_int('val.varid()')
        self.add_getter('get_varid',
                        func_body=get_varid)
        self.add_attr('varid',
                      getter_name='get_varid',
                      doc_str='var ID')

        def get_mgr(writer):
            writer.gen_return_pyobject('PyBddMgr', 'val.mgr()')
        self.add_getter('get_mgr',
                        func_body=get_mgr)
        self.add_attr('mgr',
                      getter_name='get_mgr',
                      doc_str='BddMgr')

        self.add_richcompare('eq_default')

        def hash_func(writer):
            writer.gen_return('val.hash()')
        self.add_hash(hash_func)

        self.add_conv('default')
        self.add_deconv('default')


if __name__ == '__main__':

    gen = BddVarGen()

    gen.make_header()

    gen.make_source()
