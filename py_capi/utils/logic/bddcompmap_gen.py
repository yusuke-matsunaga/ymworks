#! /usr/bin/env python3

""" PyBddCompMap を生成するスクリプト

:file: bddcompmap_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen, OptArg, KwdArg
from mk_py_capi import RawObjArg, TypedRawObjArg, ObjConvArg
from mk_py_capi import IntArg, UintArg, LongArg, UlongArg
from mk_py_capi import BoolArg, StringArg, TypedObjConvArg
from .logic_arg import BddArg, BddVarArg


class BddCompMapGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='Bdd::ComposeMap',
                         pyclassname='PyBddCompMap',
                         pyname='BddCompMap',
                         namespace='YM',
                         header_include_files=['ym/Bdd.h',
                                               'ym/BddVar.h'],
                         source_include_files=['pym/PyBddCompMap.h',
                                               'pym/PyBddVar.h',
                                               'pym/PyBdd.h'])

        def new_body(writer):
            writer.gen_auto_assign('self', 'type->tp_alloc(type, 0)')
            self.gen_obj_conv(writer, objname='self', varname='my_obj')
            writer.write_line('new (&my_obj->mVal) Bdd::ComposeMap();')
            with writer.gen_if_block('obj != nullptr'):
                writer.gen_auto_assign('emsg',
                                       '"1st argument should be a sequence of (BddVar, Bdd)"')
                with writer.gen_if_block('!PySequence_Check(obj)'):
                    writer.gen_type_error('emsg')
                with writer.gen_foreach_sequence_block('obj1', 'obj'):
                    writer.gen_vardecl(typename='PyObject*', varname='var_obj')
                    writer.gen_vardecl(typename='PyObject*', varname='bdd_obj')
                    with writer.gen_if_block('!PyArg_ParseTuple(obj1, "(OO)", &var_obj, &bdd_obj)' ):
                        writer.write_line('Py_DecRef(obj1);')
                        writer.gen_type_error('emsg')
                    with writer.gen_if_block('!PyBddVar::Check(var_obj)'):
                        writer.write_line('Py_DecRef(obj1);')
                        writer.gen_type_error('emsg')
                    with writer.gen_if_block('!PyBdd::Check(bdd_obj)'):
                        writer.write_line('Py_DecRef(obj1);')
                        writer.gen_type_error('emsg')
                    writer.gen_autoref_assign('var', 'PyBddVar::_get_ref(var_obj)')
                    writer.gen_autoref_assign('bdd', 'PyBdd::_get_ref(bdd_obj)')
                    writer.write_line('my_obj->mVal.emplace(var, bdd);')
                    writer.write_line('Py_DecRef(obj1);')
            writer.gen_return_self()
        self.add_new(new_body,
                     arg_list=[OptArg(),
                               RawObjArg(cvarname='obj',
                                         cvardefault='nullptr')])

        self.add_dealloc('default')

        def mp_length(writer):
            writer.gen_auto_assign('len_val', 'val.size()')
        def mp_subscript(writer):
            with writer.gen_if_block('!PyBddVar::Check(key)'):
                writer.gen_type_error('"1st argument should be BddVar"')
            writer.gen_autoref_assign('var', 'PyBddVar::_get_ref(key)')
            with writer.gen_if_block('val.count(var) == 0'):
                writer.gen_value_error('"key not found"')
            writer.gen_return_pyobject('PyBdd', 'val.at(var)')
        def mp_ass_subscript(writer):
            with writer.gen_if_block('!PyBddVar::Check(key)'):
                writer.gen_type_error('"1st argument should be BddVar"',
                                      error_val=-1)
            with writer.gen_if_block('!PyBdd::Check(obj)'):
                writer.gen_type_error('"2nd argument should be Bdd"',
                                      error_val=-1)
            writer.gen_autoref_assign('var', 'PyBddVar::_get_ref(key)')
            writer.gen_autoref_assign('bdd', 'PyBdd::_get_ref(obj)')
            writer.write_line('val.emplace(var, bdd);')
            writer.gen_return(0)
        self.add_mapping(mp_length=mp_length,
                         mp_subscript=mp_subscript,
                         mp_ass_subscript=mp_ass_subscript)

        self.add_deconv('default')
