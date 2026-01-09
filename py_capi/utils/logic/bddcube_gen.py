#! /usr/bin/env python3

""" PyBddCube を生成するスクリプト

:file: bddcube_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen, OptArg, KwdArg
from mk_py_capi import RawObjArg, TypedRawObjArg, ObjConvArg
from mk_py_capi import IntArg, UintArg, LongArg, UlongArg
from mk_py_capi import BoolArg, StringArg, TypedObjConvArg
from mk_py_capi import AndOp, AndIop


class BddCubeGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='BddCube',
                         pyname='BddCube',
                         namespace='YM',
                         header_include_files=['ym/BddCube.h'],
                         source_include_files=['pym/PyBddCube.h',
                                               'pym/PyBdd.h',
                                               'pym/PyBddLit.h'])

        def new_body(writer):
            writer.gen_auto_assign('self', 'type->tp_alloc(type, 0)')
            self.gen_obj_conv(writer, objname='self', varname='my_obj')
            writer.write_line('new (&my_obj->mVal) BddCube();')
            writer.gen_return_self()
        self.add_new(new_body)

        self.add_dealloc('default')

        self.add_nb_and(
            op_list1=[AndOp('PyBddLit')],
            op_list2=[AndOp('PyBddLit')]
        )
        self.add_nb_inplace_and(
            op_list1=[AndIop('PyBddLit')]
        )

        def meth_universal_cube(writer):
            writer.gen_return_pyobject('PyBddCube', 'BddCube::universal_cube()')
        self.add_static_method('universal_cube',
                               func_body=meth_universal_cube,
                               doc_str='make an universal cube(empty cube)')

        def meth_invalid(writer):
            writer.gen_return_pyobject('PyBddCube', 'BddCube::invalid()')
        self.add_static_method('invalid',
                               func_body=meth_invalid,
                               doc_str='make invalid BddCube object')

        def meth_is_universal_cube(writer):
            writer.gen_return_py_bool('val.is_universal_cube()')
        self.add_method('is_universal_cube',
                        func_body=meth_is_universal_cube,
                        doc_str='return True if univeral cube')

        def meth_is_invalid(writer):
            writer.gen_return_py_bool('val.is_invalid()')
        self.add_method('is_invalid',
                        func_body=meth_is_invalid,
                        doc_str='return True if invalid')

        def meth_bdd(writer):
            writer.gen_return_pyobject('PyBdd',
                                       'val.bdd()')
        self.add_method('bdd',
                        func_body=meth_bdd)

        self.add_richcompare('eq_default')

        def hash_func(writer):
            writer.gen_return('val.hash()')
        self.add_hash(hash_func)

        self.add_conv('default')
        self.add_deconv('default')
